add_executable(RayMarching)

set(RayMarching_SRC 
    Application.cpp
    Window.cpp
    Device.cpp
    Renderer.cpp
    Helpers.cpp
    Instance.cpp
    VkStructs.cpp
)

set(RayMarching_HDR
    Application.h
    Window.hpp
    pch.h
    Device.hpp
    Renderer.hpp
    Helpers.cpp
    Instance.hpp
    VkStructs.hpp
)

add_custom_target(RayMarching_copy_resources ALL 
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/RayMarching/Textures/ 
        ${CMAKE_BINARY_DIR}/RayMarching/Textures/
)
add_dependencies(RayMarching RayMarching_copy_resources)

add_custom_target(RayMarching_copy_models ALL 
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/RayMarching/Models/ 
        ${CMAKE_BINARY_DIR}/RayMarching/Models/
)
add_dependencies(RayMarching RayMarching_copy_models)


target_sources(RayMarching PRIVATE ${RayMarching_SRC})
target_sources(RayMarching PUBLIC ${RayMarching_HDR})

# Compiling shaders
if(WIN32)
    add_custom_target(RayMarching_compile_shaders
                       COMMAND cmd /C ${CMAKE_SOURCE_DIR}/RayMarching/Shaders/compile.bat ${CMAKE_SOURCE_DIR}/RayMarching ${CMAKE_BINARY_DIR}/RayMarching)

    add_dependencies(RayMarching RayMarching_compile_shaders)
endif()

include_directories(${CMAKE_SOURCE_DIR}/Dependencies)

find_package(Vulkan REQUIRED)
target_link_libraries(RayMarching PUBLIC glfw glm Vulkan::Vulkan)