//(N.d.).Retrieved from https ://learnopengl.com/Model-Loading/Mesh  
//Code modified from: Loay , A. (2021). Retrieved from https://www.youtube.com/watch?v=xcVYKalUQHk 
//This program successfully uses the WASD keys to zoom in/out and rotate around the 3D pyramid
// The Q and E keys are used to move the object along the y axis
// The mouse is used to change the camera view
// P key is used to toggle Ortho view



#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Sphere.h"

#include "shader.h"

#include <iostream>

#include "cylinder.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//lighting
//float xlight = 0.2f, ylight = 4.0f, zlight = 2.0f;
//glm::vec3 lightPos(0.0f, 0.5f, 0.0f);


// camera
glm::vec3 cameraPos = glm::vec3(0.0f, 4.0f, 10.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouse = true;
float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;
float MovementSpeed = 5.0f;


//Global Variable
bool ortho = false; // Sets ortho projection

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Kyle Conley - M7 Project", NULL, NULL);
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

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile our shader zprogram
	// ------------------------------------
	Shader ourShader("shaderfiles/7.3.camera.vs", "shaderfiles/7.3.camera.fs");
	//Shader lightingShader("shaderfiles/2.2.basic_lighting.vs", "shaderfiles/2.2.basic_lighting.fs");
	//Shader lightCubeShader("shaderfiles/2.2.light_cube.vs", "shaderfiles/2.2.light_cube.fs");

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	// Tray
	float vertices[] = {
		// Positions          // Normals           // Texture Coords
		-3.5f, -0.1f, -2.5f,  0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
		 3.5f, -0.1f, -2.5f,  0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
		 3.5f,  0.1f, -2.5f,  0.0f, 1.0f, 0.0f,   1.0f, 1.0f,
		 3.5f,  0.1f, -2.5f,  0.0f, 1.0f, 0.0f,   1.0f, 1.0f,
		-3.5f,  0.1f, -2.5f,  0.0f, 1.0f, 0.0f,   0.0f, 1.0f,
		-3.5f, -0.1f, -2.5f,  0.0f, 1.0f, 0.0f,   0.0f, 0.0f,

		-3.5f, -0.1f,  2.5f,  0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
		 3.5f, -0.1f,  2.5f,  0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
		 3.5f,  0.1f,  2.5f,  0.0f, 1.0f, 0.0f,   1.0f, 1.0f,
		 3.5f,  0.1f,  2.5f,  0.0f, 1.0f, 0.0f,   1.0f, 1.0f,
		-3.5f,  0.1f,  2.5f,  0.0f, 1.0f, 0.0f,   0.0f, 1.0f,
		-3.5f, -0.1f,  2.5f,  0.0f, 1.0f, 0.0f,   0.0f, 0.0f,

		-3.5f,  0.1f,  2.5f, -1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
		-3.5f,  0.1f, -2.5f, -1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
		-3.5f, -0.1f, -2.5f, -1.0f, 0.0f, 0.0f,   0.0f, 1.0f,
		-3.5f, -0.1f, -2.5f, -1.0f, 0.0f, 0.0f,   0.0f, 1.0f,
		-3.5f, -0.1f,  2.5f, -1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
		-3.5f,  0.1f,  2.5f, -1.0f, 0.0f, 0.0f,   1.0f, 0.0f,

		 3.5f,  0.1f,  2.5f,  1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
		 3.5f,  0.1f, -2.5f,  1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
		 3.5f, -0.1f, -2.5f,  1.0f, 0.0f, 0.0f,   0.0f, 1.0f,
		 3.5f, -0.1f, -2.5f,  1.0f, 0.0f, 0.0f,   0.0f, 1.0f,
		 3.5f, -0.1f,  2.5f,  1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
		 3.5f,  0.1f,  2.5f,  1.0f, 0.0f, 0.0f,   1.0f, 0.0f,

		-3.5f, -0.1f, -2.5f,  0.0f, -1.0f, 0.0f,   0.0f, 1.0f,
		 3.5f, -0.1f, -2.5f,  0.0f, -1.0f, 0.0f,   1.0f, 1.0f,
		 3.5f, -0.1f,  2.5f,  0.0f, -1.0f, 0.0f,   1.0f, 0.0f,
		 3.5f, -0.1f,  2.5f,  0.0f, -1.0f, 0.0f,   1.0f, 0.0f,
		-3.5f, -0.1f,  2.5f,  0.0f, -1.0f, 0.0f,   0.0f, 0.0f,
		-3.5f, -0.1f, -2.5f,  0.0f, -1.0f, 0.0f,   0.0f, 1.0f,

		-3.5f,  0.1f, -2.5f,  0.0f, 1.0f, 0.0f,   0.0f, 1.0f,
		 3.5f,  0.1f, -2.5f,  0.0f, 1.0f, 0.0f,   1.0f, 1.0f,
		 3.5f,  0.1f,  2.5f,  0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
		 3.5f,  0.1f,  2.5f,  0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
		-3.5f,  0.1f,  2.5f,  0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
		-3.5f,  0.1f, -2.5f,  0.0f, 1.0f, 0.0f,   0.0f, 1.0f
	};
	//plane
	float vertices2[] = {
		-6.5f, -0.0f, -6.5f,  0.0f, 0.0f,
		 6.5f, -0.0f, -6.5f,  1.0f, 0.0f,
		 6.5f,  0.0f, -6.5f,  1.0f, 1.0f,
		 6.5f,  0.0f, -6.5f,  1.0f, 1.0f,
		-6.5f,  0.0f, -6.5f,  0.0f, 1.0f,
		-6.5f, -0.0f, -6.5f,  0.0f, 0.0f,

		-6.5f, -0.0f,  6.5f,  0.0f, 0.0f,
		 6.5f, -0.0f,  6.5f,  1.0f, 0.0f,
		 6.5f,  0.0f,  6.5f,  1.0f, 1.0f,
		 6.5f,  0.0f,  6.5f,  1.0f, 1.0f,
		-6.5f,  0.0f,  6.5f,  0.0f, 1.0f,
		-6.5f, -0.0f,  6.5f,  0.0f, 0.0f,

		-6.5f,  0.0f,  6.5f,  1.0f, 0.0f,
		-6.5f,  0.0f, -6.5f,  1.0f, 1.0f,
		-6.5f, -0.0f, -6.5f,  0.0f, 1.0f,
		-6.5f, -0.0f, -6.5f,  0.0f, 1.0f,
		-6.5f, -0.0f,  6.5f,  0.0f, 0.0f,
		-6.5f,  0.0f,  6.5f,  1.0f, 0.0f,

		 6.5f,  0.0f,  6.5f,  1.0f, 0.0f,
		 6.5f,  0.0f, -6.5f,  1.0f, 1.0f,
		 6.5f, -0.0f, -6.5f,  0.0f, 1.0f,
		 6.5f, -0.0f, -6.5f,  0.0f, 1.0f,
		 6.5f, -0.0f,  6.5f,  0.0f, 0.0f,
		 6.5f,  0.0f,  6.5f,  1.0f, 0.0f,

		-6.5f, -0.0f, -6.5f,  0.0f, 1.0f,
		 6.5f, -0.0f, -6.5f,  1.0f, 1.0f,
		 6.5f, -0.0f,  6.5f,  1.0f, 0.0f,
		 6.5f, -0.0f,  6.5f,  1.0f, 0.0f,
		-6.5f, -0.0f,  6.5f,  0.0f, 0.0f,
		-6.5f, -0.0f, -6.5f,  0.0f, 1.0f,

		-6.5f,  0.0f, -6.5f,  0.0f, 1.0f,
		 6.5f,  0.0f, -6.5f,  1.0f, 1.0f,
		 6.5f,  0.0f,  6.5f,  1.0f, 0.0f,
		 6.5f,  0.0f,  6.5f,  1.0f, 0.0f,
		-6.5f,  0.0f,  6.5f,  0.0f, 0.0f,
		-6.5f,  0.0f, -6.5f,  0.0f, 1.0f
	};
	//Remote
	float vertices3[] = {
		-0.3f, 0.2f, -0.3125f,  0.0f, 0.0f,
		 0.3f, 0.2f, -0.3125f,  1.0f, 0.0f,
		 0.3f, 0.4f, -0.3125f,  1.0f, 1.0f,
		 0.3f, 0.4f, -0.3125f,  1.0f, 1.0f,
		-0.3f, 0.4f, -0.3125f,  0.0f, 1.0f,
		-0.3f, 0.2f, -0.3125f,  0.0f, 0.0f,

		-0.3f, 0.2f,  1.6125f,  0.0f, 0.0f,
		 0.3f, 0.2f,  1.6125f,  1.0f, 0.0f,
		 0.3f, 0.4f,  1.6125f,  1.0f, 1.0f,
		 0.3f, 0.4f,  1.6125f,  1.0f, 1.0f,
		-0.3f, 0.4f,  1.6125f,  0.0f, 1.0f,
		-0.3f, 0.2f,  1.6125f,  0.0f, 0.0f,

		-0.3f, 0.4f,  1.6125f,  1.0f, 0.0f,
		-0.3f, 0.4f, -0.3125f,  1.0f, 1.0f,
		-0.3f, 0.2f, -0.3125f,  0.0f, 1.0f,
		-0.3f, 0.2f, -0.3125f,  0.0f, 1.0f,
		-0.3f, 0.2f,  1.6125f,  0.0f, 0.0f,
		-0.3f, 0.4f,  1.6125f,  1.0f, 0.0f,

		 0.3f, 0.4f,  1.6125f,  1.0f, 0.0f,
		 0.3f, 0.4f, -0.3125f,  1.0f, 1.0f,
		 0.3f, 0.2f, -0.3125f,  0.0f, 1.0f,
		 0.3f, 0.2f, -0.3125f,  0.0f, 1.0f,
		 0.3f, 0.2f,  1.6125f,  0.0f, 0.0f,
		 0.3f, 0.4f,  1.6125f,  1.0f, 0.0f,

		-0.3f, 0.2f, -0.3125f,  0.0f, 1.0f,
		 0.3f, 0.2f, -0.3125f,  1.0f, 1.0f,
		 0.3f, 0.2f,  1.6125f,  1.0f, 0.0f,
		 0.3f, 0.2f,  1.6125f,  1.0f, 0.0f,
		-0.3f, 0.2f,  1.6125f,  0.0f, 0.0f,
		-0.3f, 0.2f, -0.3125f,  0.0f, 1.0f,

		-0.3f,  0.4f, -0.3125f,  0.0f, 1.0f,
		 0.3f,  0.4f, -0.3125f,  1.0f, 1.0f,
		 0.3f,  0.4f,  1.6125f,  1.0f, 0.0f,
		 0.3f,  0.4f,  1.6125f,  1.0f, 0.0f,
		-0.3f,  0.4f,  1.6125f,  0.0f, 0.0f,
		-0.3f,  0.4f, -0.3125f,  0.0f, 1.0f
	};
	//PEN TIP - PYRAMID
	float vertices4[] = {
		// Pyramid
		0.0f,  0.9f, 0.0f,  0.5f, 1.0f,
	   -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, 0.5f,  1.0f, 0.0f,

		0.0f,  0.9f, 0.0f,  0.5f, 1.0f,
		0.5f, -0.5f, 0.5f,  0.0f, 0.0f,
	   0.5f, -0.5f, -0.5f,  1.0f, 0.0f,

		0.0f,  0.9f, 0.0f,  0.5f, 1.0f,
	   0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	  -0.5f, -0.5f, -0.5f,  1.0f, 0.0f,

		0.0f,  0.9f, 0.0f,  0.5f, 1.0f,
	  -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	   -0.5f, -0.5f, 0.5f,  1.0f, 0.0f,

	   // Added Base
	  -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	   0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f, -0.5f, 0.5f,  1.0f, 1.0f,

	  -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, 0.5f,  1.0f, 1.0f,
	   -0.5f, -0.5f, 0.5f,  0.0f, 1.0f,
	};

	//Light Cube
	float verticesLight[] = {
		-0.5f, 0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

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
	//Bottom Notebook
	float verticesNotebook[] = {
			-0.8f, 0.2f, -0.5125f,  0.0f, 0.0f,
			 0.8f, 0.2f, -0.5125f,  1.0f, 0.0f,
			 0.8f, 0.4f, -0.5125f,  1.0f, 1.0f,
			 0.8f, 0.4f, -0.5125f,  1.0f, 1.0f,
			-0.8f, 0.4f, -0.5125f,  0.0f, 1.0f,
			-0.8f, 0.2f, -0.5125f,  0.0f, 0.0f,

			-0.8f, 0.2f,  1.9125f,  0.0f, 0.0f,
			 0.8f, 0.2f,  1.9125f,  1.0f, 0.0f,
			 0.8f, 0.4f,  1.9125f,  1.0f, 1.0f,
			 0.8f, 0.4f,  1.9125f,  1.0f, 1.0f,
			-0.8f, 0.4f,  1.9125f,  0.0f, 1.0f,
			-0.8f, 0.2f,  1.9125f,  0.0f, 0.0f,

			-0.8f, 0.4f,  1.9125f,  1.0f, 0.0f,
			-0.8f, 0.4f, -0.5125f,  1.0f, 1.0f,
			-0.8f, 0.2f, -0.5125f,  0.0f, 1.0f,
			-0.8f, 0.2f, -0.5125f,  0.0f, 1.0f,
			-0.8f, 0.2f,  1.9125f,  0.0f, 0.0f,
			-0.8f, 0.4f,  1.9125f,  1.0f, 0.0f,

			 0.8f, 0.4f,  1.9125f,  1.0f, 0.0f,
			 0.8f, 0.4f, -0.5125f,  1.0f, 1.0f,
			 0.8f, 0.2f, -0.5125f,  0.0f, 1.0f,
			 0.8f, 0.2f, -0.5125f,  0.0f, 1.0f,
			 0.8f, 0.2f,  1.9125f,  0.0f, 0.0f,
			 0.8f, 0.4f,  1.9125f,  1.0f, 0.0f,

			-0.8f, 0.2f, -0.5125f,  0.0f, 1.0f,
			 0.8f, 0.2f, -0.5125f,  1.0f, 1.0f,
			 0.8f, 0.2f,  1.9125f,  1.0f, 0.0f,
			 0.8f, 0.2f,  1.9125f,  1.0f, 0.0f,
			-0.8f, 0.2f,  1.9125f,  0.0f, 0.0f,
			-0.8f, 0.2f, -0.5125f,  0.0f, 1.0f,

			-0.8f,  0.4f, -0.5125f,  0.0f, 1.0f,
			 0.8f,  0.4f, -0.5125f,  1.0f, 1.0f,
			 0.8f,  0.4f,  1.9125f,  1.0f, 0.0f,
			 0.8f,  0.4f,  1.9125f,  1.0f, 0.0f,
			-0.8f,  0.4f,  1.9125f,  0.0f, 0.0f,
			-0.8f,  0.4f, -0.5125f,  0.0f, 1.0f
	};

	//Top Notebook
	float verticesNotebookTop[] = {
			-0.8f, 0.2f, -0.5125f,  0.0f, 0.0f,
			 0.8f, 0.2f, -0.5125f,  1.0f, 0.0f,
			 0.8f, 0.4f, -0.5125f,  1.0f, 1.0f,
			 0.8f, 0.4f, -0.5125f,  1.0f, 1.0f,
			-0.8f, 0.4f, -0.5125f,  0.0f, 1.0f,
			-0.8f, 0.2f, -0.5125f,  0.0f, 0.0f,

			-0.8f, 0.2f,  1.9125f,  0.0f, 0.0f,
			 0.8f, 0.2f,  1.9125f,  1.0f, 0.0f,
			 0.8f, 0.4f,  1.9125f,  1.0f, 1.0f,
			 0.8f, 0.4f,  1.9125f,  1.0f, 1.0f,
			-0.8f, 0.4f,  1.9125f,  0.0f, 1.0f,
			-0.8f, 0.2f,  1.9125f,  0.0f, 0.0f,

			-0.8f, 0.4f,  1.9125f,  1.0f, 0.0f,
			-0.8f, 0.4f, -0.5125f,  1.0f, 1.0f,
			-0.8f, 0.2f, -0.5125f,  0.0f, 1.0f,
			-0.8f, 0.2f, -0.5125f,  0.0f, 1.0f,
			-0.8f, 0.2f,  1.9125f,  0.0f, 0.0f,
			-0.8f, 0.4f,  1.9125f,  1.0f, 0.0f,

			 0.8f, 0.4f,  1.9125f,  1.0f, 0.0f,
			 0.8f, 0.4f, -0.5125f,  1.0f, 1.0f,
			 0.8f, 0.2f, -0.5125f,  0.0f, 1.0f,
			 0.8f, 0.2f, -0.5125f,  0.0f, 1.0f,
			 0.8f, 0.2f,  1.9125f,  0.0f, 0.0f,
			 0.8f, 0.4f,  1.9125f,  1.0f, 0.0f,

			-0.8f, 0.2f, -0.5125f,  0.0f, 1.0f,
			 0.8f, 0.2f, -0.5125f,  1.0f, 1.0f,
			 0.8f, 0.2f,  1.9125f,  1.0f, 0.0f,
			 0.8f, 0.2f,  1.9125f,  1.0f, 0.0f,
			-0.8f, 0.2f,  1.9125f,  0.0f, 0.0f,
			-0.8f, 0.2f, -0.5125f,  0.0f, 1.0f,

			-0.8f,  0.4f, -0.5125f,  0.0f, 1.0f,
			 0.8f,  0.4f, -0.5125f,  1.0f, 1.0f,
			 0.8f,  0.4f,  1.9125f,  1.0f, 0.0f,
			 0.8f,  0.4f,  1.9125f,  1.0f, 0.0f,
			-0.8f,  0.4f,  1.9125f,  0.0f, 0.0f,
			-0.8f,  0.4f, -0.5125f,  0.0f, 1.0f
	};

	//Book pages
	float verticesPages[] = {
			-0.7f, 0.2f, -0.5125f,  0.0f, 0.0f,
			 0.7f, 0.2f, -0.5125f,  1.0f, 0.0f,
			 0.7f, 0.4f, -0.5125f,  1.0f, 1.0f,
			 0.7f, 0.4f, -0.5125f,  1.0f, 1.0f,
			-0.7f, 0.4f, -0.5125f,  0.0f, 1.0f,
			-0.7f, 0.2f, -0.5125f,  0.0f, 0.0f,

			-0.7f, 0.2f,  1.9125f,  0.0f, 0.0f,
			 0.7f, 0.2f,  1.9125f,  1.0f, 0.0f,
			 0.7f, 0.4f,  1.9125f,  1.0f, 1.0f,
			 0.7f, 0.4f,  1.9125f,  1.0f, 1.0f,
			-0.7f, 0.4f,  1.9125f,  0.0f, 1.0f,
			-0.7f, 0.2f,  1.9125f,  0.0f, 0.0f,

			-0.7f, 0.4f,  1.9125f,  1.0f, 0.0f,
			-0.7f, 0.4f, -0.5125f,  1.0f, 1.0f,
			-0.7f, 0.2f, -0.5125f,  0.0f, 1.0f,
			-0.7f, 0.2f, -0.5125f,  0.0f, 1.0f,
			-0.7f, 0.2f,  1.9125f,  0.0f, 0.0f,
			-0.7f, 0.4f,  1.9125f,  1.0f, 0.0f,

			 0.7f, 0.4f,  1.9125f,  1.0f, 0.0f,
			 0.7f, 0.4f, -0.5125f,  1.0f, 1.0f,
			 0.7f, 0.2f, -0.5125f,  0.0f, 1.0f,
			 0.7f, 0.2f, -0.5125f,  0.0f, 1.0f,
			 0.7f, 0.2f,  1.9125f,  0.0f, 0.0f,
			 0.7f, 0.4f,  1.9125f,  1.0f, 0.0f,

			-0.7f, 0.2f, -0.5125f,  0.0f, 1.0f,
			 0.7f, 0.2f, -0.5125f,  1.0f, 1.0f,
			 0.7f, 0.2f,  1.9125f,  1.0f, 0.0f,
			 0.7f, 0.2f,  1.9125f,  1.0f, 0.0f,
			-0.7f, 0.2f,  1.9125f,  0.0f, 0.0f,
			-0.7f, 0.2f, -0.5125f,  0.0f, 1.0f,

			-0.7f,  0.4f, -0.5125f,  0.0f, 1.0f,
			 0.7f,  0.4f, -0.5125f,  1.0f, 1.0f,
			 0.7f,  0.4f,  1.9125f,  1.0f, 0.0f,
			 0.7f,  0.4f,  1.9125f,  1.0f, 0.0f,
			-0.7f,  0.4f,  1.9125f,  0.0f, 0.0f,
			-0.7f,  0.4f, -0.5125f,  0.0f, 1.0f
	};

	unsigned int VBO, VAO;   // Tray
	unsigned int VBO2, VAO2; // Candle
	unsigned int VBO3, VAO3; // Large Button/Remote
	unsigned int VBO4, VAO4; // Pen
	unsigned int VBO5, VAO5; // Plane
	unsigned int VBO6, VAO6; // Remote
	unsigned int VBO7, VAO7; // Pen Tip Pyramid
	unsigned int VBOLight, VAOLight;
	unsigned int VBOBookBottom, VAOBookBottom; // Notebook Bottom
	unsigned int VBOBookTop, VAOBookTop; // Notebook Top
	unsigned int VBOPages, VAOPages; // Notebook Top

	//Tray
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// normal attribute
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	//----------------------------------------------

	// Candle - Cylinder
	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);
	glBindVertexArray(VAO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	//----------------------------------------------
	// Large Remote Button - Cylinder
	glGenVertexArrays(1, &VAO3);
	glGenBuffers(1, &VBO3);
	glBindVertexArray(VAO3);
	glBindBuffer(GL_ARRAY_BUFFER, VBO3);
	//----------------------------------------------
	//Pen - Cylinder
	glGenVertexArrays(1, &VAO4);
	glGenBuffers(1, &VBO4);
	glBindVertexArray(VAO4);
	glBindBuffer(GL_ARRAY_BUFFER, VBO4);
	//----------------------------------------------
	//plane
	glGenVertexArrays(1, &VAO5);
	glGenBuffers(1, &VBO5);
	glBindVertexArray(VAO5);
	glBindBuffer(GL_ARRAY_BUFFER, VBO5);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);


	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//----------------------------------------------
	//Remote
	glGenVertexArrays(1, &VAO6);
	glGenBuffers(1, &VBO6);
	glBindVertexArray(VAO6);
	glBindBuffer(GL_ARRAY_BUFFER, VBO6);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices3), vertices3, GL_STATIC_DRAW);


	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//----------------------------------------------

	//PEN TIP - PYRAMID
	glGenVertexArrays(1, &VAO7);
	glGenBuffers(1, &VBO7);
	glBindVertexArray(VAO7);
	glBindBuffer(GL_ARRAY_BUFFER, VBO7);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices4), vertices4, GL_STATIC_DRAW);


	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Light Cube
	//glGenVertexArrays(1, &VAOLight);
	//glGenBuffers(1, &VBOLight);
	//glBindVertexArray(VAOLight);
	//glBindBuffer(GL_ARRAY_BUFFER, VBOLight);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(verticesLight), verticesLight, GL_STATIC_DRAW);



	// position attribute
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
//	glEnableVertexAttribArray(0);
//	// normal attribute
//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
//	glEnableVertexAttribArray(1);


	//----------------------------------------------
//Book Bottom
	glGenVertexArrays(1, &VAOBookBottom);
	glGenBuffers(1, &VBOBookBottom);
	glBindVertexArray(VAOBookBottom);
	glBindBuffer(GL_ARRAY_BUFFER, VBOBookBottom);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesNotebook), verticesNotebook, GL_STATIC_DRAW);


	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//----------------------------------------------


	//Book Top
	glGenVertexArrays(1, &VAOBookTop);
	glGenBuffers(1, &VBOBookTop);
	glBindVertexArray(VAOBookTop);
	glBindBuffer(GL_ARRAY_BUFFER, VBOBookTop);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesNotebookTop), verticesNotebookTop, GL_STATIC_DRAW);


	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//----------------------------------------------


//Pages
	glGenVertexArrays(1, &VAOPages);
	glGenBuffers(1, &VBOPages);
	glBindVertexArray(VAOPages);
	glBindBuffer(GL_ARRAY_BUFFER, VBOPages);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesPages), verticesPages, GL_STATIC_DRAW);


	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// load and create a texture 
	// -------------------------
	unsigned int texture1, texture2, texture3, texture4, texture5, texture6, texture7;
	// texture 1
	// --------- Tray -------------
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char* data = stbi_load("images/wood.jfif", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	// texture 2
	// --------- Pen / Plane -----------
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	data = stbi_load("images/screen.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		// note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// texture 3
// --------- Candle -----------
	glGenTextures(1, &texture3);
	glBindTexture(GL_TEXTURE_2D, texture3);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char* data2 = stbi_load("images/PIplastic.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data2);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data2);

	// texture 4
// ---------
	glGenTextures(1, &texture4);
	glBindTexture(GL_TEXTURE_2D, texture4);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char* data3 = stbi_load("images/leather.jfif", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data3);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data3);

	// texture 5
// ---------
	glGenTextures(1, &texture5);
	glBindTexture(GL_TEXTURE_2D, texture5);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char* data4 = stbi_load("images/rimofbulp.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data4);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data4);

	//texture 6
// ---------
	glGenTextures(1, &texture6);
	glBindTexture(GL_TEXTURE_2D, texture6);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char* data5 = stbi_load("images/steel2.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data5);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data5);

	//-------------------------------------------------------------------------------
		//texture 7
// ---------
	glGenTextures(1, &texture7);
	glBindTexture(GL_TEXTURE_2D, texture7);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char* data6 = stbi_load("images/table.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data6);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data6);

	//---------------------------------------------------------------------------

	// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
	// -------------------------------------------------------------------------------------------
	ourShader.use();
	ourShader.setInt("texture1", 0);
	ourShader.setInt("texture2", 1);
	ourShader.setInt("texture3", 2);
	ourShader.setInt("texture4", 3);
	ourShader.setInt("texture5", 4);
	ourShader.setInt("texture6", 5);
	ourShader.setInt("texture7", 6);

	glm::mat4 model;
	float angle;

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, texture3);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, texture4);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, texture5);
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, texture6);
		glActiveTexture(GL_TEXTURE6);
		glBindTexture(GL_TEXTURE_2D, texture7);

		// activate shader
		ourShader.use();

		// pass projection matrix to shader (note that in this case it could change every frame)
		//glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

		//Declares projection variable
		glm::mat4 projection;

		//Conditional to check if ortho, and set perspectives accordingly
		if (ortho) {
			float scale = 100;
			projection = glm::ortho(-((GLfloat)SCR_WIDTH / scale), ((GLfloat)SCR_WIDTH / scale), -((GLfloat)SCR_HEIGHT / scale),
				((GLfloat)SCR_HEIGHT / scale), -4.5f, 6.5f);
		}
		else {
			// Creates a perspective projection
			projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		}

		ourShader.setMat4("projection", projection);

		// camera/view transformation
		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		ourShader.setMat4("view", view);

		// render TRAY
		glBindVertexArray(VAO);

		// calculate the model matrix for each object and pass it to shader before drawing
		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first		
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));
		angle = 0.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		ourShader.setMat4("model", model);

		glDrawArrays(GL_TRIANGLES, 0, 36);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture3);


		// CANDLE/CYLINDER
		glBindVertexArray(VAO2);

		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first		
		model = glm::translate(model, glm::vec3(1.95f, 0.5f, -0.85f));
		ourShader.setMat4("model", model);


		static_meshes_3D::Cylinder C(0.47, 100, 1.75, true, true, true);
		C.render();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture2);


		//Remote large button
		glBindVertexArray(VAO3);
		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first		
		model = glm::translate(model, glm::vec3(0.0f, -0.14f, -0.4f));
		ourShader.setMat4("model", model);

		static_meshes_3D::Cylinder C2(0.20, 100, 0.05, true, true, true);
		C2.render();

		//Sphere S(1, 10, 10);
		//S.Draw();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture2);

		//Pen
		glBindVertexArray(VAO4);
		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first		
		model = glm::translate(model, glm::vec3(0.0f, -0.30f, 2.0f));
		// Apply rotation to the model matrix
		float angle = glm::radians(180.0f);  // Set the desired rotation angle in degrees
		glm::vec3 axis(1.0f, 1.0f, 0.0f);   // Choose the rotation axis (in this case, Y-axis)
		model = glm::rotate(model, angle, axis);


		ourShader.setMat4("model", model);

		static_meshes_3D::Cylinder C3(0.05, 100, 1.5, true, true, true);
		C3.render();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture2);


		// render PLANE
		glBindVertexArray(VAO5);
		// calculate the model matrix for each object and pass it to shader before drawing
		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first		
		model = glm::translate(model, glm::vec3(0.0f, -0.75f, 0.0f));
		angle = 0.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		ourShader.setMat4("model", model);



		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture7);
		glDrawArrays(GL_TRIANGLES, 0, 36);


		// render Remote
		glBindVertexArray(VAO6);
		// calculate the model matrix for each object and pass it to shader before drawing
		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first		
		model = glm::translate(model, glm::vec3(0.0f, -0.55f, -0.7f));
		angle = 0.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		ourShader.setMat4("model", model);



		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture5);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// render PYRAMID
		glBindVertexArray(VAO7);
		// calculate the model matrix for each object and pass it to shader before drawing

		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first		
		model = glm::translate(model, glm::vec3(-0.79f, -0.30f, 2.0f));
		angle = 90.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));

		// Scale the pyramid to change its size
		float scale = 0.08f; // Set the desired scale factor
		model = glm::scale(model, glm::vec3(scale));

		ourShader.setMat4("model", model);


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture3);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		// -----------------------------------
		//glBindVertexArray(VAOLight);
		//lightCubeShader.use();
		//lightingShader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
		//lightingShader.setVec3("objectColor", 0.0f, 1.0f, 0.0f);
		//lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
		//lightingShader.setVec3("lightPos", lightPos);
		//lightCubeShader.setMat4("projection", projection);
		//lightCubeShader.setMat4("view", view);

		//model = glm::mat4(1.0f);
		//model = glm::translate(model, lightPos);
		//model = glm::scale(model, glm::vec3(1.0f)); // a smaller plane
		//model = glm::rotate(model, glm::radians(angle), glm::vec3(0.5f, 0.0f, 0.25f));
		//ourShader.setMat4("model", model);
		//lightCubeShader.setMat4("model", model);


		//glDrawArrays(GL_TRIANGLES, 0, 36);

		// render Pages
		glBindVertexArray(VAOPages);
		// calculate the model matrix for each object and pass it to shader before drawing
		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first		
		model = glm::translate(model, glm::vec3(-2.0f, -0.355f, -0.7f));
		angle = 0.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		ourShader.setMat4("model", model);



		glActiveTexture(GL_TEXTURE6);
		glBindTexture(GL_TEXTURE_2D, texture4);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		//----------------------------------------------------------
		// render Book Bottom
		glBindVertexArray(VAOBookBottom);
		// calculate the model matrix for each object and pass it to shader before drawing
		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first		
		model = glm::translate(model, glm::vec3(-2.0f, -0.55f, -0.7f));
		angle = 0.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		ourShader.setMat4("model", model);



		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture4);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		//----------------------------------------------------------
		// render Book Top
		glBindVertexArray(VAOBookTop);
		// calculate the model matrix for each object and pass it to shader before drawing
		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first		
		model = glm::translate(model, glm::vec3(-2.0f, -0.15f, -0.7f));
		angle = 0.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		ourShader.setMat4("model", model);



		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, texture4);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		//----------------------------------------------------------



		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glDeleteVertexArrays(1, &VAO2);
	glDeleteBuffers(1, &VBO2);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	float cameraSpeed = MovementSpeed * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraUp;
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraUp;
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) // Changes perspective if "P" is pressed
		ortho = !ortho;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f; // change this value to your liking
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{

	MovementSpeed -= (float)yoffset;
	if (MovementSpeed < 1.0f)
		MovementSpeed = 1.0f;
	if (MovementSpeed > 10.0f)
		MovementSpeed = 10.0f;
}