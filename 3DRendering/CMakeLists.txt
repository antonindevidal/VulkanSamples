add_executable(3DRendering)

set(3DRendering_SRC 
    Application.cpp
    Window.cpp
    Device.cpp
    Renderer.cpp
    Helpers.cpp
    Instance.cpp
    VkStructs.cpp
)

set(3DRendering_HDR
    Application.h
    Window.hpp
    pch.h
    Device.hpp
    Renderer.hpp
    Helpers.cpp
    Instance.hpp
    VkStructs.hpp
)

add_custom_target(copy_resources ALL 
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/3DRendering/Textures/ 
        ${CMAKE_BINARY_DIR}/3DRendering/Textures/
)
add_dependencies(3DRendering copy_resources)

add_custom_target(copy_models ALL 
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/3DRendering/Models/ 
        ${CMAKE_BINARY_DIR}/3DRendering/Models/
)
add_dependencies(3DRendering copy_models)


target_sources(3DRendering PRIVATE ${3DRendering_SRC})
target_sources(3DRendering PUBLIC ${3DRendering_HDR})


include_directories(${CMAKE_SOURCE_DIR}/Dependencies)



find_package(Vulkan REQUIRED)
target_link_libraries(3DRendering PUBLIC glfw glm Vulkan::Vulkan)