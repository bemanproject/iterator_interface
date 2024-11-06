if(BEMAN_ITERATOR_INTERFACE26_ENABLE_TESTING)
    # Fetch GoogleTest
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG
            f8d7d77c06936315286eb55f8de22cd23c188571 # release-1.14.0
        EXCLUDE_FROM_ALL
    )
    block()
        set(INSTALL_GTEST OFF) # Disable GoogleTest installation
        set(BUILD_TESTING OFF) # Disable GoogleTest tests
        FetchContent_MakeAvailable(googletest)
    endblock()
endif()
