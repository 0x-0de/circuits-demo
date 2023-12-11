#include "canvas.h"

#include "../rendering/mesh.h"

#include <cmath>
#include <iostream>
#include <vector>

#include "node.h"
#include "queue.h"

static GLFWwindow* win;

static Mesh* quad;
static Mesh* connection;

static Vec3 camera;
static double mx, my, cmx, cmy, ox, oy;

static int scroll_level;
static int canvas_input_mode;
static int current_selection, current_hover;
static float cur_scroll;
static double canvas_clock;

static bool node_toggle, play_toggle;
static bool playing;

static std::vector<Node*> nodes;

void update_mouse_controls()
{
    double x, y;
    glfwGetCursorPos(win, &x, &y);

    if(lmb && canvas_input_mode == 0)
    {
        camera.x -= (x - mx) * (1 / cur_scroll) * 0.02f;
        camera.y += (y - my) * (1 / cur_scroll) * 0.02f;
    }

    scroll_level += scroll;

    mx = x;
    my = y;

    cmx = (x - engine_width / 2) / camera.z + camera.x * 50;
    cmy = -(y - engine_height / 2) / camera.z + camera.y * 50;
}

void canvas::init(GLFWwindow* window)
{
    camera = Vec3(0, 0, 1);

    mx = 0;
    my = 0;

    canvas_clock = 0;

    cur_scroll = 0;
    scroll_level = 0;
    current_selection = -1;
    current_hover = -1;
    canvas_input_mode = 0;

    node_toggle = false;
    play_toggle = false;

    playing = false;

    win = window;
    quad = new Mesh(1);

    quad->add_data(0, Vec3(0, 0, 0));
    quad->add_data(0, Vec3(1, 0, 0));
    quad->add_data(0, Vec3(1, 1, 0));
    quad->add_data(0, Vec3(0, 1, 0));

    quad->add_index(0);
    quad->add_index(1);
    quad->add_index(2);
    quad->add_index(0);
    quad->add_index(3);
    quad->add_index(2);

    quad->compile();

    connection = new Mesh(1);

    connection->add_data(0, Vec3(0, -0.5f, 0));
    connection->add_data(0, Vec3(0.5f, -0.5f, 0));
    connection->add_data(0, Vec3(0.5f, 0.5f, 0));
    connection->add_data(0, Vec3(0, 0.5f, 0));

    connection->add_index(0);
    connection->add_index(1);
    connection->add_index(2);
    connection->add_index(0);
    connection->add_index(3);
    connection->add_index(2);

    connection->compile();

    //TRUE gate.

    nodes.push_back(new Node(Vec3(0, 0, 0), 1));
    nodes.push_back(new Node(Vec3(0, 100, 0), 0));

    nodes[1]->add_input(nodes[0]);

    //NOT gate.

    nodes.push_back(new Node(Vec3(200, 0, 0), 1));
    nodes.push_back(new Node(Vec3(200, 100, 0), 0));
    nodes.push_back(new Node(Vec3(200, 200, 0), 0));

    nodes[3]->add_input(nodes[2]);
    nodes[4]->add_input(nodes[3]);

    //OR gate.

    nodes.push_back(new Node(Vec3(400, 0, 0), 1));
    nodes.push_back(new Node(Vec3(600, 0, 0), 1));
    nodes.push_back(new Node(Vec3(500, 100, 0), 0));
    nodes.push_back(new Node(Vec3(500, 200, 0), 0));

    nodes[7]->add_input(nodes[5]);
    nodes[7]->add_input(nodes[6]);
    nodes[8]->add_input(nodes[7]);

    //AND gate.

    nodes.push_back(new Node(Vec3(800, 0, 0), 1));
    nodes.push_back(new Node(Vec3(1000, 0, 0), 1));
    nodes.push_back(new Node(Vec3(800, 100, 0), 0));
    nodes.push_back(new Node(Vec3(900, 100, 0), 0));
    nodes.push_back(new Node(Vec3(1000, 100, 0), 0));
    nodes.push_back(new Node(Vec3(900, 200, 0), 0));

    nodes[11]->add_input(nodes[9]);
    nodes[13]->add_input(nodes[10]);
    nodes[12]->add_input(nodes[11]);
    nodes[12]->add_input(nodes[13]);
    nodes[14]->add_input(nodes[12]);

    //XOR gate.

    nodes.push_back(new Node(Vec3(1200, 0, 0), 1));
    nodes.push_back(new Node(Vec3(1400, 0, 0), 1));
    nodes.push_back(new Node(Vec3(1300, 0, 0), 0));
    nodes.push_back(new Node(Vec3(1200, 100, 0), 0));
    nodes.push_back(new Node(Vec3(1400, 100, 0), 0));
    nodes.push_back(new Node(Vec3(1300, 200, 0), 0));

    nodes[17]->add_input(nodes[15]);
    nodes[17]->add_input(nodes[16]);
    nodes[18]->add_input(nodes[15]);
    nodes[18]->add_input(nodes[17]);
    nodes[19]->add_input(nodes[16]);
    nodes[19]->add_input(nodes[17]);
    nodes[20]->add_input(nodes[18]);
    nodes[20]->add_input(nodes[19]);
}

void canvas::clean()
{
    delete quad;
}

void canvas::draw(Renderer& renderer, Shader& shader)
{
    current_hover = -1;

    update_mouse_controls();

    float zoom = std::pow(2, (float) scroll_level / 3);
    if(zoom != cur_scroll)
    {
        float diff = zoom - cur_scroll;
        cur_scroll += diff / 2;
        if(std::abs(diff) < 0.001f)
            cur_scroll = zoom;
    }

    camera.z = cur_scroll;

    shader.transfer_int("draw_mode", 0);
    shader.transfer_vec3("camera", camera);
    shader.transfer_mat4("transform", mat_transform(Vec3(-engine_width / 2, -engine_height / 2, 0), Vec3(0, 0, 0), Vec3(engine_width, engine_height, 1)));
    renderer.draw_mesh(quad);

    for(unsigned int i = 0; i < nodes.size(); i++)
    {
        nodes[i]->draw_connections(renderer, shader, win, connection, camera, canvas_clock);
    }

    for(unsigned int i = 0; i < nodes.size(); i++)
    {
        if(cmx >= nodes[i]->get_pos().x && cmx <= nodes[i]->get_pos().x + 50 && cmy >= nodes[i]->get_pos().y && cmy <= nodes[i]->get_pos().y + 50 && canvas_input_mode == 0)
        {
            current_hover = i;
            if(lmb)
            {
                current_selection = i;
                canvas_input_mode = 1;
            }
            /*
            if(rmb)
            {
                current_selection = i;
                canvas_input_mode = 2;

                ox = mx;
                oy = my;
            }
            */
        }
        nodes[i]->draw(renderer, shader, win, quad, camera, current_hover == (signed) i, canvas_clock);
    }

    if(canvas_input_mode == 1)
    {
        nodes[current_selection]->set_pos(Vec3(cmx, cmy, 0));
    }

    if(canvas_input_mode == 2)
    {
        //TODO: UI, eventually.
    }

    if(!lmb && canvas_input_mode == 1)
    {
        current_selection = -1;
        canvas_input_mode = 0;
    }

    if(glfwGetKey(win, GLFW_KEY_N) == GLFW_PRESS && !node_toggle)
    {
        node_toggle = true;
        nodes.push_back(new Node(Vec3(cmx, cmy, 0), 0));
    }
    else if(glfwGetKey(win, GLFW_KEY_N) == GLFW_RELEASE)
    {
        node_toggle = false;
    }

    if(glfwGetKey(win, GLFW_KEY_SPACE) == GLFW_PRESS && !play_toggle)
    {
        play_toggle = true;
        playing = playing ? false : true;
    }
    else if(glfwGetKey(win, GLFW_KEY_SPACE) == GLFW_RELEASE)
    {
        play_toggle = false;
    }

    if(playing)
    {
        canvas_clock += engine_delta;
        des::advance_time(engine_delta);

        std::cout << "Clock: " << canvas_clock << std::endl;
    }
}

void canvas::draw_ui(Renderer& renderer, Shader& shader)
{
    
}