#include <iostream>
#include <math.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
         0.5f, -0.5f, 0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.5f, 0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, // top left
         0.5f,  0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // perpendicular upper right
        -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // perpendicular upper left
        -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.5f, 0.0f, 1.0f, // bottom left
         0.5f, -0.5f, 1.0f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, // bottom right
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,    // first triangle
        1, 2, 3,    // second triangle
        3, 0, 4,    // perpendicular
        3, 4, 5,    // perpendicular
        4, 5, 6,    // ...
        6, 7, 4,
        1, 7, 2,
        2, 6, 7,
    };

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
    GLuint texture0, texture1;
    glGenTextures(1, &texture0);
    glGenTextures(1, &texture1);

    // texture pattern, filtering, and image #0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int texture0Width, texture0Height, texture0Channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* texture0Image = stbi_load("./resources/textures/0.jpg", &texture0Width, &texture0Height, &texture0Channels, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture0Width, texture0Height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture0Image);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(texture0Image);
    shader.setUniform("textureSampler0", 0);

    // texture pattern, filtering, and image #1
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int texture1Width, texture1Height, texture1Channels;
    unsigned char* texture1Image = stbi_load("./resources/textures/1.jpg", &texture1Width, &texture1Height, &texture1Channels, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture1Width, texture1Height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture1Image);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(texture1Image);
    shader.setUniform("textureSampler1", 1);

    // glBindBuffer(GL_ARRAY_BUFFER, 0);
    // glBindVertexArray(0);

    //  make transformations
    glm::mat4 transform = glm::mat4(1.0f);
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 10.0f);
    shader.setUniform("model", model);
    shader.setUniform("view", view);
    shader.setUniform("projection", projection);


    // play around with green values of all rgbs
    float time;
    float greenValue;

    // run depth check (Z-buffer)
    glEnable(GL_DEPTH_TEST);

    // render loop
    while (!glfwWindowShouldClose(window)) {

        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);

        // change color
        time = glfwGetTime();
        greenValue = sin(time) / 2.0f + 0.5f;
        shader.setUniform("greenValue", greenValue);

        // rotate
        transform = glm::rotate(transform, time/1000, glm::vec3(0.0f, 0.0f, 1.0f));
        shader.setUniform("transform", transform);

        glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, (void*)0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
