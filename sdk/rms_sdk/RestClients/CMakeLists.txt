cmake_minimum_required(VERSION 3.0)

set(TARGET_NAME modrestclients)

PROJECT(${TARGET_NAME} VERSION 1.0.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES_FILE
    UsageRestrictionsClient.cpp
    RestServiceUrlClient.cpp
    RestHttpClient.cpp
    AuthenticationHandler.cpp
    RestServiceUrls.cpp
    LicenseParser.cpp
    Domain.cpp
    CXMLUtils.cpp
    DnsClientResult.cpp
    DnsLookupClient.cpp
    RestClientErrorHandling.cpp
    ServiceDiscoveryClient.cpp
    RestClientCache.cpp
    TemplatesClient.cpp
    PublishClient.cpp)

set(HEAD_FILE
    RestObjects.h
    IUsageRestrictionsClient.h
    IPublishClient.h
    UsageRestrictionsClient.h
    IRestServiceUrlClient.h
    RestServiceUrlClient.h
    ServiceDiscoveryDetails.h
    RestHttpClient.h
    AuthenticationHandler.h
    RestServiceUrls.h
    LicenseParser.h
    Domain.h
    CXMLUtils.h
    DnsClientResult.h
    DnsLookupClient.h
    IDnsLookupClient.h
    RestClientErrorHandling.h
    IRestClientCache.h
    RestClientCache.h
    IServiceDiscoveryClient.h
    ServiceDiscoveryClient.h
    TemplatesClient.h
    ITemplatesClient.h
    PublishClient.h
    LicenseParserResult.h)

add_library(${TARGET_NAME} STATIC ${SOURCE_FILE} ${HEAD_FILE})
set_target_properties(${TARGET_NAME} PROPERTIES LINKER_LANGUAGE CXX)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR})
