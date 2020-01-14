#include <iostream>
#include <math.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <fstream>
#include <streambuf>

#include "util.h"

int main()
{
    // init GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // create a window
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // load opengl functions in GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // rendering window size
    glViewport(0, 0, 800, 600);

    // callback function on window resize
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // vertex shader script
    std::ifstream tv("./etc/vertex_shader.glsl");
    std::string vertexShaderScript((std::istreambuf_iterator<char>(tv)), std::istreambuf_iterator<char>());
    const char * vertexShaderSource = vertexShaderScript.c_str();
    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // fragment shader
    std::ifstream tf("./etc/fragment_shader.glsl");
    std::string fragmentShaderScript((std::istreambuf_iterator<char>(tf)), std::istreambuf_iterator<char>());
    const char * fragmentShaderSource = fragmentShaderScript.c_str();
    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // link shaders
    GLuint shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    // objects to draw
    float vertices[] = {
         0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // top right
         0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // top left
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3,   // second triangle
    };

    // generate element buffer object
    GLuint EBO;
    glGenBuffers(1, &EBO);


    // generate, vertex buffer object
    GLuint VBO;
    glGenBuffers(1, &VBO);

    // generate, bind, and init vertex array object
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // binding buffers, setting buffer data, and binding
    // vertex arrays attributes should occur in the following order
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    GLuint vertexPositionLocation = glGetAttribLocation(shaderProgram, "vertexPosition");
    glVertexAttribPointer(vertexPositionLocation, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    GLuint vertexColorLocation = glGetAttribLocation(shaderProgram, "vertexColor");
    glVertexAttribPointer(vertexColorLocation, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // glBindBuffer(GL_ARRAY_BUFFER, 0);
    // glBindVertexArray(0);

    // play around with green values of all rgbs
    float time;
    float greenValue;

    // render loop
    while (!glfwWindowShouldClose(window)) {

        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        time = glfwGetTime();
        GLuint greenValueLocation = glGetUniformLocation(shaderProgram, "greenValue");
        greenValue = sin(time) / 2.0f + 0.5f;
        glUniform1f(greenValueLocation, greenValue);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
