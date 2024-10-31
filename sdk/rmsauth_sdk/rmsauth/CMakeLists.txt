cmake_minimum_required(VERSION 3.10)

# 设置项目名称
project(rmsauth)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)


# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置相对路径
set(REPO_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../..)


set(SOURCES
    AuthenticationContext.cpp
    Url.cpp
    Guid.cpp
    utils.cpp
    Authenticator.cpp
    AuthenticatorTemplate.cpp
    AuthenticatorTemplateList.cpp
    CallState.cpp
    AcquireTokenHandlerBase.cpp
    AcquireTokenInteractiveHandler.cpp
    AcquireTokenNonInteractiveHandler.cpp
    ClientKey.cpp
    TokenCache.cpp
    AuthenticationResult.cpp
    RequestParameters.cpp
    ClientAssertionCertificate.cpp
    ClientCredential.cpp
    ClientAssertion.cpp
    IWebUI.cpp
    UserIdentifier.cpp
    RmsauthIdHelper.cpp
    Logger.cpp
    TokenCacheKey.cpp
    TokenCacheItem.cpp
    UserCredential.cpp
    UserAssertion.cpp
    UserRealmDiscoveryResponseQt.cpp
    AcquireTokenForClientHandler.cpp
    FileCacheEncrypted.cpp
    IRMSAuthEnvironment.cpp
    IRMSAuthEnvironmentImpl.cpp
    DateTime.cpp
    GuidQt.cpp
    UrlQt.cpp
    DateTimeQt.cpp
    HttpHelperQt.cpp
    OAuth2ResponseQt.cpp
    JsonUtilsQt.cpp
    RmsauthIdHelperQt.cpp
    WebUIQt.cpp
    AuthenticatorTemplateQt.cpp
    LoggerImplQt.cpp
    AuthenticationResultQt.cpp
    UserInfoQt.cpp
    FileCacheQt.cpp
    utilsQt.cpp
    TokenCacheQt.cpp
    AcquireTokenNonInteractiveHandlerQt.cpp
    RequestParametersQt.cpp
)
set(HEADER
    rmsauth/AuthenticationContext.h
    rmsauth/types.h
    rmsauth/Exceptions.h
    rmsauth/Url.h
    rmsauth/DateTime.h
    rmsauth/Guid.h
    rmsauth/utils.h
    rmsauth/Authenticator.h
    rmsauth/AuthenticatorTemplate.h
    rmsauth/AuthenticatorTemplateList.h
    rmsauth/AuthorityType.h
    rmsauth/CallState.h
    rmsauth/Constants.h
    rmsauth/AcquireTokenHandlerBase.h
    rmsauth/AcquireTokenInteractiveHandler.h
    rmsauth/AcquireTokenNonInteractiveHandler.h
    rmsauth/ClientKey.h
    rmsauth/TokenCache.h
    rmsauth/TokenSubjectType.h
    rmsauth/AuthenticationResult.h
    rmsauth/RequestParameters.h
    rmsauth/OAuthConstants.h
    rmsauth/OAuth2Response.h
    rmsauth/ClientAssertionCertificate.h
    rmsauth/ClientCredential.h
    rmsauth/ClientAssertion.h
    rmsauth/HttpHelper.h
    rmsauth/Entities.h
    rmsauth/AuthorizationResult.h
    rmsauth/IWebUI.h
    rmsauth/UserIdentifier.h
    rmsauth/RmsauthIdHelper.h
    rmsauth/Logger.h
    rmsauth/TokenCacheKey.h
    rmsauth/TokenCacheNotificationArgs.h
    rmsauth/TokenCacheItem.h
    rmsauth/UserInfo.h
    rmsauth/FileCache.h
    rmsauth/rmsauthExport.h
    rmsauth/UserCredential.h
    rmsauth/UserAssertion.h
    rmsauth/UserRealmDiscoveryResponse.h
    rmsauth/AcquireTokenForClientHandler.h
    rmsauth/FileCacheEncrypted.h
    rmsauth/IRMSAuthEnvironment.h
    rmsauth/IRMSAuthEnvironmentImpl.h
    GuidQt.h
    UrlQt.h
    DateTimeQt.h
    JsonUtilsQt.h
    HttpHelperQt.h 
)

# 创建共享库
add_library(${PROJECT_NAME} SHARED
    ${SOURCES}
    ${HEADERS}
)

# 添加头文件路径
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/rmsauth
)

# 链接依赖库，使用相对路径
target_link_directories(${PROJECT_NAME} PRIVATE ${REPO_ROOT}/bin)
target_link_libraries(${PROJECT_NAME} PRIVATE
    rmsauthWebAuthDialog
    rmscrypto
)

find_package(OpenSSL REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE OpenSSL::Crypto)
# 添加编译定义
target_compile_definitions(${PROJECT_NAME} PRIVATE RMSAUTH_LIBRARY)

# # 安装规则（仅用于 Unix 系统）
# if(UNIX)
#     include(GNUInstallDirs)
#     install(TARGETS ${PROJECT_NAME}
#         LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#     )
# endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${REPO_ROOT}/bin
    RUNTIME_OUTPUT_DIRECTORY ${REPO_ROOT}/bin
)