project(engine)

#============== Platform Specific ==================
add_compile_definitions(
    $<$<CONFIG:Debug>:NK_DEBUG>
    $<$<CONFIG:RelWithDebInfo>:NK_RELEASE_DEBUG_INFO>
    $<$<CONFIG:Release>:NK_RELEASE>
)

if (WIN32)
    add_compile_definitions(NK_PLATFORM_WINDOWS)
elseif (UNIX AND NOT APPLE)
    add_compile_definitions(NK_PLATFORM_LINUX)
endif()
#==================================================

add_library(${PROJECT_NAME} STATIC)
target_precompile_headers(${PROJECT_NAME} PUBLIC include/nkpch.h)

#================== Source files ==================
set(SOURCES
    src/test.cpp
)

target_sources(${PROJECT_NAME} PRIVATE ${SOURCES})
#==================================================

target_include_directories(${PROJECT_NAME}
PRIVATE
    # where the library itself will look for its internal headers
    ${CMAKE_CURRENT_SOURCE_DIR}/src
PUBLIC
    # where top-level project will look for the library's public headers
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
