cmake_minimum_required(VERSION 3.10)

project(rms)

set(REPO_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../..)


# 添加包含路径
include_directories(${REPO_ROOT}/sdk/rmscrypto_sdk/CryptoAPI)

# 定义为库项目
add_library(rms SHARED)

link_directories(
    ${REPO_ROOT}/bin
    ${REPO_ROOT}/bin/rms
    ${REPO_ROOT}/bin/rms/platform
)

# 编译选项
add_definitions(-DRMS_LIBRARY)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    # MSVC 警告级别设置
    string(REPLACE "/W3" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
endif()

# 源文件
target_sources(rms PRIVATE
    UserPolicy.cpp
    TemplateDescriptor.cpp
    ConsentCallbackImpl.cpp
    PolicyDescriptor.cpp
    ProtectedFileStream.cpp
    CustomProtectedStream.cpp
    HttpHelper.cpp
    IRMSEnvironment.cpp
    roles.cpp
)

# 头文件
set(HEADERS
    UserPolicy.h
    IAuthenticationCallback.h
    IConsentCallback.h
    TemplateDescriptor.h
    rights.h
    ConsentCallbackImpl.h
    IConsentCallbackImpl.h
    AuthenticationCallbackImpl.h
    IAuthenticationCallbackImpl.h
    ConsentResult.h
    IConsent.h
    PolicyDescriptor.h
    UserRights.h
    UserRoles.h
    AuthenticationParameters.h
    ConsentType.h
    ProtectedFileStream.h
    CustomProtectedStream.h
    ext/QTStreamImpl.h
    HttpHelper.h
    ModernAPIExport.h
    CacheControl.h
    RMSExceptions.h
    IRMSEnvironment.h
    roles.h
)

# 链接库
set(COMMON_LIBS
    udns
    curl
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_link_libraries(rms PRIVATE
        modprotectedfiled
        modcored
        modrestclientsd
        modconsentd
        modcommond
        modjsond
        platformhttpd
        platformjsond
        platformfilesystemd
        platformsettingsd
        rmscryptod
        ${COMMON_LIBS}
    )
else()
    target_link_libraries(rms PRIVATE
        modprotectedfile
        modcore
        modrestclients
        modconsent
        modcommon
        modjson
        platformhttp
        platformjson
        platformfilesystem
        platformsettings
        rmscrypto
        ${COMMON_LIBS}
    )
endif()

# 添加运行时库搜索路径
set_target_properties(rms PROPERTIES
    BUILD_WITH_INSTALL_RPATH TRUE
    INSTALL_RPATH "${REPO_ROOT}/bin"
    INSTALL_RPATH_USE_LINK_PATH TRUE
)

# Windows 特定设置
if(WIN32)
    target_link_libraries(rms PRIVATE
        ssleay32
        libeay32
        Gdi32
        User32
        Advapi32
    )
    # 添加资源文件
    target_sources(rms PRIVATE rmscore.rc)
else()
    target_link_libraries(rms PRIVATE
        ssl
        crypto
    )
endif()

# 安装规则 (仅用于 Unix 系统)
if(UNIX)
    include(GNUInstallDirs)
    install(TARGETS rms
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )
endif()

# 在项目定义之后添加
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# 对于 C++ 编译器特别设置
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

# 设置输出目录
set_target_properties(${PROJECT_NAME} PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${REPO_ROOT}/bin"
)