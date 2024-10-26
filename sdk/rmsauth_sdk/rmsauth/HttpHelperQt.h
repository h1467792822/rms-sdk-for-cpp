/*
 * ======================================================================
 * Copyright (c) Microsoft Open Technologies, Inc.  All rights reserved.
 * Licensed under the MIT License.
 * See LICENSE.md in the project root for license information.
 * ======================================================================
*/

#ifndef HTTPHELPERQT
#define HTTPHELPERQT

#include <CallState.h>
#include <RequestParameters.h>
#include <Entities.h>
// #include <QNetworkRequest>
// #include <QNetworkReply>

namespace rmsauth {

class HttpHelperQt
{
    static const String& Tag() {static const String tag="HttpHelperQt"; return tag;}

public:
#if 0
    static QNetworkRequest createRequest();
    static void addHeadersToRequest(QNetworkRequest& request, const Headers& headers);

    static TokenResponsePtr deserializeTokenResponse(const QByteArray& body);
    static InstanceDiscoveryResponsePtr deserializeInstanceDiscoveryResponse(const QByteArray& body);
    static ErrorResponsePtr parseResponseError(QNetworkReply* pReply);
    static void verifyCorrelationIdHeaderInReponse(QNetworkReply* pReply, CallStatePtr callState);

    static void addCorrelationIdHeadersToRequest(QNetworkRequest& request, CallStatePtr callState);

    static QByteArray jobPost(QNetworkRequest& request, const RequestParameters& requestParameters, CallStatePtr callState);
    static QByteArray jobPostRunner(QNetworkRequest& request, const RequestParameters& requestParameters, CallStatePtr callState);

    static QByteArray jobGet(QNetworkRequest& request, CallStatePtr callState);
    static QByteArray jobGetRunner(QNetworkRequest& request, CallStatePtr callState);

    static void logRequestHeaders(const QNetworkRequest& req);
    static void logResponseHeaders(QNetworkReply *pReply);
    static void logResponseBody(const QByteArray& body);
#endif

    static String jobGet(const String& url, const Headers& headers, CallStatePtr callState);
    static String jobPost(const String& url, const Headers& headers, const RequestParameters& requestParameters, CallStatePtr callState);
    static TokenResponsePtr sendPostRequestAndDeserializeJsonResponseAsync(const String& url, const RequestParameters& requestParameters, CallStatePtr callState);
    static bool addCACertificateBase64(const std::vector<uint8_t>& certificate);
    static bool addCACertificateDer(const std::vector<uint8_t>& certificate);
    static void addCorrelationIdHeadersToRequest(Headers& headers, CallStatePtr callState);
    static TokenResponsePtr deserializeTokenResponse(const String& body);
    static InstanceDiscoveryResponsePtr deserializeInstanceDiscoveryResponse(const String& body);
    static void addHeadersToRequest(Headers& headers, const Headers& newHeaders);
    static void logRequestHeaders(const Headers& headers);
    static void logResponseBody(const String& body);
    static void logResponseHeaders(const String& headers);
    static ErrorResponsePtr parseResponseError(const String& jsonBody);

};

} //namespace rmsauth {

#endif // HTTPHELPERQT

