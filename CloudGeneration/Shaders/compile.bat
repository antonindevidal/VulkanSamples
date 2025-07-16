set sourceDir=%1\Shaders
set outputDir=%2\Shaders

echo "Compiling shaders"

if not exist "%outputDir%" mkdir "%outputDir%"

glslc %sourceDir%\raymarching.vert -o %outputDir%\raymarchingVert.spv
glslc %sourceDir%\raymarching.frag -o %outputDir%\raymarchingFrag.spv

glslc %sourceDir%\color.frag -o %outputDir%\colorfrag.spv
glslc %sourceDir%\shader.frag -o %outputDir%\frag.spv
glslc %sourceDir%\shader.vert -o %outputDir%\vert.spv


pause