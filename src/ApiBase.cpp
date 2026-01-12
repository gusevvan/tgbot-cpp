#include "tgbot/ApiBase.h"

namespace TgBot {
ApiBase::ApiBase(std::string token, const std::string& url)
    : _token(std::move(token)), _tgTypeParser(), _url(url) {}

std::vector<HttpReqArg> ApiBase::getUpdatesArgs(std::int32_t offset,
                                        std::int32_t limit,
                                        std::int32_t timeout,
                                        const StringArrayPtr& allowedUpdates) const {
    std::vector<HttpReqArg> args;
    args.reserve(4);

    if (offset != 0) {
        args.emplace_back("offset", offset);
    }
    if (limit != 100) {
        args.emplace_back("limit", std::max(1, std::min(100, limit)));
    }
    if (timeout != 0) {
        args.emplace_back("timeout", timeout);
    }
    if (allowedUpdates != nullptr) {
        std::string allowedUpdatesJson = _tgTypeParser.parseArray<std::string>(
            [] (const std::string& s)->std::string {
            return s;
        }, *allowedUpdates);
        args.emplace_back("allowed_updates", allowedUpdatesJson);
    }
    return args;
}

std::vector<HttpReqArg> ApiBase::setWebhookArgs(const std::string& url,
                    InputFile::Ptr certificate,
                    std::int32_t maxConnections,
                    const StringArrayPtr& allowedUpdates,
                    const std::string& ipAddress,
                    bool dropPendingUpdates,
                    const std::string& secretToken) const {
    std::vector<HttpReqArg> args;
    args.reserve(7);

    args.emplace_back("url", url);
    if (certificate != nullptr) {
        args.emplace_back("certificate", certificate->data, true, certificate->mimeType, certificate->fileName);
    }
    if (!ipAddress.empty()) {
        args.emplace_back("ip_address", ipAddress);
    }
    if (maxConnections != 40) {
        args.emplace_back("max_connections", std::max(1, std::min(100, maxConnections)));
    }
    if (allowedUpdates != nullptr) {
        auto allowedUpdatesJson = _tgTypeParser.parseArray<std::string>(
            [] (const std::string& s)->std::string {
            return s;
        }, *allowedUpdates);
        args.emplace_back("allowed_updates", allowedUpdatesJson);
    }
    if (dropPendingUpdates) {
        args.emplace_back("drop_pending_updates", dropPendingUpdates);
    }
    if (!secretToken.empty()) {
        args.emplace_back("secret_token", secretToken);
    }
    return args;
}

std::vector<HttpReqArg> ApiBase::deleteWebhookArgs(bool dropPendingUpdates) const {
    std::vector<HttpReqArg> args;
    args.reserve(1);

    if (dropPendingUpdates) {
        args.emplace_back("drop_pending_updates", dropPendingUpdates);
    }
    return args;
}

std::vector<HttpReqArg> ApiBase::sendMessageArgs(boost::variant<std::int64_t, std::string> chatId,
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
    std::vector<HttpReqArg> args;
    args.reserve(11);

    if (!businessConnectionId.empty()) {
        args.emplace_back("business_connection_id", businessConnectionId);
    }
    args.emplace_back("chat_id", chatId);
    if (messageThreadId != 0) {
        args.emplace_back("message_thread_id", messageThreadId);
    }
    args.emplace_back("text", text);
    if (!parseMode.empty()) {
        args.emplace_back("parse_mode", parseMode);
    }
    if (!entities.empty()) {
        args.emplace_back("entities", _tgTypeParser.parseArray<MessageEntity>(&TgTypeParser::parseMessageEntity, entities));
    }
    if (linkPreviewOptions != nullptr) {
        args.emplace_back("link_preview_options", _tgTypeParser.parseLinkPreviewOptions(linkPreviewOptions));
    }
    if (disableNotification) {
        args.emplace_back("disable_notification", disableNotification);
    }
    if (protectContent) {
        args.emplace_back("protect_content", protectContent);
    }
    if (replyParameters != nullptr) {
        args.emplace_back("reply_parameters", _tgTypeParser.parseReplyParameters(replyParameters));
    }
    if (replyMarkup) {
        args.emplace_back("reply_markup", _tgTypeParser.parseGenericReply(replyMarkup));
    }
    return args;
}

std::vector<HttpReqArg> ApiBase::forwardMessageArgs(boost::variant<std::int64_t, std::string> chatId,
                                boost::variant<std::int64_t, std::string> fromChatId,
                                std::int32_t messageId,
                                bool disableNotification,
                                bool protectContent,
                                std::int32_t messageThreadId) const {
    std::vector<HttpReqArg> args;
    args.reserve(6);

    args.emplace_back("chat_id", chatId);
    if (messageThreadId != 0) {
        args.emplace_back("message_thread_id", messageThreadId);
    }
    args.emplace_back("from_chat_id", fromChatId);
    if (disableNotification) {
        args.emplace_back("disable_notification", disableNotification);
    }
    if (protectContent) {
        args.emplace_back("protect_content", protectContent);
    }
    args.emplace_back("message_id", messageId);

    return args;
}

std::vector<HttpReqArg> ApiBase::forwardMessagesArgs(boost::variant<std::int64_t, std::string> chatId,
                                                boost::variant<std::int64_t, std::string> fromChatId,
                                                const std::vector<std::int32_t>& messageIds,
                                                std::int32_t messageThreadId,
                                                bool disableNotification,
                                                bool protectContent) const {
    std::vector<HttpReqArg> args;
    args.reserve(6);

    args.emplace_back("chat_id", chatId);
    args.emplace_back("from_chat_id", fromChatId);
    if (!messageIds.empty()) {
        args.emplace_back("message_ids", _tgTypeParser.parseArray<std::int32_t>(
            [] (const std::int32_t& i)->std::int32_t {
            return i;
        }, messageIds));
    }
    if (messageThreadId != 0) {
        args.emplace_back("message_thread_id", messageThreadId);
    }
    if (disableNotification) {
        args.emplace_back("disable_notification", disableNotification);
    }
    if (protectContent) {
        args.emplace_back("protect_content", protectContent);
    }

    return args;
}

std::vector<HttpReqArg> ApiBase::copyMessageArgs(boost::variant<std::int64_t, std::string> chatId,
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
    std::vector<HttpReqArg> args;
    args.reserve(11);

    args.emplace_back("chat_id", chatId);
    if (messageThreadId != 0) {
        args.emplace_back("message_thread_id", messageThreadId);
    }
    args.emplace_back("from_chat_id", fromChatId);
    args.emplace_back("message_id", messageId);
    if (!caption.empty()) {
        args.emplace_back("caption", caption);
    }
    if (!parseMode.empty()) {
        args.emplace_back("parse_mode", parseMode);
    }
    if (!captionEntities.empty()) {
        args.emplace_back("caption_entities", _tgTypeParser.parseArray<MessageEntity>(&TgTypeParser::parseMessageEntity, captionEntities));
    }
    if (disableNotification) {
        args.emplace_back("disable_notification", disableNotification);
    }
    if (protectContent) {
        args.emplace_back("protect_content", protectContent);
    }
    if (replyParameters != nullptr) {
        args.emplace_back("reply_parameters", _tgTypeParser.parseReplyParameters(replyParameters));
    }
    if (replyMarkup) {
        args.emplace_back("reply_markup", _tgTypeParser.parseGenericReply(replyMarkup));
    }
    return args;
}

std::vector<HttpReqArg> ApiBase::copyMessagesArgs(boost::variant<std::int64_t, std::string> chatId,
                                            boost::variant<std::int64_t, std::string> fromChatId,
                                            const std::vector<std::int32_t>& messageIds,
                                            std::int32_t messageThreadId,
                                            bool disableNotification,
                                            bool protectContent,
                                            bool removeCaption) const {
    std::vector<HttpReqArg> args;
    args.reserve(7);

    args.emplace_back("chat_id", chatId);
    args.emplace_back("from_chat_id", fromChatId);

    if (!messageIds.empty()) {
        args.emplace_back("message_ids", _tgTypeParser.parseArray<std::int32_t>(
            [] (const std::int32_t& i)->std::int32_t {
            return i;
        }, messageIds));
    }
    if (messageThreadId != 0) {
        args.emplace_back("message_thread_id", messageThreadId);
    }
    if (disableNotification) {
        args.emplace_back("disable_notification", disableNotification);
    }
    if (protectContent) {
        args.emplace_back("protect_content", protectContent);
    }
    if (removeCaption) {
        args.emplace_back("remove_caption", removeCaption);
    }
    return args;
}

std::vector<HttpReqArg> ApiBase::sendPhotoArgs(boost::variant<std::int64_t, std::string> chatId,
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
    std::vector<HttpReqArg> args;
    args.reserve(12);

    if (!businessConnectionId.empty()) {
        args.emplace_back("business_connection_id", businessConnectionId);
    }
    args.emplace_back("chat_id", chatId);
    if (messageThreadId != 0) {
        args.emplace_back("message_thread_id", messageThreadId);
    }
    if (photo.which() == 0) {  // InputFile::Ptr
        auto file = boost::get<InputFile::Ptr>(photo);
        args.emplace_back("photo", file->data, true, file->mimeType, file->fileName);
    } else {  // std::string
        args.emplace_back("photo", boost::get<std::string>(photo));
    }
    if (!caption.empty()) {
        args.emplace_back("caption", caption);
    }
    if (!parseMode.empty()) {
        args.emplace_back("parse_mode", parseMode);
    }
    if (!captionEntities.empty()) {
        args.emplace_back("caption_entities", _tgTypeParser.parseArray<MessageEntity>(&TgTypeParser::parseMessageEntity, captionEntities));
    }
    if (hasSpoiler) {
        args.emplace_back("has_spoiler", hasSpoiler);
    }
    if (disableNotification) {
        args.emplace_back("disable_notification", disableNotification);
    }
    if (protectContent) {
        args.emplace_back("protect_content", protectContent);
    }
    if (replyParameters != nullptr) {
        args.emplace_back("reply_parameters", _tgTypeParser.parseReplyParameters(replyParameters));
    }
    if (replyMarkup != nullptr) {
        args.emplace_back("reply_markup", _tgTypeParser.parseGenericReply(replyMarkup));
    }
    return args;
}

std::vector<HttpReqArg> ApiBase::sendAudioArgs(boost::variant<std::int64_t, std::string> chatId,
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
    std::vector<HttpReqArg> args;
    args.reserve(15);

    if (!businessConnectionId.empty()) {
        args.emplace_back("business_connection_id", businessConnectionId);
    }
    args.emplace_back("chat_id", chatId);
    if (messageThreadId != 0) {
        args.emplace_back("message_thread_id", messageThreadId);
    }
    if (audio.which() == 0) {   // InputFile::Ptr
        auto file = boost::get<InputFile::Ptr>(audio);
        args.emplace_back("audio", file->data, true, file->mimeType, file->fileName);
    } else {    // std::string
        args.emplace_back("audio", boost::get<std::string>(audio));
    }
    if (!caption.empty()) {
        args.emplace_back("caption", caption);
    }
    if (!parseMode.empty()) {
        args.emplace_back("parse_mode", parseMode);
    }
    if (!captionEntities.empty()) {
        args.emplace_back("caption_entities", _tgTypeParser.parseArray<MessageEntity>(&TgTypeParser::parseMessageEntity, captionEntities));
    }
    if (duration) {
        args.emplace_back("duration", duration);
    }
    if (!performer.empty()) {
        args.emplace_back("performer", performer);
    }
    if (!title.empty()) {
        args.emplace_back("title", title);
    }
    if (thumbnail.which() == 0) {   // InputFile::Ptr
        auto file = boost::get<InputFile::Ptr>(thumbnail);
        args.emplace_back("thumbnail", file->data, true, file->mimeType, file->fileName);
    } else {    // std::string
        auto thumbnailStr = boost::get<std::string>(thumbnail);
        if (!thumbnailStr.empty()) {
            args.emplace_back("thumbnail", thumbnailStr);
        }
    }
    if (disableNotification) {
        args.emplace_back("disable_notification", disableNotification);
    }
    if (protectContent) {
        args.emplace_back("protect_content", protectContent);
    }
    if (replyParameters != nullptr) {
        args.emplace_back("reply_parameters", _tgTypeParser.parseReplyParameters(replyParameters));
    }
    if (replyMarkup) {
        args.emplace_back("reply_markup", _tgTypeParser.parseGenericReply(replyMarkup));
    }
    return args;
}

std::vector<HttpReqArg> ApiBase::sendDocumentArgs(boost::variant<std::int64_t, std::string> chatId,
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
    std::vector<HttpReqArg> args;
    args.reserve(13);

    if (!businessConnectionId.empty()) {
        args.emplace_back("business_connection_id", businessConnectionId);
    }
    args.emplace_back("chat_id", chatId);
    if (messageThreadId != 0) {
        args.emplace_back("message_thread_id", messageThreadId);
    }
    if (document.which() == 0) {    // InputFile::Ptr
        auto file = boost::get<InputFile::Ptr>(document);
        args.emplace_back("document", file->data, true, file->mimeType, file->fileName);
    } else {    // std::string
        args.emplace_back("document", boost::get<std::string>(document));
    }
    if (thumbnail.which() == 0) {   // InputFile::Ptr
        auto file = boost::get<InputFile::Ptr>(thumbnail);
        args.emplace_back("thumbnail", file->data, true, file->mimeType, file->fileName);
    } else {    // std::string
        auto thumbnailStr = boost::get<std::string>(thumbnail);
        if (!thumbnailStr.empty()) {
            args.emplace_back("thumbnail", thumbnailStr);
        }
    }
    if (!caption.empty()) {
        args.emplace_back("caption", caption);
    }
    if (!parseMode.empty()) {
        args.emplace_back("parse_mode", parseMode);
    }
    if (!captionEntities.empty()) {
        args.emplace_back("caption_entities", _tgTypeParser.parseArray<MessageEntity>(&TgTypeParser::parseMessageEntity, captionEntities));
    }
    if (disableContentTypeDetection) {
        args.emplace_back("disable_content_type_detection", disableContentTypeDetection);
    }
    if (disableNotification) {
        args.emplace_back("disable_notification", disableNotification);
    }
    if (protectContent) {
        args.emplace_back("protect_content", protectContent);
    }
    if (replyParameters != nullptr) {
        args.emplace_back("reply_parameters", _tgTypeParser.parseReplyParameters(replyParameters));
    }
    if (replyMarkup) {
        args.emplace_back("reply_markup", _tgTypeParser.parseGenericReply(replyMarkup));
    }
    return args;
}

std::vector<HttpReqArg> ApiBase::sendVideoArgs(boost::variant<std::int64_t, std::string> chatId,
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
    std::vector<HttpReqArg> args;
    args.reserve(17);

    if (!businessConnectionId.empty()) {
        args.emplace_back("business_connection_id", businessConnectionId);
    }
    args.emplace_back("chat_id", chatId);
    if (messageThreadId != 0) {
        args.emplace_back("message_thread_id", messageThreadId);
    }
    if (video.which() == 0) {   // InputFile::Ptr
        auto file = boost::get<InputFile::Ptr>(video);
        args.emplace_back("video", file->data, true, file->mimeType, file->fileName);
    } else {    // std::string
        args.emplace_back("video", boost::get<std::string>(video));
    }
    if (duration != 0) {
        args.emplace_back("duration", duration);
    }
    if (width != 0) {
        args.emplace_back("width", width);
    }
    if (height != 0) {
        args.emplace_back("height", height);
    }
    if (thumbnail.which() == 0) {   // InputFile::Ptr
        auto file = boost::get<InputFile::Ptr>(thumbnail);
        args.emplace_back("thumbnail", file->data, true, file->mimeType, file->fileName);
    } else {    // std::string
        auto thumbnailStr = boost::get<std::string>(thumbnail);
        if (!thumbnailStr.empty()) {
            args.emplace_back("thumbnail", thumbnailStr);
        }
    }
    if (!caption.empty()) {
        args.emplace_back("caption", caption);
    }
    if (!parseMode.empty()) {
        args.emplace_back("parse_mode", parseMode);
    }
    if (!captionEntities.empty()) {
        args.emplace_back("caption_entities", _tgTypeParser.parseArray<MessageEntity>(&TgTypeParser::parseMessageEntity, captionEntities));
    }
    if (hasSpoiler) {
        args.emplace_back("has_spoiler", hasSpoiler);
    }
    if (supportsStreaming) {
        args.emplace_back("supports_streaming", supportsStreaming);
    }
    if (disableNotification) {
        args.emplace_back("disable_notification", disableNotification);
    }
    if (protectContent) {
        args.emplace_back("protect_content", protectContent);
    }
    if (replyParameters != nullptr) {
        args.emplace_back("reply_parameters", _tgTypeParser.parseReplyParameters(replyParameters));
    }
    if (replyMarkup != nullptr) {
        args.emplace_back("reply_markup", _tgTypeParser.parseGenericReply(replyMarkup));
    }
    return args;
}

std::vector<HttpReqArg> ApiBase::sendAnimationArgs(boost::variant<std::int64_t, std::string> chatId,
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
    std::vector<HttpReqArg> args;
    args.reserve(16);

    if (!businessConnectionId.empty()) {
        args.emplace_back("business_connection_id", businessConnectionId);
    }
    args.emplace_back("chat_id", chatId);
    if (messageThreadId != 0) {
        args.emplace_back("message_thread_id", messageThreadId);
    }
    if (animation.which() == 0) {   // InputFile::Ptr
        auto file = boost::get<InputFile::Ptr>(animation);
        args.emplace_back("animation", file->data, true, file->mimeType, file->fileName);
    } else {    // std::string
        args.emplace_back("animation", boost::get<std::string>(animation));
    }
    if (duration != 0) {
        args.emplace_back("duration", duration);
    }
    if (width != 0) {
        args.emplace_back("width", width);
    }
    if (height != 0) {
        args.emplace_back("height", height);
    }
    if (thumbnail.which() == 0) {      // InputFile::Ptr
        auto file = boost::get<InputFile::Ptr>(thumbnail);
        args.emplace_back("thumbnail", file->data, true, file->mimeType, file->fileName);
    } else {    // std::string
        auto thumbnailStr = boost::get<std::string>(thumbnail);
        if (!thumbnailStr.empty()) {
            args.emplace_back("thumbnail", thumbnailStr);
        }
    }
    if (!caption.empty()) {
        args.emplace_back("caption", caption);
    }
    if (!parseMode.empty()) {
        args.emplace_back("parse_mode", parseMode);
    }
    if (!captionEntities.empty()) {
        args.emplace_back("caption_entities", _tgTypeParser.parseArray<MessageEntity>(&TgTypeParser::parseMessageEntity, captionEntities));
    }
    if (hasSpoiler) {
        args.emplace_back("has_spoiler", hasSpoiler);
    }
    if (disableNotification) {
        args.emplace_back("disable_notification", disableNotification);
    }
    if (protectContent) {
        args.emplace_back("protect_content", protectContent);
    }
    if (replyParameters != nullptr) {
        args.emplace_back("reply_parameters", _tgTypeParser.parseReplyParameters(replyParameters));
    }
    if (replyMarkup != nullptr) {
        args.emplace_back("reply_markup", _tgTypeParser.parseGenericReply(replyMarkup));
    }
    return args;
}

std::vector<HttpReqArg> ApiBase::sendVoiceArgs(boost::variant<std::int64_t, std::string> chatId,
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
    std::vector<HttpReqArg> args;
    args.reserve(12);

    if (!businessConnectionId.empty()) {
        args.emplace_back("business_connection_id", businessConnectionId);
    }
    args.emplace_back("chat_id", chatId);
    if (messageThreadId != 0) {
        args.emplace_back("message_thread_id", messageThreadId);
    }
    if (voice.which() == 0) {   // InputFile::Ptr
        auto file = boost::get<InputFile::Ptr>(voice);
        args.emplace_back("voice", file->data, true, file->mimeType, file->fileName);
    } else {    // std::string
        args.emplace_back("voice", boost::get<std::string>(voice));
    }
    if (!caption.empty()) {
        args.emplace_back("caption", caption);
    }
    if (!parseMode.empty()) {
        args.emplace_back("parse_mode", parseMode);
    }
    if (!captionEntities.empty()) {
        args.emplace_back("caption_entities", _tgTypeParser.parseArray<MessageEntity>(&TgTypeParser::parseMessageEntity, captionEntities));
    }
    if (duration) {
        args.emplace_back("duration", duration);
    }
    if (disableNotification) {
        args.emplace_back("disable_notification", disableNotification);
    }
    if (protectContent) {
        args.emplace_back("protect_content", protectContent);
    }
    if (replyParameters != nullptr) {
        args.emplace_back("reply_parameters", _tgTypeParser.parseReplyParameters(replyParameters));
    }
    if (replyMarkup) {
        args.emplace_back("reply_markup", _tgTypeParser.parseGenericReply(replyMarkup));
    }
    return args;
}

std::vector<HttpReqArg> ApiBase::sendVideoNoteArgs(boost::variant<std::int64_t, std::string> chatId,
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
    std::vector<HttpReqArg> args;
    args.reserve(11);

    if (!businessConnectionId.empty()) {
        args.emplace_back("business_connection_id", businessConnectionId);
    }
    args.emplace_back("chat_id", chatId);
    if (messageThreadId != 0) {
        args.emplace_back("message_thread_id", messageThreadId);
    }
    if (videoNote.which() == 0) {   // InputFile::Ptr
        auto file = boost::get<InputFile::Ptr>(videoNote);
        args.emplace_back("video_note", file->data, true, file->mimeType, file->fileName);
    } else {    // std::string
        args.emplace_back("video_note", boost::get<std::string>(videoNote));
    }
    if (duration) {
        args.emplace_back("duration", duration);
    }
    if (length) {
        args.emplace_back("length", length);
    }
    if (thumbnail.which() == 0) {   // InputFile::Ptr
        auto file = boost::get<InputFile::Ptr>(thumbnail);
        args.emplace_back("thumbnail", file->data, true, file->mimeType, file->fileName);
    } else {    // std::string
        auto thumbnailStr = boost::get<std::string>(thumbnail);
        if (!thumbnailStr.empty()) {
            args.emplace_back("thumbnail", thumbnailStr);
        }
    }
    if (disableNotification) {
        args.emplace_back("disable_notification", disableNotification);
    }
    if (protectContent) {
        args.emplace_back("protect_content", protectContent);
    }
    if (replyParameters != nullptr) {
        args.emplace_back("reply_parameters", _tgTypeParser.parseReplyParameters(replyParameters));
    }
    if (replyMarkup) {
        args.emplace_back("reply_markup", _tgTypeParser.parseGenericReply(replyMarkup));
    }
    return args;
}

std::vector<HttpReqArg> ApiBase::sendMediaGroupArgs(boost::variant<std::int64_t, std::string> chatId,
                                            const std::vector<InputMedia::Ptr>& media,
                                            bool disableNotification,
                                            ReplyParameters::Ptr replyParameters,
                                            std::int32_t messageThreadId,
                                            bool protectContent,
                                            const std::string& businessConnectionId) const {
    std::vector<HttpReqArg> args;
    args.reserve(7);

    if (!businessConnectionId.empty()) {
        args.emplace_back("business_connection_id", businessConnectionId);
    }
    args.emplace_back("chat_id", chatId);
    if (messageThreadId != 0) {
        args.emplace_back("message_thread_id", messageThreadId);
    }
    args.emplace_back("media", _tgTypeParser.parseArray<InputMedia>(&TgTypeParser::parseInputMedia, media));
    if (disableNotification) {
        args.emplace_back("disable_notification", disableNotification);
    }
    if (protectContent) {
        args.emplace_back("protect_content", protectContent);
    }
    if (replyParameters != nullptr) {
        args.emplace_back("reply_parameters", _tgTypeParser.parseReplyParameters(replyParameters));
    }
    return args;
}

std::vector<HttpReqArg> ApiBase::sendLocationArgs(boost::variant<std::int64_t, std::string> chatId,
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
    std::vector<HttpReqArg> args;
    args.reserve(13);

    if (!businessConnectionId.empty()) {
        args.emplace_back("business_connection_id", businessConnectionId);
    }
    args.emplace_back("chat_id", chatId);
    if (messageThreadId != 0) {
        args.emplace_back("message_thread_id", messageThreadId);
    }
    args.emplace_back("latitude", latitude);
    args.emplace_back("longitude", longitude);
    if (horizontalAccuracy) {
        args.emplace_back("horizontal_accuracy", horizontalAccuracy);
    }
    if (livePeriod) {
        args.emplace_back("live_period", std::max(60, std::min(86400, livePeriod)));
    }
    if (heading) {
        args.emplace_back("heading", std::max(1, std::min(360, heading)));
    }
    if (proximityAlertRadius) {
        args.emplace_back("proximity_alert_radius", std::max(1, std::min(100000, proximityAlertRadius)));
    }
    if (disableNotification) {
        args.emplace_back("disable_notification", disableNotification);
    }
    if (protectContent) {
        args.emplace_back("protect_content", protectContent);
    }
    if (replyParameters != nullptr) {
        args.emplace_back("reply_parameters", _tgTypeParser.parseReplyParameters(replyParameters));
    }
    if (replyMarkup) {
        args.emplace_back("reply_markup", _tgTypeParser.parseGenericReply(replyMarkup));
    }
    return args;
}

std::vector<HttpReqArg> ApiBase::editMessageLiveLocationArgs(float latitude,
                                        float longitude,
                                        boost::variant<std::int64_t, std::string> chatId,
                                        std::int32_t messageId,
                                        const std::string& inlineMessageId,
                                        InlineKeyboardMarkup::Ptr replyMarkup,
                                        float horizontalAccuracy,
                                        std::int32_t heading,
                                        std::int32_t proximityAlertRadius) const {
    std::vector<HttpReqArg> args;
    args.reserve(9);

    if (chatId.which() == 0) {   // std::int64_t
        if (boost::get<std::int64_t>(chatId) != 0) {
            args.emplace_back("chat_id", chatId);
        }
    } else {    // std::string
        if (boost::get<std::string>(chatId) != "") {
            args.emplace_back("chat_id", chatId);
        }
    }
    if (messageId) {
        args.emplace_back("message_id", messageId);
    }
    if (!inlineMessageId.empty()) {
        args.emplace_back("inline_message_id", inlineMessageId);
    }
    args.emplace_back("latitude", latitude);
    args.emplace_back("longitude", longitude);
    if (horizontalAccuracy) {
        args.emplace_back("horizontal_accuracy", horizontalAccuracy);
    }
    if (heading) {
        args.emplace_back("heading", heading);
    }
    if (proximityAlertRadius) {
        args.emplace_back("proximity_alert_radius", proximityAlertRadius);
    }
    if (replyMarkup) {
        args.emplace_back("reply_markup", _tgTypeParser.parseInlineKeyboardMarkup(replyMarkup));
    }
    return args;
}

std::vector<HttpReqArg> ApiBase::stopMessageLiveLocationArgs(boost::variant<std::int64_t, std::string> chatId,
                                        std::int32_t messageId,
                                        const std::string& inlineMessageId,
                                        InlineKeyboardMarkup::Ptr replyMarkup) const {
    std::vector<HttpReqArg> args;
    args.reserve(4);

    if (chatId.which() == 0) {   // std::int64_t
        if (boost::get<std::int64_t>(chatId) != 0) {
            args.emplace_back("chat_id", chatId);
        }
    } else {    // std::string
        if (boost::get<std::string>(chatId) != "") {
            args.emplace_back("chat_id", chatId);
        }
    }
    if (messageId) {
        args.emplace_back("message_id", messageId);
    }
    if (!inlineMessageId.empty()) {
        args.emplace_back("inline_message_id", inlineMessageId);
    }
    if (replyMarkup) {
        args.emplace_back("reply_markup", _tgTypeParser.parseInlineKeyboardMarkup(replyMarkup));
    }
    return args;
}

std::vector<HttpReqArg> ApiBase::sendVenueArgs(boost::variant<std::int64_t, std::string> chatId,
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
    std::vector<HttpReqArg> args;
    args.reserve(15);

    if (!businessConnectionId.empty()) {
        args.emplace_back("business_connection_id", businessConnectionId);
    }
    args.emplace_back("chat_id", chatId);
    if (messageThreadId != 0) {
        args.emplace_back("message_thread_id", messageThreadId);
    }
    args.emplace_back("latitude", latitude);
    args.emplace_back("longitude", longitude);
    args.emplace_back("title", title);
    args.emplace_back("address", address);
    if (!foursquareId.empty()) {
        args.emplace_back("foursquare_id", foursquareId);
    }
    if (!foursquareType.empty()) {
        args.emplace_back("foursquare_type", foursquareType);
    }
    if (!googlePlaceId.empty()) {
        args.emplace_back("google_place_id", googlePlaceId);
    }
    if (!googlePlaceType.empty()) {
        args.emplace_back("google_place_type", googlePlaceType);
    }
    if (disableNotification) {
        args.emplace_back("disable_notification", disableNotification);
    }
    if (protectContent) {
        args.emplace_back("protect_content", protectContent);
    }
    if (replyParameters != nullptr) {
        args.emplace_back("reply_parameters", _tgTypeParser.parseReplyParameters(replyParameters));
    }
    if (replyMarkup) {
        args.emplace_back("reply_markup", _tgTypeParser.parseGenericReply(replyMarkup));
    }
    return args;
}

std::vector<HttpReqArg> ApiBase::sendContactArgs(boost::variant<std::int64_t, std::string> chatId,
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
    std::vector<HttpReqArg> args;
    args.reserve(11);

    if (!businessConnectionId.empty()) {
        args.emplace_back("business_connection_id", businessConnectionId);
    }
    args.emplace_back("chat_id", chatId);
    if (messageThreadId != 0) {
        args.emplace_back("message_thread_id", messageThreadId);
    }
    args.emplace_back("phone_number", phoneNumber);
    args.emplace_back("first_name", firstName);
    if (!lastName.empty()) {
        args.emplace_back("last_name", lastName);
    }
    if (!vcard.empty()) {
        args.emplace_back("vcard", vcard);
    }
    if (disableNotification) {
        args.emplace_back("disable_notification", disableNotification);
    }
    if (protectContent) {
        args.emplace_back("protect_content", protectContent);
    }
    if (replyParameters != nullptr) {
        args.emplace_back("reply_parameters", _tgTypeParser.parseReplyParameters(replyParameters));
    }
    if (replyMarkup) {
        args.emplace_back("reply_markup", _tgTypeParser.parseGenericReply(replyMarkup));
    }
    return args;
}

std::vector<HttpReqArg> ApiBase::sendPollArgs(boost::variant<std::int64_t, std::string> chatId,
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
    std::vector<HttpReqArg> args;
    args.reserve(19);

    if (!businessConnectionId.empty()) {
        args.emplace_back("business_connection_id", businessConnectionId);
    }
    args.emplace_back("chat_id", chatId);
    if (messageThreadId != 0) {
        args.emplace_back("message_thread_id", messageThreadId);
    }
    args.emplace_back("question", question);
    args.emplace_back("options", _tgTypeParser.parseArray<std::string>(
        [](const std::string& option)->std::string {
        return "\"" + StringTools::escapeJsonString(option) + "\"";
    }, options));
    if (!isAnonymous) {
        args.emplace_back("is_anonymous", isAnonymous);
    }
    if (!type.empty()) {
        args.emplace_back("type", type);
    }
    if (allowsMultipleAnswers) {
        args.emplace_back("allows_multiple_answers", allowsMultipleAnswers);
    }
    if (correctOptionId != -1) {
        args.emplace_back("correct_option_id", correctOptionId);
    }
    if (!explanation.empty()) {
        args.emplace_back("explanation", explanation);
    }
    if (!explanationParseMode.empty()) {
        args.emplace_back("explanation_parse_mode", explanationParseMode);
    }
    if (!explanationEntities.empty()) {
        args.emplace_back("explanation_entities", _tgTypeParser.parseArray<MessageEntity>(&TgTypeParser::parseMessageEntity, explanationEntities));
    }
    if (openPeriod != 0) {
        args.emplace_back("open_period", openPeriod);
    }
    if (closeDate != 0) {
        args.emplace_back("close_date", closeDate);
    }
    if (isClosed) {
        args.emplace_back("is_closed", isClosed);
    }
    if (disableNotification) {
        args.emplace_back("disable_notification", disableNotification);
    }
    if (protectContent) {
        args.emplace_back("protect_content", protectContent);
    }
    if (replyParameters != nullptr) {
        args.emplace_back("reply_parameters", _tgTypeParser.parseReplyParameters(replyParameters));
    }
    if (replyMarkup) {
        args.emplace_back("reply_markup", _tgTypeParser.parseGenericReply(replyMarkup));
    }
    return args;
}

std::vector<HttpReqArg> ApiBase::sendDiceArgs(boost::variant<std::int64_t, std::string> chatId,
                        bool disableNotification,
                        ReplyParameters::Ptr replyParameters,
                        GenericReply::Ptr replyMarkup,
                        const std::string& emoji,
                        std::int32_t messageThreadId,
                        bool protectContent,
                        const std::string& businessConnectionId) const {
    std::vector<HttpReqArg> args;
    args.reserve(8);

    if (!businessConnectionId.empty()) {
        args.emplace_back("business_connection_id", businessConnectionId);
    }
    args.emplace_back("chat_id", chatId);
    if (messageThreadId != 0) {
        args.emplace_back("message_thread_id", messageThreadId);
    }
    if (!emoji.empty()) {
        args.emplace_back("emoji", emoji);
    }
    if (disableNotification) {
        args.emplace_back("disable_notification", disableNotification);
    }
    if (protectContent) {
        args.emplace_back("protect_content", protectContent);
    }
    if (replyParameters != nullptr) {
        args.emplace_back("reply_parameters", _tgTypeParser.parseReplyParameters(replyParameters));
    }
    if (replyMarkup) {
        args.emplace_back("reply_markup", _tgTypeParser.parseGenericReply(replyMarkup));
    }
    return args;
}

std::vector<HttpReqArg> ApiBase::setMessageReactionArgs(boost::variant<std::int64_t, std::string> chatId,
                            std::int32_t messageId,
                            const std::vector<ReactionType::Ptr>& reaction,
                            bool isBig) const {
    std::vector<HttpReqArg> args;
    args.reserve(4);

    args.emplace_back("chat_id", chatId);
    args.emplace_back("message_id", messageId);
    if (!reaction.empty()) {
        args.emplace_back("reaction", _tgTypeParser.parseArray<ReactionType>(&TgTypeParser::parseReactionType, reaction));
    }
    if (isBig) {
        args.emplace_back("is_big", isBig);
    }
    return args;
}

std::vector<HttpReqArg> ApiBase::sendChatActionArgs(std::int64_t chatId,
                        const std::string& action,
                        std::int32_t messageThreadId,
                        const std::string& businessConnectionId) const {
    std::vector<HttpReqArg> args;
    args.reserve(4);

    if (!businessConnectionId.empty()) {
        args.emplace_back("business_connection_id", businessConnectionId);
    }
    args.emplace_back("chat_id", chatId);
    args.emplace_back("action", action);
    if (messageThreadId != 0) {
        args.emplace_back("message_thread_id", messageThreadId);
    }
    return args;
}

std::vector<HttpReqArg> ApiBase::getUserProfilePhotosArgs(std::int64_t userId,
                                                std::int32_t offset,
                                                std::int32_t limit) const {
    std::vector<HttpReqArg> args;
    args.reserve(3);

    args.emplace_back("user_id", userId);
    if (offset) {
        args.emplace_back("offset", offset);
    }
    if (limit != 100) {
        args.emplace_back("limit", std::max(1, std::min(100, limit)));
    }
    return args;
}

std::vector<HttpReqArg> ApiBase::getFileArgs(const std::string& fileId) const {
    std::vector<HttpReqArg> args;
    args.reserve(1);

    args.emplace_back("file_id", fileId);
    return args;
}

std::vector<HttpReqArg> ApiBase::banChatMemberArgs(boost::variant<std::int64_t, std::string> chatId,
                    std::int64_t userId,
                    std::int32_t untilDate,
                    bool revokeMessages) const {
    std::vector<HttpReqArg> args;
    args.reserve(4);

    args.emplace_back("chat_id", chatId);
    args.emplace_back("user_id", userId);
    if (untilDate != 0) {
        args.emplace_back("until_date", untilDate);
    }
    if (revokeMessages) {
        args.emplace_back("revoke_messages", revokeMessages);
    }
    return args;
}

std::vector<HttpReqArg> ApiBase::unbanChatMemberArgs(boost::variant<std::int64_t, std::string> chatId,
                        std::int64_t userId,
                        bool onlyIfBanned) const {
    std::vector<HttpReqArg> args;
    args.reserve(3);

    args.emplace_back("chat_id", chatId);
    args.emplace_back("user_id", userId);
    if (onlyIfBanned) {
        args.emplace_back("only_if_banned", onlyIfBanned);
    }
    return args;
}

std::vector<HttpReqArg> ApiBase::restrictChatMemberArgs(boost::variant<std::int64_t, std::string> chatId,
                            std::int64_t userId,
                            TgBot::ChatPermissions::Ptr permissions,
                            std::uint32_t untilDate,
                            bool useIndependentChatPermissions) const {
    std::vector<HttpReqArg> args;
    args.reserve(5);

    args.emplace_back("chat_id", chatId);
    args.emplace_back("user_id", userId);
    args.emplace_back("permissions", _tgTypeParser.parseChatPermissions(permissions));
    if (useIndependentChatPermissions != false) {
        args.emplace_back("use_independent_chat_permissions", useIndependentChatPermissions);
    }
    if (untilDate != 0) {
        args.emplace_back("until_date", untilDate);
    }
    return args;
}

std::vector<HttpReqArg> ApiBase::promoteChatMemberArgs(boost::variant<std::int64_t, std::string> chatId,
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
    std::vector<HttpReqArg> args;
    args.reserve(17);

    args.emplace_back("chat_id", chatId);
    args.emplace_back("user_id", userId);
    if (isAnonymous != false) {
        args.emplace_back("is_anonymous", isAnonymous);
    }
    if (canManageChat != false) {
        args.emplace_back("can_manage_chat", canManageChat);
    }
    if (canPostMessages != false) {
        args.emplace_back("can_post_messages", canPostMessages);
    }
    if (canEditMessages != false) {
        args.emplace_back("can_edit_messages", canEditMessages);
    }
    if (canDeleteMessages != false) {
        args.emplace_back("can_delete_messages", canDeleteMessages);
    }
    if (canPostStories != false) {
        args.emplace_back("can_post_stories", canPostStories);
    }
    if (canEditStories != false) {
        args.emplace_back("can_edit_stories", canEditStories);
    }
    if (canDeleteStories != false) {
        args.emplace_back("can_delete_stories", canDeleteStories);
    }
    if (canManageVideoChats != false) {
        args.emplace_back("can_manage_video_chats", canManageVideoChats);
    }
    if (canRestrictMembers != false) {
        args.emplace_back("can_restrict_members", canRestrictMembers);
    }
    if (canPromoteMembers != false) {
        args.emplace_back("can_promote_members", canPromoteMembers);
    }
    if (canChangeInfo != false) {
        args.emplace_back("can_change_info", canChangeInfo);
    }
    if (canInviteUsers != false) {
        args.emplace_back("can_invite_users", canInviteUsers);
    }
    if (canPinMessages != false) {
        args.emplace_back("can_pin_messages", canPinMessages);
    }
    if (canManageTopics != false) {
        args.emplace_back("can_manage_topics", canManageTopics);
    }
    return args;
}

std::vector<HttpReqArg> ApiBase::setChatAdministratorCustomTitleArgs(boost::variant<std::int64_t, std::string> chatId,
                                        std::int64_t userId,
                                        const std::string& customTitle) const {
    std::vector<HttpReqArg> args;
    args.reserve(3);

    args.emplace_back("chat_id", chatId);
    args.emplace_back("user_id", userId);
    args.emplace_back("custom_title", customTitle);
    return args;
}

std::vector<HttpReqArg> ApiBase::banChatSenderChatArgs(boost::variant<std::int64_t, std::string> chatId,
                        std::int64_t senderChatId) const {
    std::vector<HttpReqArg> args;
    args.reserve(2);

    args.emplace_back("chat_id", chatId);
    args.emplace_back("sender_chat_id", senderChatId);
    return args;
}

std::vector<HttpReqArg> ApiBase::unbanChatSenderChatArgs(boost::variant<std::int64_t, std::string> chatId,
                            std::int64_t senderChatId) const {
    std::vector<HttpReqArg> args;
    args.reserve(2);

    args.emplace_back("chat_id", chatId);
    args.emplace_back("sender_chat_id", senderChatId);
    return args;
}

std::vector<HttpReqArg> ApiBase::setChatPermissionsArgs(boost::variant<std::int64_t, std::string> chatId,
                            ChatPermissions::Ptr permissions,
                            bool useIndependentChatPermissions) const {
    std::vector<HttpReqArg> args;
    args.reserve(3);

    args.emplace_back("chat_id", chatId);
    args.emplace_back("permissions", _tgTypeParser.parseChatPermissions(permissions));
    if (useIndependentChatPermissions) {
        args.emplace_back("use_independent_chat_permissions", useIndependentChatPermissions);
    }
    return args;
}

std::vector<HttpReqArg> ApiBase::exportChatInviteLinkArgs(boost::variant<std::int64_t, std::string> chatId) const {
    std::vector<HttpReqArg> args;
    args.reserve(1);

    args.emplace_back("chat_id", chatId);
    return args;
}

std::vector<HttpReqArg> ApiBase::createChatInviteLinkArgs(boost::variant<std::int64_t, std::string> chatId,
                                            std::int32_t expireDate,
                                            std::int32_t memberLimit,
                                            const std::string& name,
                                            bool createsJoinRequest) const {
    std::vector<HttpReqArg> args;
    args.reserve(5);

    args.emplace_back("chat_id", chatId);
    if (!name.empty()) {
        args.emplace_back("name", name);
    }
    if (expireDate != 0) {
        args.emplace_back("expire_date", expireDate);
    }
    if (memberLimit != 0) {
        args.emplace_back("member_limit", memberLimit);
    }
    if (createsJoinRequest) {
        args.emplace_back("createsJoinRequest", createsJoinRequest);
    }
    return args;
}

std::vector<HttpReqArg> ApiBase::editChatInviteLinkArgs(boost::variant<std::int64_t, std::string> chatId,
                                        const std::string& inviteLink,
                                        std::int32_t expireDate,
                                        std::int32_t memberLimit,
                                        const std::string& name,
                                        bool createsJoinRequest) const {
    std::vector<HttpReqArg> args;
    args.reserve(6);

    args.emplace_back("chat_id", chatId);
    args.emplace_back("invite_link", inviteLink);
    if (!name.empty()) {
        args.emplace_back("name", name);
    }
    if (expireDate != 0) {
        args.emplace_back("expire_date", expireDate);
    }
    if (memberLimit != 0) {
        args.emplace_back("member_limit", memberLimit);
    }
    if (createsJoinRequest) {
        args.emplace_back("createsJoinRequest", createsJoinRequest);
    }
    return args;
}

std::vector<HttpReqArg> ApiBase::revokeChatInviteLinkArgs(boost::variant<std::int64_t, std::string> chatId,
                                            const std::string& inviteLink) const {
    std::vector<HttpReqArg> args;
    args.reserve(2);

    args.emplace_back("chat_id", chatId);
    args.emplace_back("invite_link", inviteLink);
    return args;
}

std::vector<HttpReqArg> ApiBase::approveChatJoinRequestArgs(boost::variant<std::int64_t, std::string> chatId,
                                std::int64_t userId) const {
    std::vector<HttpReqArg> args;
    args.reserve(2);

    args.emplace_back("chat_id", chatId);
    args.emplace_back("user_id", userId);
    return args;
}

std::vector<HttpReqArg> ApiBase::declineChatJoinRequestArgs(boost::variant<std::int64_t, std::string> chatId,
                                std::int64_t userId) const {
    std::vector<HttpReqArg> args;
    args.reserve(2);

    args.emplace_back("chat_id", chatId);
    args.emplace_back("user_id", userId);
    return args;
}

std::vector<HttpReqArg> ApiBase::setChatPhotoArgs(boost::variant<std::int64_t, std::string> chatId,
                    const InputFile::Ptr photo) const {
    std::vector<HttpReqArg> args;
    args.reserve(2);

    args.emplace_back("chat_id", chatId);
    args.emplace_back("photo", photo->data, true, photo->mimeType, photo->fileName);
    return args;
}

std::vector<HttpReqArg> ApiBase::deleteChatPhotoArgs(boost::variant<std::int64_t, std::string> chatId) const {
    std::vector<HttpReqArg> args;
    args.reserve(1);

    args.emplace_back("chat_id", chatId);
    return args;
}

std::vector<HttpReqArg> ApiBase::setChatTitleArgs(boost::variant<std::int64_t, std::string> chatId,
                    const std::string& title) const {
    std::vector<HttpReqArg> args;
    args.reserve(2);

    args.emplace_back("chat_id", chatId);
    args.emplace_back("title", title);
    return args;
}

std::vector<HttpReqArg> ApiBase::setChatDescriptionArgs(boost::variant<std::int64_t, std::string> chatId,
                            const std::string& description) const {
    std::vector<HttpReqArg> args;
    args.reserve(2);

    args.emplace_back("chat_id", chatId);
    if (!description.empty()) {
        args.emplace_back("description", description);
    }
    return args;
}

std::vector<HttpReqArg> ApiBase::pinChatMessageArgs(boost::variant<std::int64_t, std::string> chatId,
                        std::int32_t messageId,
                        bool disableNotification) const {
    std::vector<HttpReqArg> args;
    args.reserve(3);

    args.emplace_back("chat_id", chatId);
    args.emplace_back("message_id", messageId);
    if (disableNotification) {
        args.emplace_back("disable_notification", disableNotification);
    }
    return args;
}

std::vector<HttpReqArg> ApiBase::unpinChatMessageArgs(boost::variant<std::int64_t, std::string> chatId,
                        std::int32_t messageId) const {
    std::vector<HttpReqArg> args;
    args.reserve(2);

    args.emplace_back("chat_id", chatId);
    if (messageId != 0) {
        args.emplace_back("message_id", messageId);
    }
    return args;
}

std::vector<HttpReqArg> ApiBase::unpinAllChatMessagesArgs(boost::variant<std::int64_t, std::string> chatId) const {
    std::vector<HttpReqArg> args;
    args.reserve(1);

    args.emplace_back("chat_id", chatId);
    return args;
}

std::vector<HttpReqArg> ApiBase::leaveChatArgs(boost::variant<std::int64_t, std::string> chatId) const {
    std::vector<HttpReqArg> args;
    args.reserve(1);

    args.emplace_back("chat_id", chatId);
    return args;
}

std::vector<HttpReqArg> ApiBase::getChatArgs(boost::variant<std::int64_t, std::string> chatId) const {
    std::vector<HttpReqArg> args;
    args.reserve(1);

    args.emplace_back("chat_id", chatId);
    return args;
}

std::vector<HttpReqArg> ApiBase::getChatAdministratorsArgs(boost::variant<std::int64_t, std::string> chatId) const {
    std::vector<HttpReqArg> args;
    args.reserve(1);

    args.emplace_back("chat_id", chatId);
    return args;
}

std::vector<HttpReqArg> ApiBase::getChatMemberCountArgs(boost::variant<std::int64_t, std::string> chatId) const {
    std::vector<HttpReqArg> args;
    args.reserve(1);

    args.emplace_back("chat_id", chatId);
    return args;
}

std::vector<HttpReqArg> ApiBase::getChatMemberArgs(boost::variant<std::int64_t, std::string> chatId,
                                std::int64_t userId) const {
    std::vector<HttpReqArg> args;
    args.reserve(2);

    args.emplace_back("chat_id", chatId);
    args.emplace_back("user_id", userId);
    return args;
}

std::vector<HttpReqArg> ApiBase::setChatStickerSetArgs(boost::variant<std::int64_t, std::string> chatId,
                            const std::string& stickerSetName) const {
    std::vector<HttpReqArg> args;
    args.reserve(2);

    args.emplace_back("chat_id", chatId);
    args.emplace_back("sticker_set_name	", stickerSetName);
    return args;
}

std::vector<HttpReqArg> ApiBase::deleteChatStickerSetArgs(boost::variant<std::int64_t, std::string> chatId) const {
    std::vector<HttpReqArg> args;
    args.reserve(1);

    args.emplace_back("chat_id", chatId);
    return args;
}

std::vector<HttpReqArg> ApiBase::createForumTopicArgs(boost::variant<std::int64_t, std::string> chatId,
                                      const std::string& name,
                                      std::int32_t iconColor,
                                      const std::string& iconCustomEmojiId) const {
    std::vector<HttpReqArg> args;
    args.reserve(4);

    args.emplace_back("chat_id", chatId);
    args.emplace_back("name", name);
    if (iconColor != 0) {
        args.emplace_back("icon_color", iconColor);
    }
    if (!iconCustomEmojiId.empty()) {
        args.emplace_back("icon_custom_emoji_id", iconCustomEmojiId);
    }

    return args;
}

std::vector<HttpReqArg> ApiBase::editForumTopicArgs(boost::variant<std::int64_t, std::string> chatId,
                         std::int32_t messageThreadId,
                         const std::string& name,
                         boost::variant<std::int32_t, std::string> iconCustomEmojiId) const {
    std::vector<HttpReqArg> args;
    args.reserve(4);

    args.emplace_back("chat_id", chatId);
    args.emplace_back("message_thread_id", messageThreadId);
    if (!name.empty()) {
        args.emplace_back("name", name);
    }
    if (iconCustomEmojiId.which() == 0) {   // std::int32_t
        if (boost::get<std::int32_t>(iconCustomEmojiId) != 0) {
            args.emplace_back("icon_custom_emoji_id", iconCustomEmojiId);
        }
    } else {    // std::string
        if (boost::get<std::string>(iconCustomEmojiId) != "") {
            args.emplace_back("icon_custom_emoji_id", iconCustomEmojiId);
        }
    }

    return args;
}

std::vector<HttpReqArg> ApiBase::closeForumTopicArgs(boost::variant<std::int64_t, std::string> chatId,
                          std::int32_t messageThreadId) const {
    std::vector<HttpReqArg> args;
    args.reserve(2);

    args.emplace_back("chat_id", chatId);
    args.emplace_back("message_thread_id", messageThreadId);

    return args;
 }

std::vector<HttpReqArg> ApiBase::reopenForumTopicArgs(boost::variant<std::int64_t, std::string> chatId,
                           std::int32_t messageThreadId) const {
    std::vector<HttpReqArg> args;
    args.reserve(2);

    args.emplace_back("chat_id", chatId);
    args.emplace_back("message_thread_id", messageThreadId);

    return args;
}

std::vector<HttpReqArg> ApiBase::deleteForumTopicArgs(boost::variant<std::int64_t, std::string> chatId,
                           std::int32_t messageThreadId) const {
    std::vector<HttpReqArg> args;
    args.reserve(2);

    args.emplace_back("chat_id", chatId);
    args.emplace_back("message_thread_id", messageThreadId);

    return args;
}

std::vector<HttpReqArg> ApiBase::unpinAllForumTopicMessagesArgs(boost::variant<std::int64_t, std::string> chatId,
                                     std::int32_t messageThreadId) const {
    std::vector<HttpReqArg> args;
    args.reserve(2);

    args.emplace_back("chat_id", chatId);
    args.emplace_back("message_thread_id", messageThreadId);

    return args;
}

std::vector<HttpReqArg> ApiBase::editGeneralForumTopicArgs(boost::variant<std::int64_t, std::string> chatId,
                                std::string name) const {
    std::vector<HttpReqArg> args;
    args.reserve(2);

    args.emplace_back("chat_id", chatId);
    args.emplace_back("name", name);

    return args;
}

std::vector<HttpReqArg> ApiBase::closeGeneralForumTopicArgs(boost::variant<std::int64_t, std::string> chatId) const {
    std::vector<HttpReqArg> args;
    args.reserve(1);

    args.emplace_back("chat_id", chatId);

    return args;
}

std::vector<HttpReqArg> ApiBase::reopenGeneralForumTopicArgs(boost::variant<std::int64_t, std::string> chatId) const {
    std::vector<HttpReqArg> args;
    args.reserve(1);

    args.emplace_back("chat_id", chatId);

    return args;
}

std::vector<HttpReqArg> ApiBase::hideGeneralForumTopicArgs(boost::variant<std::int64_t, std::string> chatId) const {
    std::vector<HttpReqArg> args;
    args.reserve(1);

    args.emplace_back("chat_id", chatId);

    return args;
}

std::vector<HttpReqArg> ApiBase::unhideGeneralForumTopicArgs(boost::variant<std::int64_t, std::string> chatId) const {
    std::vector<HttpReqArg> args;
    args.reserve(1);

    args.emplace_back("chat_id", chatId);

    return args;
}

std::vector<HttpReqArg> ApiBase::unpinAllGeneralForumTopicMessagesArgs(boost::variant<std::int64_t, std::string> chatId) const {
    std::vector<HttpReqArg> args;
    args.reserve(1);

    args.emplace_back("chat_id", chatId);

    return args;
}

std::vector<HttpReqArg> ApiBase::answerCallbackQueryArgs(const std::string& callbackQueryId,
                              const std::string& text,
                              bool showAlert,
                              const std::string& url,
                              std::int32_t cacheTime) const {
    std::vector<HttpReqArg> args;
    args.reserve(5);

    args.emplace_back("callback_query_id", callbackQueryId);
    if (!text.empty()) {
        args.emplace_back("text", text);
    }
    if (showAlert) {
        args.emplace_back("show_alert", showAlert);
    }
    if (!url.empty()) {
        args.emplace_back("url", url);
    }
    if (cacheTime) {
        args.emplace_back("cache_time", cacheTime);
    }

    return args;
}

std::vector<HttpReqArg> ApiBase::getUserChatBoostsArgs(boost::variant<std::int64_t, std::string> chatId,
                                           std::int32_t userId) const {
    std::vector<HttpReqArg> args;
    args.reserve(2);

    args.emplace_back("chat_id", chatId);
    args.emplace_back("user_id", userId);

    return args;
}

std::vector<HttpReqArg> ApiBase::getBusinessConnectionArgs(const std::string& businessConnectionId) const {
    std::vector<HttpReqArg> args;
    args.reserve(1);

    args.emplace_back("business_connection_id", businessConnectionId);

    return args;
}

std::vector<HttpReqArg> ApiBase::setMyCommandsArgs(const std::vector<BotCommand::Ptr>& commands,
                        BotCommandScope::Ptr scope,
                        const std::string& languageCode) const {
    std::vector<HttpReqArg> args;
    args.reserve(3);

    args.emplace_back("commands", _tgTypeParser.parseArray<BotCommand>(&TgTypeParser::parseBotCommand, commands));
    if (scope != nullptr) {
        args.emplace_back("scope", _tgTypeParser.parseBotCommandScope(scope));
    }
    if (!languageCode.empty()) {
        args.emplace_back("language_code", languageCode);
    }

    return args;
}

std::vector<HttpReqArg> ApiBase::deleteMyCommandsArgs(BotCommandScope::Ptr scope,
                           const std::string& languageCode) const {
    std::vector<HttpReqArg> args;
    args.reserve(2);

    if (scope != nullptr) {
        args.emplace_back("scope", _tgTypeParser.parseBotCommandScope(scope));
    }
    if (!languageCode.empty()) {
        args.emplace_back("language_code", languageCode);
    }

    return args;
}

std::vector<HttpReqArg> ApiBase::getMyCommandsArgs(BotCommandScope::Ptr scope,
                                                const std::string& languageCode) const {
    std::vector<HttpReqArg> args;
    args.reserve(2);

    if (scope != nullptr) {
        args.emplace_back("scope", _tgTypeParser.parseBotCommandScope(scope));
    }
    if (!languageCode.empty()) {
        args.emplace_back("language_code", languageCode);
    }

    return args;
}

std::vector<HttpReqArg> ApiBase::setMyNameArgs(const std::string& name,
                    const std::string& languageCode) const {
    std::vector<HttpReqArg> args;
    args.reserve(2);

    if (!name.empty()) {
        args.emplace_back("name", name);
    }
    if (!languageCode.empty()) {
        args.emplace_back("language_code", languageCode);
    }

    return args;
}

std::vector<HttpReqArg> ApiBase::getMyNameArgs(const std::string& languageCode) const {
    std::vector<HttpReqArg> args;
    args.reserve(1);

    if (!languageCode.empty()) {
        args.emplace_back("language_code", languageCode);
    }

    return args;
}

std::vector<HttpReqArg> ApiBase::setMyDescriptionArgs(const std::string& description,
                           const std::string& languageCode) const {
    std::vector<HttpReqArg> args;
    args.reserve(2);

    if (!description.empty()) {
        args.emplace_back("description", description);
    }
    if (!languageCode.empty()) {
        args.emplace_back("language_code", languageCode);
    }

    return args;
}

std::vector<HttpReqArg> ApiBase::getMyDescriptionArgs(const std::string& languageCode) const {
    std::vector<HttpReqArg> args;
    args.reserve(1);

    if (!languageCode.empty()) {
        args.emplace_back("language_code", languageCode);
    }

    return args;
}

std::vector<HttpReqArg> ApiBase::setMyShortDescriptionArgs(const std::string& shortDescription,
                                const std::string& languageCode) const {
    std::vector<HttpReqArg> args;
    args.reserve(2);

    if (!shortDescription.empty()) {
        args.emplace_back("short_description", shortDescription);
    }
    if (!languageCode.empty()) {
        args.emplace_back("language_code", languageCode);
    }

    return args;
}

std::vector<HttpReqArg> ApiBase::getMyShortDescriptionArgs(const std::string& languageCode) const {
    std::vector<HttpReqArg> args;
    args.reserve(1);

    if (!languageCode.empty()) {
        args.emplace_back("language_code", languageCode);
    }

    return args;
}

std::vector<HttpReqArg> ApiBase::setChatMenuButtonArgs(std::int64_t chatId,
                            MenuButton::Ptr menuButton) const {
    std::vector<HttpReqArg> args;
    args.reserve(2);

    if (chatId != 0) {
        args.emplace_back("chat_id", chatId);
    }
    if (menuButton != nullptr) {
        args.emplace_back("menu_button", _tgTypeParser.parseMenuButton(menuButton));
    }

    return args;
}

std::vector<HttpReqArg> ApiBase::getChatMenuButtonArgs(std::int64_t chatId) const {
    std::vector<HttpReqArg> args;
    args.reserve(1);

    if (chatId != 0) {
        args.emplace_back("chat_id", chatId);
    }

    return args;
}

std::vector<HttpReqArg> ApiBase::setMyDefaultAdministratorRightsArgs(ChatAdministratorRights::Ptr rights,
                                          bool forChannels) const {
    std::vector<HttpReqArg> args;
    args.reserve(2);

    if (rights != nullptr) {
        args.emplace_back("rights", _tgTypeParser.parseChatAdministratorRights(rights));
    }
    if (forChannels) {
        args.emplace_back("for_channels", forChannels);
    }

    return args;
}

std::vector<HttpReqArg> ApiBase::getMyDefaultAdministratorRightsArgs(bool forChannels) const {
    std::vector<HttpReqArg> args;
    args.reserve(1);

    if (forChannels) {
        args.emplace_back("for_channels", forChannels);
    }

    return args;
}

std::vector<HttpReqArg> ApiBase::editMessageTextArgs(const std::string& text,
                                  boost::variant<std::int64_t, std::string> chatId,
                                  std::int32_t messageId,
                                  const std::string& inlineMessageId,
                                  const std::string& parseMode,
                                  LinkPreviewOptions::Ptr linkPreviewOptions,
                                  InlineKeyboardMarkup::Ptr replyMarkup,
                                  const std::vector<MessageEntity::Ptr>& entities) const {
    std::vector<HttpReqArg> args;
    args.reserve(8);

    if (chatId.which() == 0) {   // std::int64_t
        if (boost::get<std::int64_t>(chatId) != 0) {
            args.emplace_back("chat_id", chatId);
        }
    } else {    // std::string
        if (boost::get<std::string>(chatId) != "") {
            args.emplace_back("chat_id", chatId);
        }
    }
    if (messageId) {
        args.emplace_back("message_id", messageId);
    }
    if (!inlineMessageId.empty()) {
        args.emplace_back("inline_message_id", inlineMessageId);
    }
    args.emplace_back("text", text);
    if (!parseMode.empty()) {
        args.emplace_back("parse_mode", parseMode);
    }
    if (!entities.empty()) {
        args.emplace_back("entities", _tgTypeParser.parseArray<MessageEntity>(&TgTypeParser::parseMessageEntity, entities));
    }
    if (linkPreviewOptions) {
        args.emplace_back("link_preview_options", _tgTypeParser.parseLinkPreviewOptions(linkPreviewOptions));
    }
    if (replyMarkup) {
        args.emplace_back("reply_markup", _tgTypeParser.parseInlineKeyboardMarkup(replyMarkup));
    }

    return args;
}

std::vector<HttpReqArg> ApiBase::editMessageCaptionArgs(boost::variant<std::int64_t, std::string> chatId,
                                     std::int32_t messageId,
                                     const std::string& caption,
                                     const std::string& inlineMessageId,
                                     GenericReply::Ptr replyMarkup,
                                     const std::string& parseMode,
                                     const std::vector<MessageEntity::Ptr>& captionEntities) const {
    std::vector<HttpReqArg> args;
    args.reserve(7);

    if (chatId.which() == 0) {   // std::int64_t
        if (boost::get<std::int64_t>(chatId) != 0) {
            args.emplace_back("chat_id", chatId);
        }
    } else {    // std::string
        if (boost::get<std::string>(chatId) != "") {
            args.emplace_back("chat_id", chatId);
        }
    }
    if (messageId) {
        args.emplace_back("message_id", messageId);
    }
    if (!inlineMessageId.empty()) {
        args.emplace_back("inline_message_id", inlineMessageId);
    }
    if (!caption.empty()) {
        args.emplace_back("caption", caption);
    }
    if (!parseMode.empty()) {
        args.emplace_back("parse_mode", parseMode);
    }
    if (!captionEntities.empty()) {
        args.emplace_back("caption_entities", _tgTypeParser.parseArray<MessageEntity>(&TgTypeParser::parseMessageEntity, captionEntities));
    }
    if (replyMarkup) {
        args.emplace_back("reply_markup", _tgTypeParser.parseGenericReply(replyMarkup));
    }

    return args;
}

std::vector<HttpReqArg> ApiBase::editMessageMediaArgs(InputMedia::Ptr media,
                                   boost::variant<std::int64_t, std::string> chatId,
                                   std::int32_t messageId,
                                   const std::string& inlineMessageId,
                                   GenericReply::Ptr replyMarkup) const {

    std::vector<HttpReqArg> args;
    args.reserve(5);

    if (chatId.which() == 0) {   // std::int64_t
        if (boost::get<std::int64_t>(chatId) != 0) {
            args.emplace_back("chat_id", chatId);
        }
    } else {    // std::string
        if (boost::get<std::string>(chatId) != "") {
            args.emplace_back("chat_id", chatId);
        }
    }
    args.emplace_back("media", _tgTypeParser.parseInputMedia(media));
    if (messageId) {
        args.emplace_back("message_id", messageId);
    }
    if (!inlineMessageId.empty()) {
        args.emplace_back("inline_message_id", inlineMessageId);
    }
    if (replyMarkup) {
        args.emplace_back("reply_markup", _tgTypeParser.parseGenericReply(replyMarkup));
    }

    return args;
}

std::vector<HttpReqArg> ApiBase::editMessageReplyMarkupArgs(boost::variant<std::int64_t, std::string> chatId,
                                         std::int32_t messageId,
                                         const std::string& inlineMessageId,
                                         const GenericReply::Ptr replyMarkup) const {

    std::vector<HttpReqArg> args;
    args.reserve(4);

    if (chatId.which() == 0) {   // std::int64_t
        if (boost::get<std::int64_t>(chatId) != 0) {
            args.emplace_back("chat_id", chatId);
        }
    } else {    // std::string
        if (boost::get<std::string>(chatId) != "") {
            args.emplace_back("chat_id", chatId);
        }
    }
    if (messageId) {
        args.emplace_back("message_id", messageId);
    }
    if (!inlineMessageId.empty()) {
        args.emplace_back("inline_message_id", inlineMessageId);
    }
    if (replyMarkup) {
        args.emplace_back("reply_markup", _tgTypeParser.parseGenericReply(replyMarkup));
    }

    return args;
}

std::vector<HttpReqArg> ApiBase::stopPollArgs(boost::variant<std::int64_t, std::string> chatId,
                        std::int64_t messageId,
                        const InlineKeyboardMarkup::Ptr replyMarkup) const {
    std::vector<HttpReqArg> args;
    args.reserve(3);

    args.emplace_back("chat_id", chatId);
    args.emplace_back("message_id", messageId);
    if (replyMarkup) {
        args.emplace_back("reply_markup", _tgTypeParser.parseGenericReply(replyMarkup));
    }

    return args;
}

std::vector<HttpReqArg> ApiBase::deleteMessageArgs(boost::variant<std::int64_t, std::string> chatId,
                        std::int32_t messageId) const {
    std::vector<HttpReqArg> args;
    args.reserve(2);

    args.emplace_back("chat_id", chatId);
    args.emplace_back("message_id", messageId);

    return args;
}

std::vector<HttpReqArg> ApiBase::deleteMessagesArgs(boost::variant<std::int64_t, std::string> chatId,
                         const std::vector<std::int32_t>& messageIds) const {
    std::vector<HttpReqArg> args;
    args.reserve(2);

    args.emplace_back("chat_id", chatId);
    if (!messageIds.empty()) {
        args.emplace_back("message_ids", _tgTypeParser.parseArray<std::int32_t>(
            [] (const std::int32_t& i)->std::int32_t {
            return i;
        }, messageIds));
    }

    return args;
}

std::vector<HttpReqArg> ApiBase::sendStickerArgs(boost::variant<std::int64_t, std::string> chatId,
                              boost::variant<InputFile::Ptr, std::string> sticker,
                              ReplyParameters::Ptr replyParameters,
                              GenericReply::Ptr replyMarkup,
                              bool disableNotification,
                              std::int32_t messageThreadId,
                              bool protectContent,
                              const std::string& emoji,
                              const std::string& businessConnectionId) const {
    std::vector<HttpReqArg> args;
    args.reserve(9);

    if (!businessConnectionId.empty()) {
        args.emplace_back("business_connection_id", businessConnectionId);
    }
    args.emplace_back("chat_id", chatId);
    if (messageThreadId != 0) {
        args.emplace_back("message_thread_id", messageThreadId);
    }
    if (sticker.which() == 0) { // InputFile::Ptr
        auto file = boost::get<InputFile::Ptr>(sticker);
        args.emplace_back("sticker", file->data, true, file->mimeType, file->fileName);
    } else {    // std::string
        args.emplace_back("sticker", boost::get<std::string>(sticker));
    }
    if (!emoji.empty()) {
        args.emplace_back("emoji", emoji);
    }
    if (disableNotification) {
        args.emplace_back("disable_notification", disableNotification);
    }
    if (protectContent) {
        args.emplace_back("protect_content", protectContent);
    }
    if (replyParameters != nullptr) {
        args.emplace_back("reply_parameters", _tgTypeParser.parseReplyParameters(replyParameters));
    }
    if (replyMarkup) {
        args.emplace_back("reply_markup", _tgTypeParser.parseGenericReply(replyMarkup));
    }
    
    return args;
}

std::vector<HttpReqArg> ApiBase::getStickerSetArgs(const std::string& name) const {
    std::vector<HttpReqArg> args;
    args.reserve(1);

    args.emplace_back("name", name);

    return args;
}

std::vector<HttpReqArg> ApiBase::getCustomEmojiStickersArgs(const std::vector<std::string>& customEmojiIds) const {
    std::vector<HttpReqArg> args;
    args.reserve(1);

    args.emplace_back("custom_emoji_ids", _tgTypeParser.parseArray<std::string>(
        [] (const std::string& customEmojiId) -> std::string {
        return "\"" + StringTools::escapeJsonString(customEmojiId) + "\"";
    }, customEmojiIds));

    return args;
}

std::vector<HttpReqArg> ApiBase::uploadStickerFileArgs(std::int64_t userId,
                                 InputFile::Ptr sticker,
                                 const std::string& stickerFormat) const {
    std::vector<HttpReqArg> args;
    args.reserve(3);

    args.emplace_back("user_id", userId);
    args.emplace_back("sticker", sticker->data, true, sticker->mimeType, sticker->fileName);
    args.emplace_back("sticker_format", stickerFormat);

    return args;
}

std::vector<HttpReqArg> ApiBase::createNewStickerSetArgs(std::int64_t userId,
                              const std::string& name,
                              const std::string& title,
                              const std::vector<InputSticker::Ptr>& stickers,
                              Sticker::Type stickerType,
                              bool needsRepainting) const {
    std::vector<HttpReqArg> args;
    args.reserve(6);

    args.emplace_back("user_id", userId);
    args.emplace_back("name", name);
    args.emplace_back("title", title);
    args.emplace_back("stickers", _tgTypeParser.parseArray<InputSticker>(&TgTypeParser::parseInputSticker, stickers));
    if (stickerType == Sticker::Type::Regular) {
        args.emplace_back("sticker_type", "regular");
    } else if (stickerType == Sticker::Type::Mask) {
        args.emplace_back("sticker_type", "mask");
    } else if (stickerType == Sticker::Type::CustomEmoji) {
        args.emplace_back("sticker_type", "custom_emoji");
    }
    if (needsRepainting) {
        args.emplace_back("needs_repainting", needsRepainting);
    }

    return args;
}

std::vector<HttpReqArg> ApiBase::addStickerToSetArgs(std::int64_t userId,
                          const std::string& name,
                          InputSticker::Ptr sticker) const {
    std::vector<HttpReqArg> args;
    args.reserve(3);

    args.emplace_back("user_id", userId);
    args.emplace_back("name", name);
    args.emplace_back("sticker", _tgTypeParser.parseInputSticker(sticker));

    return args;
}

std::vector<HttpReqArg> ApiBase::setStickerPositionInSetArgs(const std::string& sticker,
                                  std::int32_t position) const {
    std::vector<HttpReqArg> args;
    args.reserve(2);

    args.emplace_back("sticker", sticker);
    args.emplace_back("position", position);

    return args;
}

std::vector<HttpReqArg> ApiBase::deleteStickerFromSetArgs(const std::string& sticker) const {
    std::vector<HttpReqArg> args;
    args.reserve(1);

    args.emplace_back("sticker", sticker);

    return args;
}

std::vector<HttpReqArg> ApiBase::replaceStickerInSetArgs(std::int64_t userId,
                             const std::string& name,
                             const std::string& oldSticker,
                             InputSticker::Ptr sticker) const {
    std::vector<HttpReqArg> args;
    args.reserve(4);

    args.emplace_back("user_id", userId);
    args.emplace_back("name", name);
    args.emplace_back("old_sticker", oldSticker);
    args.emplace_back("sticker", _tgTypeParser.parseInputSticker(sticker));

    return args;
}

std::vector<HttpReqArg> ApiBase::setStickerEmojiListArgs(const std::string& sticker,
                              const std::vector<std::string>& emojiList) const {
    std::vector<HttpReqArg> args;
    args.reserve(2);

    args.emplace_back("sticker", sticker);
    args.emplace_back("emoji_list", _tgTypeParser.parseArray<std::string>(
        [](const std::string& emoji)->std::string {
        return "\"" + StringTools::escapeJsonString(emoji) + "\"";
    }, emojiList));

    return args;
}

std::vector<HttpReqArg> ApiBase::setStickerKeywordsArgs(const std::string& sticker,
                             const std::vector<std::string>& keywords) const {
    std::vector<HttpReqArg> args;
    args.reserve(2);

    args.emplace_back("sticker", sticker);
    if (!keywords.empty()) {
        args.emplace_back("keywords", _tgTypeParser.parseArray<std::string>(
            [](const std::string& keyword)->std::string {
            return "\"" + StringTools::escapeJsonString(keyword) + "\"";
        }, keywords));
    }

    return args;
}

std::vector<HttpReqArg> ApiBase::setStickerMaskPositionArgs(const std::string& sticker,
                                 MaskPosition::Ptr maskPosition) const {
    std::vector<HttpReqArg> args;
    args.reserve(2);

    args.emplace_back("sticker", sticker);
    if (maskPosition != nullptr) {
        args.emplace_back("mask_position", _tgTypeParser.parseMaskPosition(maskPosition));
    }

    return args;
}

std::vector<HttpReqArg> ApiBase::setStickerSetTitleArgs(const std::string& name,
                             const std::string& title) const {
    std::vector<HttpReqArg> args;
    args.reserve(2);

    args.emplace_back("name", name);
    args.emplace_back("title", title);

    return args;
}

std::vector<HttpReqArg> ApiBase::setStickerSetThumbnailArgs(const std::string& name,
                                 std::int64_t userId,
                                 const std::string& format,
                                 boost::variant<InputFile::Ptr, std::string> thumbnail) const {
    std::vector<HttpReqArg> args;
    args.reserve(4);

    args.emplace_back("name", name);
    args.emplace_back("user_id", userId);
    args.emplace_back("format", format);
    if (thumbnail.which() == 0) {   // InputFile::Ptr
        if (boost::get<InputFile::Ptr>(thumbnail) != nullptr) {
            auto file = boost::get<InputFile::Ptr>(thumbnail);
            args.emplace_back("thumbnail", file->data, true, file->mimeType, file->fileName);
        }
    } else {    // std::string
        if (boost::get<std::string>(thumbnail) != "") {
            args.emplace_back("thumbnail", boost::get<std::string>(thumbnail));
        }
    }

    return args;
}

std::vector<HttpReqArg> ApiBase::setCustomEmojiStickerSetThumbnailArgs(const std::string& name,
                                            const std::string& customEmojiId) const {
    std::vector<HttpReqArg> args;
    args.reserve(2);

    args.emplace_back("name", name);
    if (!customEmojiId.empty()) {
        args.emplace_back("custom_emoji_id", customEmojiId);
    }

    return args;
}

std::vector<HttpReqArg> ApiBase::deleteStickerSetArgs(const std::string& name) const {
    std::vector<HttpReqArg> args;
    args.reserve(1);

    args.emplace_back("name", name);

    return args;
}

std::vector<HttpReqArg> ApiBase::answerInlineQueryArgs(const std::string& inlineQueryId,
                            const std::vector<InlineQueryResult::Ptr>& results,
                            std::int32_t cacheTime,
                            bool isPersonal,
                            const std::string& nextOffset,
                            InlineQueryResultsButton::Ptr button) const {
    std::vector<HttpReqArg> args;
    args.reserve(6);

    args.emplace_back("inline_query_id", inlineQueryId);
    args.emplace_back("results", _tgTypeParser.parseArray<InlineQueryResult>(&TgTypeParser::parseInlineQueryResult, results));
    if (cacheTime != 300) {
        args.emplace_back("cache_time", cacheTime);
    }
    if (isPersonal != false) {
        args.emplace_back("is_personal", isPersonal);
    }
    if (!nextOffset.empty()) {
        args.emplace_back("next_offset", nextOffset);
    }
    if (button != nullptr) {
        args.emplace_back("button", _tgTypeParser.parseInlineQueryResultsButton(button));
    }

    return args;
}

std::vector<HttpReqArg> ApiBase::answerWebAppQueryArgs(const std::string& webAppQueryId,
                                              InlineQueryResult::Ptr result) const {
    std::vector<HttpReqArg> args;
    args.reserve(2);

    args.emplace_back("web_app_query_id", webAppQueryId);
    args.emplace_back("result", _tgTypeParser.parseInlineQueryResult(result));

    return args;
}

std::vector<HttpReqArg> ApiBase::sendInvoiceArgs(boost::variant<std::int64_t, std::string> chatId,
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
    std::vector<HttpReqArg> args;
    args.reserve(27);

    args.emplace_back("chat_id", chatId);
    if (messageThreadId != 0) {
        args.emplace_back("message_thread_id", messageThreadId);
    }
    args.emplace_back("title", title);
    args.emplace_back("description", description);
    args.emplace_back("payload", payload);
    args.emplace_back("provider_token", providerToken);
    args.emplace_back("currency", currency);
    args.emplace_back("prices", _tgTypeParser.parseArray<LabeledPrice>(&TgTypeParser::parseLabeledPrice, prices));
    args.emplace_back("max_tip_amount", maxTipAmount);
    if (!suggestedTipAmounts.empty()) {
        args.emplace_back("suggested_tip_amounts", _tgTypeParser.parseArray<std::int32_t>([] (const std::int32_t& option) -> std::int32_t {
            return option;
        }, suggestedTipAmounts));
    }
    if (!startParameter.empty()) {
        args.emplace_back("start_parameter", startParameter);
    }
    if (!providerData.empty()) {
        args.emplace_back("provider_data", providerData);
    }
    if (!photoUrl.empty()) {
        args.emplace_back("photo_url", photoUrl);
    }
    if (photoSize) {
        args.emplace_back("photo_size", photoSize);
    }
    if (photoWidth) {
        args.emplace_back("photo_width", photoWidth);
    }
    if (photoHeight) {
        args.emplace_back("photo_height", photoHeight);
    }
    if (needName) {
        args.emplace_back("need_name", needName);
    }
    if (needPhoneNumber) {
        args.emplace_back("need_phone_number", needPhoneNumber);
    }
    if (needEmail) {
        args.emplace_back("need_email", needEmail);
    }
    if (needShippingAddress) {
        args.emplace_back("need_shipping_address", needShippingAddress);
    }
    if (sendPhoneNumberToProvider) {
        args.emplace_back("send_phone_number_to_provider", sendPhoneNumberToProvider);
    }
    if (sendEmailToProvider) {
        args.emplace_back("send_email_to_provider", sendEmailToProvider);
    }
    if (isFlexible) {
        args.emplace_back("is_flexible", isFlexible);
    }
    if (disableNotification) {
        args.emplace_back("disable_notification", disableNotification);
    }
    if (protectContent) {
        args.emplace_back("protect_content", protectContent);
    }
    if (replyParameters != nullptr) {
        args.emplace_back("reply_parameters", _tgTypeParser.parseReplyParameters(replyParameters));
    }
    if (replyMarkup) {
        args.emplace_back("reply_markup", _tgTypeParser.parseGenericReply(replyMarkup));
    }
    
    return args;
}

std::vector<HttpReqArg> ApiBase::createInvoiceLinkArgs(const std::string& title,
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
    std::vector<HttpReqArg> args;
    args.reserve(20);

    args.emplace_back("title", title);
    args.emplace_back("description", description);
    args.emplace_back("payload", payload);
    args.emplace_back("provider_token", providerToken);
    args.emplace_back("currency", currency);
    args.emplace_back("prices", _tgTypeParser.parseArray<LabeledPrice>(&TgTypeParser::parseLabeledPrice, prices));
    args.emplace_back("max_tip_amount", maxTipAmount);
    if (!suggestedTipAmounts.empty()) {
        args.emplace_back("suggested_tip_amounts", _tgTypeParser.parseArray<std::int32_t>([] (const std::int32_t& option) -> std::int32_t {
            return option;
        }, suggestedTipAmounts));
    }
    if (!providerData.empty()) {
        args.emplace_back("provider_data", providerData);
    }
    if (!photoUrl.empty()) {
        args.emplace_back("photo_url", photoUrl);
    }
    if (photoSize) {
        args.emplace_back("photo_size", photoSize);
    }
    if (photoWidth) {
        args.emplace_back("photo_width", photoWidth);
    }
    if (photoHeight) {
        args.emplace_back("photo_height", photoHeight);
    }
    if (needName) {
        args.emplace_back("need_name", needName);
    }
    if (needPhoneNumber) {
        args.emplace_back("need_phone_number", needPhoneNumber);
    }
    if (needEmail) {
        args.emplace_back("need_email", needEmail);
    }
    if (needShippingAddress) {
        args.emplace_back("need_shipping_address", needShippingAddress);
    }
    if (sendPhoneNumberToProvider) {
        args.emplace_back("send_phone_number_to_provider", sendPhoneNumberToProvider);
    }
    if (sendEmailToProvider) {
        args.emplace_back("send_email_to_provider", sendEmailToProvider);
    }
    if (isFlexible) {
        args.emplace_back("is_flexible", isFlexible);
    }

    return args;
}

std::vector<HttpReqArg> ApiBase::answerShippingQueryArgs(const std::string& shippingQueryId,
                              bool ok,
                              const std::vector<ShippingOption::Ptr>& shippingOptions,
                              const std::string& errorMessage) const {
    std::vector<HttpReqArg> args;
    args.reserve(4);

    args.emplace_back("shipping_query_id", shippingQueryId);
    args.emplace_back("ok", ok);
    if (!shippingOptions.empty()) {
        args.emplace_back("shipping_options", _tgTypeParser.parseArray<ShippingOption>(&TgTypeParser::parseShippingOption, shippingOptions));
    }
    if (!errorMessage.empty()) {
        args.emplace_back("error_message", errorMessage);
    }

    return args;
}

std::vector<HttpReqArg> ApiBase::answerPreCheckoutQueryArgs(const std::string& preCheckoutQueryId,
                                 bool ok,
                                 const std::string& errorMessage) const {
    std::vector<HttpReqArg> args;
    args.reserve(3);

    args.emplace_back("pre_checkout_query_id", preCheckoutQueryId);
    args.emplace_back("ok", ok);
    if (!errorMessage.empty()) {
        args.emplace_back("error_message", errorMessage);
    }

    return args;
}

std::vector<HttpReqArg> ApiBase::setPassportDataErrorsArgs(std::int64_t userId,
                                const std::vector<PassportElementError::Ptr>& errors) const {
    std::vector<HttpReqArg> args;
    args.reserve(2);

    args.emplace_back("user_id", userId);
    args.emplace_back("errors", _tgTypeParser.parseArray<PassportElementError>(&TgTypeParser::parsePassportElementError, errors));

    return args;
}

std::vector<HttpReqArg> ApiBase::sendGameArgs(std::int64_t chatId,
                           const std::string& gameShortName,
                           ReplyParameters::Ptr replyParameters,
                           InlineKeyboardMarkup::Ptr replyMarkup,
                           bool disableNotification,
                           std::int32_t messageThreadId,
                           bool protectContent,
                           const std::string& businessConnectionId) const {
    std::vector<HttpReqArg> args;
    args.reserve(8);

    if (!businessConnectionId.empty()) {
        args.emplace_back("business_connection_id", businessConnectionId);
    }
    args.emplace_back("chat_id", chatId);
    if (messageThreadId != 0) {
        args.emplace_back("message_thread_id", messageThreadId);
    }
    args.emplace_back("game_short_name", gameShortName);
    if (disableNotification) {
        args.emplace_back("disable_notification", disableNotification);
    }
    if (protectContent) {
        args.emplace_back("protect_content", protectContent);
    }
    if (replyParameters != nullptr) {
        args.emplace_back("reply_parameters", _tgTypeParser.parseReplyParameters(replyParameters));
    }
    if (replyMarkup) {
        args.emplace_back("reply_markup", _tgTypeParser.parseGenericReply(replyMarkup));
    }
    
    return args;
}

std::vector<HttpReqArg> ApiBase::setGameScoreArgs(std::int64_t userId,
                               std::int32_t score,
                               bool force,
                               bool disableEditMessage,
                               std::int64_t chatId,
                               std::int32_t messageId,
                               const std::string& inlineMessageId) const {
    std::vector<HttpReqArg> args;
    args.reserve(7);

    args.emplace_back("user_id", userId);
    args.emplace_back("score", score);
    if (force) {
        args.emplace_back("force", force);
    }
    if (disableEditMessage) {
        args.emplace_back("disable_edit_message", disableEditMessage);
    }
    if (chatId) {
        args.emplace_back("chat_id", chatId);
    }
    if (messageId) {
        args.emplace_back("message_id", messageId);
    }
    if (!inlineMessageId.empty()) {
        args.emplace_back("inline_message_id", inlineMessageId);
    }

    return args;
}

std::vector<HttpReqArg> ApiBase::getGameHighScoresArgs(std::int64_t userId,
                                                  std::int64_t chatId,
                                                  std::int32_t messageId,
                                                  const std::string& inlineMessageId) const {
    std::vector<HttpReqArg> args;
    args.reserve(4);

    args.emplace_back("user_id", userId);
    if (chatId) {
        args.emplace_back("chat_id", chatId);
    }
    if (messageId) {
        args.emplace_back("message_id", messageId);
    }
    if (!inlineMessageId.empty()) {
        args.emplace_back("inline_message_id", inlineMessageId);
    }

    return args;
}
}

