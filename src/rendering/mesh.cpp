#include "mesh.h"

#include <iostream>

#include "../../lib/GL/glew.h"

Mesh::Mesh(unsigned int attrib_size)
{
    dimensions.resize(attrib_size);
    data.resize(attrib_size);
    vbos.resize(attrib_size);

    for(unsigned int i = 0; i < attrib_size; i++)
    {
        dimensions[i] = 0;
        glGenBuffers(1, &vbos[i]);
    }

    glGenBuffers(1, &ibo);

    glGenVertexArrays(1, &vao);
}

void Mesh::add_data(unsigned int attribute, float val)
{
    if(attribute >= 0 && attribute < dimensions.size())
    {
        data[attribute].push_back(val);
        dimensions[attribute] = 1;
    }
    else
    {
        std::cerr << "[FAIL] Cannot assert float to mesh " << vao << " attribute " << attribute <<
        " which has an attribute size of " << dimensions.size() << std::endl;
    }
}

void Mesh::add_data(unsigned int attribute, Vec2 val)
{
    if(attribute >= 0 && attribute < dimensions.size())
    {
        data[attribute].push_back(val.x);
        data[attribute].push_back(val.y);
        dimensions[attribute] = 2;
    }
    else
    {
        std::cerr << "[FAIL] Cannot assert Vec2 to mesh " << vao << " attribute " << attribute <<
        " which has an attribute size of " << dimensions.size() << std::endl;
    }
}

void Mesh::add_data(unsigned int attribute, Vec3 val)
{
    if(attribute >= 0 && attribute < dimensions.size())
    {
        data[attribute].push_back(val.x);
        data[attribute].push_back(val.y);
        data[attribute].push_back(val.z);
        dimensions[attribute] = 3;
    }
    else
    {
        std::cerr << "[FAIL] Cannot assert Vec3 to mesh " << vao << " attribute " << attribute <<
        " which has an attribute size of " << dimensions.size() << std::endl;
    }
}

void Mesh::add_data(unsigned int attribute, Vec4 val)
{
    if(attribute >= 0 && attribute < dimensions.size())
    {
        data[attribute].push_back(val.x);
        data[attribute].push_back(val.y);
        data[attribute].push_back(val.z);
        data[attribute].push_back(val.w);
        dimensions[attribute] = 4;
    }
    else
    {
        std::cerr << "[FAIL] Cannot assert Vec4 to mesh " << vao << " attribute " << attribute <<
        " which has an attribute size of " << dimensions.size() << std::endl;
    }
}

void Mesh::add_data(unsigned int attribute, std::vector<float>& val)
{
    if(attribute >= 0 && attribute < dimensions.size())
    {
        for(unsigned int i = 0; i < val.size(); i++)
        {
            data[attribute].push_back(val[i]);
        }
        dimensions[attribute] = 1;
    }
    else
    {
        std::cerr << "[FAIL] Cannot assert floats to mesh " << vao << " attribute " << attribute <<
        " which has an attribute size of " << dimensions.size() << std::endl;
    }
}

void Mesh::add_data(unsigned int attribute, std::vector<Vec2>& val)
{
    if(attribute >= 0 && attribute < dimensions.size())
    {
        for(unsigned int i = 0; i < val.size(); i++)
        {
            data[attribute].push_back(val[i].x);
            data[attribute].push_back(val[i].y);
        }
        dimensions[attribute] = 2;
    }
    else
    {
        std::cerr << "[FAIL] Cannot assert Vec2s to mesh " << vao << " attribute " << attribute <<
        " which has an attribute size of " << dimensions.size() << std::endl;
    }
}

void Mesh::add_data(unsigned int attribute, std::vector<Vec3>& val)
{
    if(attribute >= 0 && attribute < dimensions.size())
    {
        for(unsigned int i = 0; i < val.size(); i++)
        {
            data[attribute].push_back(val[i].x);
            data[attribute].push_back(val[i].y);
            data[attribute].push_back(val[i].z);
        }
        dimensions[attribute] = 3;
    }
    else
    {
        std::cerr << "[FAIL] Cannot assert Vec3s to mesh " << vao << " attribute " << attribute <<
        " which has an attribute size of " << dimensions.size() << std::endl;
    }
}

void Mesh::add_data(unsigned int attribute, std::vector<Vec4>& val)
{
    if(attribute >= 0 && attribute < dimensions.size())
    {
        for(unsigned int i = 0; i < val.size(); i++)
        {
            data[attribute].push_back(val[i].x);
            data[attribute].push_back(val[i].y);
            data[attribute].push_back(val[i].z);
            data[attribute].push_back(val[i].w);
        }
        dimensions[attribute] = 4;
    }
    else
    {
        std::cerr << "[FAIL] Cannot assert Vec4s to mesh " << vao << " attribute " << attribute <<
        " which has an attribute size of " << dimensions.size() << std::endl;
    }
}

void Mesh::add_index(unsigned int index)
{
    indices.push_back(index);
}

void Mesh::add_indices(unsigned int* ind, unsigned int size)
{
    for(unsigned int i = 0; i < size; i++)
    {
        indices.push_back(ind[i]);
    }
}

void Mesh::add_indices(std::vector<unsigned int>& ind)
{
    for(unsigned int i = 0; i < ind.size(); i++)
    {
        indices.push_back(ind[i]);
    }
}

void Mesh::bind()
{
    glBindVertexArray(vao);
}

void Mesh::clear_data()
{
    for(unsigned int i = 0; i < data.size(); i++)
    {
        std::vector<float>().swap(data[i]);
    }

    for(unsigned int i = 0; i < dimensions.size(); i++)
    {
        dimensions[i] = 0;
    }

    std::vector<unsigned int>().swap(indices);
}

void Mesh::compile()
{
    bind();
    for(unsigned int i = 0; i < vbos.size(); i++)
    {
        glBindBuffer(GL_ARRAY_BUFFER, vbos[i]);
        glBufferData(GL_ARRAY_BUFFER, data[i].size() * sizeof(float), data[i].data(), GL_STATIC_DRAW);
        glVertexAttribPointer(i, dimensions[i], GL_FLOAT, false, dimensions[i] * sizeof(float), 0);
        glEnableVertexAttribArray(i);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    size = indices.size();

    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    clear_data();
}

void Mesh::destroy()
{
    clear_data();

    std::vector< std::vector<float> >().swap(data);
    std::vector<unsigned char>().swap(dimensions);

    for(unsigned int i = 0; i < vbos.size(); i++)
    {
        glDeleteBuffers(1, &vbos[i]);
    }

    std::vector<unsigned int>().swap(vbos);

    glDeleteBuffers(1, &ibo);
    glDeleteVertexArrays(1, &vao);
}