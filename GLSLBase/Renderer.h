#pragma once

#include <string>
#include <cstdlib>
#include <fstream>
#include <iostream>

#include "Dependencies\glew.h"
#include "Dependencies\wglew.h"
#include "Dependencies\glm/glm.hpp"
#include "Dependencies\glm/gtc/matrix_transform.hpp"
#include "Dependencies\glm/gtx/euler_angles.hpp"

class Renderer
{
public:
	Renderer(int windowSizeX, int windowSizeY);
	~Renderer();

	GLuint CreatePngTexture(char * filePath);
	GLuint CreateBmpTexture(char * filePath);
	   
	void Test();
	void Lecture2();
	void Lecture3();
	void Lecture3_Paritcle();
	void ExamStar();
	void Lecture4_FSSandbox();

private:
	void Initialize(int windowSizeX, int windowSizeY);
	bool ReadFile(char* filename, std::string *target);
	void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType);
	GLuint CompileShaders(char* filenameVS, char* filenameFS);
	void CreateVertexBufferObjects(); 
	unsigned char * Renderer::loadBMPRaw(const char * imagepath, unsigned int& outWidth, unsigned int& outHeight);
	void CreateParticle(int count);
	bool m_Initialized = false;
	
	unsigned int m_WindowSizeX = 0;
	unsigned int m_WindowSizeY = 0;

	//camera position
	glm::vec3 m_v3Camera_Position;
	//camera lookat position
	glm::vec3 m_v3Camera_Lookat;
	//camera up vector
	glm::vec3 m_v3Camera_Up;

	glm::mat4 m_m4OrthoProj;
	glm::mat4 m_m4PersProj;
	glm::mat4 m_m4Model;
	glm::mat4 m_m4View;
	glm::mat4 m_m4ProjView;

	GLuint m_VBORect = 0;
	GLuint m_SolidRectShader = 0;

	GLuint m_VBOLecture2 = 0;
	GLuint m_VBOLecture3 = 0;
	GLuint m_VBOSingleParitcleQuad = 0;

	GLuint m_Lecture3Shader = 0;
	GLuint m_Lecture3ParitcleShader = 0;

	GLuint m_VBOManyParticle = 0;
	GLuint m_VBOManyParticleVertexCount = 0;

	GLuint m_VBOExamStar = 0;
	GLuint m_ExamStarShader = 0;

	GLuint m_VBOSandbox = 0;
	GLuint m_FSSandboxShader = 0;
};
