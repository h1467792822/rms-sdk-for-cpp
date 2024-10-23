/*
 * ======================================================================
 * Copyright (c) Microsoft Open Technologies, Inc.  All rights reserved.
 * Licensed under the MIT License.
 * See LICENSE.md in the project root for license information.
 * ======================================================================
*/

#ifndef IDOMELEMENT_H
#define IDOMELEMENT_H

#include <string>
#include "IDomNode.h"

class IDomElement : public IDomNode
{
public:
    virtual std::string	attribute(const std::string & name, const std::string & defValue = std::string()) const = 0;
    virtual std::string	text() const = 0;
};
#endif // IDOMELEMENT_H
