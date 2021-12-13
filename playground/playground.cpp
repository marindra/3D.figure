// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <common/shader.hpp>

class ForMVP {
	glm::mat4 ViewMatrix;
	glm::mat4 ProjectionMatrix;
	glm::mat4 ModelMatrix = glm::mat4(1.0f);

	// Initial position : on +Z
	glm::vec3 position = glm::vec3(0, 0, 5);
	// Initial horizontal angle : toward -Z
	float horizontalAngle = 3.14f;
	// Initial vertical angle : none
	float verticalAngle = 0.0f;
	// Initial Field of View
	float initialFoV = 45.0f;

	const float initRadius = 5.0f;
	const float initHeight = 0.0f;

	float radius = initRadius;
	float height = initHeight;

	float speed = 3.0f; // 3 units / second
	glm::vec3 objectCenter = glm::vec3(0, 0, 0);
public:

	void computeMatricesFromInputs();

	glm::mat4 getMVPMatrix() {
		return ProjectionMatrix * ViewMatrix * ModelMatrix;
	}
};

void computeMatricesFromInputs();

int main(void)
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(1024, 768, "Tutorial 04 - Colored Cube", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders("Shader.vertexshader", "Shader.fragmentshader");

	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	//glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	//glm::mat4 View = glm::lookAt(
	//	glm::vec3(0, 0, 5), // Camera is at (4,3,-3), in World Space
	//	glm::vec3(0, 0, 0), // and looks at the origin
	//	glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	//);
	// Model matrix : an identity matrix (model will be at the origin)
	//glm::mat4 Model = glm::mat4(1.0f);
	// Our ModelViewProjection : multiplication of our 3 matrices
	//glm::mat4 MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around

	/*static const GLfloat g_vertex_buffer_data[] = {
		-1.0f, -1.0f, -1.0f,
		 0.0f,  0.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 0.0f,  1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 0.0f,  0.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		 0.0f,  1.0f,  1.0f,
	};*/


	static const GLfloat g_vertex_buffer_data[] = {
		-1.0f, -1.0f, -1.0f,
		 0.0f,  0.0f, -1.0f,
		 0.0f,  1.0f, -1.0f,

		 0.0f,  0.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 0.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		 0.0f,  0.0f,  1.0f,
		 0.0f,  1.0f,  1.0f,

		 0.0f,  0.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		 0.0f,  1.0f,  1.0f,

		-1.0f, -1.0f, -1.0f,
		 0.0f,  1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,

		 0.0f,  1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 0.0f,  1.0f,  1.0f,

		-1.0f, -1.0f, -1.0f,
		 0.0f,  0.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,

		 0.0f,  0.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 0.0f,  0.0f,  1.0f,

		 0.0f,  0.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,

		 0.0f,  0.0f, -1.0f,
		 0.0f,  0.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 0.0f,  1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,

		 0.0f,  1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 0.0f,  1.0f,  1.0f,
	};

	static const GLfloat g_color_buffer_data[] = {
		0.588f,   1.0f, 0.506f,
		0.588f,   1.0f, 0.506f,
		0.588f,   1.0f, 0.506f,

		0.588f,   1.0f, 0.506f,
		0.588f,   1.0f, 0.506f,
		0.588f,   1.0f, 0.506f,

		0.196f, 0.803f, 0.196f,
		0.196f, 0.803f, 0.196f,
		0.196f, 0.803f, 0.196f,

		0.196f, 0.803f, 0.196f,
		0.196f, 0.803f, 0.196f,
		0.196f, 0.803f, 0.196f,

		0.498f,   1.0f, 0.831f,
		0.498f,   1.0f, 0.831f,
		0.498f,   1.0f, 0.831f,

		  0.0f, 0.980f, 0.604f,
		  0.0f, 0.980f, 0.604f,
		  0.0f, 0.980f, 0.604f,

		0.678f,   1.0f, 0.184f,
		0.678f,   1.0f, 0.184f,
		0.678f,   1.0f, 0.184f,

		0.437f, 0.902f, 0.190f,
		0.437f, 0.902f, 0.190f,
		0.437f, 0.902f, 0.190f,

		0.678f,   1.0f, 0.184f,
		0.678f,   1.0f, 0.184f,
		0.678f,   1.0f, 0.184f,

		0.437f, 0.902f, 0.190f,
		0.437f, 0.902f, 0.190f,
		0.437f, 0.902f, 0.190f,

		0.498f,   1.0f, 0.831f,
		0.498f,   1.0f, 0.831f,
		0.498f,   1.0f, 0.831f,

		  0.0f, 0.980f, 0.604f,
		  0.0f, 0.980f, 0.604f,
		  0.0f, 0.980f, 0.604f,
	};

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	GLuint colorbuffer;
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

	ForMVP current;

	do {

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		current.computeMatricesFromInputs();
		glm::mat4 MVP = current.getMVPMatrix(); // Remember, matrix multiplication is the other way around

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		
		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // 12*3 indices starting at 0 -> 12 triangles

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	// Cleanup VBO and shader
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteProgram(programID);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

void ForMVP::computeMatricesFromInputs() {
	static double lastTime = glfwGetTime();
	double time = glfwGetTime();
	double deltaTime = time - lastTime;

	// Move lower
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		height += deltaTime * speed;
	}
	// Move heigher
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		height -= deltaTime * speed;
	}

	// Increase radius
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		radius += deltaTime * speed;
	}

	// Decrease radius
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		radius -= deltaTime * speed;
	}

	// Go to basic line
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		height = initHeight;
		radius = initRadius;
	}

	position = objectCenter + glm::vec3(radius * cos(time), height, radius * sin(time));
	float FoV = initialFoV;
	ProjectionMatrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);

	ViewMatrix = glm::lookAt(
		position,           // Camera is here
		objectCenter, // and looks here : at the same position, plus "direction"
		glm::vec3(0, 1, 0)                  // Head is up (set to 0,-1,0 to look upside-down)
	);

	lastTime = time;
}