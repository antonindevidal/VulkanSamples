if not exist "..\..\out\build\x64-Debug\GrassGeneration\Shaders\" mkdir ..\..\out\build\x64-Debug\GrassGeneration\Shaders

C:\VulkanSDK\1.3.296.0\Bin\glslc shader.vert -o ..\..\out\build\x64-Debug\GrassGeneration\Shaders\vert.spv
C:\VulkanSDK\1.3.296.0\Bin\glslc shader.frag -o ..\..\out\build\x64-Debug\GrassGeneration\Shaders\frag.spv

C:\VulkanSDK\1.3.296.0\Bin\glslc color.frag -o ..\..\out\build\x64-Debug\GrassGeneration\Shaders\colorfrag.spv

C:\VulkanSDK\1.3.296.0\Bin\glslc grass.vert -o ..\..\out\build\x64-Debug\GrassGeneration\Shaders\grassvert.spv
C:\VulkanSDK\1.3.296.0\Bin\glslc grass.frag -o ..\..\out\build\x64-Debug\GrassGeneration\Shaders\grassfrag.spv

C:\VulkanSDK\1.3.296.0\Bin\glslc grassGen.comp -o ..\..\out\build\x64-Debug\GrassGeneration\Shaders\grassGenComp.spv

C:\VulkanSDK\1.3.296.0\Bin\glslc skybox.vert -o ..\..\out\build\x64-Debug\GrassGeneration\Shaders\skyboxvert.spv
C:\VulkanSDK\1.3.296.0\Bin\glslc skybox.frag -o ..\..\out\build\x64-Debug\GrassGeneration\Shaders\skyboxfrag.spv

pause