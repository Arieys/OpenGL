// openGL.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#include<glad\glad.h>
#include<GLFW\glfw3.h>
#include<iostream>
using namespace std;

int main()
{
	//store the information of triangle
	const float rectangle[] = {
		-0.75f,-0.5f,0.0f,
		0.25f,-0.5f,0.0f,
		-0.25f,0.5f,0.0f,
		0.75f,0.5f,0.0f
	};
	const int index[] = {
		0,1,2,
		1,2,3
	};
	//set the screen size
	int screen_width = 1280;
	int screen_height = 720;
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectangle), rectangle, GL_STATIC_DRAW);
	GLuint element_buffer_object;  //EBO
	glGenBuffers(1, &element_buffer_object);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

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
		"  FragColor = vec4(0.0f,1.0f,0.0f,1.0f);\n"
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
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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




