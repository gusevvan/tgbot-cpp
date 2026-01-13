#include "tgbot/Api.h"

#include <chrono>
#include <thread>

namespace TgBot {

Api::Api(std::string token, const HttpClient& httpClient, const std::string& url)
    : ApiBase(token, url), _httpClient(httpClient) {}

std::vector<Update::Ptr> Api::getUpdates(std::int32_t offset,
                                         std::int32_t limit,
                                         std::int32_t timeout,
                                         const StringArrayPtr& allowedUpdates) const {
    auto args = getUpdatesArgs(offset, limit, timeout, allowedUpdates);
    return _tgTypeParser.parseJsonAndGetArray<Update>(&TgTypeParser::parseJsonAndGetUpdate, sendRequest("getUpdates", args));
}

bool Api::setWebhook(const std::string& url,
                     InputFile::Ptr certificate,
                     std::int32_t maxConnections,
                     const StringArrayPtr& allowedUpdates,
                     const std::string& ipAddress,
                     bool dropPendingUpdates,
                     const std::string& secretToken) const {
    auto args = setWebhookArgs(url, certificate, maxConnections, allowedUpdates, ipAddress, dropPendingUpdates, secretToken);
    return sendRequest("setWebhook", args).get<bool>("", false);
}

bool Api::deleteWebhook(bool dropPendingUpdates) const {
    auto args = deleteWebhookArgs(dropPendingUpdates);
    return sendRequest("deleteWebhook", args).get<bool>("", false);
}

WebhookInfo::Ptr Api::getWebhookInfo() const {
    boost::property_tree::ptree p = sendRequest("getWebhookInfo");

    if (!p.get_child_optional("url")) {
        return nullptr;
    }

    if (p.get<std::string>("url", "") != std::string("")) {
        return _tgTypeParser.parseJsonAndGetWebhookInfo(p);
    } else {
        return nullptr;
    }
}

User::Ptr Api::getMe() const {
    return _tgTypeParser.parseJsonAndGetUser(sendRequest("getMe"));
}

bool Api::logOut() const {
    return sendRequest("logOut").get<bool>("", false);
}

bool Api::close() const {
    return sendRequest("close").get<bool>("", false);
}

Message::Ptr Api::sendMessage(boost::variant<std::int64_t, std::string> chatId,
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
    return _tgTypeParser.parseJsonAndGetMessage(sendRequest("sendMessage", args));
}

Message::Ptr Api::forwardMessage(boost::variant<std::int64_t, std::string> chatId,
                                 boost::variant<std::int64_t, std::string> fromChatId,
                                 std::int32_t messageId,
                                 bool disableNotification,
                                 bool protectContent,
                                 std::int32_t messageThreadId) const {
    auto args = forwardMessageArgs(chatId, fromChatId, messageId, disableNotification, protectContent, messageThreadId);
    return _tgTypeParser.parseJsonAndGetMessage(sendRequest("forwardMessage", args));
}

std::vector<MessageId::Ptr> Api::forwardMessages(boost::variant<std::int64_t, std::string> chatId,
                                                 boost::variant<std::int64_t, std::string> fromChatId,
                                                 const std::vector<std::int32_t>& messageIds,
                                                 std::int32_t messageThreadId,
                                                 bool disableNotification,
                                                 bool protectContent) const {
    auto args = forwardMessagesArgs(chatId, fromChatId, messageIds, messageThreadId, disableNotification, protectContent);
    return _tgTypeParser.parseJsonAndGetArray<MessageId>(&TgTypeParser::parseJsonAndGetMessageId, sendRequest("forwardMessages", args));
}

MessageId::Ptr Api::copyMessage(boost::variant<std::int64_t, std::string> chatId,
                                boost::variant<std::int64_t, std::string> fromChatId,
                                std::int32_t messageId,
                                const std::string& caption,
                                const std::string& parseMode,
                                const std::vector<MessageEntity::Ptr>& captionEntities,
                                bool disableNotification,
                                ReplyParameters::Ptr replyParameters,
                                GenericReply::Ptr replyMarkup,
                                bool protectContent,
                                std::int32_t messageThreadId) const {
    auto args = copyMessageArgs(chatId, fromChatId, messageId, caption, parseMode, captionEntities,
                                disableNotification, replyParameters, replyMarkup, protectContent, messageThreadId);
    return _tgTypeParser.parseJsonAndGetMessageId(sendRequest("copyMessage", args));
}

std::vector<MessageId::Ptr> Api::copyMessages(boost::variant<std::int64_t, std::string> chatId,
                                              boost::variant<std::int64_t, std::string> fromChatId,
                                              const std::vector<std::int32_t>& messageIds,
                                              std::int32_t messageThreadId,
                                              bool disableNotification,
                                              bool protectContent,
                                              bool removeCaption) const {
    auto args = copyMessagesArgs(chatId, fromChatId, messageIds, messageThreadId, disableNotification, protectContent, removeCaption);
    return _tgTypeParser.parseJsonAndGetArray<MessageId>(&TgTypeParser::parseJsonAndGetMessageId, sendRequest("copyMessages", args));
}

Message::Ptr Api::sendPhoto(boost::variant<std::int64_t, std::string> chatId,
                            boost::variant<InputFile::Ptr, std::string> photo,
                            const std::string& caption,
                            ReplyParameters::Ptr replyParameters,
                            GenericReply::Ptr replyMarkup,
                            const std::string& parseMode,
                            bool disableNotification,
                            const std::vector<MessageEntity::Ptr>& captionEntities,
                            std::int32_t messageThreadId,
                            bool protectContent,
                            bool hasSpoiler,
                            const std::string& businessConnectionId) const {
    auto args = sendPhotoArgs(chatId, photo, caption, replyParameters, replyMarkup, parseMode,
                              disableNotification, captionEntities, messageThreadId, protectContent,
                              hasSpoiler, businessConnectionId);
    return _tgTypeParser.parseJsonAndGetMessage(sendRequest("sendPhoto", args));
}

Message::Ptr Api::sendAudio(boost::variant<std::int64_t, std::string> chatId,
                            boost::variant<InputFile::Ptr, std::string> audio,
                            const std::string& caption,
                            std::int32_t duration,
                            const std::string& performer,
                            const std::string& title,
                            boost::variant<InputFile::Ptr, std::string> thumbnail,
                            ReplyParameters::Ptr replyParameters,
                            GenericReply::Ptr replyMarkup,
                            const std::string& parseMode,
                            bool disableNotification,
                            const std::vector<MessageEntity::Ptr>& captionEntities,
                            std::int32_t messageThreadId,
                            bool protectContent,
                            const std::string& businessConnectionId) const {
    auto args = sendAudioArgs(chatId, audio, caption, duration, performer, title, thumbnail,
                              replyParameters, replyMarkup, parseMode, disableNotification,
                              captionEntities, messageThreadId, protectContent, businessConnectionId);
    return _tgTypeParser.parseJsonAndGetMessage(sendRequest("sendAudio", args));
}

Message::Ptr Api::sendDocument(boost::variant<std::int64_t, std::string> chatId,
                               boost::variant<InputFile::Ptr, std::string> document,
                               boost::variant<InputFile::Ptr, std::string> thumbnail,
                               const std::string& caption,
                               ReplyParameters::Ptr replyParameters,
                               GenericReply::Ptr replyMarkup,
                               const std::string& parseMode,
                               bool disableNotification,
                               const std::vector<MessageEntity::Ptr>& captionEntities,
                               bool disableContentTypeDetection,
                               std::int32_t messageThreadId,
                               bool protectContent,
                               const std::string& businessConnectionId) const {
    auto args = sendDocumentArgs(chatId, document, thumbnail, caption, replyParameters, replyMarkup,
                                 parseMode, disableNotification, captionEntities, disableContentTypeDetection,
                                 messageThreadId, protectContent, businessConnectionId);
    return _tgTypeParser.parseJsonAndGetMessage(sendRequest("sendDocument", args));
}

Message::Ptr Api::sendVideo(boost::variant<std::int64_t, std::string> chatId,
                            boost::variant<InputFile::Ptr, std::string> video,
                            bool supportsStreaming,
                            std::int32_t duration,
                            std::int32_t width,
                            std::int32_t height,
                            boost::variant<InputFile::Ptr, std::string> thumbnail,
                            const std::string& caption ,
                            ReplyParameters::Ptr replyParameters,
                            GenericReply::Ptr replyMarkup,
                            const std::string& parseMode,
                            bool disableNotification,
                            const std::vector<MessageEntity::Ptr>& captionEntities,
                            std::int32_t messageThreadId,
                            bool protectContent,
                            bool hasSpoiler,
                            const std::string& businessConnectionId) const {
    auto args = sendVideoArgs(chatId, video, supportsStreaming, duration, width, height, thumbnail,
                              caption, replyParameters, replyMarkup, parseMode, disableNotification,
                              captionEntities, messageThreadId, protectContent, hasSpoiler, businessConnectionId);
    return _tgTypeParser.parseJsonAndGetMessage(sendRequest("sendVideo", args));
}

Message::Ptr Api::sendAnimation(boost::variant<std::int64_t, std::string> chatId,
                                boost::variant<InputFile::Ptr, std::string> animation,
                                std::int32_t duration,
                                std::int32_t width,
                                std::int32_t height,
                                boost::variant<InputFile::Ptr, std::string> thumbnail,
                                const std::string& caption,
                                ReplyParameters::Ptr replyParameters,
                                GenericReply::Ptr replyMarkup,
                                const std::string& parseMode,
                                bool disableNotification,
                                const std::vector<MessageEntity::Ptr>& captionEntities,
                                std::int32_t messageThreadId,
                                bool protectContent,
                                bool hasSpoiler,
                                const std::string& businessConnectionId) const {
    auto args = sendAnimationArgs(chatId, animation, duration, width, height, thumbnail, caption,
                                  replyParameters, replyMarkup, parseMode, disableNotification,
                                  captionEntities, messageThreadId, protectContent, hasSpoiler, businessConnectionId);
    return _tgTypeParser.parseJsonAndGetMessage(sendRequest("sendAnimation", args));
}

Message::Ptr Api::sendVoice(boost::variant<std::int64_t, std::string> chatId,
                            boost::variant<InputFile::Ptr, std::string> voice,
                            const std::string& caption,
                            std::int32_t duration,
                            ReplyParameters::Ptr replyParameters,
                            GenericReply::Ptr replyMarkup,
                            const std::string& parseMode,
                            bool disableNotification,
                            const std::vector<MessageEntity::Ptr>& captionEntities,
                            std::int32_t messageThreadId,
                            bool protectContent,
                            const std::string& businessConnectionId) const {
    auto args = sendVoiceArgs(chatId, voice, caption, duration, replyParameters, replyMarkup,
                              parseMode, disableNotification, captionEntities, messageThreadId,
                              protectContent, businessConnectionId);
    return _tgTypeParser.parseJsonAndGetMessage(sendRequest("sendVoice", args));
}

Message::Ptr Api::sendVideoNote(boost::variant<std::int64_t, std::string> chatId,
                                boost::variant<InputFile::Ptr, std::string> videoNote,
                                ReplyParameters::Ptr replyParameters,
                                bool disableNotification,
                                std::int32_t duration,
                                std::int32_t length,
                                boost::variant<InputFile::Ptr, std::string> thumbnail,
                                GenericReply::Ptr replyMarkup,
                                std::int32_t messageThreadId,
                                bool protectContent,
                                const std::string& businessConnectionId) const {
    auto args = sendVideoNoteArgs(chatId, videoNote, replyParameters, disableNotification, duration,
                                  length, thumbnail, replyMarkup, messageThreadId, protectContent, businessConnectionId);
    return _tgTypeParser.parseJsonAndGetMessage(sendRequest("sendVideoNote", args));
}

std::vector<Message::Ptr> Api::sendMediaGroup(boost::variant<std::int64_t, std::string> chatId,
                                              const std::vector<InputMedia::Ptr>& media,
                                              bool disableNotification,
                                              ReplyParameters::Ptr replyParameters,
                                              std::int32_t messageThreadId,
                                              bool protectContent,
                                              const std::string& businessConnectionId) const {
    auto args = sendMediaGroupArgs(chatId, media, disableNotification, replyParameters,
                                   messageThreadId, protectContent, businessConnectionId);
    return _tgTypeParser.parseJsonAndGetArray<Message>(&TgTypeParser::parseJsonAndGetMessage, sendRequest("sendMediaGroup", args));
}

Message::Ptr Api::sendLocation(boost::variant<std::int64_t, std::string> chatId,
                               float latitude,
                               float longitude,
                               std::int32_t livePeriod,
                               ReplyParameters::Ptr replyParameters,
                               GenericReply::Ptr replyMarkup,
                               bool disableNotification,
                               float horizontalAccuracy,
                               std::int32_t heading,
                               std::int32_t proximityAlertRadius,
                               std::int32_t messageThreadId,
                               bool protectContent,
                               const std::string& businessConnectionId) const {
    auto args = sendLocationArgs(chatId, latitude, longitude, livePeriod, replyParameters, replyMarkup,
                                 disableNotification, horizontalAccuracy, heading, proximityAlertRadius,
                                 messageThreadId, protectContent, businessConnectionId);
    return _tgTypeParser.parseJsonAndGetMessage(sendRequest("sendLocation", args));
}

Message::Ptr Api::editMessageLiveLocation(float latitude,
                                          float longitude,
                                          boost::variant<std::int64_t, std::string> chatId,
                                          std::int32_t messageId,
                                          const std::string& inlineMessageId,
                                          InlineKeyboardMarkup::Ptr replyMarkup,
                                          float horizontalAccuracy,
                                          std::int32_t heading,
                                          std::int32_t proximityAlertRadius) const {
    auto args = editMessageLiveLocationArgs(latitude, longitude, chatId, messageId, inlineMessageId,
                                            replyMarkup, horizontalAccuracy, heading, proximityAlertRadius);
    return _tgTypeParser.parseJsonAndGetMessage(sendRequest("editMessageLiveLocation", args));
}

Message::Ptr Api::stopMessageLiveLocation(boost::variant<std::int64_t, std::string> chatId,
                                          std::int32_t messageId,
                                          const std::string& inlineMessageId,
                                          InlineKeyboardMarkup::Ptr replyMarkup) const {
    auto args = stopMessageLiveLocationArgs(chatId, messageId, inlineMessageId, replyMarkup);
    return _tgTypeParser.parseJsonAndGetMessage(sendRequest("stopMessageLiveLocation", args));
}

Message::Ptr Api::sendVenue(boost::variant<std::int64_t, std::string> chatId,
                            float latitude,
                            float longitude,
                            const std::string& title,
                            const std::string& address,
                            const std::string& foursquareId,
                            const std::string& foursquareType,
                            bool disableNotification,
                            ReplyParameters::Ptr replyParameters,
                            GenericReply::Ptr replyMarkup,
                            const std::string& googlePlaceId,
                            const std::string& googlePlaceType,
                            std::int32_t messageThreadId,
                            bool protectContent,
                            const std::string& businessConnectionId) const {
    auto args = sendVenueArgs(chatId, latitude, longitude, title, address, foursquareId, foursquareType,
                              disableNotification, replyParameters, replyMarkup, googlePlaceId, googlePlaceType,
                              messageThreadId, protectContent, businessConnectionId);
    return _tgTypeParser.parseJsonAndGetMessage(sendRequest("sendVenue", args));
}

Message::Ptr Api::sendContact(boost::variant<std::int64_t, std::string> chatId,
                              const std::string& phoneNumber,
                              const std::string& firstName,
                              const std::string& lastName ,
                              const std::string& vcard,
                              bool disableNotification,
                              ReplyParameters::Ptr replyParameters,
                              GenericReply::Ptr replyMarkup,
                              std::int32_t messageThreadId,
                              bool protectContent,
                              const std::string& businessConnectionId) const {
    auto args = sendContactArgs(chatId, phoneNumber, firstName, lastName, vcard, disableNotification,
                                replyParameters, replyMarkup, messageThreadId, protectContent, businessConnectionId);
    return _tgTypeParser.parseJsonAndGetMessage(sendRequest("sendContact", args));
}

Message::Ptr Api::sendPoll(boost::variant<std::int64_t, std::string> chatId,
                           const std::string& question,
                           const std::vector<std::string>& options,
                           bool disableNotification,
                           ReplyParameters::Ptr replyParameters,
                           GenericReply::Ptr replyMarkup,
                           bool isAnonymous,
                           const std::string& type,
                           bool allowsMultipleAnswers,
                           std::int32_t correctOptionId,
                           const std::string& explanation,
                           const std::string& explanationParseMode,
                           const std::vector<MessageEntity::Ptr>& explanationEntities,
                           std::int32_t openPeriod,
                           std::int32_t closeDate,
                           bool isClosed,
                           std::int32_t messageThreadId,
                           bool protectContent,
                           const std::string& businessConnectionId) const {
    auto args = sendPollArgs(chatId, question, options, disableNotification, replyParameters, replyMarkup,
                             isAnonymous, type, allowsMultipleAnswers, correctOptionId, explanation,
                             explanationParseMode, explanationEntities, openPeriod, closeDate, isClosed,
                             messageThreadId, protectContent, businessConnectionId);
    return _tgTypeParser.parseJsonAndGetMessage(sendRequest("sendPoll", args));
}

Message::Ptr Api::sendDice(boost::variant<std::int64_t, std::string> chatId,
                           bool disableNotification,
                           ReplyParameters::Ptr replyParameters,
                           GenericReply::Ptr replyMarkup,
                           const std::string& emoji,
                           std::int32_t messageThreadId,
                           bool protectContent,
                           const std::string& businessConnectionId) const {
    auto args = sendDiceArgs(chatId, disableNotification, replyParameters, replyMarkup, emoji,
                             messageThreadId, protectContent, businessConnectionId);
    return _tgTypeParser.parseJsonAndGetMessage(sendRequest("sendDice", args));
}

bool Api::setMessageReaction(boost::variant<std::int64_t, std::string> chatId,
                             std::int32_t messageId,
                             const std::vector<ReactionType::Ptr>& reaction,
                             bool isBig) const {
    auto args = setMessageReactionArgs(chatId, messageId, reaction, isBig);
    return sendRequest("setMessageReaction", args).get<bool>("", false);
}

bool Api::sendChatAction(std::int64_t chatId,
                         const std::string& action,
                         std::int32_t messageThreadId,
                         const std::string& businessConnectionId) const {
    auto args = sendChatActionArgs(chatId, action, messageThreadId, businessConnectionId);
    return sendRequest("sendChatAction", args).get<bool>("", false);
}

UserProfilePhotos::Ptr Api::getUserProfilePhotos(std::int64_t userId,
                                                 std::int32_t offset,
                                                 std::int32_t limit) const {
    auto args = getUserProfilePhotosArgs(userId, offset, limit);
    return _tgTypeParser.parseJsonAndGetUserProfilePhotos(sendRequest("getUserProfilePhotos", args));
}

File::Ptr Api::getFile(const std::string& fileId) const {
    auto args = getFileArgs(fileId);
    return _tgTypeParser.parseJsonAndGetFile(sendRequest("getFile", args));
}

bool Api::banChatMember(boost::variant<std::int64_t, std::string> chatId,
                        std::int64_t userId,
                        std::int32_t untilDate,
                        bool revokeMessages) const {
    auto args = banChatMemberArgs(chatId, userId, untilDate, revokeMessages);
    return sendRequest("banChatMember", args).get<bool>("", false);
}

bool Api::unbanChatMember(boost::variant<std::int64_t, std::string> chatId,
                          std::int64_t userId,
                          bool onlyIfBanned) const {
    auto args = unbanChatMemberArgs(chatId, userId, onlyIfBanned);
    return sendRequest("unbanChatMember", args).get<bool>("", false);
}

bool Api::restrictChatMember(boost::variant<std::int64_t, std::string> chatId,
                             std::int64_t userId,
                             TgBot::ChatPermissions::Ptr permissions,
                             std::uint32_t untilDate,
                             bool useIndependentChatPermissions) const {
    auto args = restrictChatMemberArgs(chatId, userId, permissions, untilDate, useIndependentChatPermissions);
    return sendRequest("restrictChatMember", args).get<bool>("", false);
}

bool Api::promoteChatMember(boost::variant<std::int64_t, std::string> chatId,
                            std::int64_t userId,
                            bool canChangeInfo,
                            bool canPostMessages,
                            bool canEditMessages,
                            bool canDeleteMessages,
                            bool canInviteUsers,
                            bool canPinMessages,
                            bool canPromoteMembers,
                            bool isAnonymous,
                            bool canManageChat,
                            bool canManageVideoChats,
                            bool canRestrictMembers,
                            bool canManageTopics,
                            bool canPostStories,
                            bool canEditStories,
                            bool canDeleteStories) const {
    auto args = promoteChatMemberArgs(chatId, userId, canChangeInfo, canPostMessages, canEditMessages,
                                      canDeleteMessages, canInviteUsers, canPinMessages, canPromoteMembers,
                                      isAnonymous, canManageChat, canManageVideoChats, canRestrictMembers,
                                      canManageTopics, canPostStories, canEditStories, canDeleteStories);
    return sendRequest("promoteChatMember", args).get<bool>("", false);
}

bool Api::setChatAdministratorCustomTitle(boost::variant<std::int64_t, std::string> chatId,
                                          std::int64_t userId,
                                          const std::string& customTitle) const {
    auto args = setChatAdministratorCustomTitleArgs(chatId, userId, customTitle);
    return sendRequest("setChatAdministratorCustomTitle", args).get<bool>("", false);
}

bool Api::banChatSenderChat(boost::variant<std::int64_t, std::string> chatId,
                            std::int64_t senderChatId) const {
    auto args = banChatSenderChatArgs(chatId, senderChatId);
    return sendRequest("banChatSenderChat", args).get<bool>("", false);
}

bool Api::unbanChatSenderChat(boost::variant<std::int64_t, std::string> chatId,
                              std::int64_t senderChatId) const {
    auto args = unbanChatSenderChatArgs(chatId, senderChatId);
    return sendRequest("unbanChatSenderChat", args).get<bool>("", false);
}

bool Api::setChatPermissions(boost::variant<std::int64_t, std::string> chatId,
                             ChatPermissions::Ptr permissions,
                             bool useIndependentChatPermissions) const {
    auto args = setChatPermissionsArgs(chatId, permissions, useIndependentChatPermissions);
    return sendRequest("setChatPermissions", args).get<bool>("", false);
}

std::string Api::exportChatInviteLink(boost::variant<std::int64_t, std::string> chatId) const {
    auto args = exportChatInviteLinkArgs(chatId);
    return sendRequest("exportChatInviteLink", args).get("", "");
}

ChatInviteLink::Ptr Api::createChatInviteLink(boost::variant<std::int64_t, std::string> chatId,
                                              std::int32_t expireDate,
                                              std::int32_t memberLimit,
                                              const std::string& name,
                                              bool createsJoinRequest) const {
    auto args = createChatInviteLinkArgs(chatId, expireDate, memberLimit, name, createsJoinRequest);
    return _tgTypeParser.parseJsonAndGetChatInviteLink(sendRequest("createChatInviteLink", args));
}

ChatInviteLink::Ptr Api::editChatInviteLink(boost::variant<std::int64_t, std::string> chatId,
                                            const std::string& inviteLink,
                                            std::int32_t expireDate,
                                            std::int32_t memberLimit,
                                            const std::string& name,
                                            bool createsJoinRequest) const {
    auto args = editChatInviteLinkArgs(chatId, inviteLink, expireDate, memberLimit, name, createsJoinRequest);
    return _tgTypeParser.parseJsonAndGetChatInviteLink(sendRequest("editChatInviteLink", args));
}

ChatInviteLink::Ptr Api::revokeChatInviteLink(boost::variant<std::int64_t, std::string> chatId,
                                              const std::string& inviteLink) const {
    auto args = revokeChatInviteLinkArgs(chatId, inviteLink);
    return _tgTypeParser.parseJsonAndGetChatInviteLink(sendRequest("revokeChatInviteLink", args));
}

bool Api::approveChatJoinRequest(boost::variant<std::int64_t, std::string> chatId,
                                 std::int64_t userId) const {
    auto args = approveChatJoinRequestArgs(chatId, userId);
    return sendRequest("approveChatJoinRequest", args).get<bool>("", false);
}

bool Api::declineChatJoinRequest(boost::variant<std::int64_t, std::string> chatId,
                                 std::int64_t userId) const {
    auto args = declineChatJoinRequestArgs(chatId, userId);
    return sendRequest("declineChatJoinRequest", args).get<bool>("", false);
}

bool Api::setChatPhoto(boost::variant<std::int64_t, std::string> chatId,
                       const InputFile::Ptr photo) const {
    auto args = setChatPhotoArgs(chatId, photo);
    return sendRequest("setChatPhoto", args).get<bool>("", false);
}

bool Api::deleteChatPhoto(boost::variant<std::int64_t, std::string> chatId) const {
    auto args = deleteChatPhotoArgs(chatId);
    return sendRequest("deleteChatPhoto", args).get<bool>("", false);
}

bool Api::setChatTitle(boost::variant<std::int64_t, std::string> chatId,
                       const std::string& title) const {
    auto args = setChatTitleArgs(chatId, title);
    return sendRequest("setChatTitle", args).get<bool>("", false);
}

bool Api::setChatDescription(boost::variant<std::int64_t, std::string> chatId,
                             const std::string& description) const {
    auto args = setChatDescriptionArgs(chatId, description);
    return sendRequest("setChatDescription", args).get<bool>("", false);
}

bool Api::pinChatMessage(boost::variant<std::int64_t, std::string> chatId,
                         std::int32_t messageId,
                         bool disableNotification) const {
    auto args = pinChatMessageArgs(chatId, messageId, disableNotification);
    return sendRequest("pinChatMessage", args).get<bool>("", false);
}

bool Api::unpinChatMessage(boost::variant<std::int64_t, std::string> chatId,
                           std::int32_t messageId) const {
    auto args = unpinChatMessageArgs(chatId, messageId);
    return sendRequest("unpinChatMessage", args).get<bool>("", false);
}

bool Api::unpinAllChatMessages(boost::variant<std::int64_t, std::string> chatId) const {
    auto args = unpinAllChatMessagesArgs(chatId);
    return sendRequest("unpinAllChatMessages", args).get<bool>("", false);
}

bool Api::leaveChat(boost::variant<std::int64_t, std::string> chatId) const {
    auto args = leaveChatArgs(chatId);
    return sendRequest("leaveChat", args).get<bool>("", false);
}

Chat::Ptr Api::getChat(boost::variant<std::int64_t, std::string> chatId) const {
    auto args = getChatArgs(chatId);
    return _tgTypeParser.parseJsonAndGetChat(sendRequest("getChat", args));
}

std::vector<ChatMember::Ptr> Api::getChatAdministrators(boost::variant<std::int64_t, std::string> chatId) const {
    auto args = getChatAdministratorsArgs(chatId);
    return _tgTypeParser.parseJsonAndGetArray<ChatMember>(&TgTypeParser::parseJsonAndGetChatMember, sendRequest("getChatAdministrators", args));
}

int32_t Api::getChatMemberCount(boost::variant<std::int64_t, std::string> chatId) const {
    auto args = getChatMemberCountArgs(chatId);
    return sendRequest("getChatMemberCount", args).get<int32_t>("", 0);
}

ChatMember::Ptr Api::getChatMember(boost::variant<std::int64_t, std::string> chatId,
                                   std::int64_t userId) const {
    auto args = getChatMemberArgs(chatId, userId);
    return _tgTypeParser.parseJsonAndGetChatMember(sendRequest("getChatMember", args));
}

bool Api::setChatStickerSet(boost::variant<std::int64_t, std::string> chatId,
                            const std::string& stickerSetName) const {
    auto args = setChatStickerSetArgs(chatId, stickerSetName);
    return sendRequest("setChatStickerSet", args).get<bool>("", false);
}

bool Api::deleteChatStickerSet(boost::variant<std::int64_t, std::string> chatId) const {
    auto args = deleteChatStickerSetArgs(chatId);
    return sendRequest("deleteChatStickerSet", args).get<bool>("", false);
}

std::vector<Sticker::Ptr> Api::getForumTopicIconStickers() const {
    return _tgTypeParser.parseJsonAndGetArray<Sticker>(&TgTypeParser::parseJsonAndGetSticker, sendRequest("getForumTopicIconStickers"));
}

ForumTopic::Ptr Api::createForumTopic(boost::variant<std::int64_t, std::string> chatId,
                                      const std::string& name,
                                      std::int32_t iconColor,
                                      const std::string& iconCustomEmojiId) const {
    auto args = createForumTopicArgs(chatId, name, iconColor, iconCustomEmojiId);
    return _tgTypeParser.parseJsonAndGetForumTopic(sendRequest("createForumTopic", args));
}

bool Api::editForumTopic(boost::variant<std::int64_t, std::string> chatId,
                         std::int32_t messageThreadId,
                         const std::string& name,
                         boost::variant<std::int32_t, std::string> iconCustomEmojiId) const {
    auto args = editForumTopicArgs(chatId, messageThreadId, name, iconCustomEmojiId);
    return sendRequest("editForumTopic", args).get<bool>("", false);
}

bool Api::closeForumTopic(boost::variant<std::int64_t, std::string> chatId,
                          std::int32_t messageThreadId) const {
    auto args = closeForumTopicArgs(chatId, messageThreadId);
    return sendRequest("closeForumTopic", args).get<bool>("", false);
 }

bool Api::reopenForumTopic(boost::variant<std::int64_t, std::string> chatId,
                           std::int32_t messageThreadId) const {
    auto args = reopenForumTopicArgs(chatId, messageThreadId);
    return sendRequest("reopenForumTopic", args).get<bool>("", false);
}

bool Api::deleteForumTopic(boost::variant<std::int64_t, std::string> chatId,
                           std::int32_t messageThreadId) const {
    auto args = deleteForumTopicArgs(chatId, messageThreadId);
    return sendRequest("deleteForumTopic", args).get<bool>("", false);
}

bool Api::unpinAllForumTopicMessages(boost::variant<std::int64_t, std::string> chatId,
                                     std::int32_t messageThreadId) const {
    auto args = unpinAllForumTopicMessagesArgs(chatId, messageThreadId);
    return sendRequest("unpinAllForumTopicMessages", args).get<bool>("", false);
}

bool Api::editGeneralForumTopic(boost::variant<std::int64_t, std::string> chatId,
                                std::string name) const {
    auto args = editGeneralForumTopicArgs(chatId, name);
    return sendRequest("editGeneralForumTopic", args).get<bool>("", false);
}

bool Api::closeGeneralForumTopic(boost::variant<std::int64_t, std::string> chatId) const {
    auto args = closeGeneralForumTopicArgs(chatId);
    return sendRequest("closeGeneralForumTopic", args).get<bool>("", false);
}

bool Api::reopenGeneralForumTopic(boost::variant<std::int64_t, std::string> chatId) const {
    auto args = reopenGeneralForumTopicArgs(chatId);
    return sendRequest("reopenGeneralForumTopic", args).get<bool>("", false);
}

bool Api::hideGeneralForumTopic(boost::variant<std::int64_t, std::string> chatId) const {
    auto args = hideGeneralForumTopicArgs(chatId);
    return sendRequest("hideGeneralForumTopic", args).get<bool>("", false);
}

bool Api::unhideGeneralForumTopic(boost::variant<std::int64_t, std::string> chatId) const {
    auto args = unhideGeneralForumTopicArgs(chatId);
    return sendRequest("unhideGeneralForumTopic", args).get<bool>("", false);
}

bool Api::unpinAllGeneralForumTopicMessages(boost::variant<std::int64_t, std::string> chatId) const {
    auto args = unpinAllGeneralForumTopicMessagesArgs(chatId);
    return sendRequest("unpinAllGeneralForumTopicMessages", args).get<bool>("", false);
}

bool Api::answerCallbackQuery(const std::string& callbackQueryId,
                              const std::string& text,
                              bool showAlert,
                              const std::string& url,
                              std::int32_t cacheTime) const {
    auto args = answerCallbackQueryArgs(callbackQueryId, text, showAlert, url, cacheTime);
    return sendRequest("answerCallbackQuery", args).get<bool>("", false);
}

UserChatBoosts::Ptr Api::getUserChatBoosts(boost::variant<std::int64_t, std::string> chatId,
                                           std::int32_t userId) const {
    auto args = getUserChatBoostsArgs(chatId, userId);
    return _tgTypeParser.parseJsonAndGetUserChatBoosts(sendRequest("getUserChatBoosts", args));
}

BusinessConnection::Ptr Api::getBusinessConnection(const std::string& businessConnectionId) const {
    auto args = getBusinessConnectionArgs(businessConnectionId);
    return _tgTypeParser.parseJsonAndGetBusinessConnection(sendRequest("getBusinessConnection", args));
}

bool Api::setMyCommands(const std::vector<BotCommand::Ptr>& commands,
                        BotCommandScope::Ptr scope,
                        const std::string& languageCode) const {
    auto args = setMyCommandsArgs(commands, scope, languageCode);
    return sendRequest("setMyCommands", args).get<bool>("", false);
}

bool Api::deleteMyCommands(BotCommandScope::Ptr scope,
                           const std::string& languageCode) const {
    auto args = deleteMyCommandsArgs(scope, languageCode);
    return sendRequest("deleteMyCommands", args).get<bool>("", false);
}

std::vector<BotCommand::Ptr> Api::getMyCommands(BotCommandScope::Ptr scope,
                                                const std::string& languageCode) const {
    auto args = getMyCommandsArgs(scope, languageCode);
    return _tgTypeParser.parseJsonAndGetArray<BotCommand>(&TgTypeParser::parseJsonAndGetBotCommand, sendRequest("getMyCommands", args));
}

bool Api::setMyName(const std::string& name,
                    const std::string& languageCode) const {
    auto args = setMyNameArgs(name, languageCode);
    return sendRequest("setMyName", args).get<bool>("", false);
}

BotName::Ptr Api::getMyName(const std::string& languageCode) const {
    auto args = getMyNameArgs(languageCode);
    return _tgTypeParser.parseJsonAndGetBotName(sendRequest("getMyName", args));
}

bool Api::setMyDescription(const std::string& description,
                           const std::string& languageCode) const {
    auto args = setMyDescriptionArgs(description, languageCode);
    return sendRequest("setMyDescription", args).get<bool>("", false);
}

BotDescription::Ptr Api::getMyDescription(const std::string& languageCode) const {
    auto args = getMyDescriptionArgs(languageCode);
    return _tgTypeParser.parseJsonAndGetBotDescription(sendRequest("getMyDescription", args));
}

bool Api::setMyShortDescription(const std::string& shortDescription,
                                const std::string& languageCode) const {
    auto args = setMyShortDescriptionArgs(shortDescription, languageCode);
    return sendRequest("setMyShortDescription", args).get<bool>("", false);
}

BotShortDescription::Ptr Api::getMyShortDescription(const std::string& languageCode) const {
    auto args = getMyShortDescriptionArgs(languageCode);
    return _tgTypeParser.parseJsonAndGetBotShortDescription(sendRequest("getMyShortDescription", args));
}

bool Api::setChatMenuButton(std::int64_t chatId,
                            MenuButton::Ptr menuButton) const {
    auto args = setChatMenuButtonArgs(chatId, menuButton);
    return sendRequest("setChatMenuButton", args).get<bool>("", false);
}

MenuButton::Ptr Api::getChatMenuButton(std::int64_t chatId) const {
    auto args = getChatMenuButtonArgs(chatId);
    return _tgTypeParser.parseJsonAndGetMenuButton(sendRequest("getChatMenuButton", args));
}

bool Api::setMyDefaultAdministratorRights(ChatAdministratorRights::Ptr rights,
                                          bool forChannels) const {
    auto args = setMyDefaultAdministratorRightsArgs(rights, forChannels);
    return sendRequest("setMyDefaultAdministratorRights", args).get<bool>("", false);
}

ChatAdministratorRights::Ptr Api::getMyDefaultAdministratorRights(bool forChannels) const {
    auto args = getMyDefaultAdministratorRightsArgs(forChannels);
    return _tgTypeParser.parseJsonAndGetChatAdministratorRights(sendRequest("getMyDefaultAdministratorRights", args));
}

Message::Ptr Api::editMessageText(const std::string& text,
                                  boost::variant<std::int64_t, std::string> chatId,
                                  std::int32_t messageId,
                                  const std::string& inlineMessageId,
                                  const std::string& parseMode,
                                  LinkPreviewOptions::Ptr linkPreviewOptions,
                                  InlineKeyboardMarkup::Ptr replyMarkup,
                                  const std::vector<MessageEntity::Ptr>& entities) const {
    auto args = editMessageTextArgs(text, chatId, messageId, inlineMessageId, parseMode, linkPreviewOptions, replyMarkup, entities);
    boost::property_tree::ptree p = sendRequest("editMessageText", args);
    if (p.get_child_optional("message_id")) {
        return _tgTypeParser.parseJsonAndGetMessage(p);
    } else {
        return nullptr;
    }
}

Message::Ptr Api::editMessageCaption(boost::variant<std::int64_t, std::string> chatId,
                                     std::int32_t messageId,
                                     const std::string& caption,
                                     const std::string& inlineMessageId,
                                     GenericReply::Ptr replyMarkup,
                                     const std::string& parseMode,
                                     const std::vector<MessageEntity::Ptr>& captionEntities) const {
    auto args = editMessageCaptionArgs(chatId, messageId, caption, inlineMessageId, replyMarkup, parseMode, captionEntities);
    boost::property_tree::ptree p = sendRequest("editMessageCaption", args);
    if (p.get_child_optional("message_id")) {
        return _tgTypeParser.parseJsonAndGetMessage(p);
    } else {
        return nullptr;
    }
}

Message::Ptr Api::editMessageMedia(InputMedia::Ptr media,
                                   boost::variant<std::int64_t, std::string> chatId,
                                   std::int32_t messageId,
                                   const std::string& inlineMessageId,
                                   GenericReply::Ptr replyMarkup) const {
    auto args = editMessageMediaArgs(media, chatId, messageId, inlineMessageId, replyMarkup);
    boost::property_tree::ptree p = sendRequest("editMessageMedia", args);
    if (p.get_child_optional("message_id")) {
        return _tgTypeParser.parseJsonAndGetMessage(p);
    } else {
        return nullptr;
    }
}

Message::Ptr Api::editMessageReplyMarkup(boost::variant<std::int64_t, std::string> chatId,
                                         std::int32_t messageId,
                                         const std::string& inlineMessageId,
                                         const GenericReply::Ptr replyMarkup) const {
    auto args = editMessageReplyMarkupArgs(chatId, messageId, inlineMessageId, replyMarkup);
    boost::property_tree::ptree p = sendRequest("editMessageReplyMarkup", args);
    if (p.get_child_optional("message_id")) {
        return _tgTypeParser.parseJsonAndGetMessage(p);
    } else {
        return nullptr;
    }
}

Poll::Ptr Api::stopPoll(boost::variant<std::int64_t, std::string> chatId,
                        std::int64_t messageId,
                        const InlineKeyboardMarkup::Ptr replyMarkup) const {
    auto args = stopPollArgs(chatId, messageId, replyMarkup);
    return _tgTypeParser.parseJsonAndGetPoll(sendRequest("stopPoll", args));
}

bool Api::deleteMessage(boost::variant<std::int64_t, std::string> chatId,
                        std::int32_t messageId) const {
    auto args = deleteMessageArgs(chatId, messageId);
    return sendRequest("deleteMessage", args).get<bool>("", false);
}

bool Api::deleteMessages(boost::variant<std::int64_t, std::string> chatId,
                         const std::vector<std::int32_t>& messageIds) const {
    auto args = deleteMessagesArgs(chatId, messageIds);
    return sendRequest("deleteMessages", args).get<bool>("", false);
}

Message::Ptr Api::sendSticker(boost::variant<std::int64_t, std::string> chatId,
                              boost::variant<InputFile::Ptr, std::string> sticker,
                              ReplyParameters::Ptr replyParameters,
                              GenericReply::Ptr replyMarkup,
                              bool disableNotification,
                              std::int32_t messageThreadId,
                              bool protectContent,
                              const std::string& emoji,
                              const std::string& businessConnectionId) const {
    auto args = sendStickerArgs(chatId, sticker, replyParameters, replyMarkup, disableNotification,
                                messageThreadId, protectContent, emoji, businessConnectionId);
    return _tgTypeParser.parseJsonAndGetMessage(sendRequest("sendSticker", args));
}

StickerSet::Ptr Api::getStickerSet(const std::string& name) const {
    auto args = getStickerSetArgs(name);
    return _tgTypeParser.parseJsonAndGetStickerSet(sendRequest("getStickerSet", args));
}

std::vector<Sticker::Ptr> Api::getCustomEmojiStickers(const std::vector<std::string>& customEmojiIds) const {
    auto args = getCustomEmojiStickersArgs(customEmojiIds);
    return _tgTypeParser.parseJsonAndGetArray<Sticker>(&TgTypeParser::parseJsonAndGetSticker, sendRequest("getCustomEmojiStickers", args));
}

File::Ptr Api::uploadStickerFile(std::int64_t userId,
                                 InputFile::Ptr sticker,
                                 const std::string& stickerFormat) const {
    auto args = uploadStickerFileArgs(userId, sticker, stickerFormat);
    return _tgTypeParser.parseJsonAndGetFile(sendRequest("uploadStickerFile", args));
}

bool Api::createNewStickerSet(std::int64_t userId,
                              const std::string& name,
                              const std::string& title,
                              const std::vector<InputSticker::Ptr>& stickers,
                              Sticker::Type stickerType,
                              bool needsRepainting) const {
    auto args = createNewStickerSetArgs(userId, name, title, stickers, stickerType, needsRepainting);
    return sendRequest("createNewStickerSet", args).get<bool>("", false);
}

bool Api::addStickerToSet(std::int64_t userId,
                          const std::string& name,
                          InputSticker::Ptr sticker) const {
    auto args = addStickerToSetArgs(userId, name, sticker);
    return sendRequest("addStickerToSet", args).get<bool>("", false);
}

bool Api::setStickerPositionInSet(const std::string& sticker,
                                  std::int32_t position) const {
    auto args = setStickerPositionInSetArgs(sticker, position);
    return sendRequest("setStickerPositionInSet", args).get<bool>("", false);
}

bool Api::deleteStickerFromSet(const std::string& sticker) const {
    auto args = deleteStickerFromSetArgs(sticker);
    return sendRequest("deleteStickerFromSet", args).get<bool>("", false);
}

bool Api::replaceStickerInSet(std::int64_t userId,
                             const std::string& name,
                             const std::string& oldSticker,
                             InputSticker::Ptr sticker) const {
    auto args = replaceStickerInSetArgs(userId, name, oldSticker, sticker);
    return sendRequest("replaceStickerInSet", args).get<bool>("", false);
}

bool Api::setStickerEmojiList(const std::string& sticker,
                              const std::vector<std::string>& emojiList) const {
    auto args = setStickerEmojiListArgs(sticker, emojiList);
    return sendRequest("setStickerEmojiList", args).get<bool>("", false);
}

bool Api::setStickerKeywords(const std::string& sticker,
                             const std::vector<std::string>& keywords) const {
    auto args = setStickerKeywordsArgs(sticker, keywords);
    return sendRequest("setStickerKeywords", args).get<bool>("", false);
}

bool Api::setStickerMaskPosition(const std::string& sticker,
                                 MaskPosition::Ptr maskPosition) const {
    auto args = setStickerMaskPositionArgs(sticker, maskPosition);
    return sendRequest("setStickerMaskPosition", args).get<bool>("", false);
}

bool Api::setStickerSetTitle(const std::string& name,
                             const std::string& title) const {
    auto args = setStickerSetTitleArgs(name, title);
    return sendRequest("setStickerSetTitle", args).get<bool>("", false);
}

bool Api::setStickerSetThumbnail(const std::string& name,
                                 std::int64_t userId,
                                 const std::string& format,
                                 boost::variant<InputFile::Ptr, std::string> thumbnail) const {
    auto args = setStickerSetThumbnailArgs(name, userId, format, thumbnail);
    return sendRequest("setStickerSetThumbnail", args).get<bool>("", false);
}

bool Api::setCustomEmojiStickerSetThumbnail(const std::string& name,
                                            const std::string& customEmojiId) const {
    auto args = setCustomEmojiStickerSetThumbnailArgs(name, customEmojiId);
    return sendRequest("setCustomEmojiStickerSetThumbnail", args).get<bool>("", false);
}

bool Api::deleteStickerSet(const std::string& name) const {
    auto args = deleteStickerSetArgs(name);
    return sendRequest("deleteStickerSet", args).get<bool>("", false);
}

bool Api::answerInlineQuery(const std::string& inlineQueryId,
                            const std::vector<InlineQueryResult::Ptr>& results,
                            std::int32_t cacheTime,
                            bool isPersonal,
                            const std::string& nextOffset,
                            InlineQueryResultsButton::Ptr button) const {
    auto args = answerInlineQueryArgs(inlineQueryId, results, cacheTime, isPersonal, nextOffset, button);
    return sendRequest("answerInlineQuery", args).get<bool>("", false);
}

SentWebAppMessage::Ptr Api::answerWebAppQuery(const std::string& webAppQueryId,
                                              InlineQueryResult::Ptr result) const {
    auto args = answerWebAppQueryArgs(webAppQueryId, result);
    return _tgTypeParser.parseJsonAndGetSentWebAppMessage(sendRequest("answerWebAppQuery", args));
}

Message::Ptr Api::sendInvoice(boost::variant<std::int64_t, std::string> chatId,
                              const std::string& title,
                              const std::string& description,
                              const std::string& payload,
                              const std::string& providerToken,
                              const std::string& currency,
                              const std::vector<LabeledPrice::Ptr>& prices,
                              const std::string& providerData,
                              const std::string& photoUrl,
                              std::int32_t photoSize,
                              std::int32_t photoWidth,
                              std::int32_t photoHeight,
                              bool needName,
                              bool needPhoneNumber,
                              bool needEmail,
                              bool needShippingAddress,
                              bool sendPhoneNumberToProvider,
                              bool sendEmailToProvider,
                              bool isFlexible,
                              ReplyParameters::Ptr replyParameters,
                              GenericReply::Ptr replyMarkup,
                              bool disableNotification,
                              std::int32_t messageThreadId,
                              std::int32_t maxTipAmount,
                              const std::vector<std::int32_t>& suggestedTipAmounts,
                              const std::string& startParameter,
                              bool protectContent) const {
    auto args = sendInvoiceArgs(chatId, title, description, payload, providerToken, currency, prices,
                                providerData, photoUrl, photoSize, photoWidth, photoHeight, needName,
                                needPhoneNumber, needEmail, needShippingAddress, sendPhoneNumberToProvider,
                                sendEmailToProvider, isFlexible, replyParameters, replyMarkup, disableNotification,
                                messageThreadId, maxTipAmount, suggestedTipAmounts, startParameter, protectContent);
    return _tgTypeParser.parseJsonAndGetMessage(sendRequest("sendInvoice", args));
}

std::string Api::createInvoiceLink(const std::string& title,
                                   const std::string& description,
                                   const std::string& payload,
                                   const std::string& providerToken,
                                   const std::string& currency,
                                   const std::vector<LabeledPrice::Ptr>& prices,
                                   std::int32_t maxTipAmount,
                                   const std::vector<std::int32_t>& suggestedTipAmounts,
                                   const std::string& providerData,
                                   const std::string& photoUrl,
                                   std::int32_t photoSize,
                                   std::int32_t photoWidth,
                                   std::int32_t photoHeight,
                                   bool needName,
                                   bool needPhoneNumber,
                                   bool needEmail,
                                   bool needShippingAddress,
                                   bool sendPhoneNumberToProvider,
                                   bool sendEmailToProvider,
                                   bool isFlexible) const {
    auto args = createInvoiceLinkArgs(title, description, payload, providerToken, currency, prices,
                                      maxTipAmount, suggestedTipAmounts, providerData, photoUrl,
                                      photoSize, photoWidth, photoHeight, needName, needPhoneNumber,
                                      needEmail, needShippingAddress, sendPhoneNumberToProvider,
                                      sendEmailToProvider, isFlexible);
    return sendRequest("createInvoiceLink", args).get<std::string>("", "");
}

bool Api::answerShippingQuery(const std::string& shippingQueryId,
                              bool ok,
                              const std::vector<ShippingOption::Ptr>& shippingOptions,
                              const std::string& errorMessage) const {
    auto args = answerShippingQueryArgs(shippingQueryId, ok, shippingOptions, errorMessage);
    return sendRequest("answerShippingQuery", args).get<bool>("", false);
}

bool Api::answerPreCheckoutQuery(const std::string& preCheckoutQueryId,
                                 bool ok,
                                 const std::string& errorMessage) const {
    auto args = answerPreCheckoutQueryArgs(preCheckoutQueryId, ok, errorMessage);
    return sendRequest("answerPreCheckoutQuery", args).get<bool>("", false);
}

bool Api::setPassportDataErrors(std::int64_t userId,
                                const std::vector<PassportElementError::Ptr>& errors) const {
    auto args = setPassportDataErrorsArgs(userId, errors);
    return sendRequest("setPassportDataErrors", args).get<bool>("", false);
}

Message::Ptr Api::sendGame(std::int64_t chatId,
                           const std::string& gameShortName,
                           ReplyParameters::Ptr replyParameters,
                           InlineKeyboardMarkup::Ptr replyMarkup,
                           bool disableNotification,
                           std::int32_t messageThreadId,
                           bool protectContent,
                           const std::string& businessConnectionId) const {
    auto args = sendGameArgs(chatId, gameShortName, replyParameters, replyMarkup, disableNotification,
                             messageThreadId, protectContent, businessConnectionId);
    return _tgTypeParser.parseJsonAndGetMessage(sendRequest("sendGame", args));
}

Message::Ptr Api::setGameScore(std::int64_t userId,
                               std::int32_t score,
                               bool force,
                               bool disableEditMessage,
                               std::int64_t chatId,
                               std::int32_t messageId,
                               const std::string& inlineMessageId) const {
    auto args = setGameScoreArgs(userId, score, force, disableEditMessage, chatId, messageId, inlineMessageId);
    return _tgTypeParser.parseJsonAndGetMessage(sendRequest("setGameScore", args));
}

std::vector<GameHighScore::Ptr> Api::getGameHighScores(std::int64_t userId,
                                                  std::int64_t chatId,
                                                  std::int32_t messageId,
                                                  const std::string& inlineMessageId) const {
    auto args = getGameHighScoresArgs(userId, chatId, messageId, inlineMessageId);
    return _tgTypeParser.parseJsonAndGetArray<GameHighScore>(&TgTypeParser::parseJsonAndGetGameHighScore, sendRequest("getGameHighScores", args));
}

std::string Api::downloadFile(const std::string& filePath,
                              const std::vector<HttpReqArg>& args) const {
    std::string url(_url);
    url += "/file/bot";
    url += _token;
    url += "/";
    url += filePath;

    return _httpClient.makeRequest(url, args);
}

bool Api::blockedByUser(std::int64_t chatId) const {
    bool isBotBlocked = false;

    try {
        sendChatAction(chatId, "typing");

    } catch (std::exception& e) {
        std::string error = e.what();

        if (error.compare("Forbidden: bot was blocked by the user") == 0) {
            isBotBlocked = true;
        }
    }

    return isBotBlocked;
}

boost::property_tree::ptree Api::sendRequest(const std::string& method, const std::vector<HttpReqArg>& args) const {
    std::string url(_url);
    url += "/bot";
    url += _token;
    url += "/";
    url += method;

    int retries = 0;
    while (1)
    {
        try {
            std::string serverResponse = _httpClient.makeRequest(url, args);
            
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
                return result.get_child("result");
            } else {
                std::string message = result.get("description", "");
                size_t errorCode = result.get<size_t>("error_code", 0u);

                throw TgException(message, static_cast<TgException::ErrorCode>(errorCode));
            }
        } catch (...) {
            if ((requestMaxRetries >= 0) && (retries == requestMaxRetries)) {
                throw;
            } else {
                std::this_thread::sleep_for(std::chrono::seconds(requestBackoff));
                retries++;
                continue;
            }
        }
    }
}
}
