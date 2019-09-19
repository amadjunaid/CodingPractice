#include <iostream>
#include <string>

#include <glad/glad.h>
#include <GLFW\glfw3.h>

#define WINDOW_HEIGHT 800
#define WINDOW_WIDTH 600


//Essential Callbacks
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

//Input Processing func
void processInput(GLFWwindow *window);

//Basic Shaders
const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout(location = 1) in vec3 aColor;\n"

"out vec3 ourColor; // output a color to the fragment shader\n"

"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"	ourColor = aColor;"
"}\0";

const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(ourColor,1.f);\n"
"}\n\0";

int main()
{

	glfwInit();
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(WINDOW_HEIGHT, WINDOW_WIDTH, "LearnOpenGL", NULL, NULL);
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

	glViewport(0, 0, 800, 600);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//Shader setup		

	//Vertex Shader
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);  //Create Shader as a vertex Shader
	glShaderSource(vertexShader, 1,&vertexShaderSource, NULL);  //load the source code to shader object
	glCompileShader(vertexShader); //compile shader
	
	//Check if shader compilation was successfull
	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	
	//Fragment Shader
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	//Check if shader compilation was successfull
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::FRAGMENT::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//Create and link the shader program
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
		
	//Delete the Shaders after linking
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	//Vertex Data
	float vertices[] = {
		// positions         // colors
	 0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,// top right
	 0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,// bottom right
	-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,// bottom left
	-0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 1.0f// top left 
	};
	unsigned int indices[] = {  // note that we start from 0!
	0, 1, 3,   // first triangle
	1, 2, 3    // second triangle
	};	

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

	//Use required shader
	glUseProgram(shaderProgram);

	//Render Loop
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);		

		glUseProgram(shaderProgram);		

		glBindVertexArray(VAO);
		
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,0);

		processInput(window);

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

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}