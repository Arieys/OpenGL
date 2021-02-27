#pragma once
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
};