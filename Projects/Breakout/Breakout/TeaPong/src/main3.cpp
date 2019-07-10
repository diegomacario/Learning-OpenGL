#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "camera.h"
#include "resource_manager.h"
#include "shader_loader.h"
#include "model_loader.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

Camera camera(glm::vec3(0.0f, 0.0f, 1.0f), // Position
              glm::vec3(0.0f, 1.0f, 0.0f), // World up
              0.0f,                        // Yaw
              0.0f,                        // Pitch
              45.0f,                       // Fovy
              2.5f,                        // Movement speed
              0.1f);                       // Mouse sensitivity

float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool  firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

glm::vec3 lightPos = glm::vec3(-175.0f, 175.0f, 175.0f);

int main()
{
    // Initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create GLFW window
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Load all the OpenGL function pointers using GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Configure the global OpenGL state
    glEnable(GL_DEPTH_TEST);

    // Load the shaders
    ResourceManager<Shader> shaderManager;
    shaderManager.loadResource<ShaderLoader>("model", "shaders/16.2.model_shader.vs", "shaders/16.2.model_shader.fs");
    shaderManager.loadResource<ShaderLoader>("lamp", "shaders/6.2.lamp.vs", "shaders/6.2.lamp.fs");
    shaderManager.loadResource<ShaderLoader>("basic", "shaders/16.1.basic_shader.vs", "shaders/16.1.basic_shader.fs");
    auto modelShader = shaderManager.getResource("model");
    auto lampShader  = shaderManager.getResource("lamp");
    auto basicShader = shaderManager.getResource("basic");

    // Load the model
    ResourceManager<Model> modelManager;
    modelManager.loadResource<ModelLoader>("teapot", "objects/teapot/high_poly_with_mat/Teapot.obj");
    auto teapotModel = modelManager.getResource("teapot");

   // Create the lamp
   //                         Positions            Normals              Texture coords
   //                        <--------------->    <--------------->    <------->
   float lampVertices[] = { -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
                             0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
                             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
                             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
                            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
                            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

                            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
                             0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
                             0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
                             0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
                            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
                            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

                            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
                            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
                            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
                            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
                            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
                            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
                            
                             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
                             0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
                             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
                             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
                             0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
                             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
                            
                            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
                             0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
                             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
                             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
                            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
                            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
                            
                            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
                             0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
                             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
                             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
                            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
                            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f };

    unsigned int lampVAO, lampVBO;
    glGenVertexArrays(1, &lampVAO);
    glGenBuffers(1, &lampVBO);
    glBindVertexArray(lampVAO);
    glBindBuffer(GL_ARRAY_BUFFER, lampVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lampVertices), lampVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

    // Create the background plane
    GLfloat basicVertices[] = { // Pos            // Col
                                0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                                1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
                                0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,

                                0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                                1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
                                1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f };

    unsigned int basicVAO, basicVBO;
    glGenVertexArrays(1, &basicVAO);
    glGenBuffers(1, &basicVBO);
    glBindVertexArray(basicVAO);
    glBindBuffer(GL_ARRAY_BUFFER, basicVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(basicVertices), basicVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    // In perspective, the lower-left corner is the origin
    // In orthographic, the upper-left corner is the origin

    // Create the test plane
    GLfloat testVertices[] =  { // Pos            // Col
                                0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,   // Blue   - Upper left
                                1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,   // Green  - Lower right
                                0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,   // Red    - Lower left

                                0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // Cyan   - Upper left
                                1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,   // Yellow - Upper right
                                1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f }; // White  - Lower right

    unsigned int testVAO, testVBO;
    glGenVertexArrays(1, &testVAO);
    glGenBuffers(1, &testVBO);
    glBindVertexArray(testVAO);
    glBindBuffer(GL_ARRAY_BUFFER, testVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(testVertices), testVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    // Execute the render loop
    while (!glfwWindowShouldClose(window))
    {
        // Per-frame time logic
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Rotate the light source
        /*
        glm::vec3 rotationAxis      = glm::vec3(0.0f, 0.0f, 1.0f);
        glm::mat4 rotMatrixForLight = glm::rotate(glm::mat4(), (float) glfwGetTime(), rotationAxis);
        glm::vec3 rotatedLightPos   = ((glm::mat3) (rotMatrixForLight)) * lightPos;
        rotatedLightPos = rotatedLightPos + glm::vec3(1280.0f / 2.0f, 720.0f / 2, 0.0f);
        */

        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 projection;

        // Draw the background using an ortographic projection
        // ---------------------------------------------------

        model      = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, -165.0f));
        model      = glm::scale(model, glm::vec3(1280.0f, 720.0f, 1.0f));
        view       = glm::mat4();
        projection = glm::ortho(0.0f, 1280.0f, 720.0f, 0.0f, -200.0f, 200.0f);

        basicShader->use();
        basicShader->setMat4("model", model);
        basicShader->setMat4("view", view);
        basicShader->setMat4("projection", projection);

        glBindVertexArray(basicVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Draw a quarter of the background using a perspective projection
        // ---------------------------------------------------------------

        model      = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, 0.0f));
        model      = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
        view       = camera.getViewMatrix();
        projection = glm::perspective(glm::radians(camera.getFieldOfViewY()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        basicShader->use();
        basicShader->setMat4("model", model);
        basicShader->setMat4("view", view);
        basicShader->setMat4("projection", projection);

        glBindVertexArray(testVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Draw the model using a perspective projection
        // ---------------------------------------------------

        /*
        glm::mat4 rotMatrixForTeapot = glm::rotate(glm::mat4(), (float) glfwGetTime(), glm::vec3(1.0f, 1.0f, 1.0f));

        // Model dimensions: (24.12f, 11.81f, 15.0f)
        model      = glm::rotate(rotMatrixForTeapot, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model      = glm::scale(model, glm::vec3(0.01f));
        view       = camera.getViewMatrix();
        projection = glm::perspective(glm::radians(camera.getFieldOfViewY()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        modelShader->use();
        modelShader->setMat4("model", model);
        modelShader->setMat4("view", view);
        modelShader->setMat4("projection", projection);

        // Camera properties
        modelShader->setVec3("viewPos", camera.getPosition());

        // Light properties
        modelShader->setVec3("pointLight.position", rotatedLightPos);
        modelShader->setVec3("pointLight.color", 1.0f, 1.0f, 1.0f);
        modelShader->setFloat("pointLight.constant", 1.0f);
        modelShader->setFloat("pointLight.linear", 0.09f);
        modelShader->setFloat("pointLight.quadratic", 0.032f);

        teapotModel->render(*modelShader);
        */

        // Draw the lamp using an ortographic project
        // ---------------------------------------------------

        /*
        model      = glm::translate(glm::mat4(), rotatedLightPos);
        model      = glm::scale(model, glm::vec3(20.0f));
        view       = glm::mat4();
        projection = glm::ortho(0.0f, 1280.0f, 720.0f, 0.0f, -200.0f, 200.0f);

        lampShader->use();
        lampShader->setMat4("model", model);
        lampShader->setMat4("view", view);
        lampShader->setMat4("projection", projection);

        glBindVertexArray(lampVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        */

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &lampVAO);
    glDeleteBuffers(1, &lampVBO);

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.processKeyboard(MovementDirection::Forward, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.processKeyboard(MovementDirection::Backward, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.processKeyboard(MovementDirection::Left, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.processKeyboard(MovementDirection::Right, deltaTime);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // Make sure the viewport matches the new window dimensions
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // Reversed since Y-coordinates go from the bottom to the top

    lastX = xpos;
    lastY = ypos;

    camera.processMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.processMouseScroll(yoffset);
}
