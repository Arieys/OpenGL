// openGL.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SOIL.h"


#include"shader.h"
#include<cmath>
#include<vector>
#include<glad\glad.h>
#include<GLFW\glfw3.h>
#include<glm\glm.hpp>
#include<glm\gtc\matrix_transform.hpp>
#include<glm\gtc\type_ptr.hpp>
#include<iostream>


unsigned int load_cubemap();

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

glm::vec3 light_position = glm::vec3(0.6f, 0.6f, 1.2f);
float fov = 45.0f;     //camera view angle

float deta_time = 0;
float time_start, time_end = 0;

//screen size
int screen_width = 1280;
int screen_height = 1280;


int main()
{
	float vertex[36*6] = {
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
float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
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


	GLuint sky_VAO;
	glGenVertexArrays(1, &sky_VAO);
	glBindVertexArray(sky_VAO);

	GLuint sky_VBO;  //VBO
	glGenBuffers(1, &sky_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, sky_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//select the surface
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);

	//create vertex_shader and fragment_shader
	//source code
	Shader shader("v_shader.txt", "f_shader.txt");
	Shader l_shader("l_v_shader.txt", "l_f_shader.txt");
	Shader s_shader("s_v_shader.txt", "s_f_shader.txt");
	//start the depth test
	glEnable(GL_DEPTH_TEST);

	
	unsigned int textureID = load_cubemap();

	//set mouse and key call back function
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, MouseCallback);
	//hide cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//renew the screen
	while (!glfwWindowShouldClose(window)) {
		time_start = glfwGetTime();

		//refresh the background
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//calculate o_model and l_model matrix
		glm::mat4 o_model(1);
		o_model = glm::translate(o_model, glm::vec3(0.0f, 0.0f, 0.0f));
		o_model = glm::rotate(o_model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
		o_model = glm::scale(o_model, glm::vec3(1.0f, 1.0f, 1.0f));

		glm::mat4 l_model(1);
		l_model = glm::translate(l_model, light_position);
		l_model = glm::scale(l_model, glm::vec3(0.2f));

		//calculate view matrix
		glm::mat4 view(1);
		view = glm::lookAt(camera_position, camera_position + camera_front, camera_up);

		//calculation projection matrix
		glm::mat4 projection(1);
		projection = glm::perspective(glm::radians(fov), (float)screen_width/screen_height, 0.1f, 100.0f);
		s_shader.Use();

		//Pass the parameters to shader
		s_shader.SetMat4("view", glm::mat4(glm::mat3(glm::lookAt(camera_position, camera_position + camera_front, camera_up))));
		s_shader.SetMat4("projection", projection);

		glBindVertexArray(sky_VAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
		glDepthFunc(GL_LEQUAL);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		//use shader program
		shader.Use();

		//Pass the parameters to shader
		shader.SetMat4("model", o_model);
		shader.SetMat4("view", view);
		shader.SetMat4("projection", projection);

		shader.SetVec3("m_ambient", 1.0f, 0.5f, 0.31f);
		shader.SetVec3("m_diffuse", 1.0f, 1.0f, 1.0f);
		shader.SetVec3("m_specular", 0.5f, 0.5f, 0.5f);

		shader.SetVec3("v_position", camera_position);
		shader.SetVec3("l_position", light_position);

		shader.SetVec3("l_ambient", 0.2f, 0.2f, 0.2f);
		shader.SetVec3("l_diffuse", 0.5f, 0.5f, 0.5f);
		shader.SetVec3("l_specular", 1.0f, 1.0f, 1.0f);
		shader.SetVec3("l_attenuation", 1.0f, 0.09f, 0.032f);

		//SpotLight parameters
		shader.SetVec3("l_cutoff", cos(glm::radians(0.0f)), cos(glm::radians(90.0f)), 0.0f);

		//bind VAO and draw
		glBindVertexArray(vertex_array_object);
		glDrawArrays(GL_TRIANGLES, 0, 36);



		//swap buffer
		glfwSwapBuffers(window);
		glfwPollEvents();
		time_end = glfwGetTime();
		deta_time = time_end - time_start;
		//cout << deta_time << endl;
	}
	//clear VAO,VBO
	glDeleteVertexArrays(1, &vertex_array_object);
	glDeleteVertexArrays(1, &sky_VAO);
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

unsigned int load_cubemap()
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width[6], height[6], nrchannels[6];
	unsigned char* image[6];
	image[0] = SOIL_load_image("D:/skybox/近地轨道_LF.tga", width, height, nrchannels, SOIL_LOAD_RGB);
	image[1] = SOIL_load_image("D:/skybox/近地轨道_RT.tga", width + 1, height + 1, nrchannels + 1, SOIL_LOAD_RGB);
	image[2] = SOIL_load_image("D:/skybox/近地轨道_UP.tga", width + 2, height + 2, nrchannels + 2, SOIL_LOAD_RGB);
	image[3] = SOIL_load_image("D:/skybox/近地轨道_DN.tga", width + 3, height + 3, nrchannels + 3, SOIL_LOAD_RGB);
	image[4] = SOIL_load_image("D:/skybox/近地轨道_FR.tga", width + 4, height + 4, nrchannels + 4, SOIL_LOAD_RGB);
	image[5] = SOIL_load_image("D:/skybox/近地轨道_BK.tga", width + 5, height + 5, nrchannels + 5, SOIL_LOAD_RGB);

	for (int i = 0; i < 6; i++) {
		if (!image[i]) {
			cout << "load texture error " << i <<endl;
			cout << SOIL_last_result() << endl;
			continue;
		}
		glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
			GL_RGB, width[i], height[i], 0, GL_RGB, GL_UNSIGNED_BYTE, image[i]
		);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	return textureID;
}

