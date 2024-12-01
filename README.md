# Vulkan Samples

## Technologies 
- C++ 20
- Vulkan 1.3.296
- GLFW
- CMake

**This project requires the Vulkan SDK to be installed in order to compile and run it.**

Compiling the project only works on Windows for now. To compile it, run the CMakeList.txt file for the desired project.

## [Raymarching](/RayMarching)

This project is about rendering a scene with Raymarching.

### Features :
- Signed Distance Functions for Cube, Sphere and Plane
- Smooth union function
- Blinn Phong lighting
- Soft shadows rendering
- Cubemap reflexions

Works with camera movement and skybox created for the grass generation.

Inspiration : Inigo Quilez blog  

![Grass Generation](/Documentation/RayMarching.png)


## [Grass Generation](/GrassGeneration)

This project showcases a scene with foliage rendering and real time wind simulation.

### Features :
- Grass generation using a compute shader and perlin noise
- GPU instancing
- Blinn Phong lighting
- Skybox
- Wind simulation
- Camera movement (using WASD, spacebar and left shift)

### Parameters for grass generation can be changed at compile time in Application.h: 
- Number of blades
- Size and position of the grass field
- Wind direction, power, amplitude, frequency

Inspiration : Procedural Grass in 'Ghost of Tsushima' GDC talk

![Grass Generation](/Documentation/GrassGeneration.png)

## [3D Rendering](/3DRendering)

This project shows a simple 3D scene in Vulkan. It implements a basic rendering system with an abstraction level for the Vulkan API. 

### Features :
- Abstraction level for the Vulkan API
- Supports multiple graphics pipelines
- Supports multiple meshes and associated descriptor sets
 
![3D rendering](/Documentation/3DRenderingProject.png)
