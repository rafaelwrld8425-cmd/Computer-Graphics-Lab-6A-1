#include "glad.h"
#include "glfw3.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

// Cyan color
const char *fragmentShader1Source = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(0.0f, 1.0f, 1.0f, 1.0f);\n"
    "}\n\0";

// Magenta color
const char *fragmentShader2Source = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.0f, 1.0f, 1.0f);\n"
    "}\n\0";


int main()
{
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(
        SCR_WIDTH,
        SCR_HEIGHT,
        "MD. Mojahidul Islam Rafel",
        NULL,
        NULL
    );

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    // build and compile our shader program
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

    unsigned int fragmentShaderBlue =
        glCreateShader(GL_FRAGMENT_SHADER);

    unsigned int fragmentShaderMagenta =
        glCreateShader(GL_FRAGMENT_SHADER);

    unsigned int shaderProgramBlue = glCreateProgram();

    unsigned int shaderProgramMagenta = glCreateProgram();

    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    glShaderSource(fragmentShaderBlue, 1,
                   &fragmentShader1Source, NULL);
    glCompileShader(fragmentShaderBlue);

    glShaderSource(fragmentShaderMagenta, 1,
                   &fragmentShader2Source, NULL);
    glCompileShader(fragmentShaderMagenta);

    // First shader program - Cyan
    glAttachShader(shaderProgramBlue, vertexShader);
    glAttachShader(shaderProgramBlue, fragmentShaderBlue);
    glLinkProgram(shaderProgramBlue);

    // Second shader program - Magenta
    glAttachShader(shaderProgramMagenta, vertexShader);
    glAttachShader(shaderProgramMagenta, fragmentShaderMagenta);
    glLinkProgram(shaderProgramMagenta);


    // Cyan Square
    // Two triangles = 6 vertices
    float firstTriangle[] = {

        // Triangle 1
        -0.6f,  0.5f, 0.0f,
         0.6f,  0.5f, 0.0f,
         0.6f, -0.5f, 0.0f,

        // Triangle 2
        -0.6f,  0.5f, 0.0f,
         0.6f, -0.5f, 0.0f,
        -0.6f, -0.5f, 0.0f
    };


    // Magenta Triangle
    // Shares two top corners of the square
    float secondTriangle[] = {

        // Shared corner 1
        -0.6f,  0.5f, 0.0f,

        // Shared corner 2
         0.6f,  0.5f, 0.0f,

        // Top vertex of triangle
         0.0f,  0.95f, 0.0f
    };


    unsigned int VBOs[2], VAOs[2];

    glGenVertexArrays(2, VAOs);
    glGenBuffers(2, VBOs);


    // First triangle setup
    // --------------------
    // Cyan Square
    glBindVertexArray(VAOs[0]);

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);

    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(firstTriangle),
        firstTriangle,
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        (void*)0
    );

    glEnableVertexAttribArray(0);


    // Second triangle setup
    // ---------------------
    // Magenta Triangle
    glBindVertexArray(VAOs[1]);

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);

    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(secondTriangle),
        secondTriangle,
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        (void*)0
    );

    glEnableVertexAttribArray(0);


    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        // White background
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT);


        // Draw Cyan Square
        glUseProgram(shaderProgramBlue);

        glBindVertexArray(VAOs[0]);

        // 2 triangles = 6 vertices
        glDrawArrays(GL_TRIANGLES, 0, 6);


        // Draw Magenta Triangle
        glUseProgram(shaderProgramMagenta);

        glBindVertexArray(VAOs[1]);

        // 1 triangle = 3 vertices
        glDrawArrays(GL_TRIANGLES, 0, 3);


        // glfw: swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    // De-allocate resources
    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);

    glDeleteProgram(shaderProgramBlue);
    glDeleteProgram(shaderProgramMagenta);


    // glfw: terminate
    glfwTerminate();

    return 0;
}


// Process input
void processInput(GLFWwindow *window)
{
    // Press M to close the window
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}


// Window resize callback
void framebuffer_size_callback(
    GLFWwindow* window,
    int width,
    int height)
{
    glViewport(0, 0, width, height);
}