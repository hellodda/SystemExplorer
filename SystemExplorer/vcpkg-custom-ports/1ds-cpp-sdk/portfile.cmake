vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO microsoft/cpp_client_telemetry
    REF main
    HEAD_REF main
    SHA512 9d7e4758a2f0844b0f286f4dac7d0dedd0a7f97faa0c399ce6b8308afab3bbf3da42bd1437baf40f5c8316a1190b972a4fdf018c56372eabc7c989aff3e33942
)

vcpkg_cmake_configure(
    SOURCE_PATH "\"
    OPTIONS
        -DMATSDK_BUILD_UNIT_TESTS=OFF
        -DMATSDK_BUILD_FUNC_TESTS=OFF
        -D1DS_BUILD_TESTS=OFF
        -DMATSDK_BUILD_HEADERS=ON
        -DMATSDK_BUILD_LIBRARY=ON
        -DMATSDK_BUILD_PACKAGE=OFF
        -DMATSDK_SQLITE_PROVIDER=SYSTEM
        -DMATSDK_ZLIB_PROVIDER=SYSTEM
)

vcpkg_cmake_install()

file(REMOVE_RECURSE "\/debug/include")
file(REMOVE_RECURSE "\/debug/share")

file(INSTALL "\/LICENSE"
     DESTINATION "\/share/\"
     RENAME copyright)