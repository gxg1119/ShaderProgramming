#include "stdafx.h"
#include "Renderer.h"
#include "LoadPng.h"
#include <Windows.h>
#include <cstdlib>
#include <cassert>

Renderer::Renderer(int windowSizeX, int windowSizeY)
{
	//default settings
	glClearDepth(1.f);

	Initialize(windowSizeX, windowSizeY);
}


Renderer::~Renderer()
{
}

void Renderer::Initialize(int windowSizeX, int windowSizeY)
{
	//Set window size
	m_WindowSizeX = windowSizeX;
	m_WindowSizeY = windowSizeY;

	//Load shaders
	m_SolidRectShader = CompileShaders("./Shaders/SolidRect.vs", "./Shaders/SolidRect.fs");

	m_Lecture3Shader = CompileShaders("./Shaders/lecture3.vs", "./Shaders/lecture3.fs");

	m_Lecture3ParitcleShader = CompileShaders("./Shaders/lecture3_particle.vs", "./Shaders/lecture3_particle.fs");

	m_ExamStarShader = CompileShaders("./Shaders/star.vs", "./Shaders/star.fs");

	m_FSSandboxShader = CompileShaders("./Shaders/FSSandbox.vs", "./Shaders/FSSandbox.fs");

	//Create VBOs
	CreateVertexBufferObjects();

	//Create Particles
	//CreateParticle(1000);

	//Initialize camera settings
	m_v3Camera_Position = glm::vec3(0.f, 0.f, 1000.f);
	m_v3Camera_Lookat = glm::vec3(0.f, 0.f, 0.f);
	m_v3Camera_Up = glm::vec3(0.f, 1.f, 0.f);
	m_m4View = glm::lookAt(
		m_v3Camera_Position,
		m_v3Camera_Lookat,
		m_v3Camera_Up
	);

	//Initialize projection matrix
	m_m4OrthoProj = glm::ortho(
		-(float)windowSizeX / 2.f, (float)windowSizeX / 2.f,
		-(float)windowSizeY / 2.f, (float)windowSizeY / 2.f,
		0.0001f, 10000.f);
	m_m4PersProj = glm::perspectiveRH(45.f, 1.f, 1.f, 1000.f);

	//Initialize projection-view matrix
	m_m4ProjView = m_m4OrthoProj * m_m4View; //use ortho at this time
	//m_m4ProjView = m_m4PersProj * m_m4View;

	//Initialize model transform matrix :; used for rotating quad normal to parallel to camera direction
	m_m4Model = glm::rotate(glm::mat4(1.0f), glm::radians(0.f), glm::vec3(1.f, 0.f, 0.f));
}

void Renderer::CreateVertexBufferObjects()
{
	float rect[]
		=
	{
		-0.5, -0.5, 0.f, -0.5, 0.5, 0.f, 0.5, 0.5, 0.f, //Triangle1
		-0.5, -0.5, 0.f,  0.5, 0.5, 0.f, 0.5, -0.5, 0.f, //Triangle2
	};

	glGenBuffers(1, &m_VBORect);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW);

	float lecture2[]
		=
	{
		0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0,
	}; // 9 floats array

	glGenBuffers(1, &m_VBOLecture2);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOLecture2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lecture2), lecture2, GL_STATIC_DRAW);

	float lecture3[]
		=
	{
		0.0, 0.0, 0.0, 1, 0, 0, 1,
		1.0, 0.0, 0.0, 0, 1, 0, 1,
		1.0, 1.0, 0.0, 0, 0, 1, 1,
	}; // 21 floats array

	glGenBuffers(1, &m_VBOLecture3);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOLecture3);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lecture3), lecture3, GL_STATIC_DRAW);

	float particleSize = 0.1f;
	float lecture3_singleParticle[]
		=
	{
		-particleSize, -particleSize, 0.0, 1, 1, 1, 1,
		 particleSize,  particleSize, 0.0, 1, 1, 1, 1,
		-particleSize,  particleSize, 0.0, 1, 1, 1, 1, //triangle 1

		-particleSize, -particleSize, 0.0, 1, 1, 1, 1,
		 particleSize, -particleSize, 0.0, 1, 1, 1, 1,
		 particleSize,  particleSize, 0.0, 1, 1, 1, 1, //triangle 1
	};

	glGenBuffers(1, &m_VBOSingleParitcleQuad);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOSingleParitcleQuad);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lecture3_singleParticle), lecture3_singleParticle, GL_STATIC_DRAW);

	float Exam[]
		=
	{
		0.0,  0.0, 0.0,
		0.25,  0.5, 0.0,
		0.0,  0.5, 0.0,

		0.0,  0.0, 0.0,
		0.37, 0.25, 0.0,
		0.25,  0.5, 0.0,

		0.0,  0.0, 0.0,
		0.5,  0.0, 0.0,
		0.37, 0.25, 0.0,
		
		0.0,   0.0, 0.0,
		0.37, -0.25, 0.0,
		0.5,   0.0, 0.0,
		
		0.0,    0.0, 0.0,
		0.25, -0.5, 0.0,
		0.37,  -0.25, 0.0,
		
		0.0,    0.0, 0.0,
		0.0,   -0.5, 0.0,
		0.25, -0.5, 0.0,
		
		0.0,    0.0, 0.0,
		-0.25, -0.5, 0.0,
		0.0,   -0.5, 0.0,
		
		0.0,    0.0, 0.0,
		-0.37, -0.25, 0.0,
		-0.25, -0.5, 0.0,
		
		0.0,    0.0, 0.0,
		-0.5,   0.0, 0.0,
		-0.37, -0.25, 0.0,
		
		0.0,    0.0, 0.0,
		-0.37, 0.25, 0.0,
		-0.5,   0.0, 0.0,
		
		0.0,    0.0, 0.0,
		-0.25,  0.5, 0.0,
		-0.37, 0.25, 0.0,
		
		0.0,    0.0, 0.0,
		0.0,  0.5, 0.0,
		-0.25,  0.5, 0.0,

	}; // 21 floats array

	glGenBuffers(1, &m_VBOExamStar);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOExamStar);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Exam), Exam, GL_STATIC_DRAW);

	float rectSize = 0.5f;
	float lecture4[]
		=
	{
		-rectSize, -rectSize, 0.0, 1, 1, 1, 1,
		 rectSize,  rectSize, 0.0, 1, 1, 1, 1,
		-rectSize,  rectSize, 0.0, 1, 1, 1, 1, //triangle 1

		-rectSize, -rectSize, 0.0, 1, 1, 1, 1,
		 rectSize, -rectSize, 0.0, 1, 1, 1, 1,
		 rectSize,  rectSize, 0.0, 1, 1, 1, 1, //triangle 1
	};

	glGenBuffers(1, &m_VBOSandbox);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOSandbox);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lecture4), lecture4, GL_STATIC_DRAW);
}

void Renderer::CreateParticle(int count)
{
	int floatCount = count * (3 + 3 + 1 + 1 + 1 +1 + 1 + 4) * 3 * 2; //(x, y, z, vx, vy, vz, emit, life , amp, period, value , R, G, B, A)
	float* particleVertices = new float[floatCount];
	int vertexCount = count * 3 * 2;

	int index = 0;
	float particleSize = 0.01f;
	for (int i = 0; i < count; i++)
	{
		float randomValueX = 0.f;
		float randomValueY = 0.f;
		float randomValueZ = 0.f;

		float randomValueVX = 0.f;
		float randomValueVY = 0.f;
		float randomValueVZ = 0.f;

		float randomEmitTime = 0.f;
		float randomLifeTime = 0.f;

		float randomAmp = 0.f;
		float randomPeriod = 0.f;

		float randomValue = 0.f;

		float randomR, randomG, randomB, randomA = 0.f;

		randomValueX = 0.f;//((float)rand() / (float)RAND_MAX - 0.5f) * 2.f; //-1~1
		randomValueY = 0.f;//((float)rand() / (float)RAND_MAX - 0.5f) * 2.f; //-1~1
		randomValueZ = 0.f;

		randomValueVX = ((float)rand() / (float)RAND_MAX - 0.5f) * 0.1f; //-1~1
		randomValueVY = ((float)rand() / (float)RAND_MAX - 0.5f) * 0.1f; //-1~1
		randomValueVZ = 0.f;

		randomEmitTime = ((float)rand() / (float)RAND_MAX) * 5.f;
		randomLifeTime = ((float)rand() / (float)RAND_MAX) * 2.0f;

		randomAmp = ((float)rand() / (float)RAND_MAX) * 0.4f - 0.2f;
		randomPeriod = ((float)rand() / (float)RAND_MAX) * 2.f;

		randomValue = ((float)rand() / (float)RAND_MAX) * 1.f;

		randomR = ((float)rand() / (float)RAND_MAX) * 1.f;
		randomG = ((float)rand() / (float)RAND_MAX) * 1.f;
		randomB = ((float)rand() / (float)RAND_MAX) * 1.f;
		randomA = 1.f;

		//v0
		particleVertices[index] = -particleSize / 2.f + randomValueX;
		index++;
		particleVertices[index] = -particleSize / 2.f + randomValueY;
		index++;
		particleVertices[index] = 0.f;
		index++; //Position XYZ
		particleVertices[index] = randomValueVX;
		index++;
		particleVertices[index] = randomValueVY;
		index++;
		particleVertices[index] = 0.f;
		index++; //Velocity XYZ
		particleVertices[index] = randomEmitTime;
		index++; //EmitTime
		particleVertices[index] = randomLifeTime;
		index++; //LifeTime
		particleVertices[index] = randomAmp;
		index++; //Amp
		particleVertices[index] = randomPeriod;
		index++; //Period
		particleVertices[index] = randomValue;
		index++; //Value
		particleVertices[index] = randomR;
		index++; //R
		particleVertices[index] = randomG;
		index++; //G
		particleVertices[index] = randomB;
		index++; //B
		particleVertices[index] = randomA;
		index++; //A

		//v1
		particleVertices[index] = particleSize / 2.f + randomValueX;
		index++;
		particleVertices[index] = -particleSize / 2.f + randomValueY;
		index++;
		particleVertices[index] = 0.f;
		index++; //Position XYZ
		particleVertices[index] = randomValueVX;
		index++;
		particleVertices[index] = randomValueVY;
		index++;
		particleVertices[index] = 0.f;
		index++; //Velocity XYZ
		particleVertices[index] = randomEmitTime;
		index++; //EmitTime
		particleVertices[index] = randomLifeTime;
		index++; //LifeTime
		particleVertices[index] = randomAmp;
		index++; //Amp
		particleVertices[index] = randomPeriod;
		index++; //Period
		particleVertices[index] = randomValue;
		index++; //Value
		particleVertices[index] = randomR;
		index++; //R
		particleVertices[index] = randomG;
		index++; //G
		particleVertices[index] = randomB;
		index++; //B
		particleVertices[index] = randomA;
		index++; //A

		//v2
		particleVertices[index] = particleSize / 2.f + randomValueX;
		index++;
		particleVertices[index] = particleSize / 2.f + randomValueY;
		index++;
		particleVertices[index] = 0.f;
		index++; //Position XYZ
		particleVertices[index] = randomValueVX;
		index++;
		particleVertices[index] = randomValueVY;
		index++;
		particleVertices[index] = 0.f;
		index++; //Velocity XYZ
		particleVertices[index] = randomEmitTime;
		index++; //EmitTime
		particleVertices[index] = randomLifeTime;
		index++; //LifeTime
		particleVertices[index] = randomAmp;
		index++; //Amp
		particleVertices[index] = randomPeriod;
		index++; //Period
		particleVertices[index] = randomValue;
		index++; //Value
		particleVertices[index] = randomR;
		index++; //R
		particleVertices[index] = randomG;
		index++; //G
		particleVertices[index] = randomB;
		index++; //B
		particleVertices[index] = randomA;
		index++; //A

		//v3
		particleVertices[index] = -particleSize / 2.f + randomValueX;
		index++;
		particleVertices[index] = -particleSize / 2.f + randomValueY;
		index++;
		particleVertices[index] = 0.f;
		index++; //Position XYZ
		particleVertices[index] = randomValueVX;
		index++;
		particleVertices[index] = randomValueVY;
		index++;
		particleVertices[index] = 0.f;
		index++; //Velocity XYZ
		particleVertices[index] = randomEmitTime;
		index++; //EmitTime
		particleVertices[index] = randomLifeTime;
		index++; //LifeTime
		particleVertices[index] = randomAmp;
		index++; //Amp
		particleVertices[index] = randomPeriod;
		index++; //Period
		particleVertices[index] = randomValue;
		index++; //Value
		particleVertices[index] = randomR;
		index++; //R
		particleVertices[index] = randomG;
		index++; //G
		particleVertices[index] = randomB;
		index++; //B
		particleVertices[index] = randomA;
		index++; //A

		//v4
		particleVertices[index] = particleSize / 2.f + randomValueX;
		index++;
		particleVertices[index] = particleSize / 2.f + randomValueY;
		index++;
		particleVertices[index] = 0.f;
		index++; //Position XYZ
		particleVertices[index] = randomValueVX;
		index++;
		particleVertices[index] = randomValueVY;
		index++;
		particleVertices[index] = 0.f;
		index++; //Velocity XYZ
		particleVertices[index] = randomEmitTime;
		index++; //EmitTime
		particleVertices[index] = randomLifeTime;
		index++; //LifeTime
		particleVertices[index] = randomAmp;
		index++; //Amp
		particleVertices[index] = randomPeriod;
		index++; //Period
		particleVertices[index] = randomValue;
		index++; //Value
		particleVertices[index] = randomR;
		index++; //R
		particleVertices[index] = randomG;
		index++; //G
		particleVertices[index] = randomB;
		index++; //B
		particleVertices[index] = randomA;
		index++; //A

		//v5
		particleVertices[index] = -particleSize / 2.f + randomValueX;
		index++;
		particleVertices[index] = particleSize / 2.f + randomValueY;
		index++;
		particleVertices[index] = 0.f;
		index++; //Position XYZ
		particleVertices[index] = randomValueVX;
		index++;
		particleVertices[index] = randomValueVY;
		index++;
		particleVertices[index] = 0.f;
		index++; //Velocity XYZ
		particleVertices[index] = randomEmitTime;
		index++; //EmitTime
		particleVertices[index] = randomLifeTime;
		index++; //LifeTime
		particleVertices[index] = randomAmp;
		index++; //Amp
		particleVertices[index] = randomPeriod;
		index++; //Period
		particleVertices[index] = randomValue;
		index++; //Value
		particleVertices[index] = randomR;
		index++; //R
		particleVertices[index] = randomG;
		index++; //G
		particleVertices[index] = randomB;
		index++; //B
		particleVertices[index] = randomA;
		index++; //A
	}

	glGenBuffers(1, &m_VBOManyParticle);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOManyParticle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * floatCount, particleVertices, GL_STATIC_DRAW);
	m_VBOManyParticleVertexCount = vertexCount;

	delete[] particleVertices;
}

void Renderer::AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	//?????? ???????? ????
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
	}

	const GLchar* p[1];
	p[0] = pShaderText;
	GLint Lengths[1];
	Lengths[0] = (GLint)strlen(pShaderText);
	//?????? ?????? ?????? ?????????? ????
	glShaderSource(ShaderObj, 1, p, Lengths);

	//?????? ?????? ?????? ??????
	glCompileShader(ShaderObj);

	GLint success;
	// ShaderObj ?? ?????????? ?????? ???????? ????
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];

		//OpenGL ?? shader log ???????? ??????
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		printf("%s \n", pShaderText);
	}

	// ShaderProgram ?? attach!!
	glAttachShader(ShaderProgram, ShaderObj);
}

bool Renderer::ReadFile(char* filename, std::string *target)
{
	std::ifstream file(filename);
	if (file.fail())
	{
		std::cout << filename << " file loading failed.. \n";
		file.close();
		return false;
	}
	std::string line;
	while (getline(file, line)) {
		target->append(line.c_str());
		target->append("\n");
	}
	return true;
}

GLuint Renderer::CompileShaders(char* filenameVS, char* filenameFS)
{
	GLuint ShaderProgram = glCreateProgram(); //?? ?????? ???????? ????

	if (ShaderProgram == 0) { //?????? ?????????? ???????????? ????
		fprintf(stderr, "Error creating shader program\n");
	}

	std::string vs, fs;

	//shader.vs ?? vs ?????? ??????
	if (!ReadFile(filenameVS, &vs)) {
		printf("Error compiling vertex shader\n");
		return -1;
	};

	//shader.fs ?? fs ?????? ??????
	if (!ReadFile(filenameFS, &fs)) {
		printf("Error compiling fragment shader\n");
		return -1;
	};

	// ShaderProgram ?? vs.c_str() ?????? ???????? ???????? ?????? attach??
	AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);

	// ShaderProgram ?? fs.c_str() ?????????? ???????? ???????? ?????? attach??
	AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	//Attach ?????? shaderProgram ?? ??????
	glLinkProgram(ShaderProgram);

	//?????? ?????????? ????
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);

	if (Success == 0) {
		// shader program ?????? ??????
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << filenameVS << ", " << filenameFS << " Error linking shader program\n" << ErrorLog;
		return -1;
	}

	glValidateProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << filenameVS << ", " << filenameFS << " Error validating shader program\n" << ErrorLog;
		return -1;
	}

	glUseProgram(ShaderProgram);
	std::cout << filenameVS << ", " << filenameFS << " Shader compiling is done.\n";

	return ShaderProgram;
}

unsigned char * Renderer::loadBMPRaw(const char * imagepath, unsigned int& outWidth, unsigned int& outHeight)
{
	std::cout << "Loading bmp file " << imagepath << " ... " << std::endl;
	outWidth = -1;
	outHeight = -1;
	// Data read from the header of the BMP file
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int imageSize;
	// Actual RGB data
	unsigned char * data;

	// Open the file
	FILE * file = NULL;
	fopen_s(&file, imagepath, "rb");
	if (!file)
	{
		std::cout << "Image could not be opened, " << imagepath << " is missing. " << std::endl;
		return NULL;
	}

	if (fread(header, 1, 54, file) != 54)
	{
		std::cout << imagepath << " is not a correct BMP file. " << std::endl;
		return NULL;
	}

	if (header[0] != 'B' || header[1] != 'M')
	{
		std::cout << imagepath << " is not a correct BMP file. " << std::endl;
		return NULL;
	}

	if (*(int*)&(header[0x1E]) != 0)
	{
		std::cout << imagepath << " is not a correct BMP file. " << std::endl;
		return NULL;
	}

	if (*(int*)&(header[0x1C]) != 24)
	{
		std::cout << imagepath << " is not a correct BMP file. " << std::endl;
		return NULL;
	}

	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	outWidth = *(int*)&(header[0x12]);
	outHeight = *(int*)&(header[0x16]);

	if (imageSize == 0)
		imageSize = outWidth * outHeight * 3;

	if (dataPos == 0)
		dataPos = 54;

	data = new unsigned char[imageSize];

	fread(data, 1, imageSize, file);

	fclose(file);

	std::cout << imagepath << " is succesfully loaded. " << std::endl;

	return data;
}

GLuint Renderer::CreatePngTexture(char * filePath)
{
	//Load Pngs: Load file and decode image.
	std::vector<unsigned char> image;
	unsigned width, height;
	unsigned error = lodepng::decode(image, width, height, filePath);
	if (error != 0)
	{
		lodepng_error_text(error);
		assert(error == 0);
		return -1;
	}

	GLuint temp;
	glGenTextures(1, &temp);

	glBindTexture(GL_TEXTURE_2D, temp);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);

	return temp;
}

GLuint Renderer::CreateBmpTexture(char * filePath)
{
	//Load Bmp: Load file and decode image.
	unsigned int width, height;
	unsigned char * bmp
		= loadBMPRaw(filePath, width, height);

	if (bmp == NULL)
	{
		std::cout << "Error while loading bmp file : " << filePath << std::endl;
		assert(bmp != NULL);
		return -1;
	}

	GLuint temp;
	glGenTextures(1, &temp);

	glBindTexture(GL_TEXTURE_2D, temp);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bmp);

	return temp;
}

void Renderer::Test()
{
	glUseProgram(m_SolidRectShader);

	int attribPosition = glGetAttribLocation(m_SolidRectShader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);
}

void Renderer::Lecture2()
{
	glUseProgram(m_SolidRectShader);

	int attribPosition = glGetAttribLocation(m_SolidRectShader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOLecture2);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glDisableVertexAttribArray(attribPosition);
}

float gTime = 1.f;
float DT = 0.0001f;
void Renderer::Lecture3()
{
	GLuint shader = m_Lecture3Shader;
	glUseProgram(shader);

	int attribPosition = glGetAttribLocation(shader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOLecture3);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 7, 0);

	int attribColor = glGetAttribLocation(shader, "a_Color");
	glEnableVertexAttribArray(attribColor);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOLecture3);
	glVertexAttribPointer(attribColor, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 7, (GLvoid*)(sizeof(float) * 3));

	int uniformLocTime = glGetUniformLocation(shader, "u_Time");
	glUniform1f(uniformLocTime, gTime);

	int uniformLocColor = glGetUniformLocation(shader, "u_Color"); // per shader
	glUniform4f(uniformLocColor, 1, 1, 1, 1);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	gTime -= 0.01f;
	if (gTime < 0.f) gTime = 1.0f;

	glDisableVertexAttribArray(attribPosition);
}

void Renderer::Lecture3_Paritcle()
{
	GLuint shader = m_Lecture3ParitcleShader;
	glUseProgram(shader);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBOManyParticle);

	int attribPosition = glGetAttribLocation(shader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 15, 0);

	int attribVelocity = glGetAttribLocation(shader, "a_Velocity");
	glEnableVertexAttribArray(attribVelocity);
	glVertexAttribPointer(attribVelocity, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 15, (GLvoid*)(sizeof(float) * 3));

	int attribEmitTime = glGetAttribLocation(shader, "a_EmitTime");
	glEnableVertexAttribArray(attribEmitTime);
	glVertexAttribPointer(attribEmitTime, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 15, (GLvoid*)(sizeof(float) * 6));

	int attribLifeTime = glGetAttribLocation(shader, "a_LifeTime");
	glEnableVertexAttribArray(attribLifeTime);
	glVertexAttribPointer(attribLifeTime, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 15, (GLvoid*)(sizeof(float) * 7));

	int attribAmp = glGetAttribLocation(shader, "a_Amp");
	glEnableVertexAttribArray(attribAmp);
	glVertexAttribPointer(attribAmp, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 15, (GLvoid*)(sizeof(float) * 8));

	int attribPeriod = glGetAttribLocation(shader, "a_Period");
	glEnableVertexAttribArray(attribPeriod);
	glVertexAttribPointer(attribPeriod, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 15, (GLvoid*)(sizeof(float) * 9));

	int attribRamdomValue = glGetAttribLocation(shader, "a_RamdomValue");
	glEnableVertexAttribArray(attribRamdomValue);
	glVertexAttribPointer(attribRamdomValue, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 15, (GLvoid*)(sizeof(float) * 10));

	int attribRamdomColor = glGetAttribLocation(shader, "a_Color");
	glEnableVertexAttribArray(attribRamdomColor);
	glVertexAttribPointer(attribRamdomColor, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 15, (GLvoid*)(sizeof(float) * 11));

	int uniformTime = glGetUniformLocation(shader, "u_Time");
	glUniform1f(uniformTime, gTime);

	int uniformAccel = glGetUniformLocation(shader, "u_Accel");
	glUniform3f(uniformAccel, 0.0, 0.0, 0.0);

	gTime += 0.001;
	if (gTime > 6.f) gTime = 0.f;

	glDrawArrays(GL_TRIANGLES, 0, m_VBOManyParticleVertexCount);
	glDisableVertexAttribArray(attribPosition);
}

void Renderer::ExamStar()
{
	GLuint shader = m_ExamStarShader;

	glUseProgram(shader);

	int attribPosition = glGetAttribLocation(shader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOExamStar);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	int uniformLocTime = glGetUniformLocation(shader, "u_Time");
	glUniform1f(uniformLocTime, gTime);

	glDrawArrays(GL_TRIANGLES, 0, 36);

	gTime -= DT;
	if (gTime < 0.4f) DT *= -1;
	if(gTime > 1.f)DT *= -1;

	glDisableVertexAttribArray(attribPosition);

}

void Renderer::Lecture4_FSSandbox()
{
	GLuint shader = m_FSSandboxShader;
	glUseProgram(shader);

	int attribPosition = glGetAttribLocation(shader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOSandbox);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 7, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);
}
