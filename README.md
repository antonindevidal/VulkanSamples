# Vulkan Samples

## Technologies 
- C++ 20
- Vulkan 1.3.296
- GLFW
- CMake

**This project requires the Vulkan SDK to be installed in order to compile and run it.**


In order to compile the project, a few steps are required : 
- Run the project one time to create the build folders.
- Run the compile.bat file in the Shader folder of each project to create SpirV code.
You may need to modify this script in order to change the Vulkan SDK folder and compiler output.
- Compiling the project should work.


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


![Grass Generation](/Documentation/GrassGeneration.png)

## [3D Rendering](/3DRendering)

This project shows a simple 3D scene in Vulkan. It implements a basic rendering system with an abstraction level for the Vulkan API. 

### Features :
- Abstraction level for the Vulkan API
- Supports multiple graphics pipelines
- Supports multiple meshes and associated descriptor sets
 
![3D rendering](/Documentation/3DRenderingProject.png)
