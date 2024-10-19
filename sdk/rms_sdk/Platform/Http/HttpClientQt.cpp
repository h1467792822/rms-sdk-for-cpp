/*
 * ======================================================================
 * Copyright (c) Microsoft Open Technologies, Inc.  All rights reserved.
 * Licensed under the MIT License.
 * See LICENSE.md in the project root for license information.
 * ======================================================================
 */

#include "HttpClientQt.h"
#include "../Logger/Logger.h"
#include "../../ModernAPI/RMSExceptions.h"
#include "../../ModernAPI/HttpHelper.h"
#include "mscertificates.h"
#include "HttpClientQt.h"
#include <iterator>
#include <algorithm>
#include <utility>
#include <cstdio>
#include <cstring>
#include <cctype>

using namespace std;
using namespace rmscore::platform::logger;

namespace rmscore {
namespace platform {
namespace http {

class RespCtx {
public:
  HttpClientQt& client;
  common::ByteArray& body;
  std::shared_ptr<atomic<bool>>& cancelState;
  RespCtx(HttpClientQt& client, common::ByteArray& body, std::shared_ptr<atomic<bool>>& cancelState):
	 client(client), body(body), cancelState(cancelState) {} 
  bool cancelled() {
    return client.cancel(cancelState != nullptr && cancelState->load());
  }
  void onRespHeader(const std::string& header) {
    client.onRespHeader(header);
  }
};

static size_t ReadHeader(char* buff, size_t size, size_t nitems, void* userdata)
{
  RespCtx* ctx = reinterpret_cast<RespCtx*>(userdata);
  if (!ctx->cancelled()) {
    size_t len = size * nitems;
    ctx->onRespHeader(std::string(buff, len));
    return len;
  }
  return 0;
}

static size_t ReadBody(char* buff, size_t size, size_t nitems, void* userdata)
{
  RespCtx* ctx = reinterpret_cast<RespCtx*>(userdata);
  if (!ctx->cancelled()) {
    size_t len = size * nitems;
    ctx->body.reserve(ctx->body.size() + len);
    std::copy(buff, buff + len, std::back_inserter(ctx->body));
    return len;
  }
  return 0;
}

#if 0
common::ByteArray ReadAllBytes(QIODevice *from) {
  common::ByteArray result;
  auto bytesAvailable = from->bytesAvailable();

  if (bytesAvailable > 0) {
    result.resize(static_cast<size_t>(bytesAvailable));
    char *buf = reinterpret_cast<char *>(&result[0]);
    size_t offset = 0;
    while (bytesAvailable > 0) {
      auto read = from->read(&buf[offset], bytesAvailable);

      if (read <= 0) break;
      bytesAvailable -= read;
      offset += read;
    }
  }

  return result;
}

shared_ptr<IHttpClient> doCreate() {
  static bool initialized = false;

  // add Microsoft certificates to trust list
  if (!initialized) {
    QSslConfiguration SslConfiguration(QSslConfiguration::defaultConfiguration());

    QList<QSslCertificate> certificates = SslConfiguration.caCertificates();
    certificates.append(QSslCertificate::fromData(MicrosoftCertCA));
    certificates.append(QSslCertificate::fromData(MicrosoftCertSubCA));
    SslConfiguration.setCaCertificates(certificates);
    QSslConfiguration::setDefaultConfiguration(SslConfiguration);
    initialized = true;
  }
  return make_shared<HttpClientQt>();
}

shared_ptr<IHttpClient> IHttpClient::Create() {

  // If a QCoreApplication does not exist, create a temporary instance.
  // QCoreApplication is a singleton, but it can keep getting created and destroyed.
  // QtNetwork calls need to be made from within the scope of the QCoreApplication created.
  if(!QCoreApplication::instance())
  {
    int argc = 1;
    char name[] = "IHttpClient::Create";
    char* argv = &name[0];
    QCoreApplication a(argc, &argv);
    return doCreate();
  }

  return doCreate();
}
#endif

shared_ptr<IHttpClient> IHttpClient::Create() {
  return make_shared<HttpClientQt>();
}

#if 0
HttpClientQt::HttpClientQt() : lastReply_(nullptr) {
  this->request_.setSslConfiguration(QSslConfiguration::defaultConfiguration());
  QNetworkProxyFactory::setUseSystemConfiguration(true);
}
#endif
HttpClientQt::HttpClientQt() {
  curl = curl_easy_init();
  if (curl) {
    std::vector<uint8_t> certs(rmscore::modernapi::HttpHelper::getAllCACertificatesBase64());
    std::copy(MicrosoftCertCA, MicrosoftCertCA + sizeof(MicrosoftCertCA) - 1, std::back_inserter(certs));
    std::copy(MicrosoftCertSubCA, MicrosoftCertSubCA + sizeof(MicrosoftCertSubCA) - 1, std::back_inserter(certs));

    struct curl_blob blob;
    blob.data = (void*)certs.data();
    blob.len = certs.size();
    blob.flags = CURL_BLOB_COPY;
    curl_easy_setopt(curl, CURLOPT_CAINFO_BLOB, &blob);
  }
}

HttpClientQt::~HttpClientQt() {
  if (headers) {
    curl_slist_free_all(headers);
  }
  if (curl) {
    curl_easy_cleanup(curl);
  }
}

void HttpClientQt::AddAuthorizationHeader(const string& authToken) {
  this->AddHeader("Authorization", authToken);
}

void HttpClientQt::AddAcceptMediaTypeHeader(const string& mediaType) {
  this->AddHeader("Accept", mediaType);
}

void HttpClientQt::AddAcceptLanguageHeader(const string& language) {
  this->AddHeader("Accept-Language", language);
}

void HttpClientQt::AddHeader(const string& headerName,
                             const string& headerValue) {
  auto header = headerName + ": " + headerValue;
  headers = curl_slist_append(headers, header.c_str());
  //this->request_.setRawHeader(headerName.c_str(), headerValue.c_str());
}

#if 0
StatusCode HttpClientQt::doPost(const string& url,
                              const common::ByteArray& request,
                              const string& mediaType,
                              common::ByteArray& response,
                              std::shared_ptr<std::atomic<bool> >cancelState)
{
  Logger::Info("==> PostWithCoreAppContext %s", url.data());

  this->request_.setUrl(QUrl(url.c_str()));
  this->AddAcceptMediaTypeHeader(mediaType);

  Logger::Hidden("==> Request Headers:");
  foreach(const QByteArray &hdrName, this->request_.rawHeaderList()) {
    QByteArray hdrValue = this->request_.rawHeader(hdrName);
    Logger::Hidden("%s : %s", hdrName.data(), hdrValue.data());
  }

  std::string req(request.begin(), request.end());
  Logger::Hidden("==> Request Body: %s", req.c_str());

  // first abort previous

  lastReply_ = this->manager_.post(
    this->request_,
    QByteArray(reinterpret_cast<const char *>(request.data()),
               static_cast<int>(request.size())));

  QTimer timer;
  QEventLoop loop;
  QObject::connect(&timer,     SIGNAL(timeout()),  &loop, SLOT(quit()));
  QObject::connect(lastReply_, SIGNAL(finished()), &loop, SLOT(quit()));
  QObject::connect(lastReply_, &QNetworkReply::sslErrors,
                   [ = ](QList<QSslError>errorList) {
          for (auto& error : errorList) {
            Logger::Error("QSslError: %s",
                          error.errorString().toStdString().c_str());
            throw exceptions::RMSNetworkException(
              error.errorString().toStdString(),
              exceptions::RMSNetworkException::ServerError);
          }
        });

  do {
    timer.start(500);
    loop.exec();

    // check abandom
    if ((cancelState != nullptr) && cancelState->load()) {
      throw exceptions::RMSNetworkException(
              "Network operation was cancelled by user",
              exceptions::RMSNetworkException::CancelledByUser);
    }
  } while (!timer.isActive() || !lastReply_->isFinished());

  QVariant statusCode = lastReply_->attribute(
    QNetworkRequest::HttpStatusCodeAttribute);
  Logger::Info("Response StatusCode: %i", statusCode.toInt());

  Logger::Hidden("--> Response Headers:");
  foreach(const QNetworkReply::RawHeaderPair & pair,
          lastReply_->rawHeaderPairs()) {
    Logger::Hidden("%s : %s", pair.first.data(), pair.second.data());
  }

  response = ReadAllBytes(lastReply_);
  Logger::Hidden("--> Response Body:");
  Logger::Hidden(string(response.begin(), response.end()));

  QNetworkReply::NetworkError error_type = lastReply_->error();

  if (error_type != QNetworkReply::NoError) {
    Logger::Error(QString("error: %1").arg(
                    lastReply_->errorString()).toStdString());
  }

  return StatusCode(statusCode.toInt());
}

StatusCode HttpClientQt::Post(const string& url,
                              const common::ByteArray& request,
                              const string& mediaType,
                              common::ByteArray& response,
                              std::shared_ptr<std::atomic<bool> >cancelState)
{
  // If a QCoreApplication does not exist, create a temporary instance.
  // QCoreApplication is a singleton, but it can keep getting created and destroyed.
  // QtNetwork calls need to be made from within the scope of the QCoreApplication created.
  if (!QCoreApplication::instance()) {
    int argc = 1;
    char name[] = "HttpClientQt::Post";
    char* argv = &name[0];
    QCoreApplication a(argc, &argv);
    return doPost(url, request, mediaType, response, cancelState);
  }
  return doPost(url, request, mediaType, response, cancelState);
}
#endif 
StatusCode HttpClientQt::Post(const string& url,
                              const common::ByteArray& request,
                              const string& mediaType,
                              common::ByteArray& response,
                              std::shared_ptr<std::atomic<bool> >cancelState)
{
  Logger::Info("==> PostWithCoreAppContext %s", url.data());
  std::string req(request.begin(), request.end());
  Logger::Hidden("==> Request Body: %s", req.c_str());

  this->AddAcceptMediaTypeHeader(mediaType);
  
  curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (void*)request.size());
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, request.data());

  return doRequest(url, response, cancelState);
}
#if 0
StatusCode HttpClientQt::doGet(const string& url,
                             common::ByteArray& response,
                             std::shared_ptr<std::atomic<bool> >cancelState)
{

  Logger::Info("==> GetWithCoreAppContext %s", url.data());

  this->request_.setUrl(QUrl(url.c_str()));

  Logger::Hidden("==> Request headers:");
  foreach(const QByteArray &hdrName, this->request_.rawHeaderList()) {
    QByteArray hdrValue = this->request_.rawHeader(hdrName);
    Logger::Hidden("%s : %s", hdrName.data(), hdrValue.data());
  }

  lastReply_ = this->manager_.get(this->request_);
  QTimer timer;
  QEventLoop loop;
  QObject::connect(&timer,     SIGNAL(timeout()),  &loop, SLOT(quit()));
  QObject::connect(lastReply_, SIGNAL(finished()), &loop, SLOT(quit()));
  QObject::connect(lastReply_, &QNetworkReply::sslErrors,
                   [ = ](QList<QSslError>errorList) {
          for (auto& error : errorList) {
            Logger::Error("QSslError: %s",
                          error.errorString().toStdString().c_str());
            throw exceptions::RMSNetworkException(
              error.errorString().toStdString(),
              exceptions::RMSNetworkException::ServerError);
          }
        });

  do {
    timer.start(500);
    loop.exec();

    // check abandom
    if ((cancelState != nullptr) && cancelState->load()) {
      throw exceptions::RMSNetworkException(
              "Network operation was cancelled by user",
              exceptions::RMSNetworkException::CancelledByUser);
    }
  } while (!timer.isActive() || !lastReply_->isFinished());

  QVariant statusCode = lastReply_->attribute(
    QNetworkRequest::HttpStatusCodeAttribute);
  Logger::Info("Response StatusCode: %i", statusCode.toInt());

  Logger::Hidden("--> Response Headers:");
  foreach(const QNetworkReply::RawHeaderPair & pair,
          lastReply_->rawHeaderPairs()) {
    Logger::Hidden("%s : %s", pair.first.data(), pair.second.data());
  }

  response = ReadAllBytes(lastReply_);
  Logger::Hidden("--> Response Body:");
  Logger::Hidden(string(response.begin(), response.end()));

  QNetworkReply::NetworkError error_type = lastReply_->error();

  if (error_type != QNetworkReply::NoError) {
    Logger::Error(QString("error: %1").arg(
                    lastReply_->errorString()).toStdString());
  }

  return StatusCode(statusCode.toInt());
}

StatusCode HttpClientQt::Get(const string& url,
                             common::ByteArray& response,
                             std::shared_ptr<std::atomic<bool> >cancelState)
{
  // If a QCoreApplication does not exist, create a temporary instance.
  // QCoreApplication is a singleton, but it can keep getting created and destroyed.
  // QtNetwork calls need to be made from within the scope of the QCoreApplication created.
  if (!QCoreApplication::instance()) {
    int argc = 1;
    char name[] = "HttpClientQt::Get";
    char* argv = &name[0];
    QCoreApplication a(argc, &argv);
    return doGet(url, response, cancelState);
  }

  return doGet(url, response, cancelState);
}
#endif

// 如下简单实现是无法取消的，用一个超时时间来替代，如果超时之后又取消了，说明取消成功
StatusCode HttpClientQt::Get(const string& url,
                             common::ByteArray& response,
                             std::shared_ptr<std::atomic<bool> >cancelState)
{
  Logger::Info("==> GetWithCoreAppContext %s", url.data());
  Logger::Info("==> GetWithCoreAppContext %s", url.data());
  curl_easy_setopt(curl, CURLOPT_HTTPGET, (void*)1);
  return doRequest(url, response, cancelState);
}

StatusCode HttpClientQt::doRequest(const string& url,
  common::ByteArray& response,
  std::shared_ptr<std::atomic<bool> >& cancelState)
{
  char errbuf[CURL_ERROR_SIZE] = { 0 };
  RespCtx ctx(*this, response, cancelState);
  Logger::Hidden("==> Request headers:");
  for (curl_slist* hdr = headers; hdr; hdr = hdr->next) {
    Logger::Hidden(hdr->data);
  }

  cancelled = false;
  curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errbuf);
  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
  curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout_seconds); 
  curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, ReadHeader);
  curl_easy_setopt(curl, CURLOPT_HEADERDATA, &ctx);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, ReadBody);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ctx);

  resp_headers.clear();
  resp_status = 0;

  int nret = curl_easy_perform(curl);
  curl_slist_free_all(headers);
  headers = nullptr;

  if (this->cancelled) {
      throw exceptions::RMSNetworkException(
              "Network operation was cancelled by user",
              exceptions::RMSNetworkException::CancelledByUser);
  }

  Logger::Info("Response StatusCode: %d", resp_status);
  Logger::Hidden("--> Response Headers:");
  for(const auto& kv: resp_headers) {
    Logger::Hidden("%s: %s", kv.first.c_str(), kv.second.c_str());
  }
  Logger::Hidden("--> Response Body:");
  Logger::Hidden(string(response.begin(), response.end()).c_str());

  if (errbuf[0] || nret != CURLE_OK) {
    Logger::Error("CURL error: %d, errmsg: %s", nret, errbuf);
  }

  return StatusCode(resp_status);
}

const string HttpClientQt::GetResponseHeader(const string& headerName) {
  //return string(lastReply_->rawHeader(headerName.c_str()).data());
  return resp_headers[headerName];
}

void HttpClientQt::SetAllowUI(bool /* allow*/)
{
  throw exceptions::RMSNotFoundException("Not implemented");
}

static int ParseStatus(const std::string& header)
{
  static const char HTTP[] = "HTTP/";
  if (strncmp(header.c_str(), HTTP, sizeof(HTTP) - 1) == 0) {
    int status;
    int nret = sscanf(header.c_str(), "%*s%d", &status);
    return nret == 1 ? status : 0;
  }
  return 0;
}

static std::pair<std::string, std::string> ParseHeader(const std::string& header)
{
  auto pos = header.find(':');
  auto name = pos != std::string::npos ? header.substr(0, pos) : "";
  for (pos++; header[pos] && isspace(header[pos]); ++pos) {}
  auto value = header.substr(pos);
  return std::make_pair(name, value); 
}

void HttpClientQt::onRespHeader(const std::string& header)
{
  int status = ParseStatus(header);
  if (status == 0) {
    auto kv = ParseHeader(header);
    if (!kv.first.empty()) {
      resp_headers[kv.first] = kv.second;
    }
  } else {
    resp_status = status;
  }
}

bool HttpClientQt::cancel(bool cancelled)
{
  this->cancelled = cancelled;
  return cancelled;
}

} // namespace rmscore 
} // namespace platform
} // namespace http
