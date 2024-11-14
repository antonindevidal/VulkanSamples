if not exist "..\..\out\build\x64-Debug\3DRendering\Shaders\" mkdir ..\..\out\build\x64-Debug\3DRendering\Shaders

C:\VulkanSDK\1.3.296.0\Bin\glslc shader.vert -o ..\..\out\build\x64-Debug\3DRendering\Shaders\vert.spv
C:\VulkanSDK\1.3.296.0\Bin\glslc shader.frag -o ..\..\out\build\x64-Debug\3DRendering\Shaders\frag.spv

C:\VulkanSDK\1.3.296.0\Bin\glslc color.frag -o ..\..\out\build\x64-Debug\3DRendering\Shaders\colorfrag.spv



pause