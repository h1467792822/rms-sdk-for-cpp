/*
 * ======================================================================
 * Copyright (c) Microsoft Open Technologies, Inc.  All rights reserved.
 * Licensed under the MIT License.
 * See LICENSE.md in the project root for license information.
 * ======================================================================
*/

#ifdef QTFRAMEWORK
//#include <QDir>
//#include <QTextStream>
#include <iostream>
#include <cstdio>
#include "FileQt.h"
#include "IFileSystem.h"
#include "../Logger/Logger.h"
using namespace std;
using namespace rmscore::platform::logger;

namespace rmscore {
namespace platform {
namespace filesystem {
shared_ptr<IFile>IFile::Create(const string& path, FileOpenModes mode)
{
  // first check if directory exists
  /*
  QString directory = QString::fromStdString(path);
  auto    idx       = directory.lastIndexOf(QChar('/'));

  if ((idx > 0) && (idx < directory.size())) {
    directory.resize(idx + 1);

    if (!QFile::exists(directory)) {
      QDir dir;
      dir.mkpath(directory);
    }
  }
  */
  auto idx = path.rfind('/');
  if (idx > 0 && idx < path.size()) {
          std::string dir(path.c_str(), path.c_str() + idx);
          IFileSystem::CreateDirectory(dir);
  }

  auto p_file = make_shared<FileQt>(path);

  if (!p_file->open(mode))
  {
    return nullptr;
  }

  return p_file;
}

FileQt::FileQt(const string& path)
  :
  path(path)
{}

bool FileQt::open(FileOpenModes mode)
{
  // FIXME.shch: added QIODevice::Text
  //bool ok = this->impl_.open(FileQt::toQtMode(mode) | QIODevice::Text);
  this->mode = mode;
  file = fopen(path.c_str(), FileQt::toMode(mode));

  //if (!ok)
  if (!file)
  {
    Logger::Error("Cant't open file '%s'",
                  //this->impl_.fileName().toStdString().data());
                  path.c_str());
  }
  return !!file;
}

size_t FileQt::Read(common::CharArray& data, size_t len)
{
  if (!file) return 0;
  data.resize(len);
  //QDataStream out(&this->impl_);
  //auto readBytes = (size_t)out.readRawData(&data[0], (int)len);
  auto readBytes = fread(data.data(), 1, len, file);

  if (readBytes < len) data.resize(readBytes);
  return readBytes;
}

size_t FileQt::Write(const char *data, size_t len)
{
  if (!file) return 0;
  //QDataStream out(&this->impl_);

  //return (size_t)out.writeRawData(data, (int)len);
  return fwrite(data, 1, len, file);
}

void FileQt::Clear()
{
  if (!file) return;
  //auto openmode = this->impl_.openMode();

  //this->impl_.close();
  //this->impl_.open(openmode | QIODevice::Truncate);
  freopen(path.c_str(), FileQt::toMode(mode, true), file);
}

void FileQt::Close()
{
  if (!file) return;
  //this->impl_.close();
  fclose(file);
  file = nullptr;
}

string FileQt::ReadAllAsText()
{
  //auto data = this->impl_.readAll();

  //return string(data);

  if (!file) return "";
  size_t size = GetSize();
  std::string data;
  data.resize(size);
  fread((void*)data.data(), 1, size, file);
  return data; 
}

void FileQt::AppendText(const common::ByteArray& text)
{
  //QTextStream out(&this->impl_);

  //out << QByteArray(reinterpret_cast<const char *>(text.data()),
  //                  static_cast<int>(text.size()));

  if (!file) return;
  fwrite(text.data(), 1, text.size(), file);
}

size_t FileQt::GetSize()
{
  //return (size_t) this->impl_.size();
  if (!file) return 0;
  auto cur = ftell(file);
  fseek(file, 0, SEEK_END);
  auto end = ftell(file);
  fseek(file, cur, SEEK_SET);
  return end - cur;
}

}
}
} // namespace rmscore { namespace platform { namespace filesystem {
#endif // ifdef QTFRAMEWORK
