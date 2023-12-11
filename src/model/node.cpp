#include "node.h"

#include <iostream>

#include "queue.h"

static bool mode_toggle = false;

Node::Node(Vec3 pos, int type) : pos(pos), type(type)
{
    state = false;
    mode = true;
}

void Node::add_input(Node* node)
{
    inputs.push_back(node);
    node->add_output(this);
}

void Node::add_output(Node* node)
{
    outputs.push_back(node);
}

void Node::draw(Renderer& renderer, Shader& shader, GLFWwindow* window, Mesh* mesh, Vec3 camera, bool hovering, float timer)
{
    shader.transfer_int("draw_mode", type == 0 ? 1 : 2);

    if(glfwGetKey(window, GLFW_KEY_F) == GLFW_RELEASE)
    {
        mode_toggle = false;
    }

    if(hovering)
    {
        shader.transfer_mat4("transform", mat_transform(Vec3(Vec3(pos.x - 10, pos.y - 10, 0) - Vec3(camera.x * 50, camera.y * 50, 0)) * camera.z, Vec3(0, 0, 0), Vec3(70, 70, 1) * camera.z));
        shader.transfer_vec4("color_in", Vec4(0.2f, 0.2f, 0.2f, 1));
        renderer.draw_mesh(mesh);

        if(glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && !mode_toggle)
        {
            mode_toggle = true;
            if(type == 0)
            {
                mode = mode ? false : true;
            }
            else if(type == 1)
            {
                state = state ? false : true;
            }
            lambda(timer);
            if(type == 0) delta();
        }
    }

    shader.transfer_mat4("transform", mat_transform(Vec3(pos - Vec3(camera.x * 50, camera.y * 50, 0)) * camera.z, Vec3(0, 0, 0), Vec3(50, 50, 1) * camera.z));
    shader.transfer_vec4("color_in", state ? Vec4(0.4f, 1, 1, 1) : Vec4(0, 0.2f, 0.2f, 1));
    renderer.draw_mesh(mesh);

    if(!mode)
    {
        shader.transfer_mat4("transform", mat_transform(Vec3(Vec3(pos.x + 5, pos.y + 5, 0) - Vec3(camera.x * 50, camera.y * 50, 0)) * camera.z, Vec3(0, 0, 0), Vec3(40, 40, 1) * camera.z));
        shader.transfer_vec4("color_in", state ? Vec4(0.f, 0.2f, 0.2f, 1) : Vec4(0.4f, 1, 1, 1));
        renderer.draw_mesh(mesh);
    }
}

void Node::draw_connections(Renderer& renderer, Shader& shader, GLFWwindow* window, Mesh* mesh, Vec3 camera, float timer)
{
    for(unsigned int i = 0; i < inputs.size(); i++)
    {
        Vec3 start = inputs[i]->get_pos();
        Vec3 end = pos;

        bool begin_mode = inputs[i]->get_mode();
        bool begin_state = begin_mode ? inputs[i]->get_state() : !inputs[i]->get_state();

        bool end_state = mode ? state : !state;

        Vec3 dir = normalize(end - start);
        float length = Vec3(end - start).length();

        float a = angle(Vec3(1, 0, 0), dir);

        float dt = timer - (int) timer;

        shader.transfer_int("draw_mode", 2);
        shader.transfer_vec4("color_in", end_state ? Vec4(0.4f, 1, 1, 1) : Vec4(0, 0.2f, 0.2f, 1));
        shader.transfer_mat4("transform", mat_transform(Vec3(Vec3(start.x + 25, start.y + 25, 0) - Vec3(camera.x * 50, camera.y * 50, 0)) * camera.z, Vec3(0, 0, a * ((dir.y < 0) ? -1 : 1)), Vec3(length * 2, 0.5f * 20, 1) * camera.z));
        renderer.draw_mesh(mesh);

        /*
        if(begin_state != state)
        {
            shader.transfer_int("draw_mode", 2);
            shader.transfer_vec4("color_in", begin_state ? Vec4(0.4f, 1, 1, 1) : Vec4(0, 0.2f, 0.2f, 1));
            shader.transfer_mat4("transform", mat_transform(Vec3(Vec3(start.x + 25, start.y + 25, 0) - Vec3(camera.x, camera.y, 0)) * camera.z, Vec3(0, 0, a * ((dir.y < 0) ? -1 : 1)), Vec3(length * 2 * dt, 0.5f * 20, 1) * camera.z));
            renderer.draw_mesh(mesh);
        }
        */
    }
}

void Node::delta()
{
    state = !mode;
    for(unsigned int i = 0; i < inputs.size(); i++)
    {
        if(inputs[i]->get_state())
        {
            state = mode;
            break;
        }
    }
}

void Node::lambda(float timer)
{
    for(unsigned int i = 0; i < outputs.size(); i++)
    {
        des::Event e;

        e.time = timer + 1;
        e.target = outputs[i];
        
        des::push_event(e);
    }
}

Vec3 Node::get_pos()
{
    return pos;
}

void Node::set_pos(Vec3 value)
{
    pos = value;
}