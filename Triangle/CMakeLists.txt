add_executable(Triangle)

set(Triangle_SRC 
    Application.cpp
    Window.cpp
)

set(Triangle_HDR
    Application.h
    Window.hpp
    pch.h
)

#[[
add_custom_target(copy_resources ALL 
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Resources/ 
        ${CMAKE_BINARY_DIR}/Resources/
)

add_dependencies(Triangle copy_resources)
]]

target_sources(Triangle PRIVATE ${Triangle_SRC})
target_sources(Triangle PUBLIC ${Triangle_HDR})


include_directories(${CMAKE_SOURCE_DIR}/Dependencies)



find_package(Vulkan REQUIRED)
target_link_libraries(Triangle PUBLIC glfw glm Vulkan::Vulkan)