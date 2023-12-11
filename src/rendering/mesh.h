#ifndef _MESH_H_
#define _MESH_H_

#include <vector>

#include "../utils/utilmath.h"

class Mesh
{
    public:
        Mesh(unsigned int attrib_size);

        void add_data(unsigned int attribute, float val);
        void add_data(unsigned int attribute, Vec2 val);
        void add_data(unsigned int attribute, Vec3 val);
        void add_data(unsigned int attribute, Vec4 val);

        void add_data(unsigned int attribute, std::vector<float>& val);
        void add_data(unsigned int attribute, std::vector<Vec2>& val);
        void add_data(unsigned int attribute, std::vector<Vec3>& val);
        void add_data(unsigned int attribute, std::vector<Vec4>& val);

        void add_index(unsigned int index);
        
        void add_indices(unsigned int* indices, unsigned int size);
        void add_indices(std::vector<unsigned int>& indices);

        void bind();

        void clear_data();
        void compile();

        void destroy();

        unsigned int get_size() const { return size; }
    private:
        std::vector< std::vector<float> > data;
        std::vector<unsigned char> dimensions;

        std::vector<unsigned int> indices;

        unsigned int vao, size, ibo;
        std::vector<unsigned int> vbos;
};

#endif