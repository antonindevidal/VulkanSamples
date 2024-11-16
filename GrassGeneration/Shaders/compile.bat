if not exist "..\..\out\build\x64-Debug\GrassGeneration\Shaders\" mkdir ..\..\out\build\x64-Debug\GrassGeneration\Shaders

C:\VulkanSDK\1.3.296.0\Bin\glslc shader.vert -o ..\..\out\build\x64-Debug\GrassGeneration\Shaders\vert.spv
C:\VulkanSDK\1.3.296.0\Bin\glslc shader.frag -o ..\..\out\build\x64-Debug\GrassGeneration\Shaders\frag.spv

C:\VulkanSDK\1.3.296.0\Bin\glslc color.frag -o ..\..\out\build\x64-Debug\GrassGeneration\Shaders\colorfrag.spv

C:\VulkanSDK\1.3.296.0\Bin\glslc grass.vert -o ..\..\out\build\x64-Debug\GrassGeneration\Shaders\grassvert.spv



pause