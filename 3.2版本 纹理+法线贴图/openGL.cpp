// openGL.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SOIL.h"

#include "skybox.h"
#include "shader.h"

#include <cmath>
#include <vector>
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <iostream>

unsigned int loadTexture(const char *file);
void convertTBN(float* vertices, float *TB);

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

glm::vec3 light_position = glm::vec3(2.0f, 2.0f, 2.0f);
float fov = 45.0f;     //camera view angle

float deta_time = 0;
float time_start, time_end = 0;

//screen size
int screen_width = 1280;
int screen_height = 1280;


int main()
{
	float vertex[36*8] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,	
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,	
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
};
	//to store the vertex of the cube
	float TB[14 * 36];
	convertTBN(vertex, TB);

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
	
	//create VAO,VBO and bind
	GLuint vertex_array_object;  //VAO
	glGenVertexArrays(1, &vertex_array_object);
	glBindVertexArray(vertex_array_object);
	GLuint vertex_buffer_object;  //VBO
	glGenBuffers(1, &vertex_buffer_object);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TB), TB, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(8 * sizeof(float)));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(11 * sizeof(float)));
	glEnableVertexAttribArray(4);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLuint light_VAO;
	glGenVertexArrays(1, &light_VAO);
	glBindVertexArray(light_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//select the surface
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);

	//create vertex_shader and fragment_shader
	//source code
	Shader shader("v_shader.txt", "f_shader.txt");
	Shader l_shader("l_v_shader.txt", "l_f_shader.txt");

	skybox skybox;

	unsigned int textureID = loadTexture("D:/浙江大学/Vs Project/openGL/openGL/色彩纹理图.bmp");
	unsigned int textureNormalID = loadTexture("D:/浙江大学/Vs Project/openGL/openGL/法向图.bmp");

	//start the depth test
	glEnable(GL_DEPTH_TEST);

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

		//Draw skybox
		skybox.drawSkybox(glm::mat4(glm::mat3(glm::lookAt(camera_position, camera_position + camera_front, camera_up))),projection);

		//switch to the line_mode
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		//use shader program
		shader.Use();

		//Pass the parameters to shader
		shader.SetMat4("model", o_model);
		shader.SetMat4("view", view);
		shader.SetMat4("projection", projection);

		shader.SetVec3("m_ambient", 0.8f, 0.8f, 0.8f);
		shader.SetVec3("m_diffuse", 1.0f, 1.0f, 1.0f);
		shader.SetVec3("m_specular", 0.5f, 0.5f, 0.5f);

		shader.SetVec3("v_position", camera_position);
		shader.SetVec3("l_position", light_position);

		shader.SetVec3("l_ambient", 0.2f, 0.2f, 0.2f);
		shader.SetVec3("l_diffuse", 1.0f, 1.0f, 1.0f);
		shader.SetVec3("l_specular", 1.0f, 1.0f, 1.0f);
		shader.SetVec3("l_attenuation", 1.0f, 0.09f, 0.032f);

		//SpotLight parameters
		shader.SetVec3("l_cutoff", cos(glm::radians(0.0f)), cos(glm::radians(90.0f)), 0.0f);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);

		glUniform1i(glGetUniformLocation(shader.getShaderID(), "normalMap"), 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textureNormalID);
		//bind VAO and draw
		glBindVertexArray(vertex_array_object);
		glDrawArrays(GL_TRIANGLES, 0, 36);


		//Use shader program
		l_shader.Use();

		//Pass the parameters to shader
		l_shader.SetMat4("model", l_model);
		l_shader.SetMat4("view", view);
		l_shader.SetMat4("projection", projection);

		glBindVertexArray(light_VAO);
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

unsigned int loadTexture(const char* file)
{
	int width, height, channel;
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	unsigned char *image = SOIL_load_image(file, &width, &height, &channel, SOIL_LOAD_RGB);

	if (!image) {
		std::cout << "Load texture failed" << std::endl;
		return -1;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

	glGenerateMipmap(GL_TEXTURE_2D);

	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	return textureID;
}

void convertTBN(float* vertices, float *TB) 
{
	int k = 0;
	//use k to assign
	for (int i = 0; i <36; i += 3) 
	// input three triangles
	{
		glm::vec3  v0 = glm::vec3(vertices[8 * (i + 0)], vertices[8 * (i + 0) + 1], vertices[8 * (i + 0) + 2]);
		glm::vec3  v1 = glm::vec3(vertices[8 * (i + 1)], vertices[8 * (i + 1) + 1], vertices[8 * (i + 1) + 2]);
		glm::vec3  v2 = glm::vec3(vertices[8 * (i + 2)], vertices[8 * (i + 2) + 1], vertices[8 * (i + 2) + 2]);
		glm::vec2  uv0 = glm::vec2(vertices[8 * (i + 0) + 6], vertices[8 * (i + 0) + 7]);
		glm::vec2  uv1 = glm::vec2(vertices[8 * (i + 1) + 6], vertices[8 * (i + 1) + 7]);
		glm::vec2  uv2 = glm::vec2(vertices[8 * (i + 2) + 6], vertices[8 * (i + 2) + 7]);
		// construct the directional vector of the triangle
		glm::vec3  edge1 = v1 - v0;
		glm::vec3  edge2 = v2 - v0;
		// construct the directional vector of the u-v plantUV
		glm::vec2 deltaUV1 = uv1 - uv0;
		glm::vec2 deltaUV2 = uv2 - uv0;
		glm::vec3 tangent, binormal;
		// calculate the coefficient
		float   r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
		tangent.x = r * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent.y = r * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent.z = r * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
		tangent = glm::normalize(tangent);// normalize the result

		binormal.x = r * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		binormal.y = r * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		binormal.z = r * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
		binormal = glm::normalize(binormal);

		//copy the original array
		for (int q = 0; q < 36; q++) {
			for (int j = 0; j < 8; j++) {
				TB[14 * q + j] = vertices[8 * q + j];
			}
		}
		//copy the calculated variables
		TB[14 * ((k * 3) + 0) + 8] = tangent.x;      TB[14 * ((k * 3) + 0) + 11] = binormal.x;
		TB[14 * ((k * 3) + 0) + 9] = tangent.y;      TB[14 * ((k * 3) + 0) + 12] = binormal.y;
		TB[14 * ((k * 3) + 0) + 10] = tangent.z;     TB[14 * ((k * 3) + 0) + 13] = binormal.z;
		TB[14 * ((k * 3) + 1) + 8] = tangent.x;      TB[14 * ((k * 3) + 1) + 11] = binormal.x;
		TB[14 * ((k * 3) + 1) + 9] = tangent.y;      TB[14 * ((k * 3) + 1) + 12] = binormal.y;
		TB[14 * ((k * 3) + 1) + 10] = tangent.z;     TB[14 * ((k * 3) + 1) + 13] = binormal.z;
		TB[14 * ((k * 3) + 2) + 8] = tangent.x;      TB[14 * ((k * 3) + 2) + 11] = binormal.x;
		TB[14 * ((k * 3) + 2) + 9] = tangent.y;      TB[14 * ((k * 3) + 2) + 12] = binormal.y;
		TB[14 * ((k * 3) + 2) + 10] = tangent.z;     TB[14 * ((k * 3) + 2) + 13] = binormal.z;
		k++;
	}
}


