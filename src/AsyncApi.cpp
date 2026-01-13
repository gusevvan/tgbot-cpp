#include "tgbot/AsyncApi.h"

#include <boost/asio/steady_timer.hpp>
#include <boost/asio/use_awaitable.hpp>

#include <chrono>
#include <thread>

namespace TgBot {

AsyncApi::AsyncApi(std::string token, const AsyncHttpClient& httpClient, const std::string& url)
    : ApiBase(token, url), _httpClient(httpClient) {
}

net::awaitable<User::Ptr> AsyncApi::getMe() const {
    co_return _tgTypeParser.parseJsonAndGetUser(co_await sendRequest("getMe"));
}

net::awaitable<Message::Ptr> AsyncApi::sendMessage(boost::variant<std::int64_t, std::string> chatId,
                              const std::string& text,
                              LinkPreviewOptions::Ptr linkPreviewOptions,
                              ReplyParameters::Ptr replyParameters,
                              GenericReply::Ptr replyMarkup,
                              const std::string& parseMode,
                              bool disableNotification,
                              const std::vector<MessageEntity::Ptr>& entities,
                              std::int32_t messageThreadId,
                              bool protectContent,
                              const std::string& businessConnectionId) const {
    auto args = sendMessageArgs(chatId, text, linkPreviewOptions, replyParameters, replyMarkup, 
                                parseMode, disableNotification, entities, messageThreadId, 
                                protectContent, businessConnectionId);
    co_return _tgTypeParser.parseJsonAndGetMessage(co_await sendRequest("sendMessage", args));
}

net::awaitable<Message::Ptr> AsyncApi::editMessageText(const std::string& text,
                                  boost::variant<std::int64_t, std::string> chatId,
                                  std::int32_t messageId,
                                  const std::string& inlineMessageId,
                                  const std::string& parseMode,
                                  LinkPreviewOptions::Ptr linkPreviewOptions,
                                  InlineKeyboardMarkup::Ptr replyMarkup,
                                  const std::vector<MessageEntity::Ptr>& entities) const {
    auto args = editMessageTextArgs(text, chatId, messageId, inlineMessageId, parseMode,
        linkPreviewOptions, replyMarkup, entities
    );
    boost::property_tree::ptree p = co_await sendRequest("editMessageText", args);
    if (p.get_child_optional("message_id")) {
        co_return _tgTypeParser.parseJsonAndGetMessage(p);
    } else {
        co_return nullptr;
    }
}

net::awaitable<bool> AsyncApi::deleteMessage(boost::variant<std::int64_t, std::string> chatId,
                        std::int32_t messageId) const {
    auto args = deleteMessageArgs(chatId, messageId);
    boost::property_tree::ptree p = co_await sendRequest("deleteMessage", args);
    co_return p.get<bool>("", false);
}

net::awaitable<bool> AsyncApi::answerCallbackQuery(const std::string& callbackQueryId,
                             const std::string& text,
                             bool showAlert,
                             const std::string& url,
                             std::int32_t cacheTime) const {
    auto args = answerCallbackQueryArgs(callbackQueryId, text, showAlert, url, cacheTime);
    co_return (co_await sendRequest("answerCallbackQuery", args)).get<bool>("", false);
}

net::awaitable<bool> AsyncApi::answerInlineQuery(const std::string& inlineQueryId,
                            const std::vector<InlineQueryResult::Ptr>& results,
                            std::int32_t cacheTime,
                            bool isPersonal,
                            const std::string& nextOffset,
                            InlineQueryResultsButton::Ptr button) const {
    auto args = answerInlineQueryArgs(inlineQueryId, results, cacheTime, isPersonal, nextOffset, button);
    co_return (co_await sendRequest("answerInlineQuery", args)).get<bool>("", false);
}
                                  
net::awaitable<boost::property_tree::ptree> AsyncApi::sendRequest(const std::string& method, const std::vector<HttpReqArg>& args) const {
    std::string url(_url);
    url += "/bot";
    url += _token;
    url += "/";
    url += method;

    int retries = 0;
    while (1) 
    {
        try {
            std::string serverResponse = co_await _httpClient.makeRequest(url, args);
            
            if (!serverResponse.compare(0, 6, "<html>")) {
                std::string message = "tgbot-cpp library have got html page instead of json response. Maybe you entered wrong bot token.";
                throw TgException(message, TgException::ErrorCode::HtmlResponse);
            }

            boost::property_tree::ptree result; 
            try {
                result = _tgTypeParser.parseJson(serverResponse);
            } catch (boost::property_tree::ptree_error& e) {
                std::string message = "tgbot-cpp library can't parse json response. " + std::string(e.what());
                throw TgException(message, TgException::ErrorCode::InvalidJson);
            }

            if (result.get<bool>("ok", false)) {
                co_return result.get_child("result");
            } else {
                std::string message = result.get("description", "");
                size_t errorCode = result.get<size_t>("error_code", 0u);

                throw TgException(message, static_cast<TgException::ErrorCode>(errorCode));
            }
        } catch (...) {
            if ((requestMaxRetries >= 0) && (retries == requestMaxRetries)) {
                throw;
            } else {
                retries++;
            }
        }
        net::steady_timer timer{co_await net::this_coro::executor};
        timer.expires_after(std::chrono::seconds(requestBackoff));
        co_await timer.async_wait(net::use_awaitable);
    }
}

}
