# Package manager
option(ENABLE_HUNTER "Enable Hunter package manager for this project." ON)

if(ENABLE_HUNTER)

    if(NOT EXISTS "${CMAKE_BINARY_DIR}/HunterGate.cmake")
        message(
            STATUS
            "Downloading HunterGate.cmake..."
            )

        file(DOWNLOAD "https://raw.githubusercontent.com/cpp-pm/gate/master/cmake/HunterGate.cmake"
            "${CMAKE_BINARY_DIR}/HunterGate.cmake"
            )
        message(STATUS "HunterGate was downloaded succesfully.")
    endif()

    include(${CMAKE_BINARY_DIR}/HunterGate.cmake)

    HunterGate(
        URL "https://github.com/cpp-pm/hunter/archive/v0.23.314.tar.gz"
        SHA1 "95c47c92f68edb091b5d6d18924baabe02a6962a"
        )

endif()
