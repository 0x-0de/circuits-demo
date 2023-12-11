#ifndef _CANVAS_H_
#define _CANVAS_H_

#include "../rendering/renderer.h"
#include "../rendering/shader.h"

#include "../../lib/GLFW/glfw3.h"

namespace canvas
{
    void init(GLFWwindow* window);
    void clean();

    void draw(Renderer& renderer, Shader& shader);
    void draw_ui(Renderer& renderer, Shader& shader);
}

#endif