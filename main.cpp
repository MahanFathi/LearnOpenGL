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

#include "model.h"
#include "shader.h"
#include "util.h"
#include "common.h"
#include "filesystem.h"

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
    Shader objectShader("./etc/object_shaders/vertex_shader.glsl", "./etc/object_shaders/fragment_shader.glsl");

    // model
    Model model(FileSystem::getPath("./resources/nanosuit/nanosuit.obj"), false);

    //  make transformations
    //  projection
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 1000.0f);
    // translate
    glm::mat4 modelTranslation = glm::mat4(1.0f);
    modelTranslation = glm::translate(modelTranslation, glm::vec3(1.0f, 1.0f, 1.0f));
    objectShader.setUniform("model", modelTranslation);

    // lighting position
    glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 lightPosition = glm::vec3(1.0f, 1.0f, -3.0f);

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

        // render objects with objectShader
        objectShader.use();
        objectShader.setUniform("projection", projection);
        objectShader.setUniform("cameraPosition", cameraPos);
        objectShader.setUniform("lightPosition", lightPosition);
        objectShader.setUniform("lightColor", lightColor);
        objectShader.setUniform("material.ambientStrength", 0.2f);
        objectShader.setUniform("material.diffuseStrength", 0.7f);
        objectShader.setUniform("material.specularStrength", 0.5f);
        objectShader.setUniform("material.shininess", 16.0f);
        objectShader.setUniform("material.textureSampler", 0);
        objectShader.setUniform("material.textureSamplerSpecular", 1);
        // rotate
        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::rotate(transform, time, glm::vec3(0.0f, 1.0f, 0.0f));
        objectShader.setUniform("transform", transform);
        // draw object
        model.Draw(objectShader);

        // cam might be readjusted
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        objectShader.setUniform("view", view);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
