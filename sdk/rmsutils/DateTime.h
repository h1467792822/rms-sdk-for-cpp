#ifndef RMS_UTILS_DATETIME_H
#define RMS_UTILS_DATETIME_H

#include <cassert>
#include <cstdint>
#include <cstring>
#include <ctime>
#include <string>

namespace rmsutils {

enum DateFormat {
    TextData = 0,
    ISODate = 1,
    RFC2822Date = 8,
    ISODateWidthMs = 9,
};

class DateTime {
    time_t time;
    int gmt_off;

    static int get_gmt_off() {
            time_t t = 0;
            struct tm gt = {};
            gmtime_r(&t, &gt);
            return -mktime(&gt);
    }

    static int format_len(const char* first) {
            if (first[1] == first[0]) {
                    if (first[2] == first[0]) {
                            if (first[3] == first[0]) return 4;
                            return 3;
                    }
                    return 2;
            }
            return 1;
    }

    static std::string to_cstd_format(const std::string& format)
    {
            static const char* format_map[][5] = {
                    {"y", "y", "%y", "%yy", "%Y" },
                    {"d", "%e", "%d", "%a", 0 },
                    {"M", "M", "%m", "%b", 0 },
                    {"H", "%k", "%H", 0, 0 },
                    {"m", "m", "%M", 0, 0 },
                    {"s", "s", "%S", 0, 0 },
                    {"z", "z", "zz", "", 0 },
            };
            static int format_map_size = sizeof(format_map) / sizeof(format_map[0]);
            std::string std_format;
            std_format.reserve(format.size());
            auto data = format.data();
            for (int i = 0; i < format.size(); ++i) {
                    if (data[i] == '%') {
                            std_format.push_back('%');
                            if (data[i + 1]) std_format.push_back(data[i + 1]);
                            ++i;
                            continue;
                    }

                    int j = 0;
                    for (; j < format_map_size; ++j) {
                            if (data[i] == format_map[j][0][0]) {
                                    int len = format_len(&data[i]);
                                    while (format_map[j][len] == 0) {
                                            --len;
                                    }
                                    std_format.append(format_map[j][len]);
                                    i += len - 1;
                                    break;
                            }
                    }
                    if (j == format_map_size) {
                            std_format.push_back(data[i]);
                    }
            }
            return std_format;
    }
public:
    DateTime(): time(::time(0)), gmt_off(0) {}
    DateTime(time_t time, int gmt_off = 0) : time(time), gmt_off(gmt_off) {}
    DateTime(int year, int month, int day, int gmt_off = 0): gmt_off(gmt_off)
    {
        struct tm tm = {};
        tm.tm_year = year - 1900;
        tm.tm_mon = month;
        tm.tm_mday = day;
        time = mktime(&tm);
    }
    int64_t msecsTo(const DateTime& other) const { return (other.time - time) * 1000000; }
    std::string toString(DateFormat format = ISODate) const
    {
        assert(format == ISODate);
        return toString("%T-%m-%dT%H:%M:%SZ");
    }

    std::string toString(const std::string& format) const
    {
        char buf[32];
        struct tm tm;
        time_t lt = time - gmt_off;
        localtime_r(&lt, &tm);
        std::string cformat = to_cstd_format(format);
        size_t len = strftime(buf, sizeof(buf), cformat.c_str(), &tm);
        buf[len] = 0;
        return buf;
    }

    bool isNull() const { return false; }

    DateTime toLocalTime() const
    {
        return DateTime(time, 0);
    }

    DateTime toUTC() const
    {
        return DateTime(time, get_gmt_off());
    }

    DateTime addSecs(int64_t s) const
    {
            return DateTime(time + s, gmt_off);
    }

    DateTime addMSecs(int64_t msecs) const
    {
            return DateTime(time + msecs / 1000000, gmt_off);
    }

    DateTime addDays(int64_t ndays) const
    {
            return DateTime(time + ndays * 24 * 3600, gmt_off);
    }

    DateTime addMonths(int nmonths) const
    {
            struct tm tm = {};
            localtime_r(&time, &tm);
            tm.tm_year += (tm.tm_mon + nmonths) / 12;
            tm.tm_mon += (tm.tm_mon + nmonths) % 12;
            return DateTime(mktime(&tm), gmt_off);
    }

    DateTime addYears(int nyears) const
    {
            struct tm tm = {};
            localtime_r(&time, &tm);
            tm.tm_year += nyears;
            return DateTime(mktime(&tm), gmt_off);
    }

    time_t toTime_t() const { return time; }

    bool operator>(const DateTime& other) const { return time > other.time; }

    static DateTime currentDateTime() { return DateTime(); }
    static DateTime currentDateTimeUtc() { return DateTime(::time(0), get_gmt_off()); }
    static DateTime fromString(const char* str, DateFormat format = ISODate)
    {
        assert(format == ISODate);
        struct tm tm = {};
        sscanf(str, "%d-%d-%dT%d:%d:%d", &tm.tm_year, &tm.tm_mon, &tm.tm_mday, &tm.tm_hour, &tm.tm_min, &tm.tm_sec);
        return DateTime(mktime(&tm));
    }
    static DateTime fromTime_t(time_t time) { return DateTime(time); }
};

}    // namespace rmsutils

#endif
