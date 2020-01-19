#include <iostream>
#include <math.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <fstream>
#include <streambuf>

#include "stb_image.h"

#include "shader.h"
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

    // shader
    Shader shader("./etc/vertex_shader.glsl", "./etc/fragment_shader.glsl");
    shader.use();

    // objects to draw
    float vertices[] = { // x, y, z, r, g, b, s, t
         0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, // top left
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3,   // second triangle
    };

    // load texture image
    int textureWidth, textureHeight, textureChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* textureImage = stbi_load("./resources/textures/0.jpg", &textureWidth, &textureHeight, &textureChannels, 0);

    // generate and bind vertex array object
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // generate and bind vertex buffer object
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // generate and bind element buffer object
    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    // binding buffers, setting buffer data, and binding
    // vertex arrays attributes should occur in the following order
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // send positions and colors to vertex shader
    GLuint vertexPositionLocation = glGetAttribLocation(shader.ID, "vertexPosition");
    glVertexAttribPointer(vertexPositionLocation, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(vertexPositionLocation);
    GLuint vertexColorLocation = glGetAttribLocation(shader.ID, "vertexColor");
    glVertexAttribPointer(vertexColorLocation, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(vertexColorLocation);
    GLuint vertexTextureLocation = glGetAttribLocation(shader.ID, "vertexTexture");
    glVertexAttribPointer(vertexTextureLocation, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6* sizeof(float)));
    glEnableVertexAttribArray(vertexTextureLocation);

    // generate and bind texture
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // texture pattern, filtering, and image
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureImage);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(textureImage);

    // glBindBuffer(GL_ARRAY_BUFFER, 0);
    // glBindVertexArray(0);

    // play around with green values of all rgbs
    float time;
    float greenValue;

    shader.setUniform("textureSampler", 0);

    // render loop
    while (!glfwWindowShouldClose(window)) {

        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        time = glfwGetTime();
        greenValue = sin(time) / 2.0f + 0.5f;
        shader.setUniform("greenValue", greenValue);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
