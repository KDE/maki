# SPDX-FileCopyrightText: 2026 Alex Huber <dev.flu7x@gmail.com>
# SPDX-License-Identifier: BSD-2-Clause

find_package(Qt6 CONFIG REQUIRED Test)

configure_file(testconfig.h.in ${CMAKE_CURRENT_BINARY_DIR}/testconfig.h @ONLY)

ecm_add_test(videotest.cpp 
    TEST_NAME "videotest"
    LINK_LIBRARIES makiLib Qt6::Test Qt6::Core
)

ecm_add_test(texttest.cpp 
    TEST_NAME "texttest"
    LINK_LIBRARIES makiLib Qt6::Test Qt6::Core
)

ecm_add_test(imagetest.cpp 
    TEST_NAME "imagetest"
    LINK_LIBRARIES makiLib Qt6::Test Qt6::Core Qt6::Quick KF6::Archive
)

ecm_add_test(audiotest.cpp 
    TEST_NAME "audiotest"
    LINK_LIBRARIES makiLib Qt6::Test Qt6::Core Qt6::Quick KF6::FileMetaData
)

ecm_add_test(archivetest.cpp 
    TEST_NAME "archivetest"
    LINK_LIBRARIES makiLib Qt6::Test Qt6::Core Qt6::Quick KF6::Archive
)

ecm_add_test(documenttest.cpp 
    TEST_NAME "documenttest"
    LINK_LIBRARIES makiLib Qt6::Test Qt6::Core Qt6::Quick Qt6::Concurrent Poppler::Qt6
)