/*
 * ======================================================================
 * Copyright (c) Microsoft Open Technologies, Inc.  All rights reserved.
 * Licensed under the MIT License.
 * See LICENSE.md in the project root for license information.
 * ======================================================================
*/

#ifndef IDOMELEMENTQTIMPL_H
#define IDOMELEMENTQTIMPL_H

#include "IDomElement.h"
#include <QDomElement>

class DomElementQt : public IDomElement
{
private:
    std::string	attribute(const std::string & name, const std::string & defValue = std::string()) const override;
    std::string	text() const override;

// form IDomNode
    sp<IDomElement>     toElement() const override;

public:
    DomElementQt(const QDomElement& other):impl_(other){}

private:
    QDomElement impl_;
//    friend class DomNodeQt;
//    friend class DomDocumentQt;
//    friend class DomAttributeQt;

};

#endif // IDOMELEMENTQTIMPL_H
