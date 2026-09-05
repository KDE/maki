# SPDX-FileCopyrightText: 2026 Alex Huber <dev.flu7x@gmail.com>
# SPDX-License-Identifier: BSD-2-Clause

find_package(Qt6 CONFIG REQUIRED Test)

configure_file(testconfig.h.in ${CMAKE_CURRENT_BINARY_DIR}/testconfig.h @ONLY)

ecm_add_test(videotest.cpp 
    ../src/backend/PreviewHandler.h
    ../src/backend/VideoHandler.h
    ../src/backend/VideoHandler.cpp
    TEST_NAME "videotest"
    LINK_LIBRARIES Qt6::Test Qt6::Core
)

ecm_add_test(texttest.cpp 
    ../src/backend/PreviewHandler.h
    ../src/backend/TextHandler.h
    ../src/backend/TextHandler.cpp
    TEST_NAME "texttest"
    LINK_LIBRARIES Qt6::Test Qt6::Core
)

ecm_add_test(imagetest.cpp 
    ../src/backend/PreviewHandler.h
    ../src/backend/ImageHandler.h
    ../src/backend/ImageHandler.cpp
    TEST_NAME "imagetest"
    LINK_LIBRARIES Qt6::Test Qt6::Core Qt6::Quick KF6::Archive
)

ecm_add_test(audiotest.cpp 
    ../src/backend/PreviewHandler.h
    ../src/backend/AudioHandler.h
    ../src/backend/AudioHandler.cpp
    TEST_NAME "audiotest"
    LINK_LIBRARIES Qt6::Test Qt6::Core Qt6::Quick KF6::FileMetaData
)