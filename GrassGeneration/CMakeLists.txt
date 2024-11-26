add_executable(GrassGeneration)

set(GrassGeneration_SRC 
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

set(GrassGeneration_HDR
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

# Create directories


install(DIRECTORY DESTINATION ${CMAKE_BINARY_DIR}/GrassGeneration)
install(DIRECTORY DESTINATION ${CMAKE_BINARY_DIR}/GrassGeneration/Textures/)
install(DIRECTORY DESTINATION ${CMAKE_BINARY_DIR}/GrassGeneration/Models/)

# Copy resources
add_custom_target(GrassGeneration_copy_resources ALL 
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/GrassGeneration/Textures/ 
        ${CMAKE_BINARY_DIR}/GrassGeneration/Textures/
)
add_dependencies(GrassGeneration GrassGeneration_copy_resources)

add_custom_target(GrassGeneration_copy_models ALL 
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/GrassGeneration/Models/ 
        ${CMAKE_BINARY_DIR}/GrassGeneration/Models/
)
add_dependencies(GrassGeneration GrassGeneration_copy_models)


target_sources(GrassGeneration PRIVATE ${GrassGeneration_SRC})
target_sources(GrassGeneration PUBLIC ${GrassGeneration_HDR})


include_directories(${CMAKE_SOURCE_DIR}/Dependencies)



find_package(Vulkan REQUIRED)
target_link_libraries(GrassGeneration PUBLIC glfw glm Vulkan::Vulkan)