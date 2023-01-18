#ifndef SHAPES_H
#define SHAPES_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
#include <vector>

#include <iostream>

class Prism
{
public:
    unsigned int nsides;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    Prism(unsigned int n)
    {
        nsides = n;
        glm::vec4 point = glm::vec4(0.0f, 1.0f, 0.5f, 1.0f);

        glm::mat4 trans = glm::mat4(1.0f);
        float theta = 2.0 * M_PI / nsides;
        trans = glm::rotate(trans, theta, glm::vec3(0.0, 0.0, 1.0));

        for (int i = 0; i < nsides; i++)
        {
            vertices.push_back(point.x);
            vertices.push_back(point.y);
            vertices.push_back(point.z);
            point = trans * point;
        }

        vertices.push_back(0.0);
        vertices.push_back(0.0);
        vertices.push_back(0.5);
        point = glm::vec4(0.0f, 1.0f, -0.5f, 1.0f);

        for (int i = 0; i < nsides; i++)
        {
            vertices.push_back(point.x);
            vertices.push_back(point.y);
            vertices.push_back(point.z);
            point = trans * point;
        }
        vertices.push_back(0.0);
        vertices.push_back(0.0);
        vertices.push_back(-0.5);

        for (int i = 0; i < nsides; i++)
        {
            indices.push_back(nsides);
            indices.push_back((i + 1) % nsides);
            indices.push_back(i);
            
        }
        for (int i = 0; i < nsides; i++)
        {
            indices.push_back(2 * nsides + 1);
            indices.push_back(nsides + i + 1);
            indices.push_back(nsides + (i + 1) % nsides + 1);
        }
        for (int i = 0; i < nsides; i++)
        {
            indices.push_back(i);
            indices.push_back((i + 1) % nsides);
            indices.push_back(nsides + 1 + i);

            indices.push_back((i + 1) % nsides);
            indices.push_back(nsides + 1 + (i + 1) % nsides);
            indices.push_back(nsides + 1 + i);
        }
    }

    void initBuffers(unsigned int *VAO, unsigned int *VBO, unsigned int *EBO)
    {
        glGenVertexArrays(1, VAO);
        glGenBuffers(1, VBO);
        glGenBuffers(1, EBO);
        // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
        glBindVertexArray(*VAO);

        // init the VBO
        glBindBuffer(GL_ARRAY_BUFFER, *VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

        // init the EBO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // declare attributes
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
        // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
        glBindVertexArray(0);
    }

    void draw(unsigned int *VAO, unsigned int shaderProg)
    {
        srand(0);
        glBindVertexArray(*VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        int vertexColorLocation = glGetUniformLocation(shaderProg, "color");
        glUniform4f(vertexColorLocation, 0.0f, 1.0f, 0.0f, 1.0f);

        glDrawElements(GL_TRIANGLES, 6 * nsides, GL_UNSIGNED_INT, (void *)0);
        double theta;
        for (int i = 0; i < nsides; i++)
        {
            theta = 2.0 * M_PI * i / nsides;
            glUniform4f(vertexColorLocation, (float)rand()/RAND_MAX, (float)rand()/RAND_MAX, (float)rand()/RAND_MAX, 1.0);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(6 * (nsides + i) * sizeof(unsigned int)));
        }
        glBindVertexArray(0);
    }
};

class Pyramid
{
public:
    unsigned int nsides;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    Pyramid(unsigned int n)
    {
        nsides = n;
        glm::vec4 point = glm::vec4(0.0f, 1.0f, 0.5f, 1.0f);

        glm::mat4 trans = glm::mat4(1.0f);
        float theta = 2.0 * M_PI / nsides;
        trans = glm::rotate(trans, theta, glm::vec3(0.0, 0.0, 1.0));

        vertices.push_back(0.0);
        vertices.push_back(0.0);
        vertices.push_back(0.5);
        point = glm::vec4(0.0f, 1.0f, -0.5f, 1.0f);

        for (int i = 0; i < nsides; i++)
        {
            vertices.push_back(point.x);
            vertices.push_back(point.y);
            vertices.push_back(point.z);
            point = trans * point;
        }
        vertices.push_back(0.0);
        vertices.push_back(0.0);
        vertices.push_back(-0.5);

        for (int i = 0; i < nsides; i++)
        {
            indices.push_back(0);
            indices.push_back((i + 1) % nsides + 1);
            indices.push_back(i + 1);
            
        }
        for (int i = 0; i < nsides; i++)
        {
            indices.push_back(nsides + 1);
            indices.push_back(i + 1);
            indices.push_back((i + 1) % nsides + 1);
        }
    }

    void initBuffers(unsigned int *VAO, unsigned int *VBO, unsigned int *EBO)
    {
        glGenVertexArrays(1, VAO);
        glGenBuffers(1, VBO);
        glGenBuffers(1, EBO);
        // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
        glBindVertexArray(*VAO);

        // init the VBO
        glBindBuffer(GL_ARRAY_BUFFER, *VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

        // init the EBO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // declare attributes
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
        // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
        glBindVertexArray(0);
    }

    void draw(unsigned int *VAO, unsigned int shaderProg)
    {
        srand(0);
        glBindVertexArray(*VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        int vertexColorLocation = glGetUniformLocation(shaderProg, "color");
        glUniform4f(vertexColorLocation, 0.0f, 1.0f, 0.0f, 1.0f);

        glDrawElements(GL_TRIANGLES, 3 * nsides, GL_UNSIGNED_INT, (void *)(3 * nsides * sizeof(unsigned int)));
        double theta;
        for (int i = 0; i < nsides; i++)
        {
            theta = 2.0 * M_PI * i / nsides;
            glUniform4f(vertexColorLocation, (float)rand()/RAND_MAX, (float)rand()/RAND_MAX, (float)rand()/RAND_MAX, 1.0);
            glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void *)(3 * i * sizeof(unsigned int)));
        }
        glBindVertexArray(0);
    }
};

#endif