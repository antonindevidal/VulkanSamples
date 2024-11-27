if not exist "%2\GrassGeneration\Shaders\" mkdir "%2\GrassGeneration\Shaders

echo %2

glslc %1\GrassGeneration\Shaders\shader.vert -o %2\GrassGeneration\Shaders\vert.spv
glslc %1\GrassGeneration\Shaders\shader.frag -o %2\GrassGeneration\Shaders\frag.spv

glslc %1\GrassGeneration\Shaders\color.frag -o %2\GrassGeneration\Shaders\colorfrag.spv

glslc %1\GrassGeneration\Shaders\grass.vert -o %2\GrassGeneration\Shaders\grassvert.spv
glslc %1\GrassGeneration\Shaders\grass.frag -o %2\GrassGeneration\Shaders\grassfrag.spv

glslc %1\GrassGeneration\Shaders\grassGen.comp -o %2\GrassGeneration\Shaders\grassGenComp.spv

glslc %1\GrassGeneration\Shaders\skybox.vert -o %2\GrassGeneration\Shaders\skyboxvert.spv
glslc %1\GrassGeneration\Shaders\skybox.frag -o %2\GrassGeneration\Shaders\skyboxfrag.spv

pause