/*
 * ======================================================================
 * Copyright (c) Microsoft Open Technologies, Inc.  All rights reserved.
 * Licensed under the MIT License.
 * See LICENSE.md in the project root for license information.
 * ======================================================================
*/

#ifndef _RMS_LIB_FRAMEWORKSPESIFIC_H_
#define _RMS_LIB_FRAMEWORKSPESIFIC_H_
#include <stdint.h>
//#include <QDateTime>
#include <chrono>
#include <mutex>
#include <locale>
#include <QDataStream>
#include "CommonTypes.h"
#include "DateTime.h"

namespace rmscore {
namespace common {
//using DateTime = QDateTime;
//using Event = std::string;
using Mutex = std::mutex;
using MutexLocker = std::lock_guard<std::mutex>;
using Locale = std::locale;
using DataStream = QDataStream;
using IODevice = QIODevice;
} // namespace common
} // namespace rmscore
#endif // _RMS_LIB_FRAMEWORKSPESIFIC_H_
