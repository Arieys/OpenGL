// openGL.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include"shader.h"
#include<cmath>
#include<vector>
#include<glad\glad.h>
#include<GLFW\glfw3.h>
#include<glm\glm.hpp>
#include<glm\gtc\matrix_transform.hpp>
#include<glm\gtc\type_ptr.hpp>
#include<iostream>
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void MouseCallback(GLFWwindow*window, double xpos, double ypos);
// set the segmentation number
#define PI 3.1415926
#define THETA_SEGMENT 65
#define BETA_SEGMENT 65
using namespace std;

glm::vec3 camera_position = glm::vec3(0.0f, 0.0f, 3.0f);   // camera position
glm::vec3 camera_front = glm::vec3(0.0f, 0.0f, -1.0f);     // camera angle
glm::vec3 camera_up = glm::vec3(0.0f, 1.0f, 0.0f);         // camera up vector

float fov = 45.0f;     //camera view angle

float deta_time = 0;
float time_start, time_end = 0;

//screen size
int screen_width = 1280;
int screen_height = 1280;

int main()
{
	float vertex[36 * 6] = {
		-0.5f, -0.5f, -0.5f, 1.0f,0.5f,0.6f,
		0.5f, -0.5f, -0.5f, 0.2f,1.0f,0.6f,
		0.5f, 0.5f, -0.5f, 0.5f,1.0f,0.5f,
		0.5f, 0.5f, -0.5f, 1.0f,0.7f,0.2f,
		-0.5f, 0.5f, -0.5f, 0.5f,0.4f,0.3f,
		-0.5f, -0.5f, -0.5f, 0.6f,1.0f,0.6f,

		-0.5f, -0.5f, 0.5f, 0.5f,1.0f,0.3f,
		0.5f, -0.5f, 0.5f, 0.6f,1.0f,0.4f,
		0.5f, 0.5f, 0.5f, 0.5f,0.4f,0.3f,
		0.5f, 0.5f, 0.5f, 0.3f,1.0f,0.5f,
		-0.5f, 0.5f, 0.5f, 0.5f,0.3f,1.0f,
		-0.5f, -0.5f, 0.5f, 0.2f,0.1f,0.5f,

		-0.5f, 0.5f, 0.5f, 0.2f,1.0f,0.5f, 
		-0.5f, 0.5f, -0.5f, 0.5f,0.3f,0.4f,
		-0.5f, -0.5f, -0.5f, 0.2f,0.3f,0.2f,
		-0.5f, -0.5f, -0.5f, 0.5f,0.6f,0.7f,
		-0.5f, -0.5f, 0.5f, 0.2f,0.5f,0.1f,
		-0.5f, 0.5f, 0.5f, 0.5f,0.5f,0.5f,

		0.5f, 0.5f, 0.5f, 0.5f,0.9f,0.3f,
		0.5f, 0.5f, -0.5f, 0.6f,0.1f,0.5f,
		0.5f, -0.5f, -0.5f, 0.3f,0.2f,0.5f,
		0.5f, -0.5f, -0.5f, 0.1f,0.6f,0.2f,
		0.5f, -0.5f, 0.5f, 0.6f,0.6f,0.23f,
		0.5f, 0.5f, 0.5f, 0.36f,0.5f,0.5f,

		-0.5f, -0.5f, -0.5f, 0.6f,0.09f,0.9f,
		0.5f, -0.5f, -0.5f, 0.7f,0.1f,0.2f,
		0.5f, -0.5f, 0.5f, 0.5f,0.6f,0.4f,
		0.5f, -0.5f, 0.5f, 0.6f,0.5f,0.3f,
		-0.5f, -0.5f, 0.5f, 0.5f,0.5f,0.5f,
		-0.5f, -0.5f, -0.5f, 0.6f,0.5f,0.3f,

		-0.5f, 0.5f, -0.5f, 0.02f,0.02f,0.6f,
		0.5f, 0.5f, -0.5f, 0.2f,0.03f,0.6f,
		0.5f, 0.5f, 0.5f, 0.6f,0.3f,0.4f,
		0.5f, 0.5f, 0.5f, 0.2f,0.5f,0.6f,
		-0.5f, 0.5f, 0.5f, 0.6f,0.7f,1.0f,
		-0.5f, 0.5f, -0.5f, 0.02f,0.6f,0.3f
	};
	//to store the vertex of the cube


	//glfw initialize
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, false);
	//create window
	auto window = glfwCreateWindow(screen_width, screen_height, "OPENGL", nullptr, nullptr);
	if (window == nullptr) {
		cout << "Fail to create window" << endl;
		return -1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Fail to init glad" << endl;
		return -1;
	}
	//create view
	glViewport(0, 0, screen_width, screen_height);
	
	//switch to the line_mode
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	

	//create VAO,VBO and bind
	GLuint vertex_array_object;  //VAO
	glGenVertexArrays(1, &vertex_array_object);
	glBindVertexArray(vertex_array_object);
	GLuint vertex_buffer_object;  //VBO
	glGenBuffers(1, &vertex_buffer_object);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	

	//select the surface
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);

	//create vertex_shader and fragment_shader
	//source code
	Shader shader("v_shader.txt", "f_shader.txt");

	//start the depth test
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	


	//set mouse and key call back function
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, MouseCallback);
	//hide cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//renew the screen
	while (!glfwWindowShouldClose(window)) {
		time_start = glfwGetTime();

		//refresh the background
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//calculate model matrix
		glm::mat4 model(1);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

		//calculate view matrix
		glm::mat4 view(1);
		view = glm::lookAt(camera_position, camera_position + camera_front, camera_up);

		//calculation projection matrix
		glm::mat4 projection(1);
		projection = glm::perspective(glm::radians(fov), (float)screen_width/screen_height, 0.1f, 100.0f);

		//get the location of these three variable in shader
		int model_location = glGetUniformLocation(shader.getShaderID(), "model");
		int view_location = glGetUniformLocation(shader.getShaderID(), "view");
		int projection_location = glGetUniformLocation(shader.getShaderID(), "projection");
		//int color_location = glGetUniformLocation(shader.getShaderID(), "C");

		//set the value of the cube color
		//float green_value = (sin(glfwGetTime() / 2.0f)) + 0.5f;
		//float red_value = sin(glfwGetTime());
		//float blue_value = cos(glfwGetTime());
		//pass these variable to function
		glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projection_location, 1, GL_FALSE, glm::value_ptr(projection));
		//glUniform4f(color_location, red_value, green_value, blue_value, 1.0f);

		//use shader program
		shader.Use();
		//bind VAO and draw
		glBindVertexArray(vertex_array_object);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		//swap buffer
		glfwSwapBuffers(window);
		glfwPollEvents();
		time_end = glfwGetTime();
		deta_time = time_end - time_start;
		//cout << deta_time << endl;
	}
	//clear VAO,VBO
	glDeleteVertexArrays(1, &vertex_array_object);
	glDeleteBuffers(1, &vertex_buffer_object);

	glfwTerminate();
	return 0;

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
//key callback function is to process the action of pressing keys
{
	glm::vec3 camera_up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 camera_left = glm::vec3(-1.0f, 0.0f, 0.0f);
	float speed = deta_time*10.0f;
	if (key == GLFW_KEY_W) camera_position += speed*camera_up;
	else if (key == GLFW_KEY_S) camera_position -= speed*camera_up;
	else if (key == GLFW_KEY_A) camera_position += speed*camera_left;
	else if (key == GLFW_KEY_D) camera_position -= speed*camera_left;
	else if (key == GLFW_KEY_Q) camera_position += speed*camera_front;
	else if (key == GLFW_KEY_Z) camera_position -= speed*camera_front;
	else if (key == GLFW_KEY_ESCAPE) glfwTerminate();
}

float lastX = screen_width / 2.0f;
float lastY = screen_height / 2.0f;
float yaw = 0, pitch = 0;
bool firstMouse = true;

void MouseCallback(GLFWwindow*window, double xpos, double ypos) 
//mouse callback function is to process the event of mousemove
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.05;
	xoffset *= sensitivity;
	yoffset *= sensitivity;
	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)	pitch = 89.0f;
	if (pitch < -89.0f)	pitch = -89.0f;

	glm::vec3 front;
	front.z = -cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.x = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	camera_front = glm::normalize(front);
	cout << camera_front.x <<" "<< camera_front.y<<" " << camera_front.z << endl;
}



