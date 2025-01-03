set sourceDir=%1\Shaders
set outputDir=%2\Shaders


if not exist "%outputDir%" mkdir "%outputDir%"

glslc %sourceDir%\raymarching.vert -o %outputDir%\raymarchingVert.spv
glslc %sourceDir%\raymarching.frag -o %outputDir%\raymarchingFrag.spv

glslc %sourceDir%\skybox.vert -o %outputDir%\skyboxVert.spv
glslc %sourceDir%\skybox.frag -o %outputDir%\skyboxFrag.spv

pause