#ifndef TGBOT_ASYNC_HTTPCLIENT_H
#define TGBOT_ASYNC_HTTPCLIENT_H

#include "tgbot/net/Url.h"
#include "tgbot/net/HttpReqArg.h"
#include "tgbot/net/HttpParser.h"

#include <boost/asio/awaitable.hpp>
#include <boost/beast/http.hpp>

#include <string>
#include <vector>

namespace TgBot {

namespace net = boost::asio;
namespace http = boost::beast::http;

class TGBOT_API AsyncHttpClient {

public:
    AsyncHttpClient();

    http::request<http::string_body> generateBeastRequest(const Url& url, const std::vector<HttpReqArg>& args) const;
    net::awaitable<std::string> makeRequest(const Url& url, const std::vector<HttpReqArg>& args) const;

private:
    const HttpParser _httpParser;

    int requestMaxRetries = 3;
    int requestBackoff = 1;
};

}

#endif //TGBOT_ASYNC_HTTPCLIENT_H
