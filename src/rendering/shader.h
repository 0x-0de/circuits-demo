#ifndef _SHADER_H_
#define _SHADER_H_

#include <vector>

#include "../utils/utilmath.h"

class Shader
{
    public:
        Shader();

        void add_shader(bool is_path, const char* source, unsigned int type);

        void compile();
        void destroy();

        void transfer_int(const char* name, int val);
        void transfer_float(const char* name, float val);
        void transfer_mat4(const char* name, Mat4x4 mat);

        void transfer_vec2(const char* name, Vec2 vec);
        void transfer_vec3(const char* name, Vec3 vec);
        void transfer_vec4(const char* name, Vec4 vec);

        void use();
    private:
        unsigned int program;
        std::vector<unsigned int> shaders;
};

#endif