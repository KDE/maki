# SPDX-FileCopyrightText: 2026 Alex Huber <dev.flu7x@gmail.com>
# SPDX-License-Identifier: BSD-2-Clause

cmake_minimum_required(VERSION 3.20)

# KDE Application Version, managed by release script later
set (RELEASE_SERVICE_VERSION_MAJOR "00")
set (RELEASE_SERVICE_VERSION_MINOR "02")
set (RELEASE_SERVICE_VERSION_MICRO "10")
set (RELEASE_SERVICE_VERSION "${RELEASE_SERVICE_VERSION_MAJOR}.${RELEASE_SERVICE_VERSION_MINOR}.${RELEASE_SERVICE_VERSION_MICRO}")
project(maki VERSION ${RELEASE_SERVICE_VERSION})

find_package(ECM 6.0.0 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDEClangFormat)
include(KDEGitCommitHooks)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(ECMFindQmlModule)
include(ECMQmlModule)
include(ECMUninstallTarget)
include(ECMAddTests)
include(ECMSetupVersion)

ecm_setup_version(${RELEASE_SERVICE_VERSION}
    VARIABLE_PREFIX MAKI
    VERSION_HEADER MakiVersion.h
)

# Core packages for most of the work
find_package(Qt6 REQUIRED COMPONENTS
    Core
    Concurrent
    Quick
    Gui
    Widgets
    Multimedia
    Pdf
)
    
find_package(KF6 REQUIRED COMPONENTS
    Kirigami
    KirigamiAddons
    I18n
    CoreAddons
    IconThemes
    KIO
    Archive
    DBusAddons
    FileMetaData
    SyntaxHighlighting
    Service
)

# package for video playback
find_package(MpvQt REQUIRED)

ecm_find_qmlmodule(org.kde.kirigami REQUIRED)

add_subdirectory(src)

if(BUILD_TESTING)
    add_subdirectory(autotests)
endif()

ki18n_install(po)

configure_file(org.kde.maki.desktop.in ${CMAKE_CURRENT_BINARY_DIR}/org.kde.maki.desktop)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/org.kde.maki.desktop DESTINATION ${KDE_INSTALL_AUTOSTARTDIR})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/org.kde.maki.desktop DESTINATION ${KDE_INSTALL_APPDIR})

install(PROGRAMS org.kde.maki.servicemenu.desktop DESTINATION ${KDE_INSTALL_DATADIR}/kio/servicemenus)

configure_file(org.kde.maki.service.in ${CMAKE_CURRENT_BINARY_DIR}/org.kde.maki.service)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/org.kde.maki.service DESTINATION ${KDE_INSTALL_DBUSSERVICEDIR})

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)

file(GLOB_RECURSE ALL_CLANG_FORMAT_SOURCE_FILES *.cpp *.h)
kde_clang_format(${ALL_CLANG_FORMAT_SOURCE_FILES})
kde_configure_git_pre_commit_hook(CHECKS CLANG_FORMAT)
