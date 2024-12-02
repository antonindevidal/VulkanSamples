add_executable(3DRendering)

set(3DRendering_SRC 
    src/Engine.cpp
    src/Application.cpp
    src/Window.cpp
    src/Device.cpp
    src/Renderer.cpp
    src/Helpers.cpp
    src/Instance.cpp
    src/VkStructs.cpp
)

set(3DRendering_HDR
    src/Engine.hpp
    src/Application.h
    src/Window.hpp
    src/pch.h
    src/Device.hpp
    src/Renderer.hpp
    src/Helpers.cpp
    src/Instance.hpp
    src/VkStructs.hpp
)

add_custom_target(3DRendering_copy_resources ALL 
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/3DRendering/resources/ 
        ${CMAKE_BINARY_DIR}/3DRendering/resources/
)
add_dependencies(3DRendering 3DRendering_copy_resources)



target_sources(3DRendering PRIVATE ${3DRendering_SRC})
target_sources(3DRendering PUBLIC ${3DRendering_HDR})

# Compiling shaders
if(WIN32)
    add_custom_target(3DRendering_compile_shaders
                       COMMAND cmd /C ${CMAKE_SOURCE_DIR}/3DRendering/src/shaders/compile.bat ${CMAKE_SOURCE_DIR}/3DRendering ${CMAKE_BINARY_DIR}/3DRendering)

    add_dependencies(3DRendering 3DRendering_compile_shaders)
endif()

include_directories(${CMAKE_SOURCE_DIR}/Dependencies)

find_package(Vulkan REQUIRED)
target_link_libraries(3DRendering PUBLIC glfw glm Vulkan::Vulkan)