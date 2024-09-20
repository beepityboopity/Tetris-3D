#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <texture setup/stb_image.h>
#include <iostream>
#include <shaders/shader_class.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <random>

/* random int generation

    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(0, 10); // define the range

    for(int n=0; n<40; ++n)
        std::cout << distr(gen) << ' '; // generate numbers

*/


void frameCallback(GLFWwindow* window, int width, int height);
void camInput(GLFWwindow* window);
void moveActive(GLFWwindow* window);
bool collisionTest();
void newPieceSpawn();
void gravity();
void orderArray();
void printCubeArray();
void completionCheck();

int rotationVar1 = 20;
int rotationVar2 = 45;
float zoomVar = -10;

double lastMove = 0.0;
double lastGrav = 0.0;
int currentSpawn = 0;
glm::vec3 temp[8];

glm::vec4 cubeArray[1000] = {};
glm::vec4 completionQueue[1000] = {};

glm::vec3 octoCube[8] = {

glm::vec3(5, 5, 5),
glm::vec3(5, 5, 15),
glm::vec3(5, 15, 5),
glm::vec3(5, 15, 15),
glm::vec3(15, 5, 5),
glm::vec3(15, 5, 15),
glm::vec3(15, 15, 5),
glm::vec3(15, 15, 15),

};

glm::vec3 octoLine[8] = {

glm::vec3(5, 5, 5),
glm::vec3(5, 15, 5),
glm::vec3(5, 25, 5),
glm::vec3(5, 35, 5),
glm::vec3(5, 45, 5),
glm::vec3(5, 55, 5),
glm::vec3(5, 65, 5),
glm::vec3(5, 75, 5),

};

glm::vec3 octoI[8] = {

glm::vec3(5, 5, 5),
glm::vec3(15, 5, 5),
glm::vec3(25, 5, 5),
glm::vec3(15, 15, 5),
glm::vec3(15, 25, 5),
glm::vec3(5, 35, 5),
glm::vec3(15, 35, 5),
glm::vec3(25, 35, 5),

};

glm::vec3 activePiece[8];


glm::vec3 spawns[6] {

    glm::vec3(0, 10, 0),  // bottom
    glm::vec3(0, -10, 0), // top
    glm::vec3(10, 0, 0),  // front right
    glm::vec3(-10, 0, 0), // front left
    glm::vec3(0, 0, 10),  // back right
    glm::vec3(0, 0, -10), // back left

};

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> distr(0, 5);

int main() {
    for(int i = 0; i < 8; i++) activePiece[i] = octoCube[i];
    for(int i = 0; i < 8; i++) activePiece[i].y += 100;

    for(int i = 0; i < 1000; i++) cubeArray[i] = glm::vec4(-1, -1, -1, -1.0);

    for(int i = -45; i < 30; i += 10) {
        cubeArray[i + 45] = glm::vec4(i, -45, 45, 1.0);
    }

    for(int i = -45; i < 30; i += 10) {
        cubeArray[i + 45 + 1] = glm::vec4(i, -25, 45, 1.0);
    }

    orderArray();

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
        moveActive(window);
        gravity();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // camera
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

        baseShader.setInt("currentTexture", 1);
        // shape test
        for(int i = 0; i < 8; i++) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3((float)activePiece[i].x / 100, (float)activePiece[i].y / 100, (float)activePiece[i].z / 100));
            model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));
            baseShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // cube array
        for(int i = 0; i < 1000; i++) {
            if(cubeArray[i].w != -1.0) {
                model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3((float)cubeArray[i].x / 100, (float)cubeArray[i].y / 100, (float)cubeArray[i].z / 100));
                model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));
                baseShader.setMat4("model", model);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
        }


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
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) rotationVar1 += 1;
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) rotationVar1 -= 1;

    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) rotationVar2 += 1;
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) rotationVar2 -= 1;

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) zoomVar -= 0.5;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) zoomVar += 0.5;

    if(zoomVar > -1.1) zoomVar = -1.1;
    rotationVar1 = std::min(rotationVar1, 90);
    rotationVar1 = std::max(rotationVar1, -90);
    if(rotationVar2 > 360) rotationVar2 -= 360;
    if(rotationVar2 < -360) rotationVar2 += 360;
}

void moveActive(GLFWwindow* window) {
    if(glfwGetTime() - lastMove < 0.15) return;

    for(int i = 0; i < 8; i++) temp[i] = activePiece[i];

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        if(spawns[currentSpawn].y != 0) {
            for(int i = 0; i < 8; i++) activePiece[i].x += 10;

        }
        else {
            for(int i = 0; i < 8; i++) activePiece[i].y += 10;
        }
        lastMove = glfwGetTime();
    }

    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        if(spawns[currentSpawn].y != 0) {
            for(int i = 0; i < 8; i++) activePiece[i].x += -10;

        }
        else {
            for(int i = 0; i < 8; i++) activePiece[i].y += -10;
        }
        lastMove = glfwGetTime();
    }

    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        if(spawns[currentSpawn].z != 0) {
            for(int i = 0; i < 8; i++) activePiece[i].x += -10;

        }
        else {
            for(int i = 0; i < 8; i++) activePiece[i].z += -10;
        }
        lastMove = glfwGetTime();
    }

    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        if(spawns[currentSpawn].z != 0) {
            for(int i = 0; i < 8; i++) activePiece[i].x += 10;

        }
        else {
            for(int i = 0; i < 8; i++) activePiece[i].z += 10;
        }
        lastMove = glfwGetTime();
    }

    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
        newPieceSpawn();
        lastMove = glfwGetTime();
    }
    if(!collisionTest()) {
        for(int i = 0; i < 8; i++) activePiece[i] = temp[i];
    }
}

bool collisionTest() {
    for(int i = 0; i < 8; i++) {

        if(spawns[currentSpawn].x != 0) {
            if((activePiece[i].y > 45 || activePiece[i].y < -45) || (activePiece[i].z > 45 || activePiece[i].z < -45)) return false;
        }
        else if(spawns[currentSpawn].y != 0) {
            if((activePiece[i].x > 45 || activePiece[i].x < -45) || (activePiece[i].z > 45 || activePiece[i].z < -45)) return false;
        }
        else{
            if((activePiece[i].y > 45 || activePiece[i].y < -45) || (activePiece[i].x > 45 || activePiece[i].x < -45)) return false;
        }

        if(spawns[currentSpawn].x == 10 && activePiece[i].x < -45) return false;
        else if(spawns[currentSpawn].x == -10 && activePiece[i].x > 45) return false;
        else if(spawns[currentSpawn].y == 10 && activePiece[i].y < -45) return false;
        else if(spawns[currentSpawn].y == -10 && activePiece[i].y > 45) return false;
        else if(spawns[currentSpawn].z == 10 && activePiece[i].z < -45) return false;
        else if(spawns[currentSpawn].z == -10 && activePiece[i].z > 45) return false;

        int j = 0;
        while(cubeArray[j].w != -1.0) {
            if(glm::vec3(cubeArray[j].x, cubeArray[j].y, cubeArray[j].z) == activePiece[i]) {
                return false;
            }
            j += 1;
        }
    }
    return true;
}

void newPieceSpawn() {

    currentSpawn = distr(gen);
    int endInd = 0;
    while(cubeArray[endInd].w != -1.0) endInd += 1;
    for(int i = 0; i < 8; i++) cubeArray[endInd + i] = glm::vec4(activePiece[i].x, activePiece[i].y, activePiece[i].z, 1.0);
    for(int i = 0; i < 8; i++) activePiece[i] = octoCube[i] + (spawns[currentSpawn] * glm::vec3(10));
 }

void gravity() {
    if(glfwGetTime() - lastGrav < 1.0) return;
    for(int i = 0; i < 8; i++) temp[i] = activePiece[i];
    for(int i = 0; i < 8; i++) {
        activePiece[i] -= spawns[currentSpawn];
    }
    if(!collisionTest()) {
        for(int i = 0; i < 8; i++) activePiece[i] = temp[i];
        newPieceSpawn();
    }
    lastGrav = glfwGetTime();
}

void orderArray() {
    int endInd = 0;
    while(cubeArray[endInd].x != -1) endInd++;
    for(int i = endInd; i < 1000; i++) {
        if(cubeArray[i].x != -1) {
            cubeArray[endInd] = cubeArray[i];
            cubeArray[i] = glm::vec4(-1, -1, -1, -1.0);
            endInd++;
        }
    }
}

void printCubeArray() {
    std::cout << "printing cubes array\n";
    for(int i = 0; i < 1000; i += 4) std::cout << "(" << cubeArray[i].x << ", " << cubeArray[i].y << ", " << cubeArray[i].z << ")" << "(" << cubeArray[i + 1].x << ", "  << cubeArray[i + 1].y << ", " << cubeArray[i + 1].z << ")"<< "(" << cubeArray[i + 2].x << ", " << cubeArray[i + 2].y << ", " << cubeArray[i + 2].z << ")" << "(" << cubeArray[i + 3].x << ", " << cubeArray[i + 3].y << ", " << cubeArray[i + 3].z << ")\n";

}

void completionCheck() {

}







