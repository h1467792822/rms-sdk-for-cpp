/*
 * ======================================================================
 * Copyright (c) Microsoft Open Technologies, Inc.  All rights reserved.
 * Licensed under the MIT License.
 * See LICENSE.md in the project root for license information.
 * ======================================================================
 */

#include <Logger.h>
#include <FileCache.h>
#include <Constants.h>
#include <QFile>
// #include <QDir>
#include <QFileInfo>
#include <QStandardPaths>
#include <filesystem>

#include "../../rmsutils/RMSDir.h"

using namespace std;

namespace fs = std::filesystem;

namespace rmsauth {
Mutex FileCache::fileLock_;

FileCache::FileCache(const String& filePath)
{
  if (filePath.empty()) // using default settings
  {
    const std::string fileName = "token_cache.dat";
    auto path              = QStandardPaths::writableLocation(
      QStandardPaths::HomeLocation) + "/.ms-ad";

    if (!rmscore::common::RMSDir::mkpath(path.toStdString()))
    {
      throw RmsauthException("Can't create cache directory");
    }
    // QFileInfo fi(QDir(path), fileName);
    // cacheFilePath_ = fi.absoluteFilePath().toStdString();
    cacheFilePath_ = fs::absolute(fs::path(path.toStdString())/fs::path(fileName)).string();
  }
  else
  {
    // QFileInfo fi(QString::fromStdString(filePath));
    // auto userDefinedCacheDir = fi.path();

    if (!fs::exists(filePath))
    {
      throw RmsauthException("Can't find user defined cache directory",
                             filePath);
    }
    // cacheFilePath_ = fi.absoluteFilePath().toStdString();
    cacheFilePath_ = fs::absolute(fs::path(filePath)).string();
  }

  Logger::info(Tag(), "path: %", cacheFilePath_);
}

void FileCache::readCache()
{
  Logger::info(Tag(), "readCache");
  ifstream ifs(cacheFilePath_, ios::binary | ios::ate);

  if (!ifs.is_open())
  {
    Logger::info(Tag(), "Cache file doesn't exist! '%'", cacheFilePath_);
    return;
  }
  ifstream::pos_type pos = ifs.tellg();

  ByteArray cacheData(pos);

  ifs.seekg(0, ios::beg);
  ifs.read(&cacheData[0], pos);
  deserialize(cacheData);
  ifs.close();
}

void FileCache::writeCache()
{
  Logger::info(Tag(), "writeCache");
  ofstream ofs(cacheFilePath_, ios::binary | ios::trunc);

  if (!ofs.is_open())
  {
    Logger::info(Tag(), "Can't open cache file for writing! '%'", cacheFilePath_);
    return;
  }

  auto cacheData = serialize();
  ofs.write(&cacheData[0], cacheData.size());
  ofs.close();
}

void FileCache::clear()
{
  Logger::info(Tag(), "clear");
  TokenCache::clear();
  Lock l(fileLock_);
  bool ok = QFile::remove(cacheFilePath_.c_str());

  if (!ok)
  {
    Logger::error(Tag(), "clear: Failed to delete a file: ");
  }
}

void FileCache::onBeforeAccess(const TokenCacheNotificationArgs& /* args*/)
{
  Logger::info(Tag(), "onBeforeAccess");
  Lock l(fileLock_);

  readCache();
}

void FileCache::onAfterAccess(const TokenCacheNotificationArgs& /* args*/)
{
  Logger::info(Tag(), "onAfterAccess");
  Lock l(fileLock_);

  // if the access operation resulted in a cache update
  if (hasStateChanged_)
  {
    writeCache();
    hasStateChanged_ = false;
  }
}
} // namespace rmsauth {
