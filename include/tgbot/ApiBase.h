#ifndef TGBOT_API_BASE_H
#define TGBOT_API_BASE_H

#include "tgbot/TgException.h"
#include "tgbot/TgTypeParser.h"
#include "tgbot/net/HttpClient.h"
#include "tgbot/net/HttpReqArg.h"
#include "tgbot/tools/StringTools.h"
#include "tgbot/types/User.h"
#include "tgbot/types/Message.h"
#include "tgbot/types/MessageId.h"
#include "tgbot/types/GenericReply.h"
#include "tgbot/types/InputFile.h"
#include "tgbot/types/UserProfilePhotos.h"
#include "tgbot/types/Update.h"
#include "tgbot/types/InlineQueryResult.h"
#include "tgbot/types/Venue.h"
#include "tgbot/types/WebhookInfo.h"
#include "tgbot/types/ChatMember.h"
#include "tgbot/types/Sticker.h"
#include "tgbot/types/StickerSet.h"
#include "tgbot/types/File.h"
#include "tgbot/types/InputMedia.h"
#include "tgbot/types/GameHighScore.h"
#include "tgbot/types/SentWebAppMessage.h"
#include "tgbot/types/LabeledPrice.h"
#include "tgbot/types/ShippingOption.h"
#include "tgbot/types/BotCommand.h"
#include "tgbot/types/ForumTopic.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/variant.hpp>

#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace TgBot {

class Bot;

class TGBOT_API ApiBase {

friend class Bot;

protected:
    ApiBase(std::string token, const std::string& url);

    typedef std::shared_ptr<std::vector<std::string>> StringArrayPtr;

    std::vector<HttpReqArg> getUpdatesArgs(std::int32_t offset,
                                         std::int32_t limit,
                                         std::int32_t timeout,
                                         const StringArrayPtr& allowedUpdates) const;
                            
    std::vector<HttpReqArg> setWebhookArgs(const std::string& url,
                     InputFile::Ptr certificate,
                     std::int32_t maxConnections,
                     const StringArrayPtr& allowedUpdates,
                     const std::string& ipAddress,
                     bool dropPendingUpdates,
                     const std::string& secretToken) const;

    std::vector<HttpReqArg> deleteWebhookArgs(bool dropPendingUpdates) const;

    std::vector<HttpReqArg> sendMessageArgs(boost::variant<std::int64_t, std::string> chatId,
                                const std::string& text,
                                LinkPreviewOptions::Ptr linkPreviewOptions,
                                ReplyParameters::Ptr replyParameters,
                                GenericReply::Ptr replyMarkup,
                                const std::string& parseMode,
                                bool disableNotification,
                                const std::vector<MessageEntity::Ptr>& entities,
                                std::int32_t messageThreadId,
                                bool protectContent,
                                const std::string& businessConnectionId) const;

    std::vector<HttpReqArg> forwardMessageArgs(boost::variant<std::int64_t, std::string> chatId,
                                 boost::variant<std::int64_t, std::string> fromChatId,
                                 std::int32_t messageId,
                                 bool disableNotification,
                                 bool protectContent,
                                 std::int32_t messageThreadId) const;

    std::vector<HttpReqArg> forwardMessagesArgs(boost::variant<std::int64_t, std::string> chatId,
                                                 boost::variant<std::int64_t, std::string> fromChatId,
                                                 const std::vector<std::int32_t>& messageIds,
                                                 std::int32_t messageThreadId,
                                                 bool disableNotification,
                                                 bool protectContent) const;

    std::vector<HttpReqArg> copyMessageArgs(boost::variant<std::int64_t, std::string> chatId,
                                boost::variant<std::int64_t, std::string> fromChatId,
                                std::int32_t messageId,
                                const std::string& caption,
                                const std::string& parseMode,
                                const std::vector<MessageEntity::Ptr>& captionEntities,
                                bool disableNotification,
                                ReplyParameters::Ptr replyParameters,
                                GenericReply::Ptr replyMarkup,
                                bool protectContent,
                                std::int32_t messageThreadId) const;

    std::vector<HttpReqArg> copyMessagesArgs(boost::variant<std::int64_t, std::string> chatId,
                                              boost::variant<std::int64_t, std::string> fromChatId,
                                              const std::vector<std::int32_t>& messageIds,
                                              std::int32_t messageThreadId,
                                              bool disableNotification,
                                              bool protectContent,
                                              bool removeCaption) const;

    std::vector<HttpReqArg> sendPhotoArgs(boost::variant<std::int64_t, std::string> chatId,
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
                            const std::string& businessConnectionId) const;

    std::vector<HttpReqArg> sendAudioArgs(boost::variant<std::int64_t, std::string> chatId,
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
                            const std::string& businessConnectionId) const;

    std::vector<HttpReqArg> sendDocumentArgs(boost::variant<std::int64_t, std::string> chatId,
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
                               const std::string& businessConnectionId) const;

    std::vector<HttpReqArg> sendVideoArgs(boost::variant<std::int64_t, std::string> chatId,
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
                            const std::string& businessConnectionId) const;

    std::vector<HttpReqArg> sendAnimationArgs(boost::variant<std::int64_t, std::string> chatId,
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
                                const std::string& businessConnectionId) const;

    std::vector<HttpReqArg> sendVoiceArgs(boost::variant<std::int64_t, std::string> chatId,
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
                            const std::string& businessConnectionId) const;

    std::vector<HttpReqArg> sendVideoNoteArgs(boost::variant<std::int64_t, std::string> chatId,
                                boost::variant<InputFile::Ptr, std::string> videoNote,
                                ReplyParameters::Ptr replyParameters,
                                bool disableNotification,
                                std::int32_t duration,
                                std::int32_t length,
                                boost::variant<InputFile::Ptr, std::string> thumbnail,
                                GenericReply::Ptr replyMarkup,
                                std::int32_t messageThreadId,
                                bool protectContent,
                                const std::string& businessConnectionId) const;

    std::vector<HttpReqArg> sendMediaGroupArgs(boost::variant<std::int64_t, std::string> chatId,
                                              const std::vector<InputMedia::Ptr>& media,
                                              bool disableNotification,
                                              ReplyParameters::Ptr replyParameters,
                                              std::int32_t messageThreadId,
                                              bool protectContent,
                                              const std::string& businessConnectionId) const;

    std::vector<HttpReqArg> sendLocationArgs(boost::variant<std::int64_t, std::string> chatId,
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
                               const std::string& businessConnectionId) const;

    std::vector<HttpReqArg> editMessageLiveLocationArgs(float latitude,
                                          float longitude,
                                          boost::variant<std::int64_t, std::string> chatId,
                                          std::int32_t messageId,
                                          const std::string& inlineMessageId,
                                          InlineKeyboardMarkup::Ptr replyMarkup,
                                          float horizontalAccuracy,
                                          std::int32_t heading,
                                          std::int32_t proximityAlertRadius) const;

    std::vector<HttpReqArg> stopMessageLiveLocationArgs(boost::variant<std::int64_t, std::string> chatId,
                                          std::int32_t messageId,
                                          const std::string& inlineMessageId,
                                          InlineKeyboardMarkup::Ptr replyMarkup) const;

    std::vector<HttpReqArg> sendVenueArgs(boost::variant<std::int64_t, std::string> chatId,
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
                            const std::string& businessConnectionId) const;

    std::vector<HttpReqArg> sendContactArgs(boost::variant<std::int64_t, std::string> chatId,
                              const std::string& phoneNumber,
                              const std::string& firstName,
                              const std::string& lastName ,
                              const std::string& vcard,
                              bool disableNotification,
                              ReplyParameters::Ptr replyParameters,
                              GenericReply::Ptr replyMarkup,
                              std::int32_t messageThreadId,
                              bool protectContent,
                              const std::string& businessConnectionId) const;

    std::vector<HttpReqArg> sendPollArgs(boost::variant<std::int64_t, std::string> chatId,
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
                           const std::string& businessConnectionId) const;

    std::vector<HttpReqArg> sendDiceArgs(boost::variant<std::int64_t, std::string> chatId,
                           bool disableNotification,
                           ReplyParameters::Ptr replyParameters,
                           GenericReply::Ptr replyMarkup,
                           const std::string& emoji,
                           std::int32_t messageThreadId,
                           bool protectContent,
                           const std::string& businessConnectionId) const;

    std::vector<HttpReqArg> setMessageReactionArgs(boost::variant<std::int64_t, std::string> chatId,
                             std::int32_t messageId,
                             const std::vector<ReactionType::Ptr>& reaction,
                             bool isBig) const;

    std::vector<HttpReqArg> sendChatActionArgs(std::int64_t chatId,
                         const std::string& action,
                         std::int32_t messageThreadId,
                         const std::string& businessConnectionId) const;

    std::vector<HttpReqArg> getUserProfilePhotosArgs(std::int64_t userId,
                                                 std::int32_t offset,
                                                 std::int32_t limit) const;

    std::vector<HttpReqArg> getFileArgs(const std::string& fileId) const;

    std::vector<HttpReqArg> banChatMemberArgs(boost::variant<std::int64_t, std::string> chatId,
                        std::int64_t userId,
                        std::int32_t untilDate,
                        bool revokeMessages) const;

    std::vector<HttpReqArg> unbanChatMemberArgs(boost::variant<std::int64_t, std::string> chatId,
                          std::int64_t userId,
                          bool onlyIfBanned) const;

    std::vector<HttpReqArg> restrictChatMemberArgs(boost::variant<std::int64_t, std::string> chatId,
                             std::int64_t userId,
                             TgBot::ChatPermissions::Ptr permissions,
                             std::uint32_t untilDate,
                             bool useIndependentChatPermissions) const;

    std::vector<HttpReqArg> promoteChatMemberArgs(boost::variant<std::int64_t, std::string> chatId,
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
                            bool canDeleteStories) const;
    
    std::vector<HttpReqArg> setChatAdministratorCustomTitleArgs(boost::variant<std::int64_t, std::string> chatId,
                                          std::int64_t userId,
                                          const std::string& customTitle) const;

    std::vector<HttpReqArg> banChatSenderChatArgs(boost::variant<std::int64_t, std::string> chatId,
                            std::int64_t senderChatId) const;

    std::vector<HttpReqArg> unbanChatSenderChatArgs(boost::variant<std::int64_t, std::string> chatId,
                              std::int64_t senderChatId) const;

    std::vector<HttpReqArg> setChatPermissionsArgs(boost::variant<std::int64_t, std::string> chatId,
                             ChatPermissions::Ptr permissions,
                             bool useIndependentChatPermissions) const;

    std::vector<HttpReqArg> exportChatInviteLinkArgs(boost::variant<std::int64_t, std::string> chatId) const;

    std::vector<HttpReqArg> createChatInviteLinkArgs(boost::variant<std::int64_t, std::string> chatId,
                                              std::int32_t expireDate,
                                              std::int32_t memberLimit,
                                              const std::string& name,
                                              bool createsJoinRequest) const;

    std::vector<HttpReqArg> editChatInviteLinkArgs(boost::variant<std::int64_t, std::string> chatId,
                                            const std::string& inviteLink,
                                            std::int32_t expireDate,
                                            std::int32_t memberLimit,
                                            const std::string& name,
                                            bool createsJoinRequest) const;

    std::vector<HttpReqArg> revokeChatInviteLinkArgs(boost::variant<std::int64_t, std::string> chatId,
                                              const std::string& inviteLink) const;

    std::vector<HttpReqArg> approveChatJoinRequestArgs(boost::variant<std::int64_t, std::string> chatId,
                                 std::int64_t userId) const;

    std::vector<HttpReqArg> declineChatJoinRequestArgs(boost::variant<std::int64_t, std::string> chatId,
                                 std::int64_t userId) const;

    std::vector<HttpReqArg> setChatPhotoArgs(boost::variant<std::int64_t, std::string> chatId,
                        const InputFile::Ptr photo) const;

    std::vector<HttpReqArg> deleteChatPhotoArgs(boost::variant<std::int64_t, std::string> chatId) const;

    std::vector<HttpReqArg> setChatTitleArgs(boost::variant<std::int64_t, std::string> chatId,
                        const std::string& title) const;

    std::vector<HttpReqArg> setChatDescriptionArgs(boost::variant<std::int64_t, std::string> chatId,
                                const std::string& description) const;

    std::vector<HttpReqArg> pinChatMessageArgs(boost::variant<std::int64_t, std::string> chatId,
                            std::int32_t messageId,
                            bool disableNotification) const;

    std::vector<HttpReqArg> unpinChatMessageArgs(boost::variant<std::int64_t, std::string> chatId,
                            std::int32_t messageId) const;

    std::vector<HttpReqArg> unpinAllChatMessagesArgs(boost::variant<std::int64_t, std::string> chatId) const;

    std::vector<HttpReqArg> leaveChatArgs(boost::variant<std::int64_t, std::string> chatId) const;

    std::vector<HttpReqArg> getChatArgs(boost::variant<std::int64_t, std::string> chatId) const;

    std::vector<HttpReqArg> getChatAdministratorsArgs(boost::variant<std::int64_t, std::string> chatId) const;

    std::vector<HttpReqArg> getChatMemberCountArgs(boost::variant<std::int64_t, std::string> chatId) const;

    std::vector<HttpReqArg> getChatMemberArgs(boost::variant<std::int64_t, std::string> chatId,
                                    std::int64_t userId) const;

    std::vector<HttpReqArg> setChatStickerSetArgs(boost::variant<std::int64_t, std::string> chatId,
                                const std::string& stickerSetName) const;

    std::vector<HttpReqArg> deleteChatStickerSetArgs(boost::variant<std::int64_t, std::string> chatId) const;

    std::vector<HttpReqArg> createForumTopicArgs(boost::variant<std::int64_t, std::string> chatId,
                                        const std::string& name,
                                        std::int32_t iconColor,
                                        const std::string& iconCustomEmojiId) const;

    std::vector<HttpReqArg> editForumTopicArgs(boost::variant<std::int64_t, std::string> chatId,
                            std::int32_t messageThreadId,
                            const std::string& name,
                            boost::variant<std::int32_t, std::string> iconCustomEmojiId) const;

    std::vector<HttpReqArg> closeForumTopicArgs(boost::variant<std::int64_t, std::string> chatId,
                            std::int32_t messageThreadId) const;

    std::vector<HttpReqArg> reopenForumTopicArgs(boost::variant<std::int64_t, std::string> chatId,
                            std::int32_t messageThreadId) const;

    std::vector<HttpReqArg> deleteForumTopicArgs(boost::variant<std::int64_t, std::string> chatId,
                            std::int32_t messageThreadId) const;

    std::vector<HttpReqArg> unpinAllForumTopicMessagesArgs(boost::variant<std::int64_t, std::string> chatId,
                                        std::int32_t messageThreadId) const;

    std::vector<HttpReqArg> editGeneralForumTopicArgs(boost::variant<std::int64_t, std::string> chatId,
                                    std::string name) const;

    std::vector<HttpReqArg> closeGeneralForumTopicArgs(boost::variant<std::int64_t, std::string> chatId) const;

    std::vector<HttpReqArg> reopenGeneralForumTopicArgs(boost::variant<std::int64_t, std::string> chatId) const;

    std::vector<HttpReqArg> hideGeneralForumTopicArgs(boost::variant<std::int64_t, std::string> chatId) const;

    std::vector<HttpReqArg> unhideGeneralForumTopicArgs(boost::variant<std::int64_t, std::string> chatId) const;

    std::vector<HttpReqArg> unpinAllGeneralForumTopicMessagesArgs(boost::variant<std::int64_t, std::string> chatId) const;

    std::vector<HttpReqArg> answerCallbackQueryArgs(const std::string& callbackQueryId,
                                const std::string& text,
                                bool showAlert,
                                const std::string& url,
                                std::int32_t cacheTime) const;

    std::vector<HttpReqArg> getUserChatBoostsArgs(boost::variant<std::int64_t, std::string> chatId,
                                            std::int32_t userId) const;

    std::vector<HttpReqArg> getBusinessConnectionArgs(const std::string& businessConnectionId) const;

    std::vector<HttpReqArg> setMyCommandsArgs(const std::vector<BotCommand::Ptr>& commands,
                            BotCommandScope::Ptr scope,
                            const std::string& languageCode) const;

    std::vector<HttpReqArg> deleteMyCommandsArgs(BotCommandScope::Ptr scope,
                            const std::string& languageCode) const;

    std::vector<HttpReqArg> getMyCommandsArgs(BotCommandScope::Ptr scope,
                                                    const std::string& languageCode) const;

    std::vector<HttpReqArg> setMyNameArgs(const std::string& name,
                        const std::string& languageCode) const;

    std::vector<HttpReqArg> getMyNameArgs(const std::string& languageCode) const;

    std::vector<HttpReqArg> setMyDescriptionArgs(const std::string& description,
                            const std::string& languageCode) const;

    std::vector<HttpReqArg> getMyDescriptionArgs(const std::string& languageCode) const;

    std::vector<HttpReqArg> setMyShortDescriptionArgs(const std::string& shortDescription,
                                    const std::string& languageCode) const;

    std::vector<HttpReqArg> getMyShortDescriptionArgs(const std::string& languageCode) const;

    std::vector<HttpReqArg> setChatMenuButtonArgs(std::int64_t chatId,
                                MenuButton::Ptr menuButton) const;
        
    std::vector<HttpReqArg> getChatMenuButtonArgs(std::int64_t chatId) const;

    std::vector<HttpReqArg> setMyDefaultAdministratorRightsArgs(ChatAdministratorRights::Ptr rights,
                                            bool forChannels) const;

    std::vector<HttpReqArg> getMyDefaultAdministratorRightsArgs(bool forChannels) const;

    std::vector<HttpReqArg> editMessageTextArgs(const std::string& text,
                                    boost::variant<std::int64_t, std::string> chatId,
                                    std::int32_t messageId,
                                    const std::string& inlineMessageId,
                                    const std::string& parseMode,
                                    LinkPreviewOptions::Ptr linkPreviewOptions,
                                    InlineKeyboardMarkup::Ptr replyMarkup,
                                    const std::vector<MessageEntity::Ptr>& entities) const;

    std::vector<HttpReqArg> editMessageCaptionArgs(boost::variant<std::int64_t, std::string> chatId,
                                        std::int32_t messageId,
                                        const std::string& caption,
                                        const std::string& inlineMessageId,
                                        GenericReply::Ptr replyMarkup,
                                        const std::string& parseMode,
                                        const std::vector<MessageEntity::Ptr>& captionEntities) const;

    std::vector<HttpReqArg> editMessageMediaArgs(InputMedia::Ptr media,
                                    boost::variant<std::int64_t, std::string> chatId,
                                    std::int32_t messageId,
                                    const std::string& inlineMessageId,
                                    GenericReply::Ptr replyMarkup) const;

    std::vector<HttpReqArg> editMessageReplyMarkupArgs(boost::variant<std::int64_t, std::string> chatId,
                                            std::int32_t messageId,
                                            const std::string& inlineMessageId,
                                            const GenericReply::Ptr replyMarkup) const;

    std::vector<HttpReqArg> stopPollArgs(boost::variant<std::int64_t, std::string> chatId,
                            std::int64_t messageId,
                            const InlineKeyboardMarkup::Ptr replyMarkup) const;

    std::vector<HttpReqArg> deleteMessageArgs(boost::variant<std::int64_t, std::string> chatId,
                            std::int32_t messageId) const;

    std::vector<HttpReqArg> deleteMessagesArgs(boost::variant<std::int64_t, std::string> chatId,
                            const std::vector<std::int32_t>& messageIds) const;

    std::vector<HttpReqArg> sendStickerArgs(boost::variant<std::int64_t, std::string> chatId,
                                boost::variant<InputFile::Ptr, std::string> sticker,
                                ReplyParameters::Ptr replyParameters,
                                GenericReply::Ptr replyMarkup,
                                bool disableNotification,
                                std::int32_t messageThreadId,
                                bool protectContent,
                                const std::string& emoji,
                                const std::string& businessConnectionId) const;

    std::vector<HttpReqArg> getStickerSetArgs(const std::string& name) const;

    std::vector<HttpReqArg> getCustomEmojiStickersArgs(const std::vector<std::string>& customEmojiIds) const;

    std::vector<HttpReqArg> uploadStickerFileArgs(std::int64_t userId,
                                    InputFile::Ptr sticker,
                                    const std::string& stickerFormat) const;

    std::vector<HttpReqArg> createNewStickerSetArgs(std::int64_t userId,
                                const std::string& name,
                                const std::string& title,
                                const std::vector<InputSticker::Ptr>& stickers,
                                Sticker::Type stickerType,
                                bool needsRepainting) const;

    std::vector<HttpReqArg> addStickerToSetArgs(std::int64_t userId,
                            const std::string& name,
                            InputSticker::Ptr sticker) const;

    std::vector<HttpReqArg> setStickerPositionInSetArgs(const std::string& sticker,
                                    std::int32_t position) const;

    std::vector<HttpReqArg> deleteStickerFromSetArgs(const std::string& sticker) const;

    std::vector<HttpReqArg> replaceStickerInSetArgs(std::int64_t userId,
                                const std::string& name,
                                const std::string& oldSticker,
                                InputSticker::Ptr sticker) const;

    std::vector<HttpReqArg> setStickerEmojiListArgs(const std::string& sticker,
                                const std::vector<std::string>& emojiList) const;

    std::vector<HttpReqArg> setStickerKeywordsArgs(const std::string& sticker,
                                const std::vector<std::string>& keywords) const;

    std::vector<HttpReqArg> setStickerMaskPositionArgs(const std::string& sticker,
                                    MaskPosition::Ptr maskPosition) const;

    std::vector<HttpReqArg> setStickerSetTitleArgs(const std::string& name,
                                const std::string& title) const;

    std::vector<HttpReqArg> setStickerSetThumbnailArgs(const std::string& name,
                                    std::int64_t userId,
                                    const std::string& format,
                                    boost::variant<InputFile::Ptr, std::string> thumbnail) const;

    std::vector<HttpReqArg> setCustomEmojiStickerSetThumbnailArgs(const std::string& name,
                                                const std::string& customEmojiId) const;

    std::vector<HttpReqArg> deleteStickerSetArgs(const std::string& name) const;

    std::vector<HttpReqArg> answerInlineQueryArgs(const std::string& inlineQueryId,
                                const std::vector<InlineQueryResult::Ptr>& results,
                                std::int32_t cacheTime,
                                bool isPersonal,
                                const std::string& nextOffset,
                                InlineQueryResultsButton::Ptr button) const;

    std::vector<HttpReqArg> answerWebAppQueryArgs(const std::string& webAppQueryId,
                                                InlineQueryResult::Ptr result) const;

    std::vector<HttpReqArg> sendInvoiceArgs(boost::variant<std::int64_t, std::string> chatId,
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
                                bool protectContent) const;

    std::vector<HttpReqArg> createInvoiceLinkArgs(const std::string& title,
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
                                    bool isFlexible) const;

    std::vector<HttpReqArg> answerShippingQueryArgs(const std::string& shippingQueryId,
                                bool ok,
                                const std::vector<ShippingOption::Ptr>& shippingOptions,
                                const std::string& errorMessage) const;

    std::vector<HttpReqArg> answerPreCheckoutQueryArgs(const std::string& preCheckoutQueryId,
                                    bool ok,
                                    const std::string& errorMessage) const;

    std::vector<HttpReqArg> setPassportDataErrorsArgs(std::int64_t userId,
                                    const std::vector<PassportElementError::Ptr>& errors) const;

    std::vector<HttpReqArg> sendGameArgs(std::int64_t chatId,
                            const std::string& gameShortName,
                            ReplyParameters::Ptr replyParameters,
                            InlineKeyboardMarkup::Ptr replyMarkup,
                            bool disableNotification,
                            std::int32_t messageThreadId,
                            bool protectContent,
                            const std::string& businessConnectionId) const;

    std::vector<HttpReqArg> setGameScoreArgs(std::int64_t userId,
                                std::int32_t score,
                                bool force,
                                bool disableEditMessage,
                                std::int64_t chatId,
                                std::int32_t messageId,
                                const std::string& inlineMessageId) const;

    std::vector<HttpReqArg> getGameHighScoresArgs(std::int64_t userId,
                                                    std::int64_t chatId,
                                                    std::int32_t messageId,
                                                    const std::string& inlineMessageId) const;

    const std::string _token;
    const TgTypeParser _tgTypeParser;
    const std::string _url;
};

}

#endif //TGBOT_API_BASE_H
