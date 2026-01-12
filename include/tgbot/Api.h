#ifndef TGBOT_API_H
#define TGBOT_API_H

#include "tgbot/ApiBase.h"

namespace TgBot {

class Api: public ApiBase {
public:
    Api(std::string token, const HttpClient& httpClient, const std::string& url);
    
    std::vector<Update::Ptr> getUpdates(std::int32_t offset = 0,
                                        std::int32_t limit = 100,
                                        std::int32_t timeout = 0,
                                        const StringArrayPtr& allowedUpdates = nullptr) const;

    bool setWebhook(const std::string& url,
                    InputFile::Ptr certificate = nullptr,
                    std::int32_t maxConnections = 40,
                    const StringArrayPtr& allowedUpdates = nullptr,
                    const std::string& ipAddress = "",
                    bool dropPendingUpdates = false,
                    const std::string& secretToken = "") const;

    bool deleteWebhook(bool dropPendingUpdates = false) const;

    WebhookInfo::Ptr getWebhookInfo() const;

    User::Ptr getMe() const;

    bool logOut() const;

    bool close() const;

    Message::Ptr sendMessage(boost::variant<std::int64_t, std::string> chatId,
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

    Message::Ptr forwardMessage(boost::variant<std::int64_t, std::string> chatId,
                                boost::variant<std::int64_t, std::string> fromChatId,
                                std::int32_t messageId,
                                bool disableNotification = false,
                                bool protectContent = false,
                                std::int32_t messageThreadId = 0) const;

    std::vector<MessageId::Ptr> forwardMessages(boost::variant<std::int64_t, std::string> chatId,
                                                boost::variant<std::int64_t, std::string> fromChatId,
                                                const std::vector<std::int32_t>& messageIds,
                                                std::int32_t messageThreadId = 0,
                                                bool disableNotification = false,
                                                bool protectContent = false) const;

    MessageId::Ptr copyMessage(boost::variant<std::int64_t, std::string> chatId,
                               boost::variant<std::int64_t, std::string> fromChatId,
                               std::int32_t messageId,
                               const std::string& caption = "",
                               const std::string& parseMode = "",
                               const std::vector<MessageEntity::Ptr>& captionEntities = std::vector<MessageEntity::Ptr>(),
                               bool disableNotification = false,
                               ReplyParameters::Ptr replyParameters = nullptr,
                               GenericReply::Ptr replyMarkup = std::make_shared<GenericReply>(),
                               bool protectContent = false,
                               std::int32_t messageThreadId = 0) const;

    std::vector<MessageId::Ptr> copyMessages(boost::variant<std::int64_t, std::string> chatId,
                                             boost::variant<std::int64_t, std::string> fromChatId,
                                             const std::vector<std::int32_t>& messageIds,
                                             std::int32_t messageThreadId = 0,
                                             bool disableNotification = false,
                                             bool protectContent = false,
                                             bool removeCaption = false) const;

    Message::Ptr sendPhoto(boost::variant<std::int64_t, std::string> chatId,
                           boost::variant<InputFile::Ptr, std::string> photo,
                           const std::string& caption = "",
                           ReplyParameters::Ptr replyParameters = nullptr,
                           GenericReply::Ptr replyMarkup = nullptr,
                           const std::string& parseMode = "",
                           bool disableNotification = false,
                           const std::vector<MessageEntity::Ptr>& captionEntities = std::vector<MessageEntity::Ptr>(),
                           std::int32_t messageThreadId = 0,
                           bool protectContent = false,
                           bool hasSpoiler = false,
                           const std::string& businessConnectionId = "") const;

    Message::Ptr sendAudio(boost::variant<std::int64_t, std::string> chatId,
                           boost::variant<InputFile::Ptr, std::string> audio,
                           const std::string& caption = "",
                           std::int32_t duration = 0,
                           const std::string& performer = "",
                           const std::string& title = "",
                           boost::variant<InputFile::Ptr, std::string> thumbnail = "",
                           ReplyParameters::Ptr replyParameters = nullptr,
                           GenericReply::Ptr replyMarkup = nullptr,
                           const std::string& parseMode = "",
                           bool disableNotification = false,
                           const std::vector<MessageEntity::Ptr>& captionEntities = std::vector<MessageEntity::Ptr>(),
                           std::int32_t messageThreadId = 0,
                           bool protectContent = false,
                           const std::string& businessConnectionId = "") const;

    Message::Ptr sendDocument(boost::variant<std::int64_t, std::string> chatId,
                              boost::variant<InputFile::Ptr, std::string> document,
                              boost::variant<InputFile::Ptr, std::string> thumbnail = "",
                              const std::string& caption = "",
                              ReplyParameters::Ptr replyParameters = nullptr,
                              GenericReply::Ptr replyMarkup = nullptr,
                              const std::string& parseMode = "",
                              bool disableNotification = false,
                              const std::vector<MessageEntity::Ptr>& captionEntities = std::vector<MessageEntity::Ptr>(),
                              bool disableContentTypeDetection = false,
                              std::int32_t messageThreadId = 0,
                              bool protectContent = false,
                              const std::string& businessConnectionId = "") const;

    Message::Ptr sendVideo(boost::variant<std::int64_t, std::string> chatId,
                           boost::variant<InputFile::Ptr, std::string> video,
                           bool supportsStreaming = false,
                           std::int32_t duration = 0,
                           std::int32_t width = 0,
                           std::int32_t height = 0,
                           boost::variant<InputFile::Ptr, std::string> thumbnail = "",
                           const std::string& caption = "",
                           ReplyParameters::Ptr replyParameters = nullptr,
                           GenericReply::Ptr replyMarkup = nullptr,
                           const std::string& parseMode = "",
                           bool disableNotification = false,
                           const std::vector<MessageEntity::Ptr>& captionEntities = std::vector<MessageEntity::Ptr>(),
                           std::int32_t messageThreadId = 0,
                           bool protectContent = false,
                           bool hasSpoiler = false,
                           const std::string& businessConnectionId = "") const;

    Message::Ptr sendAnimation(boost::variant<std::int64_t, std::string> chatId,
                               boost::variant<InputFile::Ptr, std::string> animation,
                               std::int32_t duration = 0,
                               std::int32_t width = 0,
                               std::int32_t height = 0,
                               boost::variant<InputFile::Ptr, std::string> thumbnail = "",
                               const std::string& caption = "",
                               ReplyParameters::Ptr replyParameters = nullptr,
                               GenericReply::Ptr replyMarkup = nullptr,
                               const std::string& parseMode = "",
                               bool disableNotification = false,
                               const std::vector<MessageEntity::Ptr>& captionEntities = std::vector<MessageEntity::Ptr>(),
                               std::int32_t messageThreadId = 0,
                               bool protectContent = false,
                               bool hasSpoiler = false,
                               const std::string& businessConnectionId = "") const;

    Message::Ptr sendVoice(boost::variant<std::int64_t, std::string> chatId,
                           boost::variant<InputFile::Ptr, std::string> voice,
                           const std::string& caption = "",
                           std::int32_t duration = 0,
                           ReplyParameters::Ptr replyParameters = nullptr,
                           GenericReply::Ptr replyMarkup = nullptr,
                           const std::string& parseMode = "",
                           bool disableNotification = false,
                           const std::vector<MessageEntity::Ptr>& captionEntities = std::vector<MessageEntity::Ptr>(),
                           std::int32_t messageThreadId = 0,
                           bool protectContent = false,
                           const std::string& businessConnectionId = "") const;

    Message::Ptr sendVideoNote(boost::variant<std::int64_t, std::string> chatId,
                               boost::variant<InputFile::Ptr, std::string> videoNote,
                               ReplyParameters::Ptr replyParameters = nullptr,
                               bool disableNotification = false,
                               std::int32_t duration = 0,
                               std::int32_t length = 0,
                               boost::variant<InputFile::Ptr, std::string> thumbnail = "",
                               GenericReply::Ptr replyMarkup = nullptr,
                               std::int32_t messageThreadId = 0,
                               bool protectContent = false,
                               const std::string& businessConnectionId = "") const;

    std::vector<Message::Ptr> sendMediaGroup(boost::variant<std::int64_t, std::string> chatId,
                                             const std::vector<InputMedia::Ptr>& media,
                                             bool disableNotification = false,
                                             ReplyParameters::Ptr replyParameters = nullptr,
                                             std::int32_t messageThreadId = 0,
                                             bool protectContent = false,
                                             const std::string& businessConnectionId = "") const;

    Message::Ptr sendLocation(boost::variant<std::int64_t, std::string> chatId,
                              float latitude,
                              float longitude,
                              std::int32_t livePeriod = 0,
                              ReplyParameters::Ptr replyParameters = nullptr,
                              GenericReply::Ptr replyMarkup = nullptr,
                              bool disableNotification = false,
                              float horizontalAccuracy = 0,
                              std::int32_t heading = 0,
                              std::int32_t proximityAlertRadius = 0,
                              std::int32_t messageThreadId = 0,
                              bool protectContent = false,
                              const std::string& businessConnectionId = "") const;

    Message::Ptr editMessageLiveLocation(float latitude,
                                         float longitude,
                                         boost::variant<std::int64_t, std::string> chatId = "",
                                         std::int32_t messageId = 0,
                                         const std::string& inlineMessageId = "",
                                         InlineKeyboardMarkup::Ptr replyMarkup = std::make_shared<InlineKeyboardMarkup>(),
                                         float horizontalAccuracy = 0,
                                         std::int32_t heading = 0,
                                         std::int32_t proximityAlertRadius = 0) const;

    Message::Ptr stopMessageLiveLocation(boost::variant<std::int64_t, std::string> chatId = "",
                                         std::int32_t messageId = 0,
                                         const std::string& inlineMessageId = "",
                                         InlineKeyboardMarkup::Ptr replyMarkup = std::make_shared<InlineKeyboardMarkup>()) const;

    Message::Ptr sendVenue(boost::variant<std::int64_t, std::string> chatId,
                           float latitude,
                           float longitude,
                           const std::string& title,
                           const std::string& address,
                           const std::string& foursquareId = "",
                           const std::string& foursquareType = "",
                           bool disableNotification = false,
                           ReplyParameters::Ptr replyParameters = nullptr,
                           GenericReply::Ptr replyMarkup = nullptr,
                           const std::string& googlePlaceId = "",
                           const std::string& googlePlaceType = "",
                           std::int32_t messageThreadId = 0,
                           bool protectContent = false,
                           const std::string& businessConnectionId = "") const;

    Message::Ptr sendContact(boost::variant<std::int64_t, std::string> chatId,
                             const std::string& phoneNumber,
                             const std::string& firstName,
                             const std::string& lastName = "",
                             const std::string& vcard = "",
                             bool disableNotification = false,
                             ReplyParameters::Ptr replyParameters = nullptr,
                             GenericReply::Ptr replyMarkup = nullptr,
                             std::int32_t messageThreadId = 0,
                             bool protectContent = false,
                             const std::string& businessConnectionId = "") const;

    Message::Ptr sendPoll(boost::variant<std::int64_t, std::string> chatId,
                          const std::string& question,
                          const std::vector<std::string>& options,
                          bool disableNotification = false,
                          ReplyParameters::Ptr replyParameters = nullptr,
                          GenericReply::Ptr replyMarkup = nullptr,
                          bool isAnonymous = true,
                          const std::string& type = "",
                          bool allowsMultipleAnswers = false,
                          std::int32_t correctOptionId = -1,
                          const std::string& explanation = "",
                          const std::string& explanationParseMode = "",
                          const std::vector<MessageEntity::Ptr>& explanationEntities = std::vector<MessageEntity::Ptr>(),
                          std::int32_t openPeriod = 0,
                          std::int32_t closeDate = 0,
                          bool isClosed = false,
                          std::int32_t messageThreadId = 0,
                          bool protectContent = false,
                          const std::string& businessConnectionId = "") const;

    Message::Ptr sendDice(boost::variant<std::int64_t, std::string> chatId,
                          bool disableNotification = false,
                          ReplyParameters::Ptr replyParameters = nullptr,
                          GenericReply::Ptr replyMarkup = nullptr,
                          const std::string& emoji = "",
                          std::int32_t messageThreadId = 0,
                          bool protectContent = false,
                          const std::string& businessConnectionId = "") const;

    bool setMessageReaction(boost::variant<std::int64_t, std::string> chatId,
                            std::int32_t messageId = 0,
                            const std::vector<ReactionType::Ptr>& reaction = std::vector<ReactionType::Ptr>(),
                            bool isBig = false) const;

    bool sendChatAction(std::int64_t chatId,
                        const std::string& action,
                        std::int32_t messageThreadId = 0,
                        const std::string& businessConnectionId = "") const;

    UserProfilePhotos::Ptr getUserProfilePhotos(std::int64_t userId,
                                                std::int32_t offset = 0,
                                                std::int32_t limit = 100) const;

    File::Ptr getFile(const std::string& fileId) const;

    bool banChatMember(boost::variant<std::int64_t, std::string> chatId,
                       std::int64_t userId,
                       std::int32_t untilDate = 0,
                       bool revokeMessages = true) const;

    bool unbanChatMember(boost::variant<std::int64_t, std::string> chatId,
                         std::int64_t userId,
                         bool onlyIfBanned = false) const;

    bool restrictChatMember(boost::variant<std::int64_t, std::string> chatId,
                            std::int64_t userId,
                            ChatPermissions::Ptr permissions,
                            std::uint32_t untilDate = 0,
                            bool useIndependentChatPermissions = false) const;

    bool promoteChatMember(boost::variant<std::int64_t, std::string> chatId,
                           std::int64_t userId,
                           bool canChangeInfo = false,
                           bool canPostMessages = false,
                           bool canEditMessages = false,
                           bool canDeleteMessages = false,
                           bool canInviteUsers = false,
                           bool canPinMessages = false,
                           bool canPromoteMembers = false,
                           bool isAnonymous = false,
                           bool canManageChat = false,
                           bool canManageVideoChats = false,
                           bool canRestrictMembers = false,
                           bool canManageTopics = false,
                           bool canPostStories = false,
                           bool canEditStories = false,
                           bool canDeleteStories = false) const;

    bool setChatAdministratorCustomTitle(boost::variant<std::int64_t, std::string> chatId,
                                         std::int64_t userId,
                                         const std::string& customTitle) const;

    bool banChatSenderChat(boost::variant<std::int64_t, std::string> chatId,
                           std::int64_t senderChatId) const;

    bool unbanChatSenderChat(boost::variant<std::int64_t, std::string> chatId,
                             std::int64_t senderChatId) const;

    bool setChatPermissions(boost::variant<std::int64_t, std::string> chatId,
                            ChatPermissions::Ptr permissions,
                            bool useIndependentChatPermissions = false) const;

    std::string exportChatInviteLink(boost::variant<std::int64_t, std::string> chatId) const;

    ChatInviteLink::Ptr createChatInviteLink(boost::variant<std::int64_t, std::string> chatId,
                                             std::int32_t expireDate = 0,
                                             std::int32_t memberLimit = 0,
                                             const std::string& name = "",
                                             bool createsJoinRequest = false) const;

    ChatInviteLink::Ptr editChatInviteLink(boost::variant<std::int64_t, std::string> chatId,
                                           const std::string& inviteLink,
                                           std::int32_t expireDate = 0,
                                           std::int32_t memberLimit = 0,
                                           const std::string& name = "",
                                           bool createsJoinRequest = false) const;

    ChatInviteLink::Ptr revokeChatInviteLink(boost::variant<std::int64_t, std::string> chatId,
                                             const std::string& inviteLink) const;

    bool approveChatJoinRequest(boost::variant<std::int64_t, std::string> chatId,
                                std::int64_t userId) const;

    bool declineChatJoinRequest(boost::variant<std::int64_t, std::string> chatId,
                                std::int64_t userId) const;

    bool setChatPhoto(boost::variant<std::int64_t, std::string> chatId,
                      InputFile::Ptr photo) const;

    bool deleteChatPhoto(boost::variant<std::int64_t, std::string> chatId) const;

    bool setChatTitle(boost::variant<std::int64_t, std::string> chatId,
                      const std::string& title) const;

    bool setChatDescription(boost::variant<std::int64_t, std::string> chatId,
                            const std::string& description = "") const;

    bool pinChatMessage(boost::variant<std::int64_t, std::string> chatId,
                        std::int32_t messageId,
                        bool disableNotification = false) const;

    bool unpinChatMessage(boost::variant<std::int64_t, std::string> chatId,
                          std::int32_t messageId = 0) const;

    bool unpinAllChatMessages(boost::variant<std::int64_t, std::string> chatId) const;

    bool leaveChat(boost::variant<std::int64_t, std::string> chatId) const;

    Chat::Ptr getChat(boost::variant<std::int64_t, std::string> chatId) const;

    std::vector<ChatMember::Ptr> getChatAdministrators(boost::variant<std::int64_t, std::string> chatId) const;

    std::int32_t getChatMemberCount(boost::variant<std::int64_t, std::string> chatId) const;

    ChatMember::Ptr getChatMember(boost::variant<std::int64_t, std::string> chatId,
                                  std::int64_t userId) const;

    bool setChatStickerSet(boost::variant<std::int64_t, std::string> chatId,
                           const std::string& stickerSetName) const;

    bool deleteChatStickerSet(boost::variant<std::int64_t, std::string> chatId) const;

    std::vector<Sticker::Ptr> getForumTopicIconStickers() const;

    ForumTopic::Ptr createForumTopic(boost::variant<std::int64_t, std::string> chatId,
                                     const std::string& name,
                                     std::int32_t iconColor = 0,
                                     const std::string& iconCustomEmojiId = "") const;

    bool editForumTopic(boost::variant<std::int64_t, std::string> chatId,
                        std::int32_t messageThreadId,
                        const std::string& name = "",
                        boost::variant<std::int32_t, std::string> iconCustomEmojiId = 0) const;

    bool closeForumTopic(boost::variant<std::int64_t, std::string> chatId,
                         std::int32_t messageThreadId) const;

    bool reopenForumTopic(boost::variant<std::int64_t, std::string> chatId,
                          std::int32_t messageThreadId) const;

    bool deleteForumTopic(boost::variant<std::int64_t, std::string> chatId,
                          std::int32_t messageThreadId) const;

    bool unpinAllForumTopicMessages(boost::variant<std::int64_t, std::string> chatId,
                                    std::int32_t messageThreadId) const;

    bool editGeneralForumTopic(boost::variant<std::int64_t, std::string> chatId,
                               std::string name) const;

    bool closeGeneralForumTopic(boost::variant<std::int64_t, std::string> chatId) const;

    bool reopenGeneralForumTopic(boost::variant<std::int64_t, std::string> chatId) const;

    bool hideGeneralForumTopic(boost::variant<std::int64_t, std::string> chatId) const;

    bool unhideGeneralForumTopic(boost::variant<std::int64_t, std::string> chatId) const;

    bool unpinAllGeneralForumTopicMessages(boost::variant<std::int64_t, std::string> chatId) const;

    bool answerCallbackQuery(const std::string& callbackQueryId,
                             const std::string& text = "",
                             bool showAlert = false,
                             const std::string& url = "",
                             std::int32_t cacheTime = 0) const;

    UserChatBoosts::Ptr getUserChatBoosts(boost::variant<std::int64_t, std::string> chatId,
                                          std::int32_t userId) const;

    BusinessConnection::Ptr getBusinessConnection(const std::string& businessConnectionId) const;

    bool setMyCommands(const std::vector<BotCommand::Ptr>& commands,
                       BotCommandScope::Ptr scope = nullptr,
                       const std::string& languageCode = "") const;

    bool deleteMyCommands(BotCommandScope::Ptr scope = nullptr,
                          const std::string& languageCode = "") const;

    std::vector<BotCommand::Ptr> getMyCommands(BotCommandScope::Ptr scope = nullptr,
                                               const std::string& languageCode = "") const;

    bool setMyName(const std::string& name = "",
                   const std::string& languageCode = "") const;

    BotName::Ptr getMyName(const std::string& languageCode = "") const;

    bool setMyDescription(const std::string& description = "",
                          const std::string& languageCode = "") const;

    BotDescription::Ptr getMyDescription(const std::string& languageCode = "") const;

    bool setMyShortDescription(const std::string& shortDescription = "",
                               const std::string& languageCode = "") const;

    BotShortDescription::Ptr getMyShortDescription(const std::string& languageCode = "") const;

    bool setChatMenuButton(std::int64_t chatId = 0,
                           MenuButton::Ptr menuButton = nullptr) const;

    MenuButton::Ptr getChatMenuButton(std::int64_t chatId = 0) const;

    bool setMyDefaultAdministratorRights(ChatAdministratorRights::Ptr rights = nullptr,
                                         bool forChannels = false) const;

    ChatAdministratorRights::Ptr getMyDefaultAdministratorRights(bool forChannels = false) const;

    Message::Ptr editMessageText(const std::string& text,
                                 boost::variant<std::int64_t, std::string> chatId = 0,
                                 std::int32_t messageId = 0,
                                 const std::string& inlineMessageId = "",
                                 const std::string& parseMode = "",
                                 LinkPreviewOptions::Ptr linkPreviewOptions = nullptr,
                                 InlineKeyboardMarkup::Ptr replyMarkup = nullptr,
                                 const std::vector<MessageEntity::Ptr>& entities = std::vector<MessageEntity::Ptr>()) const;

    Message::Ptr editMessageCaption(boost::variant<std::int64_t, std::string> chatId = 0,
                                    std::int32_t messageId = 0,
                                    const std::string& caption = "",
                                    const std::string& inlineMessageId = "",
                                    GenericReply::Ptr replyMarkup = nullptr,
                                    const std::string& parseMode = "",
                                    const std::vector<MessageEntity::Ptr>& captionEntities = std::vector<MessageEntity::Ptr>()) const;

    Message::Ptr editMessageMedia(InputMedia::Ptr media,
                                  boost::variant<std::int64_t, std::string> chatId = 0,
                                  std::int32_t messageId = 0,
                                  const std::string& inlineMessageId = "",
                                  GenericReply::Ptr replyMarkup = nullptr) const;

    Message::Ptr editMessageReplyMarkup(boost::variant<std::int64_t, std::string> chatId = 0,
                                        std::int32_t messageId = 0,
                                        const std::string& inlineMessageId = "",
                                        GenericReply::Ptr replyMarkup = nullptr) const;

    Poll::Ptr stopPoll(boost::variant<std::int64_t, std::string> chatId,
                       std::int64_t messageId,
                       InlineKeyboardMarkup::Ptr replyMarkup = std::make_shared<InlineKeyboardMarkup>()) const;

    bool deleteMessage(boost::variant<std::int64_t, std::string> chatId,
                       std::int32_t messageId) const;

    bool deleteMessages(boost::variant<std::int64_t, std::string> chatId,
                        const std::vector<std::int32_t>& messageIds) const;

    Message::Ptr sendSticker(boost::variant<std::int64_t, std::string> chatId,
                             boost::variant<InputFile::Ptr, std::string> sticker,
                             ReplyParameters::Ptr replyParameters = nullptr,
                             GenericReply::Ptr replyMarkup = nullptr,
                             bool disableNotification = false,
                             std::int32_t messageThreadId = 0,
                             bool protectContent = false,
                             const std::string& emoji = "",
                             const std::string& businessConnectionId = "") const;

    StickerSet::Ptr getStickerSet(const std::string& name) const;

    std::vector<Sticker::Ptr> getCustomEmojiStickers(const std::vector<std::string>& customEmojiIds) const;

    File::Ptr uploadStickerFile(std::int64_t userId,
                                InputFile::Ptr sticker,
                                const std::string& stickerFormat) const;

    bool createNewStickerSet(std::int64_t userId,
                             const std::string& name,
                             const std::string& title,
                             const std::vector<InputSticker::Ptr>& stickers,
                             Sticker::Type stickerType = Sticker::Type::Regular,
                             bool needsRepainting = false) const;

    bool addStickerToSet(std::int64_t userId,
                         const std::string& name,
                         InputSticker::Ptr sticker) const;

    bool setStickerPositionInSet(const std::string& sticker,
                                 std::int32_t position) const;

    bool deleteStickerFromSet(const std::string& sticker) const;

    bool replaceStickerInSet(std::int64_t userId,
                             const std::string& name,
                             const std::string& oldSticker,
                             InputSticker::Ptr sticker) const;

    bool setStickerEmojiList(const std::string& sticker,
                             const std::vector<std::string>& emojiList) const;

    bool setStickerKeywords(const std::string& sticker,
                            const std::vector<std::string>& keywords = std::vector<std::string>()) const;

    bool setStickerMaskPosition(const std::string& sticker,
                                MaskPosition::Ptr maskPosition = nullptr) const;

    bool setStickerSetTitle(const std::string& name,
                            const std::string& title) const;

    bool setStickerSetThumbnail(const std::string& name,
                                std::int64_t userId,
                                const std::string& format,
                                boost::variant<InputFile::Ptr, std::string> thumbnail = "") const;

    bool setCustomEmojiStickerSetThumbnail(const std::string& name,
                                           const std::string& customEmojiId = "") const;

    bool deleteStickerSet(const std::string& name) const;

    bool answerInlineQuery(const std::string& inlineQueryId,
                           const std::vector<InlineQueryResult::Ptr>& results,
                           std::int32_t cacheTime = 300,
                           bool isPersonal = false,
                           const std::string& nextOffset = "",
                           InlineQueryResultsButton::Ptr button = nullptr) const;

    SentWebAppMessage::Ptr answerWebAppQuery(const std::string& webAppQueryId,
                                             InlineQueryResult::Ptr result) const;

    Message::Ptr sendInvoice(boost::variant<std::int64_t, std::string> chatId,
                             const std::string& title,
                             const std::string& description,
                             const std::string& payload,
                             const std::string& providerToken,
                             const std::string& currency,
                             const std::vector<LabeledPrice::Ptr>& prices,
                             const std::string& providerData = "",
                             const std::string& photoUrl = "",
                             std::int32_t photoSize = 0,
                             std::int32_t photoWidth = 0,
                             std::int32_t photoHeight = 0,
                             bool needName = false,
                             bool needPhoneNumber = false,
                             bool needEmail = false,
                             bool needShippingAddress = false,
                             bool sendPhoneNumberToProvider = false,
                             bool sendEmailToProvider = false,
                             bool isFlexible = false,
                             ReplyParameters::Ptr replyParameters = nullptr,
                             GenericReply::Ptr replyMarkup = nullptr,
                             bool disableNotification = false,
                             std::int32_t messageThreadId = 0,
                             std::int32_t maxTipAmount = 0,
                             const std::vector<std::int32_t>& suggestedTipAmounts = std::vector<std::int32_t>(),
                             const std::string& startParameter = "",
                             bool protectContent = false) const;

    std::string createInvoiceLink(const std::string& title,
                                  const std::string& description,
                                  const std::string& payload,
                                  const std::string& providerToken,
                                  const std::string& currency,
                                  const std::vector<LabeledPrice::Ptr>& prices,
                                  std::int32_t maxTipAmount = 0,
                                  const std::vector<std::int32_t>& suggestedTipAmounts = std::vector<std::int32_t>(),
                                  const std::string& providerData = "",
                                  const std::string& photoUrl = "",
                                  std::int32_t photoSize = 0,
                                  std::int32_t photoWidth = 0,
                                  std::int32_t photoHeight = 0,
                                  bool needName = false,
                                  bool needPhoneNumber = false,
                                  bool needEmail = false,
                                  bool needShippingAddress = false,
                                  bool sendPhoneNumberToProvider = false,
                                  bool sendEmailToProvider = false,
                                  bool isFlexible = false) const;

    bool answerShippingQuery(const std::string& shippingQueryId,
                             bool ok,
                             const std::vector<ShippingOption::Ptr>& shippingOptions = std::vector<ShippingOption::Ptr>(),
                             const std::string& errorMessage = "") const;

    bool answerPreCheckoutQuery(const std::string& preCheckoutQueryId,
                                bool ok,
                                const std::string& errorMessage = "") const;

    bool setPassportDataErrors(std::int64_t userId,
                               const std::vector<PassportElementError::Ptr>& errors) const;

    Message::Ptr sendGame(std::int64_t chatId,
                          const std::string& gameShortName,
                          ReplyParameters::Ptr replyParameters = nullptr,
                          InlineKeyboardMarkup::Ptr replyMarkup = std::make_shared<InlineKeyboardMarkup>(),
                          bool disableNotification = false,
                          std::int32_t messageThreadId = 0,
                          bool protectContent = false,
                          const std::string& businessConnectionId = "") const;

    Message::Ptr setGameScore(std::int64_t userId,
                              std::int32_t score,
                              bool force = false,
                              bool disableEditMessage = false,
                              std::int64_t chatId = 0,
                              std::int32_t messageId = 0,
                              const std::string& inlineMessageId = "") const;

    std::vector<GameHighScore::Ptr> getGameHighScores(std::int64_t userId,
                                                      std::int64_t chatId = 0,
                                                      std::int32_t messageId = 0,
                                                      const std::string& inlineMessageId = "") const;

    std::string downloadFile(const std::string& filePath,
                             const std::vector<HttpReqArg>& args = std::vector<HttpReqArg>()) const;

    bool blockedByUser(std::int64_t chatId) const;

    const HttpClient& _httpClient;

protected:
    boost::property_tree::ptree sendRequest(const std::string& method, const std::vector<HttpReqArg>& args = std::vector<HttpReqArg>()) const;
};

}

#endif //TGBOT_API_H
