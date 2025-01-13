#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <texture setup/stb_image.h>
#include <iostream>
#include <shaders/shader_class.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <random>
#include <vector>


// Function definition ------------------------------------------------------------------------------------------------

// graphics specific
void frameCallback(GLFWwindow* window, int width, int height);
void imguiSetup(GLFWwindow* window, const char* glsl_version);
void drawStuff(Shader shader, glm::mat4 model, glm::mat4 view, glm::mat4 projection);

// user input
void camInput(GLFWwindow* window);
void moveActive(GLFWwindow* window);
void otherInput(GLFWwindow* window);
void rotateActive(GLFWwindow* window);

// internal logic
bool collisionTest();
void newPieceSpawn();
void gravity();
void orderArray();
void completionCheck();
std::vector<glm::vec3> getAxis(std::vector<glm::vec3> cubeList);
std::vector<std::vector<int>> getRows(std::vector<glm::vec3> axis);

// other
int randInt(int low, int high);
void printCubeArray();

// Variables -----------------------------------------------------------------------------------------------------------

int score = 0;
bool fail = false;
bool startGame = false;
bool paused = false;

int rotationVar2 = 45;
float zoomVar = -7;

double lastMove = 0.0;
double lastGrav = 0.0;
double lastRotate = 0.0;
double lastInput = 0.0;
int currentSpawn = 0;
int currentPiece = 0;

glm::vec3 temp = glm::vec3(0, 0, 0);
glm::vec3 activeOffset = glm::vec3(0, 0, 0);

std::vector<std::vector<glm::vec3>> shapes = {
    {glm::vec3(5, 5, 5), glm::vec3(5, 5, -5), glm::vec3(5, -5, 5), glm::vec3(5, -5, -5), glm::vec3(-5, 5, 5), glm::vec3(-5, 5, -5), glm::vec3(-5, -5, 5), glm::vec3(-5, -5, -5)} , //cube
    {glm::vec3(-5, -5, 5), glm::vec3(5, -5, 5), glm::vec3(15, -5, 5), glm::vec3(5, 5, 5), glm::vec3(5, 15, 5), glm::vec3(-5, 25, 5), glm::vec3(5, 25, 5), glm::vec3(15, 25, 5)}, // I
    {glm::vec3(5, 15, 5), glm::vec3(5, 15, 15), glm::vec3(5, 5, 15), glm::vec3(5, -5, 15), glm::vec3(5, -5, 5), glm::vec3(5, -5, -5), glm::vec3(5, 5, -5), glm::vec3(5, 15, -5)}, // Donut
    {glm::vec3(5, -5, -5), glm::vec3(5, -5, 5), glm::vec3(5, 5, 5), glm::vec3(5, 5, 15), glm::vec3(15, -5, -5), glm::vec3(15, -5, 5), glm::vec3(15, 5, 5), glm::vec3(15, 5, 15)}, // S
    {glm::vec3(5, 5, -5), glm::vec3(5, -5, 5), glm::vec3(5, 5, 5), glm::vec3(5, -5, 15), glm::vec3(15, 5, -5), glm::vec3(15, -5, 5), glm::vec3(15, 5, 5), glm::vec3(15, -5, 15)}, // Z
    {glm::vec3(-15, 5, -5), glm::vec3(-5, 5, -5), glm::vec3(5, 5, -5), glm::vec3(15, 5, -5), glm::vec3(-15, 5, 5), glm::vec3(-5, 5, 5), glm::vec3(5, 5, 5), glm::vec3(15, 5, 5)}, // thick l
    {glm::vec3(-15, -5, 5), glm::vec3(-15, 5, 5), glm::vec3(-5, -5, 5), glm::vec3(5, -5, 5), glm::vec3(5, 5, 5), glm::vec3(15, -5, 5), glm::vec3(25, -5, 5), glm::vec3(25, 5, 5)}, // claw
    {glm::vec3(5, -5, 5), glm::vec3(-5, 5, 5), glm::vec3(5, 5, 5), glm::vec3(15, 5, 5), glm::vec3(-5, 15, 5), glm::vec3(15, 15, 5), glm::vec3(-5, 25, 5), glm::vec3(15, 25, 5)}, // cup
    {glm::vec3(5, -5, 5), glm::vec3(5, -5, -5), glm::vec3(5, 5, -5), glm::vec3(-5, 5, -5), glm::vec3(-5, 15, -5), glm::vec3(-5, 15, 5), glm::vec3(-5, 25, 5), glm::vec3(5, 25, 5)}, // staircase
    {glm::vec3(5, 5, 5), glm::vec3(5, 5, -5), glm::vec3(-5, 5, 5), glm::vec3(-5, 5, -5), glm::vec3(5, 15, 5), glm::vec3(5, 25, 5), glm::vec3(-5, -5, -5), glm::vec3(-5, -15, -5)}, // reel
};
glm::vec3 colors[10] = {
    glm::vec3(0.8, 0.0, 0.0), //red
    glm::vec3(0.4, 0.22, 0.77), // purple
    glm::vec3(0.16, 0.43, 0.75), // blue
    glm::vec3(0.11, 0.58, 0.22), // green
    glm::vec3(0.91, 0.32, 0.85), // pink
    glm::vec3(0.84, 0.49, 0.06), // orange
    glm::vec3(0.4, 0.88, 0.68), // aqua
    glm::vec3(0.93, 0.87, 0.18), // yellow
    glm::vec3(0.56, 0.01, 0.01), // evil red
    glm::vec3(0.94, 0.16, 0.53), // hot pink
};

glm::vec4 cubeArray[1000] = {};
glm::vec3 activePiece[8];
std::vector<glm::vec3> cubesCheck = {};

glm::vec3 spawns[6] {

    glm::vec3(0, 10, 0),  // bottom
    glm::vec3(0, -10, 0), // top
    glm::vec3(10, 0, 0),  // front right
    glm::vec3(-10, 0, 0), // front left
    glm::vec3(0, 0, 10),  // back right
    glm::vec3(0, 0, -10), // back left

};

// Main --------------------------------------------------------------------------------------------------------------------

int main() {

    // Window setup ------------------------------------------------------------------------------------------------------

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 100);
    GLFWwindow* window = glfwCreateWindow(1250, 750, "Octris", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, frameCallback);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    imguiSetup(window, "#version 130");
    Shader baseShader("vertex1.vs", "frag1.fs");
    baseShader.use();
    /*
    float vert[] = {
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
    */
    float vertices[] = {
        /// for a full cube
        -0.5f, -0.5f, -0.5f,        0.0f, 0.0f, // bottom left
         0.5f, -0.5f, -0.5f,        1.0f, 0.0f, // bottom right
         0.5f,  0.5f, -0.5f,        1.0f, 1.0f, // top right
         0.5f,  0.5f, -0.5f,        1.0f, 1.0f, // top right
        -0.5f,  0.5f, -0.5f,        0.0f, 1.0f, // top left
        -0.5f, -0.5f, -0.5f,        0.0f, 0.0f, // bottom left
        ///
        -0.5f, -0.5f,  0.5f,        0.0f, 0.0f, // bottom left
         0.5f, -0.5f,  0.5f,        1.0f, 0.0f, // bottom right
         0.5f,  0.5f,  0.5f,        1.0f, 1.0f, // top right
         0.5f,  0.5f,  0.5f,        1.0f, 1.0f, // top right
        -0.5f,  0.5f,  0.5f,        0.0f, 1.0f, // top left
        -0.5f, -0.5f,  0.5f,        0.0f, 0.0f, // bottom left
        ///
        -0.5f, -0.5f,  0.5f,        0.0f, 0.0f, // bottom left
        -0.5f,  0.5f,  0.5f,        1.0f, 0.0f, // bottom right
        -0.5f,  0.5f, -0.5f,        1.0f, 1.0f, // top right
        -0.5f,  0.5f, -0.5f,        1.0f, 1.0f, // top right
        -0.5f, -0.5f, -0.5f,        0.0f, 1.0f, // top left
        -0.5f, -0.5f,  0.5f,        0.0f, 0.0f, // bottom left
        ///
         0.5f,  0.5f,  0.5f,        0.0f, 0.0f, // bottom left
         0.5f, -0.5f,  0.5f,        1.0f, 0.0f, // bottom right
         0.5f, -0.5f, -0.5f,        1.0f, 1.0f, // top right
         0.5f, -0.5f, -0.5f,        1.0f, 1.0f, // top right
         0.5f,  0.5f, -0.5f,        0.0f, 1.0f, // top left
         0.5f,  0.5f,  0.5f,        0.0f, 0.0f, // bottom left
         ///
          0.5f, -0.5f,  0.5f,        0.0f, 0.0f, // bottom left
         -0.5f, -0.5f,  0.5f,        1.0f, 0.0f, // bottom right
         -0.5f, -0.5f, -0.5f,        1.0f, 1.0f, // top right
         -0.5f, -0.5f, -0.5f,        1.0f, 1.0f, // top right
          0.5f, -0.5f, -0.5f,        0.0f, 1.0f, // top left
          0.5f, -0.5f,  0.5f,        0.0f, 0.0f, // bottom left
          ///
          -0.5f,  0.5f,  0.5f,        0.0f, 0.0f, // bottom left
           0.5f,  0.5f,  0.5f,        1.0f, 0.0f, // bottom right
           0.5f,  0.5f, -0.5f,        1.0f, 1.0f, // top right
           0.5f,  0.5f, -0.5f,        1.0f, 1.0f, // top right
          -0.5f,  0.5f, -0.5f,        0.0f, 1.0f, // top left
          -0.5f,  0.5f,  0.5f,        0.0f, 0.0f, // bottom left
    };

    // Buffers -------------------------------------------------------------------------------------------------------------

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Textures ------------------------------------------------------------------------------------------------------------

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
    unsigned char* data = stbi_load("boxFrame.png", &width, &height, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gridTexture);
    baseShader.setInt("currentTexture", 0);

    glActiveTexture(GL_TEXTURE1);
    unsigned int cubeTexture;
    int width1, height1, nrChannels1;

    glGenTextures(1, &cubeTexture);
    glBindTexture(GL_TEXTURE_2D, cubeTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    unsigned char* data1 = stbi_load("boxRed.png", &width1, &height1, &nrChannels1, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width1, height1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data1);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data1);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, cubeTexture);

    // Game Setup ------------------------------------------------------------------------------------------------------------

    currentPiece = randInt(0, 9);
    for (int i = 0; i < 8; i++) activePiece[i] = shapes[currentPiece][i];
    activeOffset.y += 100;

    for (int i = 0; i < 1000; i++) cubeArray[i] = glm::vec4(-1, -1, -1, -1);
    orderArray(); // breaks collision if not called here

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    baseShader.setMat4("model", model);
    baseShader.setMat4("view", view);
    baseShader.setMat4("projection", projection);

    while (!glfwWindowShouldClose(window) && !fail) {
        
        // Input ------------------------------------------------------------------------------

        camInput(window);
        otherInput(window);

        if (!paused && startGame) {
            moveActive(window);
            rotateActive(window);
            gravity();
        }
        
        // Graphics ---------------------------------------------------------------------------------

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindVertexArray(VAO);

        // Imgui --------------------------------------------------------------------------------------------

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Menu");
        if (ImGui::Button("Start")) startGame = true;
        ImGui::Text("Score: %i", score);
        ImGui::End();

        // Draw cameras -------------------------------------------------------------------------------------------

        // camera
        view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, (float)zoomVar / 3));
        view = glm::rotate(view, glm::radians((float)20), glm::vec3(1.0, 0.0, 0.0)); // vertical x
        view = glm::rotate(view, glm::radians((float)rotationVar2), glm::vec3(0.0, 1.0, 0.0)); // horizontal y
        baseShader.setMat4("view", view);
        projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
        baseShader.setMat4("projection", projection);

        // main screen
        glViewport(0, 0, 1000, 750);
        drawStuff(baseShader, model, view, projection);

        // x view
        view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, (float)zoomVar / 2.75));
        baseShader.setMat4("view", view);
        projection = glm::perspective(glm::radians(30.0f), 800.0f / 600.0f, 0.1f, 100.0f);
        baseShader.setMat4("projection", projection);
        glViewport(1000, 0, 250, 250);
        drawStuff(baseShader, model, view, projection);

        // y view
        view = glm::rotate(view, glm::radians((float)90), glm::vec3(1.0, 0.0, 0.0));
        baseShader.setMat4("view", view);
        glViewport(1000, 250, 250, 250);
        drawStuff(baseShader, model, view, projection);

        // z view
        view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, (float)zoomVar / 2.75));
        view = glm::rotate(view, glm::radians((float)90), glm::vec3(0.0, 1.0, 0.0));
        baseShader.setMat4("view", view);
        glViewport(1000, 500, 250, 250);
        drawStuff(baseShader, model, view, projection);
        
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Shutdown --------------------------------------------------------------------------------------------------

    std::cout << "GAME OVER\n";
    std::cout << "Score: " << score << "\n";
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}


// Function Implementation ------------------------------------------------------------------------------------------------------------

void frameCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, 200, 500);
}

void imguiSetup(GLFWwindow* window, const char* glsl_version) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    ImGui::StyleColorsClassic();
}

void camInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) rotationVar2 += 1;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) rotationVar2 -= 1;

    if(zoomVar > -1.1) zoomVar = -1.1;
    if(rotationVar2 > 360) rotationVar2 -= 360;
    if(rotationVar2 < -360) rotationVar2 += 360;
}

void moveActive(GLFWwindow* window) {
    if(glfwGetTime() - lastMove < 0.15) return;

    temp = activeOffset;

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        if(spawns[currentSpawn].y != 0) {
            activeOffset.x += 10;

        }
        else {
            activeOffset.y += 10;
        }
        lastMove = glfwGetTime();
    }

    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        if(spawns[currentSpawn].y != 0) {
            activeOffset.x += -10;

        }
        else {
            activeOffset.y += -10;
        }
        lastMove = glfwGetTime();
    }

    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        if(spawns[currentSpawn].z != 0) {
            activeOffset.x += -10;

        }
        else {
            activeOffset.z += -10;
        }
        lastMove = glfwGetTime();
    }

    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        if(spawns[currentSpawn].z != 0) {
            activeOffset.x += 10;

        }
        else {
            activeOffset.z += 10;
        }
        lastMove = glfwGetTime();
    }
    if(!collisionTest()) {
        activeOffset = temp;
    }
}

bool collisionTest() {
    for(int i = 0; i < 8; i++) {
        glm::vec3 geezer = glm::vec3(activePiece[i].x + activeOffset.x, activePiece[i].y + activeOffset.y, activePiece[i].z + activeOffset.z);
        if(spawns[currentSpawn].x != 0) {
            if((geezer.y > 45 || geezer.y < -45) || (geezer.z > 45 || geezer.z < -45)) return false;
            
        }
        else if(spawns[currentSpawn].y != 0) {
            if((geezer.x > 45 || geezer.x < -45) || (geezer.z > 45 || geezer.z < -45)) return false;
        }
        else{
            if((geezer.y > 45 || geezer.y < -45) || (geezer.x > 45 || geezer.x < -45)) return false;
        }

        if(spawns[currentSpawn].x == 10 && geezer.x < -45) return false;
        else if(spawns[currentSpawn].x == -10 && geezer.x > 45) return false;
        else if(spawns[currentSpawn].y == 10 && geezer.y < -45) return false;
        else if(spawns[currentSpawn].y == -10 && geezer.y > 45) return false;
        else if(spawns[currentSpawn].z == 10 && geezer.z < -45) return false;
        else if(spawns[currentSpawn].z == -10 && geezer.z > 45) return false;

        int j = 0;
        while(cubeArray[j].w != -1) {
            if(glm::vec3(cubeArray[j].x, cubeArray[j].y, cubeArray[j].z) == geezer) {
                return false;
            }
            j += 1;
        }
    }
    return true;
}

void newPieceSpawn() {
    int endInd = 0;
    while(cubeArray[endInd].w != -1) endInd += 1;
    for (int i = 0; i < 8; i++) {
        glm::vec3 geezer = glm::vec3(activePiece[i].x + activeOffset.x, activePiece[i].y + activeOffset.y, activePiece[i].z + activeOffset.z);
        if ((geezer.x > 45 || geezer.x < -45) || ((geezer.y > 45 || geezer.y < -45) || (geezer.z > 45 || geezer.z < -45))) {
            fail = true;
            return;
        }
        cubeArray[endInd + i] = glm::vec4(geezer.x, geezer.y, geezer.z, currentPiece);
        cubesCheck.push_back(geezer);
    }
    score += 5;
    completionCheck();
    orderArray();
    currentSpawn = randInt(0, 5);
    currentPiece = randInt(0, 9);
    for(int i = 0; i < 8; i++) activePiece[i] = shapes[currentPiece][i];
    activeOffset = glm::vec3(0, 0, 0) + (spawns[currentSpawn] * glm::vec3(10));
 }

void gravity() {
    if(glfwGetTime() - lastGrav < 0.75) return;
    temp = activeOffset;
    activeOffset -= spawns[currentSpawn];
    if(!collisionTest()) {
        activeOffset = temp;
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
            cubeArray[i] = glm::vec4(-1, -1, -1, -1);
            endInd++;
        }
    }
}

void printCubeArray() {
    std::cout << "printing cubes array\n";
    for(int i = 0; i < 1000; i += 4) std::cout << "(" << cubeArray[i].x << ", " << cubeArray[i].y << ", " << cubeArray[i].z << ")" << "(" << cubeArray[i + 1].x << ", "  << cubeArray[i + 1].y << ", " << cubeArray[i + 1].z << ")"<< "(" << cubeArray[i + 2].x << ", " << cubeArray[i + 2].y << ", " << cubeArray[i + 2].z << ")" << "(" << cubeArray[i + 3].x << ", " << cubeArray[i + 3].y << ", " << cubeArray[i + 3].z << ")\n";

}

void completionCheck() {
    std::vector<glm::vec3> balls = getAxis(cubesCheck);
    cubesCheck.clear();
    std::vector<std::vector<int>> rowy = getRows(balls);

    std::vector<glm::vec3> completedCubes = {};
    for(int i = 0; i < rowy.size(); i++) {
        score += 100;
        for(int j = 0; j < rowy[i].size(); j++) {
            completedCubes.push_back(glm::vec3(cubeArray[rowy[i][j]].x, cubeArray[rowy[i][j]].y, cubeArray[rowy[i][j]].z));
            cubeArray[rowy[i][j]] = glm::vec4(-1, -1, -1, -1);
        }
    }


    for(int i = 0; i < completedCubes.size(); i++) {
        for(int j = 0; j < 1000; j++) {
            if(currentSpawn == 0) { // +y
                if((cubeArray[j].x == completedCubes[i].x && cubeArray[j].z == completedCubes[i].z) && (cubeArray[j].y > completedCubes[i].y)) {
                    cubeArray[j].y -= 10;
                    cubesCheck.push_back(glm::vec3(cubeArray[j].x, cubeArray[j].y, cubeArray[j].z));
                }
            }
            else if(currentSpawn == 1) { // -y
                if((cubeArray[j].x == completedCubes[i].x && cubeArray[j].z == completedCubes[i].z) && (cubeArray[j].y < completedCubes[i].y)) {
                    cubeArray[j].y += 10;
                    cubesCheck.push_back(glm::vec3(cubeArray[j].x, cubeArray[j].y, cubeArray[j].z));
                }
            }
            else if(currentSpawn == 2) { // +x
                if((cubeArray[j].y == completedCubes[i].y && cubeArray[j].z == completedCubes[i].z) && (cubeArray[j].x > completedCubes[i].x)) {
                    cubeArray[j].x -= 10;
                    cubesCheck.push_back(glm::vec3(cubeArray[j].x, cubeArray[j].y, cubeArray[j].z));
                }
            }
            else if(currentSpawn == 3) { // -x
                if((cubeArray[j].y == completedCubes[i].y && cubeArray[j].z == completedCubes[i].z) && (cubeArray[j].x < completedCubes[i].x)) {
                    cubeArray[j].x += 10;
                    cubesCheck.push_back(glm::vec3(cubeArray[j].x, cubeArray[j].y, cubeArray[j].z));
                }
            }
            else if(currentSpawn == 4) { // +z
                if((cubeArray[j].y == completedCubes[i].y && cubeArray[j].x == completedCubes[i].x) && (cubeArray[j].z > completedCubes[i].z)) {
                    cubeArray[j].z -= 10;
                    cubesCheck.push_back(glm::vec3(cubeArray[j].x, cubeArray[j].y, cubeArray[j].z));
                }
            }
            else { // -z
                if((cubeArray[j].y == completedCubes[i].y && cubeArray[j].x == completedCubes[i].x) && (cubeArray[j].z < completedCubes[i].z)) {
                    cubeArray[j].z += 10;
                    cubesCheck.push_back(glm::vec3(cubeArray[j].x, cubeArray[j].y, cubeArray[j].z));
                }
            }
        }
    }
    if(cubesCheck.size() > 0) completionCheck();
}

std::vector<glm::vec3> getAxis(std::vector<glm::vec3> cubeList) {
    std::vector<glm::vec3> axisList = {};
    for(int i = 0; i < cubeList.size(); i++) {
        std::vector<glm::vec3> cubeAxis = {glm::vec3(cubeList[i].x, cubeList[i].y, 1000), // xy
                                           glm::vec3(cubeList[i].x, 1000, cubeList[i].z), // xz
                                           glm::vec3(1000, cubeList[i].y, cubeList[i].z)}; // yz

        for(int j = 0; j < axisList.size(); j++) {
            for(int k = 0; k < cubeAxis.size(); k++) {
                if(axisList[j] == cubeAxis[k]) {
                    cubeAxis.erase(cubeAxis.begin() + k);
                    break;
                }
            }
        }

        for(int j = 0; j < cubeAxis.size(); j++) axisList.push_back(cubeAxis[j]);
    }

    return axisList;
}

std::vector<std::vector<int>> getRows(std::vector<glm::vec3> axis) {
    std::vector<std::vector<int>> rows = {};

    for(int i = 0; i < axis.size(); i++) {
        std::vector<int> oneRow = {};
        if(axis[i].x == 1000) {
            for(int j = 0; j < 1000; j++) {
                if(cubeArray[j].y == axis[i].y && cubeArray[j].z == axis[i].z) {
                    oneRow.push_back(j);
                }
            }
        }
        else if(axis[i].y == 1000) {
            for(int j = 0; j < 1000; j++) {
                if(cubeArray[j].x == axis[i].x && cubeArray[j].z == axis[i].z) {
                    oneRow.push_back(j);
                }
            }
        }
        else {
            for(int j = 0; j < 1000; j++) {
                if(cubeArray[j].x == axis[i].x && cubeArray[j].y == axis[i].y) {
                    oneRow.push_back(j);
                }
            }
        }

        if(oneRow.size() == 10) rows.push_back(oneRow);
    }
    return rows;
}

void rotateActive(GLFWwindow* window) {
    if(glfwGetTime() - lastRotate < 0.15) return;

    temp = activeOffset;
    glm::vec3 eeb[8];
    for (int i = 0; i < 8; i++) {
        eeb[i] = activePiece[i];
    }
    int rCos = glm::cos(glm::radians((float)90));
    int rSin = glm::sin(glm::radians((float)90));
    glm::vec3 five = glm::vec3(5);

    glm::mat3 xRot = glm::mat3(1, 0, 0,
        0, rCos, -rSin,
        0, rSin, rCos);
    glm::mat3 yRot = glm::mat3(rCos, 0, rSin,
        0, 1, 0,
        -rSin, 0, rCos);
    glm::mat3 zRot = glm::mat3(rCos, rSin, 0,
        -rSin, rCos, 0,
        0, 0, 1);

    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
        for (int i = 0; i < 8; i++) {
            activePiece[i] = (xRot * (activePiece[i]-five) + five);
        }
        lastRotate = glfwGetTime();
    }
    else if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
        for (int i = 0; i < 8; i++) {
            activePiece[i] = (yRot * (activePiece[i] - five) + five);
        }
        lastRotate = glfwGetTime();
    }
    else if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        for (int i = 0; i < 8; i++) {
            activePiece[i] = (zRot * (activePiece[i] - five) + five);
        }
        lastRotate = glfwGetTime();
    }

    if (!collisionTest()) {
        activeOffset = temp;
        for (int i = 0; i < 8; i++) {
            activePiece[i] = eeb[i];
        }
    }
}

int randInt(int low, int high) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(low, high); // define the range
    return distr(gen);
}

void otherInput(GLFWwindow* window) {
    if (glfwGetTime() - lastInput < 0.1) return;
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        paused = !paused;
        lastInput = glfwGetTime();
    }
}

void drawStuff(Shader baseShader, glm::mat4 model, glm::mat4 view, glm::mat4 projection) {
    // grid
    model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(1.01, 1.01, 1.01));
    baseShader.setMat4("model", model);
    baseShader.setInt("currentTexture", 0);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // shape
    baseShader.setInt("currentTexture", 1);
    glm::vec3 colory = colors[currentPiece];
    for (int i = 0; i < 8; i++) {
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(((float)activePiece[i].x + activeOffset.x) / 100, ((float)activePiece[i].y + activeOffset.y) / 100, ((float)activePiece[i].z + activeOffset.z) / 100));
        model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));
        baseShader.setMat4("model", model);
        baseShader.setVec3("boxColor", colory.x, colory.y, colory.z);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // cube array
    for (int i = 0; i < 1000; i++) {
        if (cubeArray[i].w != -1) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3((float)cubeArray[i].x / 100, (float)cubeArray[i].y / 100, (float)cubeArray[i].z / 100));
            model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));
            baseShader.setMat4("model", model);
            baseShader.setVec3("boxColor", colors[(int)cubeArray[i].w].x, colors[(int)cubeArray[i].w].y, colors[(int)cubeArray[i].w].z);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
    }
}