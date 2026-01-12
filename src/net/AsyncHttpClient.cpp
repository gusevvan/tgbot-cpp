#include "tgbot/net/AsyncHttpClient.h"

#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/as_tuple.hpp>
#include <boost/asio/awaitable.hpp>

#include <cstddef>
#include <vector>
#include <iostream>

namespace net = boost::asio;
namespace beast = boost::beast;
namespace http = boost::beast::http;
namespace ssl = boost::asio::ssl;
using tcp = boost::asio::ip::tcp; 
using executor_with_default = net::use_awaitable_t<>::executor_with_default<net::any_io_executor>;
using tcp_stream = typename beast::tcp_stream::rebind_executor<executor_with_default>::other;

namespace TgBot {

AsyncHttpClient::AsyncHttpClient() : _httpParser() {}


http::request<http::string_body> AsyncHttpClient::generateBeastRequest(const Url& url, const std::vector<HttpReqArg>& args) const {
    http::request<http::string_body> request;
    if (args.empty()) {
        request.method(http::verb::get);
    } else {
        request.method(http::verb::post);
    }
    request.version(11);
    request.target(url.path + (url.query.empty() ? "" : "?" + url.query));
    request.set(http::field::host, url.host);
    std::string boundary = _httpParser.generateMultipartBoundary(args);
    std::string body;
    if (boundary.empty()) {
        request.set(http::field::content_type, "application/x-www-form-urlencoded");
        body = _httpParser.generateWwwFormUrlencoded(args);
    } else {
        request.set(http::field::content_type, "multipart/form-data; boundary=" + boundary);
        body = _httpParser.generateMultipartFormData(args, boundary);
    }
    if (!body.empty()) {
        request.set(http::field::content_length, std::to_string(body.size()));
        request.body() = body;
    }
    return request;
}

net::awaitable<std::string> AsyncHttpClient::makeRequest(const Url& url, const std::vector<HttpReqArg>& args) const {
    ssl::context ctx{ssl::context::tlsv12_client};
    ctx.set_default_verify_paths();
    ctx.set_verify_mode(ssl::verify_peer);
    auto resolver = net::use_awaitable.as_default_on(tcp::resolver(co_await net::this_coro::executor));
    const auto results = co_await resolver.async_resolve(url.host, "443");
    beast::ssl_stream<tcp_stream> stream{
        net::use_awaitable.as_default_on(beast::tcp_stream(co_await net::this_coro::executor)),
        ctx};
    if(! SSL_set_tlsext_host_name(stream.native_handle(), url.host.c_str())) {
        throw boost::system::system_error(static_cast<int>(::ERR_get_error()), net::error::get_ssl_category());
    }
    beast::get_lowest_layer(stream).expires_after(std::chrono::seconds(30));
    co_await beast::get_lowest_layer(stream).async_connect(results);
    beast::get_lowest_layer(stream).expires_after(std::chrono::seconds(30));
    co_await stream.async_handshake(ssl::stream_base::client);
    auto request = generateBeastRequest(url, args);
    beast::get_lowest_layer(stream).expires_after(std::chrono::seconds(30));
    co_await http::async_write(stream, request);
    beast::flat_buffer buffer;
    http::response<http::dynamic_body> res;
    co_await http::async_read(stream, buffer, res);
    beast::get_lowest_layer(stream).expires_after(std::chrono::seconds(30));
    co_await stream.async_shutdown(net::as_tuple(net::use_awaitable));
    co_return beast::buffers_to_string(res.body().data());
}

}
