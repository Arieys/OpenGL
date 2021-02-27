#include"skybox.h"

unsigned int skybox::load_cubemap()
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
			std::cout << "load texture error " << i << std::endl;
			std::cout << SOIL_last_result() << std::endl;
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

skybox::skybox()
{
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

	s = new Shader("s_v_shader.txt", "s_f_shader.txt");

	unsigned int textureID = load_cubemap();

	this->textureID = textureID;
	this->sky_VAO = sky_VAO;
}

void skybox::drawSkybox(glm::mat4 view,glm::mat4 projection)
{
	s->Use();

	//Pass the parameters to shader
	s->SetMat4("view", view);
	s->SetMat4("projection", projection);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindVertexArray(sky_VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

skybox::~skybox()
{
	glDeleteVertexArrays(1, &sky_VAO);
}