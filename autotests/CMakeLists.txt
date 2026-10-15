# SPDX-FileCopyrightText: 2026 Alex Huber <dev.flu7x@gmail.com>
# SPDX-License-Identifier: BSD-2-Clause

find_package(Qt6 CONFIG REQUIRED Test)

ecm_add_test(FileRoutingTest.cpp
    TEST_NAME "fileroutingtest"
    LINK_LIBRARIES makiLib Qt6::Test Qt6::Core
)

ecm_add_test(ArchiveTest.cpp 
    TEST_NAME "archivetest"
    LINK_LIBRARIES makiLib Qt6::Test Qt6::Core Qt6::Quick KF6::Archive
)

ecm_add_test(AudioTest.cpp 
    TEST_NAME "audiotest"
    LINK_LIBRARIES makiLib Qt6::Test Qt6::Core Qt6::Quick KF6::FileMetaData
)

ecm_add_test(DocumentTest.cpp 
    TEST_NAME "documenttest"
    LINK_LIBRARIES makiLib Qt6::Test Qt6::Core Qt6::Quick Qt6::Concurrent
)

ecm_add_test(ImageTest.cpp 
    TEST_NAME "imagetest"
    LINK_LIBRARIES makiLib Qt6::Test Qt6::Core Qt6::Quick KF6::Archive
)

ecm_add_test(TextTest.cpp 
    TEST_NAME "texttest"
    LINK_LIBRARIES makiLib Qt6::Test Qt6::Core
)

ecm_add_test(VideoTest.cpp 
    TEST_NAME "videotest"
    LINK_LIBRARIES makiLib Qt6::Test Qt6::Core
)
