/*
 * ======================================================================
 * Copyright (c) Microsoft Open Technologies, Inc.  All rights reserved.
 * Licensed under the MIT License.
 * See LICENSE.md in the project root for license information.
 * ======================================================================
*/

#ifndef DATEQT_H
#define DATEQT_H

#include <DateTime.h>
#include <../../rmsutils/DateTime.h>

namespace rmsauth {

class DateTimeQt : public IDateTime
{
public:
    DateTimeQt();
    DateTimeQt(int64_t sec);

private:
    void addDays(int64_t ndays) override;
    void addMSecs(int64_t msecs) override;
    void addMonths(int nmonths) override;
    void addSecs(int64_t s) override;
    void addYears(int nyears) override;
    int64_t toTime() override;
    virtual String toString(const String& format) override;

private:
    rmsutils::DateTime dateTime_;
    friend class DateTime;
};

} //namespace rmsauth {

#endif // DATEQT_H
