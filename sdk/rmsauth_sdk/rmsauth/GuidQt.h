/*
 * ======================================================================
 * Copyright (c) Microsoft Open Technologies, Inc.  All rights reserved.
 * Licensed under the MIT License.
 * See LICENSE.md in the project root for license information.
 * ======================================================================
*/

#ifndef GUIDQT_H
#define GUIDQT_H

#include <Guid.h>
// #include <QUuid>
#include <array>
#include <string>

namespace rmsauth {

class GuidQt : public IGuid
{
public:
    GuidQt();
    explicit GuidQt(const std::string& str);

    virtual std::string toString() const override;
    virtual bool empty() const override;

    static GuidQt newGuid();

private:
    std::array<unsigned char, 16> data;
    void parse(const std::string& str);
};

} // namespace rmsauth {

#endif // GUIDQT_H
