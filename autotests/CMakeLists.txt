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