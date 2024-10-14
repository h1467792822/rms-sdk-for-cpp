/*
 * ======================================================================
 * Copyright (c) Microsoft Open Technologies, Inc.  All rights reserved.
 * Licensed under the MIT License.
 * See LICENSE.md in the project root for license information.
 * ======================================================================
*/

#include <AuthenticationResult.h>
#include <Exceptions.h>
#include <Logger.h>
#include "JsonUtilsQt.h"
#include <nlohmann/json.hpp>

namespace rmsauth {

AuthenticationResultPtr AuthenticationResult::deserialize(const String& jsonString)
{
    Logger::info(Tag(), "deserialize");
    AuthenticationResultPtr result = nullptr;

    nlohmann::json qobj;
    try {
        qobj = nlohmann::json::parse(jsonString);
    }
    catch(std::exception& e)
    {
        throw RmsauthException(String("AuthenticationResult::deserialize: ") + e.what());
    }

    result.reset(new AuthenticationResult());

    result->accessTokenType_                   = JsonUtilsQt::getStringOrDefault(qobj, AuthenticationResult::JsonNames.accessTokenType);
    result->accessToken_                       = JsonUtilsQt::getStringOrDefault(qobj, AuthenticationResult::JsonNames.accessToken);
    result->resource_                          = JsonUtilsQt::getStringOrDefault(qobj, AuthenticationResult::JsonNames.resource);
    result->refreshToken_                      = JsonUtilsQt::getStringOrDefault(qobj, AuthenticationResult::JsonNames.refreshToken);
    result->expiresOn_                         = JsonUtilsQt::getIntOrDefault(qobj, AuthenticationResult::JsonNames.expiresOn);
    result->tenantId_                          = JsonUtilsQt::getStringOrDefault(qobj, AuthenticationResult::JsonNames.tenantId);

    result->userInfo_                          = JsonUtilsQt::parseObject<UserInfoPtr>(qobj, AuthenticationResult::JsonNames.userInfo, &UserInfo::deserialize);

    result->idToken_                           = JsonUtilsQt::getStringOrDefault(qobj, AuthenticationResult::JsonNames.idToken);
    result->isMultipleResourceRefreshToken_    = JsonUtilsQt::getBoolOrDefault(qobj, AuthenticationResult::JsonNames.isMultipleResourceRefreshToken);

    return result;
}
String AuthenticationResult::serialize()
{
    Logger::info(Tag(), "serialize");
    nlohmann::json qobj;
    JsonUtilsQt::insertString(qobj, AuthenticationResult::JsonNames.accessTokenType, accessTokenType_);
    JsonUtilsQt::insertString(qobj, AuthenticationResult::JsonNames.accessToken, accessToken_);
    JsonUtilsQt::insertString(qobj, AuthenticationResult::JsonNames.resource, resource_);
    JsonUtilsQt::insertString(qobj, AuthenticationResult::JsonNames.refreshToken, refreshToken_);
    qobj[AuthenticationResult::JsonNames.expiresOn] = (int)expiresOn_;
    JsonUtilsQt::insertString(qobj, AuthenticationResult::JsonNames.tenantId, tenantId_);
    if(userInfo_ != nullptr)
        JsonUtilsQt::insertObject(qobj, AuthenticationResult::JsonNames.userInfo, userInfo_->serialize());
    JsonUtilsQt::insertString(qobj, AuthenticationResult::JsonNames.idToken, idToken_);
    qobj[AuthenticationResult::JsonNames.isMultipleResourceRefreshToken] = isMultipleResourceRefreshToken_;

    return qobj.dump();
}

} // namespace rmsauth {
