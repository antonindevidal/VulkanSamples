add_executable(Triangle)

set(Triangle_SRC 
    Application.cpp
    Window.cpp
    Device.cpp
    Buffer.cpp
    Vertex.cpp
    Mesh.cpp
    Renderer.cpp
    Helpers.cpp
    Texture.cpp
    DescriptorSet.cpp
    Instance.cpp
    GraphicsPipeline.cpp
)

set(Triangle_HDR
    Application.h
    Window.hpp
    pch.h
    Device.hpp
    Buffer.hpp
    Vertex.hpp
    Mesh.hpp
    Renderer.hpp
    Helpers.cpp
    Texture.hpp
    DescriptorSet.hpp
    Instance.hpp
    GraphicsPipeline.hpp
)

add_custom_target(copy_resources ALL 
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Triangle/Textures/ 
        ${CMAKE_BINARY_DIR}/Triangle/Textures/
)
add_dependencies(Triangle copy_resources)

add_custom_target(copy_models ALL 
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Triangle/Models/ 
        ${CMAKE_BINARY_DIR}/Triangle/Models/
)
add_dependencies(Triangle copy_models)


target_sources(Triangle PRIVATE ${Triangle_SRC})
target_sources(Triangle PUBLIC ${Triangle_HDR})


include_directories(${CMAKE_SOURCE_DIR}/Dependencies)



find_package(Vulkan REQUIRED)
target_link_libraries(Triangle PUBLIC glfw glm Vulkan::Vulkan)