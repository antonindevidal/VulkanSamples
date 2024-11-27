set sourceDir=%1\GrassGeneration\Shaders
set outputDir=%2\GrassGeneration\Shaders


if not exist "%outputDir%" mkdir "%outputDir%"


glslc %sourceDir%\shader.vert -o %outputDir%\vert.spv
glslc %sourceDir%\shader.frag -o %outputDir%\frag.spv

glslc %sourceDir%\color.frag -o %outputDir%\colorfrag.spv

glslc %sourceDir%\grass.vert -o %outputDir%\grassvert.spv
glslc %sourceDir%\grass.frag -o %outputDir%\grassfrag.spv

glslc %sourceDir%\grassGen.comp -o %outputDir%\grassGenComp.spv

glslc %sourceDir%\skybox.vert -o %outputDir%\skyboxvert.spv
glslc %sourceDir%\skybox.frag -o %outputDir%\skyboxfrag.spv

pause