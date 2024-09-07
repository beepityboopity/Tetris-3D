#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <texture setup/stb_image.h>
#include <iostream>
#include <shaders/shader_class.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void frameCallback(GLFWwindow* window, int width, int height);
void camInput(GLFWwindow* window);
void moveCube(GLFWwindow* window);


int rotationVar1 = 20;
int rotationVar2 = 45;
float zoomVar = -20;

glm::vec3 cubePos = glm::vec3(-0.45, 0.45, 0.45);
double lastMove = 0.0;

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(800, 600, "3D Tetris", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, frameCallback);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glEnable(GL_DEPTH_TEST);

    Shader baseShader("vertex1.vs", "frag1.fs");
    baseShader.use();

    float vertices[] = {
        /// for a full cube
        -0.5f, -0.5f, -0.5f,        0.0f, 0.0f, 0.0f, 0.0f, -1.0f,// bottom left
         0.5f, -0.5f, -0.5f,        1.0f, 0.0f, 0.0f, 0.0f, -1.0f,// bottom right
         0.5f,  0.5f, -0.5f,        1.0f, 1.0f, 0.0f, 0.0f, -1.0f,// top right
         0.5f,  0.5f, -0.5f,        1.0f, 1.0f, 0.0f, 0.0f, -1.0f,// top right
        -0.5f,  0.5f, -0.5f,        0.0f, 1.0f, 0.0f, 0.0f, -1.0f,// top left
        -0.5f, -0.5f, -0.5f,        0.0f, 0.0f, 0.0f, 0.0f, -1.0f,// bottom left
        ///
        -0.5f, -0.5f,  0.5f,        0.0f, 0.0f, 0.0f, 0.0f, 1.0f,// bottom left
         0.5f, -0.5f,  0.5f,        1.0f, 0.0f, 0.0f, 0.0f, 1.0f,// bottom right
         0.5f,  0.5f,  0.5f,        1.0f, 1.0f, 0.0f, 0.0f, 1.0f,// top right
         0.5f,  0.5f,  0.5f,        1.0f, 1.0f, 0.0f, 0.0f, 1.0f,// top right
        -0.5f,  0.5f,  0.5f,        0.0f, 1.0f, 0.0f, 0.0f, 1.0f,// top left
        -0.5f, -0.5f,  0.5f,        0.0f, 0.0f, 0.0f, 0.0f, 1.0f,// bottom left
        ///
        -0.5f, -0.5f,  0.5f,        0.0f, 0.0f, -1.0f, 0.0f, 0.0f,// bottom left
        -0.5f,  0.5f,  0.5f,        1.0f, 0.0f, -1.0f, 0.0f, 0.0f,// bottom right
        -0.5f,  0.5f, -0.5f,        1.0f, 1.0f, -1.0f, 0.0f, 0.0f,// top right
        -0.5f,  0.5f, -0.5f,        1.0f, 1.0f, -1.0f, 0.0f, 0.0f,// top right
        -0.5f, -0.5f, -0.5f,        0.0f, 1.0f, -1.0f, 0.0f, 0.0f,// top left
        -0.5f, -0.5f,  0.5f,        0.0f, 0.0f, -1.0f, 0.0f, 0.0f,// bottom left
        ///
         0.5f,  0.5f,  0.5f,        0.0f, 0.0f, 1.0f, 0.0f, 0.0f,// bottom left
         0.5f, -0.5f,  0.5f,        1.0f, 0.0f, 1.0f, 0.0f, 0.0f,// bottom right
         0.5f, -0.5f, -0.5f,        1.0f, 1.0f, 1.0f, 0.0f, 0.0f,// top right
         0.5f, -0.5f, -0.5f,        1.0f, 1.0f, 1.0f, 0.0f, 0.0f,// top right
         0.5f,  0.5f, -0.5f,        0.0f, 1.0f, 1.0f, 0.0f, 0.0f,// top left
         0.5f,  0.5f,  0.5f,        0.0f, 0.0f, 1.0f, 0.0f, 0.0f,// bottom left
        ///
         0.5f, -0.5f,  0.5f,        0.0f, 0.0f, 0.0f, -1.0f, 0.0f,// bottom left
        -0.5f, -0.5f,  0.5f,        1.0f, 0.0f, 0.0f, -1.0f, 0.0f,// bottom right
        -0.5f, -0.5f, -0.5f,        1.0f, 1.0f, 0.0f, -1.0f, 0.0f,// top right
        -0.5f, -0.5f, -0.5f,        1.0f, 1.0f, 0.0f, -1.0f, 0.0f,// top right
         0.5f, -0.5f, -0.5f,        0.0f, 1.0f, 0.0f, -1.0f, 0.0f,// top left
         0.5f, -0.5f,  0.5f,        0.0f, 0.0f, 0.0f, -1.0f, 0.0f,// bottom left
        ///
        -0.5f,  0.5f,  0.5f,        0.0f, 0.0f, 0.0f, 1.0f, 0.0f,// bottom left
         0.5f,  0.5f,  0.5f,        1.0f, 0.0f, 0.0f, 1.0f, 0.0f,// bottom right
         0.5f,  0.5f, -0.5f,        1.0f, 1.0f, 0.0f, 1.0f, 0.0f,// top right
         0.5f,  0.5f, -0.5f,        1.0f, 1.0f, 0.0f, 1.0f, 0.0f,// top right
        -0.5f,  0.5f, -0.5f,        0.0f, 1.0f, 0.0f, 1.0f, 0.0f,// top left
        -0.5f,  0.5f,  0.5f,        0.0f, 0.0f, 0.0f, 1.0f, 0.0f,// bottom left
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    unsigned int gridTexture;
    int width, height, nrChannels;
    // grid
    glGenTextures(1, &gridTexture);
    glBindTexture(GL_TEXTURE_2D, gridTexture);
    // wrapping and filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // load image
    unsigned char* data = stbi_load("boxFrame.png", &width, &height, &nrChannels, 0); // loads image data and stores its dimensions
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); // generates the texture
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gridTexture);
    baseShader.setInt("currentTexture", 0);

    glActiveTexture(GL_TEXTURE1);
    unsigned int cubeTexture;
    int width1, height1, nrChannels1;
    // grid
    glGenTextures(1, &cubeTexture);
    glBindTexture(GL_TEXTURE_2D, cubeTexture);
    // wrapping and filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // load image
    unsigned char* data1 = stbi_load("boxRed.png", &width1, &height1, &nrChannels1, 0); // loads image data and stores its dimensions
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width1, height1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data1); // generates the texture
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data1);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, cubeTexture);

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    baseShader.setMat4("model", model);
    baseShader.setMat4("view", view);
    baseShader.setMat4("projection", projection);



    while (!glfwWindowShouldClose(window)) {

        camInput(window);
        moveCube(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



        view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, (float)zoomVar / 3));
        view = glm::rotate(view, glm::radians((float)rotationVar1), glm::vec3(1.0, 0.0, 0.0)); // vertical x
        view = glm::rotate(view, glm::radians((float)rotationVar2), glm::vec3(0.0, 1.0, 0.0)); // horizontal y
        baseShader.setMat4("view", view);


        // grid
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(1.01, 1.01, 1.01));
        baseShader.setMat4("model", model);
        baseShader.setInt("currentTexture", 0);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // cube
        model = glm::mat4(1.0f);
        model = glm::translate(model, cubePos);
        model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));
        baseShader.setInt("currentTexture", 1);
        baseShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return 0;
}

void frameCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void camInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) rotationVar1 += 1;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) rotationVar1 -= 1;

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) rotationVar2 += 1;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) rotationVar2 -= 1;

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) zoomVar -= 0.5;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) zoomVar += 0.5;

    if(zoomVar > -1.1) zoomVar = -1.1;
    rotationVar1 = std::min(rotationVar1, 90);
    rotationVar1 = std::max(rotationVar1, -90);
    if(rotationVar2 > 360) rotationVar2 -= 360;
    if(rotationVar2 < -360) rotationVar2 += 360;
}

void moveCube(GLFWwindow* window) {
    if(glfwGetTime() - lastMove < 0.25) return;
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
        cubePos.y += 0.1;
        lastMove = glfwGetTime();
    }
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
        cubePos.y -= 0.1;
        lastMove = glfwGetTime();
    }
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
        cubePos.x += 0.1;
        lastMove = glfwGetTime();
    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        cubePos.x -= 0.1;
        lastMove = glfwGetTime();
    }
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
        cubePos.z -= 0.1;
        lastMove = glfwGetTime();
    }
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
        cubePos.z += 0.1;
        lastMove = glfwGetTime();
    }
}

