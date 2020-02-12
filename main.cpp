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
#include "common.h"

// cam settings
float deltaTime = 0.0f;
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

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

    // hide and caputure the cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // load opengl functions in GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // rendering window size
    glViewport(0, 0, 800, 600);

    // assign callback functions
glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
glfwSetCursorPosCallback(window, mouse_callback);

// shader
Shader shader("./etc/vertex_shader.glsl", "./etc/fragment_shader.glsl");
    shader.use();

    // objects to draw
    float vertices[] = { // x, y, z, nx, ny, nz
          0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
          0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
          0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

          0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
          0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
          0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

          0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
          0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
          0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
          0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
          0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
          0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,

         -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
         -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
         -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
         -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
         -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
         -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,


          0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
          0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
          0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,

          0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
          0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
          0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    };
   
    glm::vec3 cubePositions[] = {   // spawn cubes at these positions
      glm::vec3( 0.0f,  0.0f,  0.1f),
      glm::vec3( 2.0f,  5.0f, -15.0f), 
      glm::vec3(-1.5f, -2.2f, -2.5f),  
      glm::vec3(-3.8f, -2.0f, -12.3f),  
      glm::vec3( 2.4f, -0.4f, -3.5f),  
      glm::vec3(-1.7f,  3.0f, -7.5f),  
      glm::vec3( 1.3f, -2.0f, -2.5f),  
      glm::vec3( 1.5f,  2.0f, -2.5f), 
      glm::vec3( 1.5f,  0.2f, -1.5f), 
      glm::vec3(-1.3f,  1.0f, -1.5f)  
    };

    // generate and bind vertex array object
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // generate and bind vertex buffer object
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // binding buffers, setting buffer data, and binding
    // vertex arrays attributes should occur in the following order
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // send positions and colors to vertex shader
    GLuint vertexPositionLocation = glGetAttribLocation(shader.ID, "vertexPosition");
    glVertexAttribPointer(vertexPositionLocation, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(vertexPositionLocation);
    GLuint vertexNormalLocation = glGetAttribLocation(shader.ID, "vertexNormal");
    glVertexAttribPointer(vertexNormalLocation, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(vertexNormalLocation);

    // glBindBuffer(GL_ARRAY_BUFFER, 0);
    // glBindVertexArray(0);

    //  make transformations
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 1000.0f);
    shader.setUniform("projection", projection);

    // set color
    glm::vec3 cubeColor = glm::vec3(0.19f, 0.84f, 0.78f);
    shader.setUniform("cubeColor", cubeColor);

    // lighting position
    glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 lightPosition = glm::vec3(1.0f, 1.0f, 5.0f);
    shader.setUniform("lightPosition", lightPosition);
    shader.setUniform("lightColor", lightColor);

    // run depth check (Z-buffer)
    glEnable(GL_DEPTH_TEST);

    // render loop
    float time;
    float lastFrameTime = 0.0f;
    while (!glfwWindowShouldClose(window)) {

        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);

        // change color
        time = glfwGetTime();
        deltaTime = time - lastFrameTime;
        lastFrameTime = time;

        for (auto position : cubePositions) {

            // rotate
            glm::mat4 transform = glm::mat4(1.0f);
            transform = glm::rotate(transform, time, glm::normalize(position));
            shader.setUniform("transform", transform);

            // translate
            glm::mat4 modelTranslation = glm::mat4(1.0f);
            modelTranslation = glm::translate(modelTranslation, position);
            shader.setUniform("model", modelTranslation);

            glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, (void*)0);
            glDrawArrays(GL_TRIANGLES, 0, 36);


        }

        // cam might be readjusted
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        shader.setUniform("view", view);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
