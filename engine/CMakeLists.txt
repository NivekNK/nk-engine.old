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

find_package(Vulkan REQUIRED)

add_subdirectory(vendor/tlsf)
add_subdirectory(vendor/eventpp)

add_library(${PROJECT_NAME} STATIC)
target_precompile_headers(${PROJECT_NAME} PUBLIC include/nkpch.h)

#================== Source files ==================
set(SOURCES
    src/core/file.cpp

    src/memory/allocator.cpp
    src/memory/malloc_allocator.cpp

    src/system/logging.cpp
    src/system/input.cpp

    src/nk/memory_manager.cpp
    src/nk/app.cpp
    src/nk/app_creator.cpp
    src/nk/clock.cpp
    src/nk/window.cpp

    src/renderer/renderer_backend.cpp
    src/renderer/instance.cpp
    src/renderer/device.cpp
    src/renderer/image.cpp
    src/renderer/framebuffer.cpp
    src/renderer/swapchain.cpp
    src/renderer/render_pass.cpp
)

if (WIN32)
    list(APPEND SOURCES
        src/platform/platform_win32.cpp
    )
elseif (UNIX AND NOT APPLE)
    message(WARNING "Linux specific sources not yet implemented!")
endif()

target_sources(${PROJECT_NAME} PRIVATE ${SOURCES})
#==================================================

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

target_link_libraries(${PROJECT_NAME}
PRIVATE
    Threads::Threads
    tlsf
    eventpp::eventpp
    Vulkan::Vulkan
)

target_include_directories(${PROJECT_NAME}
PRIVATE
    # where the library itself will look for its internal headers
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${Vulkan_INCLUDE_DIR}
PUBLIC
    # where top-level project will look for the library's public headers
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    #Libraries
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/internal
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/tlsf/include
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/eventpp/include
)
