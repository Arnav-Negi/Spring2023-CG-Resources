#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
#include <vector>
#include "shapes.hpp"

#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int PYRAMID = 0;

Prism shapePrism(3);
Pyramid shapePyramid(3);

const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "uniform mat4 transform, view, projection;\n"
                                 "out vec4 color;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = projection  * transform * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                 "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "uniform vec4 color;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   FragColor = color;\n"
                                   "}\n\0";

int main(int argc, char **argv)
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }
    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    if (argc != 2)
    {
        std::cout << "SYNTAX ERROR: Should be ./app [no. of vertices].\n";
        exit(1);
    }

    int nsides = atoi(argv[1]);
    if (nsides <= 2)
    {
        std::cout << "ERROR: no. of vertices should be >= 3.\n";
        exit(1);
    }

    shapePrism = Prism((unsigned int)nsides);
    shapePyramid = Pyramid((unsigned int)nsides);

    unsigned int VBO_Prism, VBO_Pyramid, EBO_Prism, EBO_Pyramid, VAO_Prism, VAO_Pyramid;
    shapePrism.initBuffers(&VAO_Prism, &VBO_Prism, &EBO_Prism);
    shapePyramid.initBuffers(&VAO_Pyramid, &VBO_Pyramid, &EBO_Pyramid);

    //  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // render loop
    // -----------
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        glUseProgram(shaderProgram);
        
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);

        int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        if (PYRAMID != 1)
            shapePrism.draw(&VAO_Prism, shaderProgram);
        else
            shapePyramid.draw(&VAO_Pyramid, shaderProgram);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO_Prism);
    glDeleteBuffers(1, &EBO_Prism);
    glDeleteVertexArrays(1, &VAO_Pyramid);
    glDeleteBuffers(1, &EBO_Pyramid);
    glDeleteProgram(shaderProgram);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
    {
        if (PYRAMID == 0) PYRAMID = 2;
        if (PYRAMID == 1) PYRAMID = 3;
    }
    else if (glfwGetKey(window, GLFW_KEY_T) == GLFW_RELEASE){
        if (PYRAMID == 2) PYRAMID = 1;
        if (PYRAMID == 3) PYRAMID = 0;
    }
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
        shapePrism.shiftVertices(0,0.02,0);
        shapePyramid.shiftVertices(0,0.02,0);
    }
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
        shapePrism.shiftVertices(0,-0.02,0);
        shapePyramid.shiftVertices(0,-0.02,0);
    }
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
        shapePrism.shiftVertices(0.02,0,0);
        shapePyramid.shiftVertices(0.02,0,0);
    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        shapePrism.shiftVertices(-0.02,0,0);
        shapePyramid.shiftVertices(-0.02,0,0);
    }
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
        shapePrism.shiftVertices(0,0,0.02);
        shapePyramid.shiftVertices(0,0,0.02);
    }
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
        shapePrism.shiftVertices(0,0,-0.02);
        shapePyramid.shiftVertices(0,0,-0.02);
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
