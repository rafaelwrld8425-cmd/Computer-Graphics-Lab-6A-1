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

// 
const char *fragmentShader2Source = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);\n"
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
        "0432410005101011",
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

    unsigned int fragmentShaderGreen =
        glCreateShader(GL_FRAGMENT_SHADER);

    unsigned int shaderProgramBlue = glCreateProgram();

    unsigned int shaderProgramGreen = glCreateProgram();

    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    glShaderSource(fragmentShaderBlue, 1,
                   &fragmentShader1Source, NULL);
    glCompileShader(fragmentShaderBlue);

    glShaderSource(fragmentShaderGreen, 1,
                   &fragmentShader2Source, NULL);
    glCompileShader(fragmentShaderGreen);

    // First shader program
    glAttachShader(shaderProgramBlue, vertexShader);
    glAttachShader(shaderProgramBlue, fragmentShaderBlue);
    glLinkProgram(shaderProgramBlue);

    // Second shader program
    glAttachShader(shaderProgramGreen, vertexShader);
    glAttachShader(shaderProgramGreen, fragmentShaderGreen);
    glLinkProgram(shaderProgramGreen);


    // Star made using 5 triangles
    float firstTriangle[] = {

        // Triangle 1 - Top
         0.0f,  0.85f, 0.0f,
        -0.40f,  0.05f, 0.0f,
         0.40f,  0.05f, 0.0f,

        // Triangle 2 - Right Upper
         0.75f,  0.25f, 0.0f,
         0.05f,  0.40f, 0.0f,
         0.05f, -0.25f, 0.0f,

        // Triangle 3 - Right Lower
         0.45f, -0.70f, 0.0f,
         0.25f,  0.05f, 0.0f,
        -0.35f, -0.15f, 0.0f,

        // Triangle 4 - Left Lower
        -0.45f, -0.70f, 0.0f,
        -0.25f,  0.05f, 0.0f,
         0.35f, -0.15f, 0.0f,

        // Triangle 5 - Left Upper
        -0.75f,  0.25f, 0.0f,
        -0.05f,  0.40f, 0.0f,
        -0.05f, -0.25f, 0.0f
    };


    // 
    float secondTriangle[] = {
        0.0f, -0.5f, 0.0f,
        0.9f, -0.5f, 0.0f,
        0.45f, 0.5f, 0.0f
    };


    unsigned int VBOs[2], VAOs[2];

    glGenVertexArrays(2, VAOs);
    glGenBuffers(2, VBOs);


    // First triangle setup
    // --------------------
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
    // 

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
        0,
        (void*)0
    );

    glEnableVertexAttribArray(0);


    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        // Yellow background
        glClearColor(1.0f, 1.0f, 0.0f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT);


        // Draw Cyan Star
        glUseProgram(shaderProgramBlue);

        glBindVertexArray(VAOs[0]);

        // 5 triangles = 15 vertices
        glDrawArrays(GL_TRIANGLES, 0, 15);


        // glfw: swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    // De-allocate resources
    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);

    glDeleteProgram(shaderProgramBlue);
    glDeleteProgram(shaderProgramGreen);


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