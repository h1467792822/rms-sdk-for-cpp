/*
 * ======================================================================
 * Copyright (c) Microsoft Open Technologies, Inc.  All rights reserved.
 * Licensed under the MIT License.
 * See LICENSE.md in the project root for license information.
 * ======================================================================
*/

#include "tools.h"
#include <openssl/evp.h>
#include <vector>
#include <string>
#include <iostream>
#include <random>
#include <sstream>

#define TIME_CONVERSION_MS_TO_100NS 10000

using namespace std;

namespace rmscore {
namespace common {
uint64_t timeToWinFileTime(const DateTime& dateTime) {
  // Definition of FILETIME from MSDN:
  // Contains a 64-bit value representing the number of 100-nanosecond intervals
  // since January 1, 1601 (UTC).
  //QDateTime origin(QDate(1601, 1, 1), QTime(0, 0, 0, 0), Qt::UTC);
  DateTime origin(1601, 1, 1);

  // Get offset - note we need 100-nanosecond intervals, hence we multiply by
  // 10000.
  return TIME_CONVERSION_MS_TO_100NS * origin.msecsTo(dateTime);
}

ByteArray ConvertBase64ToBytes(const ByteArray& base64str) {
  std::string encoded_string(base64str.begin(), base64str.end());

  int padding = 0;
  if (base64str.size() >= 2) {
    if (base64str[base64str.size() - 1] == '=') padding++;
    if (base64str[base64str.size() - 2] == '=') padding++;
  }

  int encoded_size = base64str.size();
  int decoded_size = (encoded_size * 3) / 4 - padding;

  ByteArray decoded_data(decoded_size);

  int result = EVP_DecodeBlock(decoded_data.data(),
                               reinterpret_cast<const unsigned char*>(base64str.data()),
                               base64str.size());

  if (result < 0) {
    throw std::runtime_error("Base64 decoding failed.");
  }

  decoded_data.resize(result - padding);
  return decoded_data;
  // QByteArray ba;

  // ba.append(QByteArray(reinterpret_cast<const char *>(base64str.data()),
  //                      static_cast<int>(base64str.size())));

  // auto convArray = QByteArray::fromBase64(ba);

  // return ByteArray(convArray.begin(), convArray.end());
}

string timeToString(const DateTime& dateTime) {
  if (!dateTime.isNull())
  {
    //    return dateTime.toString("yyyy-MM-DDThh:mm:ssZ").toStdString();
    //return dateTime.toString(Qt::ISODate).toStdString();
    return dateTime.toString();
  }
  else
  {
    return string();
  }
}

ByteArray ConvertBytesToBase64(const ByteArray& bytes) {
  return ConvertBytesToBase64(bytes.data(), bytes.size());
}

ByteArray encode_base64_openssl(const uint8_t* bytes, size_t size) {
  size_t encoded_size = 4 * ((size + 2) / 3);

  ByteArray encoded_data(encoded_size);

  int result = EVP_EncodeBlock(encoded_data.data(), bytes, static_cast<int>(size));

  if (result < 0) {
    throw std::runtime_error("Base64 encoding failed.");
  }

  return encoded_data;
}

ByteArray ConvertBytesToBase64(const void *bytes, const size_t size)
{
   return encode_base64_openssl(static_cast<const uint8_t*>(bytes), size);
}

string GenerateAGuid()
{
  std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint32_t> dis(0, 15);
    std::uniform_int_distribution<uint32_t> dis2(8, 11);

    std::stringstream ss;
    ss << std::hex;
    ss << std::uppercase;

    ss << '{';
    for (int i = 0; i < 8; ++i)
        ss << dis(gen);

    ss << "-";
    for (int i = 0; i < 4; ++i)
        ss << dis(gen);

    ss << "-4";  // UUID version 4
    for (int i = 0; i < 3; ++i)
        ss << dis(gen);

    ss << "-";
    ss << dis2(gen);  // UUID variant
    for (int i = 0; i < 3; ++i)
        ss << dis(gen);

    ss << "-";
    for (int i = 0; i < 12; ++i)
        ss << dis(gen);
    ss << '}';

    return ss.str();
}
} // namespace common
} // namespace rmscore
