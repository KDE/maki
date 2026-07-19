# SPDX-FileCopyrightText: 2026 Alex Huber <dev.flu7x@gmail.com>
# SPDX-License-Identifier: BSD-2-Clause

add_executable(Maki)

ecm_add_qml_module(Maki
    URI
    io.github.FluxMusic.Maki
)

target_include_directories(Maki
    PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/backend"
    "${CMAKE_CURRENT_SOURCE_DIR}/mpv"
    ${CMAKE_CURRENT_BINARY_DIR}
)

set(Maki_SRCS
    main.cpp
    backend/WindowManager.cpp
    backend/WindowManager.h
    backend/ImageProvider.h
    backend/DBusInterface.cpp
    backend/PreviewHandler.h
    backend/ImageHandler.h
    backend/ImageHandler.cpp
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

set(Maki_dbusXML ${CMAKE_SOURCE_DIR}/dbus/io.github.FluxMusic.Maki.xml)

qt_add_dbus_adaptor(Maki_SRCS ${Maki_dbusXML} backend/DBusInterface.h DBusInterface)

target_sources(Maki
    PRIVATE
    ${Maki_SRCS}
)

ecm_target_qml_sources(Maki
    SOURCES
    Main.qml
    qml/ImagePreview.qml
    qml/FallbackPreview.qml
    qml/TextPreview.qml
    qml/AudioPreview.qml
    qml/VideoPreview.qml
    qml/DocumentPreview.qml
    qml/ArchivePreview.qml
    qml/DefaultAppButton.qml
)

target_link_libraries(Maki
    PRIVATE
    Qt6::Quick
    Qt6::Qml
    Qt6::Gui
    Qt6::Widgets
    Qt6::Multimedia
    Qt6::Concurrent
    KF6::I18n
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

install(TARGETS Maki ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES ${Maki_dbusXML} DESTINATION ${KDE_INSTALL_DBUSINTERFACEDIR})