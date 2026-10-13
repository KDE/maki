# SPDX-FileCopyrightText: 2026 Alex Huber <dev.flu7x@gmail.com>
# SPDX-License-Identifier: BSD-2-Clause

include(ECMGenerateExportHeader)

add_library(makiLib SHARED)

ecm_generate_export_header(makiLib
    BASE_NAME makiLib
    EXPORT_FILE_NAME MakiLibExport.h
)


ecm_add_qml_module(makiLib
    URI
    org.kde.maki
    GENERATE_PLUGIN_SOURCE
)

target_include_directories(makiLib
    PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/backend"
    "${CMAKE_CURRENT_SOURCE_DIR}/mpv"
    ${CMAKE_CURRENT_BINARY_DIR}
)

set(makiLib_SRCS
    backend/WindowManager.cpp
    backend/WindowManager.h
    backend/ImageProvider.h
    backend/DBusInterface.cpp
    backend/PreviewHandler.h
    backend/ImageHandler.h
    backend/ImageHandler.cpp
    backend/KritaHandler.h
    backend/KritaHandler.cpp
    backend/TextHandler.h
    backend/TextHandler.cpp
    backend/AudioHandler.h
    backend/AudioHandler.cpp
    backend/VideoHandler.h
    backend/VideoHandler.cpp
    backend/DocumentHandler.h
    backend/DocumentHandler.cpp
    backend/ArchiveHandler.h
    backend/ArchiveHandler.cpp
    mpv/MpvItem.h
    mpv/MpvItem.cpp
)

set(maki_dbusXML ${CMAKE_SOURCE_DIR}/dbus/org.kde.maki.xml)

qt_add_dbus_adaptor(makiLib_SRCS ${maki_dbusXML} backend/DBusInterface.h DBusInterface)

target_sources(makiLib
    PRIVATE
    ${makiLib_SRCS}
)

ecm_target_qml_sources(makiLib
    SOURCES
    Main.qml
    qml/ImagePreview.qml
    qml/FallbackPreview.qml
    qml/TextPreview.qml
    qml/AudioPreview.qml
    qml/VideoPreview.qml
    qml/DocumentPreview.qml
    qml/ArchivePreview.qml
    qml/HeaderBar.qml
)

target_link_libraries(makiLib
    PUBLIC
    Qt6::Quick
    Qt6::Qml
    Qt6::Gui
    Qt6::Widgets
    Qt6::Multimedia
    Qt6::Concurrent
    KF6::I18n
    KF6::I18nQml
    KF6::CoreAddons
    KF6::DBusAddons
    KF6::FileMetaData
    KF6::IconThemes
    KF6::Archive
    KF6::KIOCore
    KF6::KIOGui
    KF6::SyntaxHighlighting
    KF6::Service
    Poppler::Qt6
    MpvQt::MpvQt
)
add_executable(maki main.cpp)
target_link_libraries(maki PRIVATE makiLib)

install(TARGETS maki makiLib ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES ${maki_dbusXML} DESTINATION ${KDE_INSTALL_DBUSINTERFACEDIR})