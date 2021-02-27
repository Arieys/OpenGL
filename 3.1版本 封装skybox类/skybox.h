#pragma once
#include <iostream>
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include "shader.h"
#include "SOIL.h"

class skybox {
private:
	Shader *s;
	int textureID;
	GLuint sky_VAO;
public:
	~skybox();
	skybox();
	unsigned int load_cubemap();
	void skybox::drawSkybox(glm::mat4 view, glm::mat4 projection);
};

