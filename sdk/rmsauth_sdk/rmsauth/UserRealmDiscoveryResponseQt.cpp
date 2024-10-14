/*
 * ======================================================================
 * Copyright (c) Microsoft Open Technologies, Inc.  All rights reserved.
 * Licensed under the MIT License.
 * See LICENSE.md in the project root for license information.
 * ======================================================================
*/

#include <UserRealmDiscoveryResponse.h>
#include <Logger.h>
#include <RmsauthIdHelper.h>
#include <QNetworkRequest>
#include <QCoreApplication>
#include <nlohmann/json.hpp>
#include "HttpHelperQt.h"
#include "JsonUtilsQt.h"

namespace rmsauth {

static UserRealmDiscoveryResponse deserializeUserRealmDiscoveryResponse(const QByteArray& body)
{
    Logger::info("deserializeUserRealmDiscoveryResponse", "jsonObject: %", String(body.begin(), body.end()));

    nlohmann::json qobj;
    try {
        qobj = nlohmann::json::parse(String(body.begin(), body.end()));
    }
    catch(std::exception& e)
    {
        throw RmsauthException(String("deserializeUserRealmDiscoveryResponse: ") + e.what());
    }

    UserRealmDiscoveryResponse userRealmResponse;

    userRealmResponse.version(JsonUtilsQt::getStringOrDefault(qobj, UserRealmDiscoveryResponse::jsonNames().version));
    userRealmResponse.accountType(JsonUtilsQt::getStringOrDefault(qobj, UserRealmDiscoveryResponse::jsonNames().accountType));
    userRealmResponse.federationProtocol(JsonUtilsQt::getStringOrDefault(qobj, UserRealmDiscoveryResponse::jsonNames().federationProtocol));
    userRealmResponse.federationMetadataUrl(JsonUtilsQt::getStringOrDefault(qobj, UserRealmDiscoveryResponse::jsonNames().federationMetadataUrl));
    userRealmResponse.federationActiveAuthUrl(JsonUtilsQt::getStringOrDefault(qobj, UserRealmDiscoveryResponse::jsonNames().federationActiveAuthUrl));

    return std::move(userRealmResponse);
}

UserRealmDiscoveryResponse UserRealmDiscoveryResponse::createByDiscoveryAsync(const String& userRealmUri, const String& userName, CallStatePtr callState)
{
    String userRealmEndpoint = userRealmUri + userName + "?api-version=1.0";

//  userRealmEndpoint = HttpHelper.CheckForExtraQueryParameter(userRealmEndpoint);
    Logger::info(Tag(), "Sending user realm discovery request to '%'", userRealmEndpoint);

    QNetworkRequest request = HttpHelperQt::createRequest();
    request.setUrl(QUrl(userRealmEndpoint.data()));
    HttpHelperQt::addHeadersToRequest(request, RmsauthIdHelper::getPlatformHeaders());
    HttpHelperQt::addHeadersToRequest(request, RmsauthIdHelper::getProductHeaders());
    request.setRawHeader("Accept", "application/json");

    if(QCoreApplication::instance() == nullptr)
    {
        auto fut = std::async(&HttpHelperQt::jobGetRunner, std::ref(request), callState);
        auto body = fut.get();
        auto userRealmResponse = deserializeUserRealmDiscoveryResponse(body);
        return std::move(userRealmResponse);
    }

    auto body = HttpHelperQt::jobGet(request, callState);
    auto userRealmResponse = deserializeUserRealmDiscoveryResponse(body);
    return std::move(userRealmResponse);
}

} // namespace rmsauth {
