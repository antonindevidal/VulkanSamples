add_executable(CloudGeneration)

file(GLOB CloudGeneration_SRC
     src/*.cpp
     src/backend/*.cpp
)

set(CloudGeneration_HDR
    src
    src/backend
)

add_custom_target(CloudGeneration_copy_resources ALL 
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/CloudGeneration/resources/ 
        ${CMAKE_BINARY_DIR}/CloudGeneration/resources/
)
add_dependencies(CloudGeneration CloudGeneration_copy_resources)



target_sources(CloudGeneration PRIVATE ${CloudGeneration_SRC})
target_include_directories(CloudGeneration PUBLIC ${CloudGeneration_HDR})
# Compiling shaders
file( GLOB CloudGeneration_Shaders
    src/shaders/*.frag
    src/shaders/*.vert
)

add_custom_command(
    TARGET CloudGeneration
    COMMAND cmd /C ${CMAKE_SOURCE_DIR}/CloudGeneration/src/shaders/compile.bat ${CMAKE_SOURCE_DIR}/CloudGeneration ${CMAKE_BINARY_DIR}/CloudGeneration
    DEPENDS CloudGeneration_Shaders
)

include_directories(${CMAKE_SOURCE_DIR}/Dependencies)

find_package(Vulkan REQUIRED)
target_link_libraries(CloudGeneration PUBLIC glfw glm Vulkan::Vulkan)