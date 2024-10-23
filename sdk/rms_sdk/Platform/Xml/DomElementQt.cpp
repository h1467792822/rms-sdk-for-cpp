/*
 * ======================================================================
 * Copyright (c) Microsoft Open Technologies, Inc.  All rights reserved.
 * Licensed under the MIT License.
 * See LICENSE.md in the project root for license information.
 * ======================================================================
*/

#ifdef QTFRAMEWORK
#include "DomElementQt.h"

std::string	DomElementQt::attribute(const std::string & name, const std::string & defValue/* = std::string()*/) const
{
    return this->impl_.attribute(QString::fromStdString(name), QString::fromStdString(defValue)).toStdString();
}

std::string	DomElementQt::text() const
{
    return this->impl_.text().toStdString();
}

// from IDomNode
sp<IDomElement>     DomElementQt::toElement() const
{
    QDomElement elem = this->impl_.toElement();
    return  std::make_shared<DomElementQt>(elem);
}

#endif
