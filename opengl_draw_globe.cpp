// openGL.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include<cmath>

#include<glad\glad.h>
#include<GLFW\glfw3.h>
#include<iostream>

// set the segmentation number
#define PI 3.1415926
#define THETA_SEGMENT 65
#define BETA_SEGMENT 65
using namespace std;



int main()
{
	//set the init angle of the globe
	int k = 0;
	float vertex[(THETA_SEGMENT + 1)*(BETA_SEGMENT + 1) * 3];         //to store the vertex of the globe
	int index[THETA_SEGMENT * BETA_SEGMENT * 3 * 6];                  //to store the index of the triangle
	float theta = 0;
	float beta = 0;
	float R = 0.8;             //radius
	//set the screen size
	int screen_width = 1000;
	int screen_height = 1000;
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
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//set the vertex sequence
	float current_x, current_y, current_z;
	for (theta = 0; theta <= THETA_SEGMENT; theta++) {
		for (beta = 0; beta <= BETA_SEGMENT; beta++) {
			current_x = R*sin(beta / BETA_SEGMENT*PI)*cos(theta / THETA_SEGMENT*PI * 2);
			current_y = R*cos(beta / BETA_SEGMENT*PI);
			current_z = R*sin(beta / BETA_SEGMENT*PI)*sin(theta / THETA_SEGMENT*PI * 2);
			vertex[k++] = current_x;
			vertex[k++] = current_y;
			vertex[k++] = current_z;
		}
	}
	//set the index sequence
	k = 0;
	for (int i = 0; i < THETA_SEGMENT; i++)
	{
		for (int j = 0; j < BETA_SEGMENT; j++)
		{

			index[k++] = i * (BETA_SEGMENT + 1) + j;
			index[k++] = (i + 1) * (BETA_SEGMENT + 1) + j;
			index[k++] = (i + 1) * (BETA_SEGMENT + 1) + j + 1;

			index[k++] = i * (BETA_SEGMENT + 1) + j;
			index[k++] = (i + 1) * (BETA_SEGMENT + 1) + j + 1;
			index[k++] = i * (BETA_SEGMENT + 1) + j + 1;
		}
	}


	//create VAO,VBO and bind
	GLuint vertex_array_object;  //VAO
	glGenVertexArrays(1, &vertex_array_object);
	glBindVertexArray(vertex_array_object);
	GLuint vertex_buffer_object;  //VBO
	glGenBuffers(1, &vertex_buffer_object);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
	GLuint element_buffer_object;  //EBO
	glGenBuffers(1, &element_buffer_object);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//select the surface
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	//create vertex_shader and fragment_shader
	
	//source code
	const char *vertex_shader_source =
		"#version 330 core\n"
		"layout (location=0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"  gl_Position = vec4(aPos,1.0);\n"
		"}\n\0";
	const char *fragment_shader_source =
		"#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"  FragColor = vec4(1.0f,1.0f,0.0f,1.0f);\n"
		"}\n\0";
	//compile vertex shader
	int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
	glCompileShader(vertex_shader);
	int success;
	char info_log[512];

	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
		cout << "ERROR SHADER VERTEX COMPILE FAILED" << endl << info_log << endl;
	}
	//compile fragment shader
	int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
	glCompileShader(fragment_shader);

	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
		cout << "ERROR SHADER FRAGMENT COMPILE FAILED" << endl << info_log << endl;
	}
	//compile shader program
	int shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);

	glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shader_program, 512, NULL, info_log);
		cout << "ERROR SHADER PROGRAM LINKING FAILED" << endl << info_log << endl;
	}
	//delete vertex and fragment shader
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	//renew the screen
	while (!glfwWindowShouldClose(window)) {
		//refresh the background
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//use shader program
		glUseProgram(shader_program);
		//bind VAO and draw
		glBindVertexArray(vertex_array_object);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, THETA_SEGMENT * BETA_SEGMENT * 3 * 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		//swap buffer
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	//clear VAO,VBO
	glDeleteVertexArrays(1, &vertex_array_object);
	glDeleteBuffers(1, &vertex_buffer_object);

	glfwTerminate();
	return 0;

}




