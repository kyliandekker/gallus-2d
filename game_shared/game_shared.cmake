project(game_shared)

file(GLOB_RECURSE HEADERS ${CMAKE_SOURCE_DIR}/game_shared/src/*.h)
file(GLOB_RECURSE SOURCES ${CMAKE_SOURCE_DIR}/game_shared/src/*.cpp)

set(IMGUI
    ${CMAKE_SOURCE_DIR}/external/imgui/imgui.cpp
    ${CMAKE_SOURCE_DIR}/external/imgui/imgui_draw.cpp
    ${CMAKE_SOURCE_DIR}/external/imgui/backends/imgui_impl_dx12.cpp
    ${CMAKE_SOURCE_DIR}/external/imgui/backends/imgui_impl_win32.cpp
    ${CMAKE_SOURCE_DIR}/external/imgui/imgui_widgets.cpp
    ${CMAKE_SOURCE_DIR}/external/imgui/imgui_helpers.cpp
    ${CMAKE_SOURCE_DIR}/external/imgui/imgui_tables.cpp
    ${CMAKE_SOURCE_DIR}/external/imgui/implot.cpp
    ${CMAKE_SOURCE_DIR}/external/imgui/implot_items.cpp
    ${CMAKE_SOURCE_DIR}/external/imgui/ImGuizmo.cpp
)

set(DX12
    ${CMAKE_SOURCE_DIR}/external/dx12/directx/d3dx12_property_format_table.cpp
)

set(TINY_GLTF
    ${CMAKE_SOURCE_DIR}/external/tiny_gltf/tiny_gltf.cc
)

add_library(${PROJECT_NAME} STATIC ${HEADERS} ${SOURCES} ${DX12} ${TINY_GLTF})

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_SOURCE_DIR}/engine/src
    ${CMAKE_SOURCE_DIR}/game_shared/src
    ${CMAKE_SOURCE_DIR}/external
)

# Define preprocessor definitions for different configurations
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:${DEBUG}>:${PREDEFINITIONS_GAME_DEBUG}>"
    "$<$<CONFIG:${RELEASE}>:${PREDEFINITIONS_GAME_RELEASE}>"
)

# Set C++ standard
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 20
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Shcore.lib dxgi.lib d3d12.lib d3dcompiler.lib dxguid.lib Shlwapi.lib engine.lib
)
target_link_libraries(${PROJECT_NAME} PRIVATE engine)
target_link_directories(${PROJECT_NAME} PRIVATE
    "${CMAKE_SOURCE_DIR}/../$<CONFIG>"
)

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:${DEBUG}>:/Od>"   # Disable optimizations for Debug
        "$<$<CONFIG:${RELEASE}>:/O2>"  # Enable optimizations for Release
        "$<$<CONFIG:${EDITOR_DEBUG}>:/Od>"   # Disable optimizations for Editor Debug
        "$<$<CONFIG:${EDITOR_DEBUG_HYBRID}>:/Od>"   # Disable optimizations for Editor Debug Hybrid
        "$<$<CONFIG:${EDITOR_RELEASE}>:/Od>"   # Disable optimizations for Editor Release
    )
endif()

# For GCC/Clang (if applicable), set optimization level to 0 for debugging
target_compile_options(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:${DEBUG}>:-O0>"  # Disable optimizations for Debug
    "$<$<CONFIG:${RELEASE}>:-O2>"  # Optimize for Release
    "$<$<CONFIG:${EDITOR_DEBUG}>:-O0>"  # Disable optimizations for Editor Debug
    "$<$<CONFIG:${EDITOR_DEBUG_HYBRID}>:-O0>"  # Disable optimizations for Editor Debug Hybrid
    "$<$<CONFIG:${EDITOR_RELEASE}>:-O0>"  # Disable optimizations for Editor Release
)

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:${DEBUG}>:/MTd>"
        "$<$<CONFIG:${RELEASE}>:/MT>"
        "$<$<CONFIG:${EDITOR_DEBUG}>:/MTd>"
        "$<$<CONFIG:${EDITOR_DEBUG_HYBRID}>:/MTd>"
        "$<$<CONFIG:${EDITOR_RELEASE}>:/MT>"
    )
endif()

if(MSVC)
    # Set Debug Information Format (/ZI) for Debug configuration
    set_property(TARGET ${PROJECT_NAME} APPEND_STRING PROPERTY COMPILE_OPTIONS "/ZI")

    # Set the Linker Debug flag for /DEBUG (for Debug configurations)
    set_property(TARGET ${PROJECT_NAME} APPEND_STRING PROPERTY LINK_OPTIONS "/DEBUG")
endif()