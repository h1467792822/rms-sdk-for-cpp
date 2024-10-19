/*
 * ======================================================================
 * Copyright (c) Microsoft Open Technologies, Inc.  All rights reserved.
 * Licensed under the MIT License.
 * See LICENSE.md in the project root for license information.
 * ======================================================================
 */

#ifndef _RMS_LIB_LOGGER_H_
#define _RMS_LIB_LOGGER_H_

#include <string>
#include <cstdio>
#include <cstdlib>
#include <syslog.h>
#include "../Settings/IRMSEnvironmentImpl.h"

namespace rmscore {
namespace platform {
namespace logger {
class Logger {
  //static const int max_length = 1024000;

public:

  template<typename ... Arguments>
  static void Append(int priority,
                     const char* format,
                     Arguments ...      arguments) {
    //auto cArgs =  sizeof ... (Arguments);

    auto env   = settings::IRMSEnvironmentImpl::Environment();
    if (!env || (env->LogOption() == modernapi::IRMSEnvironment::LoggerOption::Never)) {
      return;
    }
    syslog(priority, format, arguments ...);
#if 0
    if (cArgs > 0) {
      std::string buff(max_length, '-');
#ifdef Q_OS_WIN32
      int num_bytes = sprintf_s(&buff[0], max_length,
                                record.c_str(), arguments ...);
#else // ifdef Q_OS_WIN32
            # if defined(__clang__)
                #  pragma clang diagnostic push
                #  pragma clang diagnostic ignored "-Wformat-security"
            # endif // if defined(__clang__)
            # if defined(__GNUC__)
                #  pragma GCC diagnostic ignored "-Wformat-security"
            # endif // if defined(__GNUC__)
      int num_bytes = std::snprintf(&buff[0], max_length,
                                    record.c_str(), arguments ...);
            # if defined(__clang__)
                #  pragma clang diagnostic pop
            # endif // if defined(__clang__)
#endif              // ifdef Q_OS_WIN32

      if (num_bytes < max_length) {
        buff.resize(num_bytes);
      }
      Logger::instance().append(prefix, buff);
    } else {
      Logger::instance().append(prefix, record);
    }
#endif
  }

  template<typename ... Arguments>
  static void Info(const char* format, Arguments ... arguments) {
    Logger::Append(LOG_INFO, format, arguments ...);
  }

  template<typename ... Arguments>
  static void Warning(const char* format, Arguments ... arguments) {
    Logger::Append(LOG_WARNING, format, arguments ...);
  }

  template<typename ... Arguments>
  static void Error(const char* format, Arguments ... arguments) {
    Logger::Append(LOG_ERR, format, arguments ...);
  }

  template<typename ... Arguments>
  static void Hidden(const char* format, Arguments ... arguments) {
    // read env var
    const char* env = getenv("RMS_HIDDEN_LOG");
    // if set
    if (env && strcmp(env, "ON") == 0)
    {
      Logger::Append(LOG_DEBUG, format, arguments ...);
    }
  }

  //virtual ~Logger() {}

protected:

  // virtual void append(const std::string& prefix, const std::string& record) = 0;

private:

  // static Logger& instance();
};
} // namespace logger
} // namespace platform
} // namespace rmscore
#endif // _RMS_LIB_LOGGER_H_
