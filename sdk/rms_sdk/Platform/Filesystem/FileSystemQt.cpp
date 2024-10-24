/*
 * ======================================================================
 * Copyright (c) Microsoft Open Technologies, Inc.  All rights reserved.
 * Licensed under the MIT License.
 * See LICENSE.md in the project root for license information.
 * ======================================================================
*/

#ifdef QTFRAMEWORK
#include<QDir>
#include<vector>
#include"FileSystemQt.h"
#include"FileQt.h"
#include "../../Platform/Logger/Logger.h"
#include "../../rmsutils/RMSDir.h"

using namespace rmscore::platform::logger;

namespace rmscore { namespace platform { namespace filesystem {

std::shared_ptr<IFileSystem> IFileSystem::Create()
{
    return std::make_shared<FileSystemQt>();
}

std::shared_ptr<IFile> FileSystemQt::OpenLocalStorageFile(const std::string& filePath, FileOpenModes mode)
{
    return IFile::Create(filePath, mode);
}

std::shared_ptr<IFile> FileSystemQt::OpenProtectedLocalStorageFile(const std::string& filePath, FileOpenModes mode)
{
    return IFile::Create(filePath, mode);
}

void FileSystemQt::DeleteLocalStorageFile(const std::string& filePath)
{
    bool ok = std::filesystem::remove(filePath.c_str());
    if(!ok)
    {
        Logger::Hidden("Failed to delete a file: %s", filePath.c_str());
    }
}

void IFileSystem::CreateDirectory(const std::string& dirPath) {
    auto directory = QString::fromStdString(dirPath);
    if (!std::filesystem::exists(directory.toStdString())) {
      // QDir dir;
      // dir.mkpath(directory);
      common::RMSDir::mkpath(dirPath);
    }
}

std::vector<std::string> FileSystemQt::QueryLocalStorageFiles(const std::string& folder, const std::string& pattern)
{
    Logger::Info("+FileSystemQt::QueryLocalStorageFiles");
    Logger::Hidden("Looking for pattern %s in folder %s.\n", pattern.c_str(), folder.c_str());
    // QStringList filters;
    // filters << pattern.c_str();
    // QDir dir(folder.c_str());
    // dir.setNameFilters(filters);
    // std::vector<std::string> res;
    // for( QString f : dir.entryList(filters))
    // {
    //     res.push_back(f.toStdString());
    // }
    std::vector<std::string> filters = { pattern.c_str() };
    std::vector<std::string> res = common::RMSDir::entryList(folder, filters);
    Logger::Info("-FileSystemQt::QueryLocalStorageFiles");
    return res;
}

}}} // namespace rmscore { namespace platform { namespace filesystem {
#endif
