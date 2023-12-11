#ifndef _NODE_H_
#define _NODE_H_

#include "../utils/utilmath.h"

#include "../rendering/mesh.h"
#include "../rendering/renderer.h"
#include "../rendering/shader.h"

#include "../../lib/GLFW/glfw3.h"

class Node
{
    public:
        Node(Vec3 pos, int type);

        void add_input(Node* node);

        void draw(Renderer& renderer, Shader& shader, GLFWwindow* window, Mesh* mesh, Vec3 camera, bool hovering, float timer);
        void draw_connections(Renderer& renderer, Shader& shader, GLFWwindow* window, Mesh* mesh, Vec3 camera, float timer);

        void delta();
        void lambda(float time);

        bool get_mode() const { return mode; }
        bool get_state() const { return state; }

        bool get_output() const { return output; }

        Vec3 get_pos();
        void set_pos(Vec3 value);
    private:
        Vec3 pos;
        int type;

        bool state, mode, output;

        void add_output(Node* node);

        std::vector<Node*> inputs;
        std::vector<Node*> outputs;
};

#endif