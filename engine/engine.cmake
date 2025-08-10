project(engine_noimgui)

file(GLOB_RECURSE HEADERS ${CMAKE_SOURCE_DIR}/engine/src/*.h)
file(GLOB_RECURSE SOURCES ${CMAKE_SOURCE_DIR}/engine/src/*.cpp)

set(DX12
    ${CMAKE_SOURCE_DIR}/external/dx12/directx/d3dx12_property_format_table.cpp
)

set(TINY_GLTF
    ${CMAKE_SOURCE_DIR}/external/tiny_gltf/tiny_gltf.cc
)

add_library(engine STATIC ${HEADERS} ${SOURCES} ${DX12} ${TINY_GLTF})
target_compile_definitions(engine PRIVATE ENGINE_WITH_IMGUI)

add_library(engine_noimgui STATIC ${HEADERS} ${SOURCES} ${DX12} ${TINY_GLTF})
target_compile_definitions(engine_noimgui PRIVATE IMGUI_DISABLE)
set_target_properties(engine_noimgui PROPERTIES FOLDER "Hidden")

target_include_directories(engine_noimgui PUBLIC
    ${CMAKE_SOURCE_DIR}/engine/src
    ${CMAKE_SOURCE_DIR}/external
)
target_include_directories(engine PUBLIC
    ${CMAKE_SOURCE_DIR}/engine/src
    ${CMAKE_SOURCE_DIR}/external
)

# Define preprocessor definitions for different configurations
target_compile_definitions(engine_noimgui PRIVATE
    "$<$<CONFIG:${DEBUG}>:${PREDEFINITIONS_GAME_DEBUG}>"
    "$<$<CONFIG:${RELEASE}>:${PREDEFINITIONS_GAME_RELEASE}>"
)
target_compile_definitions(engine PRIVATE
    "$<$<CONFIG:${DEBUG}>:${PREDEFINITIONS_EDITOR_DEBUG}>"
    "$<$<CONFIG:${RELEASE}>:${PREDEFINITIONS_EDITOR_RELEASE}>"
)

# Set C++ standard
set_target_properties(engine_noimgui PROPERTIES
    CXX_STANDARD 20
)
set_target_properties(engine PROPERTIES
    CXX_STANDARD 20
)

target_link_libraries(engine_noimgui PRIVATE
    Shcore.lib dxgi.lib d3d12.lib d3dcompiler.lib dxguid.lib Shlwapi.lib
)
target_link_libraries(engine PRIVATE
    Shcore.lib dxgi.lib d3d12.lib d3dcompiler.lib dxguid.lib Shlwapi.lib
)

if(MSVC)
    target_compile_options(engine_noimgui PRIVATE
        "$<$<CONFIG:${DEBUG}>:/Od>"   # Disable optimizations for Debug
        "$<$<CONFIG:${RELEASE}>:/O2>"  # Enable optimizations for Release
    )
    target_compile_options(engine PRIVATE
        "$<$<CONFIG:${DEBUG}>:/Od>"   # Disable optimizations for Debug
        "$<$<CONFIG:${RELEASE}>:/O2>"  # Enable optimizations for Release
    )
endif()

# For GCC/Clang (if applicable), set optimization level to 0 for debugging
target_compile_options(engine_noimgui PRIVATE
    "$<$<CONFIG:${DEBUG}>:-O0>"  # Disable optimizations for Debug
    "$<$<CONFIG:${RELEASE}>:-O2>"  # Optimize for Release
)
target_compile_options(engine PRIVATE
    "$<$<CONFIG:${DEBUG}>:-O0>"  # Disable optimizations for Debug
    "$<$<CONFIG:${RELEASE}>:-O2>"  # Optimize for Release
)

if(MSVC)
    target_compile_options(engine_noimgui PRIVATE
        "$<$<CONFIG:${DEBUG}>:/MTd>"
        "$<$<CONFIG:${RELEASE}>:/MT>"
    )
    target_compile_options(engine PRIVATE
        "$<$<CONFIG:${DEBUG}>:/MTd>"
        "$<$<CONFIG:${RELEASE}>:/MT>"
    )
endif()

if(MSVC)
    # Set Debug Information Format (/ZI) for Debug configuration
    set_property(TARGET engine_noimgui APPEND_STRING PROPERTY COMPILE_OPTIONS "/ZI")
    set_property(TARGET engine APPEND_STRING PROPERTY COMPILE_OPTIONS "/ZI")

    # Set the Linker Debug flag for /DEBUG (for Debug configurations)
    set_property(TARGET engine_noimgui APPEND_STRING PROPERTY LINK_OPTIONS "/DEBUG")
    set_property(TARGET engine APPEND_STRING PROPERTY LINK_OPTIONS "/DEBUG")
endif()