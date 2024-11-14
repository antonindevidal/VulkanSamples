cmake_minimum_required (VERSION 3.25)

#SET CPP Version for all platforms and projects
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project ("VulkanSamples")

# Include subprojects
add_subdirectory ("3DRendering")
add_subdirectory("Dependencies")
