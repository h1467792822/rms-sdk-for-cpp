/*
 * ======================================================================
 * Copyright (c) Microsoft Open Technologies, Inc.  All rights reserved.
 * Licensed under the MIT License.
 * See LICENSE.md in the project root for license information.
 * ======================================================================
*/

#include <openssl/bio.h>
#include <openssl/pem.h>
#include <openssl/x509.h>
#include <algorithm>
#include "HttpHelper.h"
#include "../../rmsutils/Guard.h"

namespace rmscore {
namespace modernapi {

static std::vector<uint8_t> all_certs;

static const std::vector<uint8_t>& getAllCACertificatesBase64()
{
  return all_certs;
}

#if 0
bool addCACertificate(const std::vector<uint8_t>& certificate,
                      QSsl::EncodingFormat        format) {
  QSslConfiguration SslConfiguration(QSslConfiguration::defaultConfiguration());

  QList<QSslCertificate> certificates = SslConfiguration.caCertificates();

  QList<QSslCertificate> cert = QSslCertificate::fromData(
    QByteArray(reinterpret_cast<const char *>(certificate.data()),
               static_cast<int>(certificate.size())), format);

  if (cert.length() == 0) return false;

  certificates.append(cert);

  SslConfiguration.setCaCertificates(certificates);
  QSslConfiguration::setDefaultConfiguration(SslConfiguration);

  return true;
}
#endif

static void addCertificate(const uint8_t* data, size_t size)
{
  std::copy(data, data + size, std::back_inserter(all_certs));
}

bool HttpHelper::addCACertificateBase64(const std::vector<uint8_t>& certificate) {
  BIO* bio = BIO_new_mem_buf(certificate.data(), certificate.size());
  if (bio == nullptr) {
    return false;
  }
  MAKE_GUARD([=]() { BIO_free(bio); });

  X509* x509 = PEM_read_bio_X509(bio, 0, 0, 0);
  if (x509 == nullptr) {
    return false;
  }
  MAKE_GUARD([=]() { X509_free(x509); });

  addCertificate(certificate.data(), certificate.size());
  return true;
}

bool HttpHelper::addCACertificateDer(const std::vector<uint8_t>& certificate) {
  const unsigned char* data = (const unsigned char*)certificate.data();
  X509* x509 = d2i_X509(nullptr, &data, certificate.size());
  if (!x509) {
    return false;
  }
  MAKE_GUARD([=]() { X509_free(x509); });

  BIO* bio = BIO_new(BIO_s_mem());
  if (!bio) {
    return false;
  }
  MAKE_GUARD([=]() { BIO_free(bio); });

  int nret = PEM_write_bio_X509(bio, x509);
  char* buf = 0;
  auto len = BIO_get_mem_data(bio, &buf);
  
  if (nret == 0 || len == 0 || buf == nullptr) {
    return false;
  }

  addCertificate((const uint8_t*)buf, len);
  return true;
}
} // namespace modernapi
} // namespace rmscore
