#include <iostream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"

#include "Shader.h"
#include "Camera.hpp"
#define WINDOW_HEIGHT 600
#define WINDOW_WIDTH 800


//Essential Callbacks
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

//Input Processing func
void processInput(GLFWwindow *window);


//Camera Setup
Camera g_camera;

//Time tracking
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

//Mouse Control
float mouseLastX = float(WINDOW_WIDTH) / 2.f, mouseLastY = float(WINDOW_HEIGHT) / 2.f;
bool firstMouse = true;

//Light Settings
glm::vec3 lightPos(1.5f, 1.f, 0.5f);


int main()
{

    glfwInit();
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    Shader lightingShader("shaders\\LightingShader.vs", "shaders\\LightingShader.fs");

    //Vertex Data
	float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

    unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
    };

    ////GL Texture generation
    //unsigned int texture1;
    //glGenTextures(1, &texture1);
    //glBindTexture(GL_TEXTURE_2D, texture1);
    //// set the texture wrapping/filtering options (on the currently bound texture object)
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    ////Load Image 1
    //int width, height, nrChannels;
    //unsigned char *data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);

    ////Load image to OpenGL texture
    //if (data)
    //{
    //    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    //    glGenerateMipmap(GL_TEXTURE_2D);
    //}
    //else
    //{
    //    std::cout << "Failed to load texture" << std::endl;
    //}
    //stbi_image_free(data);

    ////GL Texture generation
    //unsigned int texture2;
    //glGenTextures(1, &texture2);
    //glBindTexture(GL_TEXTURE_2D, texture2);
    //// set the texture wrapping/filtering options (on the currently bound texture object)
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    ////Load Image 2
    //data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);

    ////Load image to OpenGL texture
    //if (data)
    //{
    //    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    //    glGenerateMipmap(GL_TEXTURE_2D);
    //}
    //else
    //{
    //    std::cout << "Failed to load texture" << std::endl;
    //}
    //stbi_image_free(data);

    //Create a Vertex Attribute Array so it can easily be bound for defined Vertex attributes	
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);   //All the VBOs and VA pointers bound later are bound to the currently boud VAO


    //Vertex Data to GPU
    unsigned int VBO;
    glGenBuffers(1, &VBO);  //Create Buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);    //Bind Buffer as an Array Buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);  //Send data to GPU

    //Index Data to GPU
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //Linking Vertex Attributes
    //Specifiy format of vertex data etc. location in GLSL is the first attribute here. VA pointer is defined for the currently bound VBO
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //Setup Light Shader
    Shader lampShader("shaders\\LampShader.vs", "shaders\\LampShader.fs");
    //Vertex Attribute Array for light
    unsigned int lampVAO;
    glGenVertexArrays(1, &lampVAO);
    glBindVertexArray(lampVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //

    //MVP matrices
    //ModelMatrix
    glm::mat4 model = glm::mat4(1.0f);

    //View Matrix
    glm::mat4 view = glm::mat4(1.0f);
    // note that we're translating the scene in the reverse direction of where we want to move
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -4.0f));

    //Projection Matrix
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), float(WINDOW_WIDTH) / float(WINDOW_HEIGHT), 0.1f, 100.0f);

    //Camera setup			



    glEnable(GL_DEPTH_TEST);
    //Render Loop
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Time tracking
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        lightingShader.use();

        processInput(window);        

        // create transformations
        model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        view = glm::mat4(1.0f);
        projection = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.f, -1.f, -1.f));

        projection = glm::perspective(glm::radians(g_camera.m_fov), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
        
        view = glm::lookAt(g_camera.m_pos, g_camera.m_pos + g_camera.m_front, g_camera.m_up);
        // retrieve the matrix uniform locations
        lightingShader.use();        
        // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        lightingShader.setMat4("model", model);
        lightingShader.setMat4("view", view);
        lightingShader.setMat4("projection", projection);
        
        lightingShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
		lightingShader.setVec3("lightPos", lightPos);
        glBindVertexArray(VAO);

        //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //Render Light
        model = glm::mat4(1.f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));

        lampShader.use();
        lampShader.setMat4("model", model);
        lampShader.setMat4("view", view);
        lampShader.setMat4("projection", projection);

        glBindVertexArray(lampVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}



void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse) // this bool variable is initially set to true
    {
        mouseLastX = xpos;
        mouseLastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - mouseLastX;
    float yoffset = mouseLastY - ypos; // reversed since y-coordinates range from bottom to top
    mouseLastX = xpos;
    mouseLastY = ypos;

    float sensitivity = 0.05f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    g_camera.rotate(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    g_camera.zoom(xoffset, yoffset);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = 2.5f  *deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        g_camera.moveForward(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        g_camera.moveBackward(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        g_camera.moveLeft(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        g_camera.moveRight(deltaTime);
}