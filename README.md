# Vulkan Samples

These projects were only tested on 1 hardware. If you want to compile the projects, you will need to run the [compile.bat](https://github.com/antonindevidal/VulkanSamples/blob/main/3DRendering/Shaders/compile.bat) file first in order to compile the shaders. This requires to modify the [compile.bat](https://github.com/antonindevidal/VulkanSamples/blob/main/3DRendering/Shaders/compile.bat) file and change the glslc program filepath and output path for the SpirV files.


## [3D Rendering](https://github.com/antonindevidal/VulkanSamples/tree/main/3DRendering)

This project shows a simple 3D scene in Vulkan. It implements a basic rendering system with an abstraction level for the Vulkan API. 

Features :
- Abstraction level for the Vulkan API
- Supports multiple graphics pipelines
- Supports multiple meshes and associated descriptor sets
 
![3D rendering](https://github.com/antonindevidal/VulkanSamples/blob/main/Documentation/3DRenderingProject.png)
