#ifndef RMS_SDK_DATETIME_H
#define RMS_SDK_DATETIME_H

#include <cassert>
#include <cstdint>
#include <cstring>
#include <ctime>
#include <string>

namespace rmscore {
namespace common {

enum DateFormat {
    TextData = 0,
    ISODate = 1,
    RFC2822Date = 8,
    ISODateWidthMs = 9,
};

class DateTime {
    time_t time;

public:
    DateTime(time_t time) : time(time) {}
    DateTime(int year, int month, int day)
    {
        struct tm tm = {};
        tm.tm_year = year - 1900;
        tm.tm_mon = month;
        tm.tm_mday = day;
        time = mktime(&tm);
    }
    uint64_t msecsTo(const DateTime& other) const { return difftime(other.time, time) * 1000000; }
    std::string toString(DateFormat format) const
    {
        assert(format == ISODate);
        char buf[32];
        struct tm tm;
        gmtime_r(&time, &tm);
        size_t len = strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%SZ", &tm);
        buf[len] = 0;
        return buf;
    }
    bool isNull() const { return false; }

    DateTime toLocalTime() const
    {
        struct tm tm;
        localtime_r(&time, &tm);
        return DateTime(mktime(&tm));
    }

    DateTime toUTC() const
    {
        struct tm tm;
        gmtime_r(&time, &tm);
        return DateTime(mktime(&tm));
    }

    time_t toTime_t() const { return time; }

    DateTime addSecs(uint64_t s) const { return DateTime(time + s); }

    bool operator>(const DateTime& other) const { return time > other.time; }

    static DateTime currentDateTime() { return DateTime(::time(0)); }
    static DateTime fromString(const char* str, DateFormat format)
    {
        assert(format == ISODate);
        struct tm tm = {};
        sscanf(str, "%d-%d-%dT%d:%d:%d", &tm.tm_year, &tm.tm_mon, &tm.tm_mday, &tm.tm_hour, &tm.tm_min, &tm.tm_sec);
        return DateTime(mktime(&tm));
    }
    static DateTime fromTime_t(time_t time) { return DateTime(time); }
};

}    // namespace common
}    // namespace rmscore

#endif
