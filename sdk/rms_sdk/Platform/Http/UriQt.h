/*
 * ======================================================================
 * Copyright (c) Microsoft Open Technologies, Inc.  All rights reserved.
 * Licensed under the MIT License.
 * See LICENSE.md in the project root for license information.
 * ======================================================================
*/

#ifndef IURIQTIMPL
#define IURIQTIMPL

#include "IUri.h"
// #include "QUrl"

namespace rmscore { namespace platform { namespace http {

class UriQt : public IUri
{
public:
    UriQt(const std::string& uri);
    ~UriQt();

    virtual const std::string GetScheme() const override;
    virtual const std::string GetHost() const override;
    virtual int GetPort() const override;
    virtual const std::string ToString()const override;

private:
    // QUrl* pImpl_;
    std::string uri_;
    std::string scheme_;
    std::string host_;
    int port_;
};

}}} // namespace rmscore { namespace platform { namespace http {

#endif // IURIQTIMPL

