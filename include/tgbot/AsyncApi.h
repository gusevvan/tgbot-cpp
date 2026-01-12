#ifndef TGBOT_ASYNC_API_H
#define TGBOT_ASYNC_API_H

#include "tgbot/net/AsyncHttpClient.h"
#include "tgbot/ApiBase.h"

namespace TgBot {

class Bot;

class AsyncApi: public ApiBase {

typedef std::shared_ptr<std::vector<std::string>> StringArrayPtr;

friend class Bot;

public:
    AsyncApi(std::string token, const AsyncHttpClient& httpClient, const std::string& url);

    net::awaitable<User::Ptr> getMe() const;


    net::awaitable<Message::Ptr> sendMessage(boost::variant<std::int64_t, std::string> chatId,
                             const std::string& text,
                             LinkPreviewOptions::Ptr linkPreviewOptions = nullptr,
                             ReplyParameters::Ptr replyParameters = nullptr,
                             GenericReply::Ptr replyMarkup = nullptr,
                             const std::string& parseMode = "",
                             bool disableNotification = false,
                             const std::vector<MessageEntity::Ptr>& entities = std::vector<MessageEntity::Ptr>(),
                             std::int32_t messageThreadId = 0,
                             bool protectContent = false,
                             const std::string& businessConnectionId = "") const;

    net::awaitable<Message::Ptr> editMessageText(const std::string& text,
                                 boost::variant<std::int64_t, std::string> chatId = 0,
                                 std::int32_t messageId = 0,
                                 const std::string& inlineMessageId = "",
                                 const std::string& parseMode = "",
                                 LinkPreviewOptions::Ptr linkPreviewOptions = nullptr,
                                 InlineKeyboardMarkup::Ptr replyMarkup = nullptr,
                                 const std::vector<MessageEntity::Ptr>& entities = std::vector<MessageEntity::Ptr>()) const;

    net::awaitable<bool> deleteMessage(boost::variant<std::int64_t, std::string> chatId,
                       std::int32_t messageId) const;

    net::awaitable<bool> answerCallbackQuery(const std::string& callbackQueryId,
                             const std::string& text = "",
                             bool showAlert = false,
                             const std::string& url = "",
                             std::int32_t cacheTime = 0) const;

    net::awaitable<bool> answerInlineQuery(const std::string& inlineQueryId,
                           const std::vector<InlineQueryResult::Ptr>& results,
                           std::int32_t cacheTime = 300,
                           bool isPersonal = false,
                           const std::string& nextOffset = "",
                           InlineQueryResultsButton::Ptr button = nullptr) const;

    const AsyncHttpClient& _httpClient;
    
protected:
    net::awaitable<boost::property_tree::ptree> sendRequest(const std::string& method, const std::vector<HttpReqArg>& args = std::vector<HttpReqArg>()) const;
};

}

#endif //TGBOT_ASYNC_API_H
