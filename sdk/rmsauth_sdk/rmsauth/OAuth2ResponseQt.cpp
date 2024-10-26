/*
 * ======================================================================
 * Copyright (c) Microsoft Open Technologies, Inc.  All rights reserved.
 * Licensed under the MIT License.
 * See LICENSE.md in the project root for license information.
 * ======================================================================
*/

#include <OAuth2Response.h>
#include <AuthenticationResult.h>
#include <AuthorizationResult.h>
#include <Exceptions.h>
#include <Constants.h>
#include <OAuthConstants.h>
#include <utils.h>
#include <Logger.h>
#include "JsonUtilsQt.h"
#include "../../rmsutils/DateTime.h"
// #include <QByteArray>
// #include <QUrl>
// #include <QUrlQuery>
#include <regex>
#include <nlohmann/json.hpp>

namespace rmsauth {

AuthenticationResultPtr OAuth2Response::parseTokenResponse(TokenResponsePtr tokenResponse, CallStatePtr/* callState*/)
{
    Logger::info(Tag(), "parseTokenResponse");
    AuthenticationResultPtr result = nullptr;

    if (!tokenResponse->accessToken.empty())
    {
        auto curTimeUts = rmsutils::DateTime::currentDateTimeUtc();
        Logger::info(Tag(), "curTimeUTC: %; expiresIn: %", curTimeUts.toString("HH:mm:ss MM.dd.yy"), tokenResponse->expiresIn);

        auto expiresUtc = curTimeUts.addSecs(tokenResponse->expiresIn);
        DateTimeOffset expiresOn = expiresUtc.toTime_t();

        Logger::info(Tag(), "tonken expiresOn: % (%)", expiresUtc.toString("HH:mm:ss MM.dd.yy"), expiresOn);

        result = std::make_shared<AuthenticationResult>(tokenResponse->tokenType, tokenResponse->accessToken, tokenResponse->refreshToken, expiresOn);
        // This is only needed for AcquireTokenByAuthorizationCode in which parameter resource is optional and we need
        // to get it from the STS response.
        result->resource(tokenResponse->resource);
        result->isMultipleResourceRefreshToken(!tokenResponse->refreshToken.empty() && !tokenResponse->resource.empty());

        IdTokenPtr idToken = OAuth2Response::parseIdToken(tokenResponse->idToken);
        if (idToken != nullptr)
        {
            String tenantId = idToken->tenantId;
            String uniqueId;
            String displayableId;

            if (!idToken->objectId.empty())
            {
                uniqueId = idToken->objectId;
            }
            else if (!idToken->subject.empty())
            {
                uniqueId = idToken->subject;
            }

            if (!idToken->UPN.empty())
            {
                displayableId = idToken->UPN;
            }
            else if (!idToken->email.empty())
            {
                displayableId = idToken->email;
            }

            String givenName = idToken->givenName;
            String familyName = idToken->familyName;
            String identityProvider = idToken->identityProvider.empty() ? idToken->issuer : idToken->identityProvider;
            DateTimeOffset passwordExpiresOn = 0;
            if (idToken->passwordExpiration > 0)
            {
                passwordExpiresOn = rmsutils::DateTime::currentDateTimeUtc().addSecs(idToken->passwordExpiration).toTime_t();
            }

            String changePasswordUri;
            if (!idToken->passwordChangeUrl.empty())
            {
                changePasswordUri = idToken->passwordChangeUrl;
            }

            auto userInfo = std::make_shared<UserInfo>();
            userInfo->uniqueId(uniqueId);
            userInfo->displayableId(displayableId);
            userInfo->givenName(givenName);
            userInfo->familyName(familyName);
            userInfo->identityProvider(identityProvider);
            userInfo->passwordExpiresOn(passwordExpiresOn);
            userInfo->passwordChangeUrl(changePasswordUri);

            result->updateTenantAndUserInfo(tenantId, tokenResponse->idToken, userInfo );
        }
    }
    else if (!tokenResponse->error.empty())
    {
        Logger::error(Tag(), "error: %, description: %", tokenResponse->error, tokenResponse->errorDescription);
        throw RmsauthException(tokenResponse->error, tokenResponse->errorDescription);
    }
    else
    {
        throw RmsauthException(Constants::rmsauthError().Unknown);
    }

    return result;
}

IdTokenPtr OAuth2Response::parseIdToken(const String& idToken)
{
    Logger::info(Tag(), "parseIdToken");
    Logger::hidden(Tag(), "idToken: " + idToken);
    IdTokenPtr parseResult = nullptr;
    if (!idToken.empty())
    {
        StringArray idTokenSegments = StringUtils::split(idToken, '.');

        // If Id token format is invalid, we silently ignore the id token
        if (idTokenSegments.size() != 3)
        {
            throw RmsauthException("idTokenSegments.size() != 3");
        }

        // QByteArray ba;
        // ba.append(idTokenSegments[1].data());
        // String jsonString = QByteArray::fromBase64(ba).data();
        String jsonString = StringUtils::base64Decode(idTokenSegments[1]);

	nlohmann::json qobj;
	try {
	    qobj = nlohmann::json::parse(jsonString);
	}
	catch(std::exception& e)
        {
            throw RmsauthException(String("deserializeTokenResponse: ") + e.what());
        }

        parseResult.reset(new IdToken());

        parseResult->objectId              = JsonUtilsQt::getStringOrDefault(qobj, IdToken::jsonNames().objectId);
        parseResult->subject               = JsonUtilsQt::getStringOrDefault(qobj, IdToken::jsonNames().subject);
        parseResult->tenantId              = JsonUtilsQt::getStringOrDefault(qobj, IdToken::jsonNames().tenantId);
        parseResult->UPN                   = JsonUtilsQt::getStringOrDefault(qobj, IdToken::jsonNames().UPN);
        parseResult->givenName             = JsonUtilsQt::getStringOrDefault(qobj, IdToken::jsonNames().givenName);
        parseResult->familyName            = JsonUtilsQt::getStringOrDefault(qobj, IdToken::jsonNames().familyName);
        parseResult->email                 = JsonUtilsQt::getStringOrDefault(qobj, IdToken::jsonNames().email);
        parseResult->passwordChangeUrl     = JsonUtilsQt::getStringOrDefault(qobj, IdToken::jsonNames().passwordChangeUrl);
        parseResult->identityProvider      = JsonUtilsQt::getStringOrDefault(qobj, IdToken::jsonNames().identityProvider);
        parseResult->issuer                = JsonUtilsQt::getStringOrDefault(qobj, IdToken::jsonNames().issuer);
        parseResult->passwordExpiration    = JsonUtilsQt::getStringAsIntOrDefault(qobj, IdToken::jsonNames().passwordExpiration);
    }

    return parseResult;
}

AuthorizationResultPtr OAuth2Response::parseAuthorizeResponse(const String& webAuthenticationResult, CallStatePtr/* callState*/)
{
    Logger::info(Tag(), "parseAuthorizeResponse");
    Logger::hidden(Tag(), "webAuthenticationResult: " + webAuthenticationResult);

    AuthorizationResultPtr parseResult = nullptr;

    std::smatch matches;

    String url(webAuthenticationResult);
    if (url.find('?') != std::string::npos) {
        String queryWith = "=([^&#]+)";
        String code_string = OAuthConstants::oAuthReservedClaim().Code + queryWith;
        std::regex code_regex(code_string);
        String error_string = OAuthConstants::oAuthReservedClaim().Error + queryWith;
        std::regex error_regex(error_string);

        if (std::regex_search(url, matches, code_regex) && matches.size() > 1) {
            parseResult = std::make_shared<AuthorizationResult>(matches[1].str());
        } else if (std::regex_search(url, matches, error_regex) && matches.size() > 1) {
            String error = matches[1].str();
            String errorDescription_string = OAuthConstants::oAuthReservedClaim().ErrorDescription + queryWith;
            std::regex errorDescription_regex(errorDescription_string);
            String errorDesc = (std::regex_search(url, matches, errorDescription_regex) && matches.size() > 1) ? matches[1].str() : "";
            parseResult = std::make_shared<AuthorizationResult>(
                error,
                StringUtils::replaceAll(errorDesc, '+', ' '));
        }
        else
        {
            parseResult = std::make_shared<AuthorizationResult>(
                Constants::rmsauthError().AuthenticationFailed,
                Constants::rmsauthErrorMessage().AuthorizationServerInvalidResponse);
        }
    }

    return parseResult;
}

} // namespace rmsauth {
