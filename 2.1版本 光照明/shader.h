#pragma once
#include<glm\glm.hpp>
#include<glm\gtc\matrix_transform.hpp>
#include<glm\gtc\type_ptr.hpp>
#include <glad\glad.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

class Shader {
private:
	int shader_Id;
public:
	Shader(const char* vertex_shader,const char* fragment_shader);
	~Shader();
	void Use();
	int getShaderID() const;
	void SetVec3(char *name, float x, float y, float z);
	void SetVec3(char *name, glm::vec3 v);
	void SetMat4(char *name, glm::mat4 m);
	void SetVec4(char *name, float x, float y, float z, float s);
	void SetVec4(char *name, glm::vec4 v);
	int GetLocation(char *name);
};