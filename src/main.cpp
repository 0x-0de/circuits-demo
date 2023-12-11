#include <ctime>
#include <iostream>

#include "../lib/GL/glew.h"
#include "../lib/GLFW/glfw3.h"

#include "rendering/renderer.h"
#include "rendering/shader.h"

#include "utils/texture.h"
#include "utils/ui.h"

#include "model/canvas.h"

double engine_delta, engine_time;
int engine_width, engine_height, cursor_x, cursor_y, input_mode, scroll;
bool lmb, rmb, backspace;

std::string str_input;

const char* vs_main = R"VOGON(
#version 430 core
layout(location = 0) in vec3 pos;

out vec3 f_pos, t_pos;

uniform vec3 camera;
uniform int draw_mode;

uniform mat4 transform, projection;

void main()
{
    f_pos = pos;
    vec4 t = transform * vec4(pos, 1);
    t_pos = t.xyz;
    gl_Position = projection * t;
}
)VOGON";

const char* fs_main = R"VOGON(
#version 430 core
out vec4 color;

in vec3 f_pos, t_pos;

uniform vec3 camera;
uniform int draw_mode;

uniform vec4 color_in;

void main()
{
    vec4 col = vec4(0, 0, 0, 1);
    float scale = 50 * camera.z;

    vec3 ps, pf;

    switch(draw_mode)
    {
        case 0:
            ps = t_pos / scale + vec3(camera.x, camera.y, 0);
            pf = vec3(floor(ps.x), floor(ps.y), floor(ps.z));

            if(abs(ps.x - pf.x) < 0.1f && abs(ps.y - pf.y) < 0.1f)
            {
                col = vec4(0.2f, 0.2f, 0.2f, 1);
            }
            break;
        case 1:
            if(sqrt(pow(f_pos.x - 0.5f, 2) + pow(f_pos.y - 0.5f, 2)) > 0.5f) discard;
            col = color_in;
            break;
        case 2:
            col = color_in;
            break;
    }

    color = col;
}
)VOGON";

void callback_text(GLFWwindow* window, unsigned int text)
{
    str_input += (char) text;
}

void callback_key(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        if(key == GLFW_KEY_BACKSPACE)
        {
            backspace = true;
        }
    }
}

void callback_scroll(GLFWwindow* window, double x, double y)
{
    scroll = y;
}

void loop(GLFWwindow* window)
{
    engine_delta = glfwGetTime() - engine_time;
    engine_time = glfwGetTime();

    glfwGetWindowSize(window, &engine_width, &engine_height);

    lmb = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS;
    rmb = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS;

    double mx, my;
    glfwGetCursorPos(window, &mx, &my);

    cursor_x = mx - engine_width / 2;
    cursor_y = -(my - engine_height / 2);

    glViewport(0, 0, engine_width, engine_height);

    glfwSwapBuffers(window);
    glfwPollEvents();
}

int main()
{
    engine_time = 0;
    str_input = "";

    backspace = false;

    glfwInit();
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Circuitbuilder", NULL, NULL);

    glfwSetCharCallback(window, callback_text);
    glfwSetKeyCallback(window, callback_key);
    glfwSetScrollCallback(window, callback_scroll);

    engine_width = 1280;
    engine_height = 720;

    glfwMakeContextCurrent(window);
    glewInit();

    Renderer renderer;
    Shader shader_main;

    shader_main.add_shader(false, vs_main, GL_VERTEX_SHADER);
    shader_main.add_shader(false, fs_main, GL_FRAGMENT_SHADER);

    shader_main.compile();

    canvas::init(window);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glfwSwapInterval(1);

    while(!glfwWindowShouldClose(window))
    {
        loop(window);
        renderer.prepare();

        shader_main.use();
        shader_main.transfer_mat4("projection", mat_ortho(0, engine_width, 0, engine_height, -1, 1));
        shader_main.transfer_mat4("transform", mat_transform(Vec3(0, 0, 0), Vec3(0, 0, 0), Vec3(100, 100, 1)));

        canvas::draw(renderer, shader_main);

        if(scroll != 0) scroll = 0;
    }
    
    shader_main.destroy();

    canvas::clean();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}