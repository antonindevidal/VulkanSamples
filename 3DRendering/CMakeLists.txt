add_executable(3DRendering)

file(GLOB 3DRendering_SRC
     src/*.cpp
     src/backend/*.cpp
)

set(3DRendering_HDR
    src
    src/backend
)

add_custom_target(3DRendering_copy_resources ALL 
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/3DRendering/resources/ 
        ${CMAKE_BINARY_DIR}/3DRendering/resources/
)
add_dependencies(3DRendering 3DRendering_copy_resources)



target_sources(3DRendering PRIVATE ${3DRendering_SRC})
target_include_directories(3DRendering PUBLIC ${3DRendering_HDR})
# Compiling shaders
file( GLOB 3DRendering_Shaders
    src/shaders/*.frag
    src/shaders/*.vert
)

add_custom_command(
    TARGET 3DRendering
    COMMAND cmd /C ${CMAKE_SOURCE_DIR}/3DRendering/src/shaders/compile.bat ${CMAKE_SOURCE_DIR}/3DRendering ${CMAKE_BINARY_DIR}/3DRendering
    DEPENDS 3DRendering_Shaders
)

include_directories(${CMAKE_SOURCE_DIR}/Dependencies)

find_package(Vulkan REQUIRED)
target_link_libraries(3DRendering PUBLIC glfw glm Vulkan::Vulkan)