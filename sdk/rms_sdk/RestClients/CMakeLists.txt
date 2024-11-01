cmake_minimum_required(VERSION 3.14)
project(modrestclients)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置输出目录
set(REPO_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../..)
# find_package(LibXml2 REQUIRED)
# pkg_check_modules(GLIB REQUIRED libxml)
# include_directories(${GLIB_INCLUDE_DIRS})
# link_directories(${GLIB_LIBRARY_DIRS})


# Debug 模式下添加 'd' 后缀
set(CMAKE_DEBUG_POSTFIX "d")

# 源文件
set(SOURCES
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
    PublishClient.cpp
)

# 头文件
set(HEADERS
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
    LicenseParserResult.h
)

# 创建静态库
add_library(${PROJECT_NAME} STATIC
    ${SOURCES}
    ${HEADERS}
)


target_link_libraries(${PROJECT_NAME} xml2)

# 设置包含目录
target_include_directories(${PROJECT_NAME} PRIVATE
    ${REPO_ROOT}/sdk/rmscrypto_sdk/CryptoAPI
)

target_include_directories(${PROJECT_NAME} PRIVATE
    /usr/include/libxml2
)

# 设置输出目录
set_target_properties(${PROJECT_NAME} PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${REPO_ROOT}/bin/rms"
)
