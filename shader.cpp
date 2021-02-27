#include "include\glad\glad.h"
#include"shader.h"

Shader::Shader(const char* vertex_path, const char* fragment_path)
{
	//read shader source
	std::string vertex_code;
	std::string fragment_code;

	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		vShaderFile.open(vertex_path);
		fShaderFile.open(fragment_path);

		std::stringstream vShaderStream, fShaderStream;

		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		vertex_code = vShaderStream.str();
		fragment_code = fShaderStream.str();
	}
	catch (std::ifstream::failure e) {
		std::cerr << "Failed read file" << std::endl;
	}

	const char *vertex_shader_source = vertex_code.c_str();
	const char *fragment_shader_source = fragment_code.c_str();

	//create vertex shader
	int vertex_shader, fragment_shader;
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
	glCompileShader(vertex_shader);

	//to store the information of comiling the shader and program
	int success;
	char info_log[512];

	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
		std::cout << "Vertex Shader Compile Error" << " " << info_log << std::endl;
	}

	//create fragment shader
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
	glCompileShader(fragment_shader);

	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
		std::cout << "Fragment Shader Compile Error" << " " << info_log << std::endl;
	}

	//create program shader and log the Id to private variable
	int program_shader;
	program_shader = glCreateProgram();
	this->shader_Id = program_shader;
	glAttachShader(program_shader, vertex_shader);
	glAttachShader(program_shader, fragment_shader);
	glLinkProgram(program_shader);
	glGetProgramiv(this->shader_Id, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(this->shader_Id, 512, NULL, info_log);
		std::cout << "ERROR SHADER PROGRAM LINKING FAILED" << " " << info_log << std::endl;
	}
	//delete vertex and fragment shader
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
}

Shader::~Shader()
{

}

void Shader::Use()
{
	glUseProgram(this->shader_Id);
}

int Shader::getShaderID() const
{
	return this->shader_Id;
}