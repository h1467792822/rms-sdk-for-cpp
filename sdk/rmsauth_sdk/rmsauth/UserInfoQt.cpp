/*
 * ======================================================================
 * Copyright (c) Microsoft Open Technologies, Inc.  All rights reserved.
 * Licensed under the MIT License.
 * See LICENSE.md in the project root for license information.
 * ======================================================================
*/

#include <UserInfo.h>
#include "JsonUtilsQt.h"
#include <nlohmann/json.hpp>

namespace rmsauth {

UserInfoPtr UserInfo::deserialize(const String& jsonString)
{
    nlohmann::json qobj;
    try {
        qobj = nlohmann::json::parse(jsonString);
    }
    catch(std::exception& e)
    {
        throw RmsauthJsonParsingException("UserInfo::deserialize: ", e.what());
    }
    auto userInfo = std::make_shared<UserInfo>();
    userInfo->uniqueId_          = JsonUtilsQt::getStringOrDefault(qobj, UserInfo::jsonNames().uniqueId);
    userInfo->displayableId_     = JsonUtilsQt::getStringOrDefault(qobj, UserInfo::jsonNames().displayableId);
    userInfo->givenName_          = JsonUtilsQt::getStringOrDefault(qobj, UserInfo::jsonNames().givenName);
    userInfo->familyName_         = JsonUtilsQt::getStringOrDefault(qobj, UserInfo::jsonNames().familyName);
    userInfo->identityProvider_   = JsonUtilsQt::getStringOrDefault(qobj, UserInfo::jsonNames().identityProvider);
    userInfo->passwordChangeUrl_  = JsonUtilsQt::getStringOrDefault(qobj, UserInfo::jsonNames().passwordChangeUrl);
    userInfo->passwordExpiresOn_  = (DateTimeOffset)JsonUtilsQt::getIntOrDefault(qobj, UserInfo::jsonNames().passwordExpiresOn);
    userInfo->forcePrompt_        = JsonUtilsQt::getBoolOrDefault(qobj, UserInfo::jsonNames().forcePrompt);

    return userInfo;
}

String UserInfo::serialize()
{
    nlohmann::json qobj;
    JsonUtilsQt::insertString(qobj, UserInfo::jsonNames().uniqueId, uniqueId_);
    JsonUtilsQt::insertString(qobj, UserInfo::jsonNames().displayableId, displayableId_);
    JsonUtilsQt::insertString(qobj, UserInfo::jsonNames().givenName, givenName_);
    JsonUtilsQt::insertString(qobj, UserInfo::jsonNames().familyName, familyName_);
    JsonUtilsQt::insertString(qobj, UserInfo::jsonNames().identityProvider, identityProvider_);
    JsonUtilsQt::insertString(qobj, UserInfo::jsonNames().passwordChangeUrl, passwordChangeUrl_);
    qobj[UserInfo::jsonNames().passwordExpiresOn] =  (int)passwordExpiresOn_;
    qobj[UserInfo::jsonNames().forcePrompt] = forcePrompt_;

    return qobj.dump();
}

} // namespace rmsauth {
