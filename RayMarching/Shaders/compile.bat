set sourceDir=%1\Shaders
set outputDir=%2\Shaders


if not exist "%outputDir%" mkdir "%outputDir%"

glslc %sourceDir%\shader.vert -o %outputDir%\vert.spv
glslc %sourceDir%\shader.frag -o %outputDir%\frag.spv

glslc %sourceDir%\color.frag -o %outputDir%\colorfrag.spv


pause