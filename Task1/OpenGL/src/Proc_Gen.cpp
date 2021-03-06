#include "Proc_Gen.h"


#include <gl_core_4_4.h>
#include <GLFW/glfw3.h>

#include "ShaderLoader.h"
#include "Camera.h"
#include <stb-master/stb_image.h>
ProcGen::ProcGen(unsigned int _rows, unsigned int _cols, GLFWwindow *_pWindow)
{
	m_pWindow = _pWindow;
	m_rows = _rows;
	m_cols = _cols;

	m_seed = 100.0f;

	m_specPow = 128;

	m_lightDir = glm::vec3(1, 1, 0);
	m_lightCol = glm::vec3(1, 1, 1);

	m_amplitude = 1.0f;
	m_persistence = 0.2f;

	StartUp();
}

ProcGen::~ProcGen()
{

}

void ProcGen::StartUp()
{
	GenerateGrid(m_rows, m_cols);
	CreateShaders();
	GenerateNoise();
}

void ProcGen::GenerateGrid( unsigned int rows, unsigned int cols )
{
	ProcGenVertex* aoVertices = new ProcGenVertex[ rows * cols ];

	for ( unsigned int r = 0 ; r < rows ; ++r ) 
	{
		for ( unsigned int c = 0 ; c < cols ; ++c ) 
		{
			aoVertices[ r * cols + c ].position = glm::vec4((float)c, 0, (float)r, 1);
			aoVertices[r*cols+c].texcoord = glm::vec2((float)c / cols, (float)r / rows);
			//set colour here
			aoVertices[ r * cols + c ].colour = glm::vec4( 1, 1, 1, 1 );
		}
	}

	// defining index count based off quad count (2 triangles per quad)
	unsigned int* auiIndices = new unsigned int[ (rows - 1) * (cols - 1) * 6 ];
	unsigned int index = 0;
	for ( unsigned int r = 0 ; r < (rows - 1) ; ++r ) 
	{
		for ( unsigned int c = 0 ; c < (cols - 1) ; ++c ) 
		{
			// triangle 1
			auiIndices[ index++ ] = r * cols + c;
			auiIndices[ index++ ] = (r + 1) * cols + c;
			auiIndices[ index++ ] = (r + 1) * cols + (c + 1);
			// triangle 2
			auiIndices[ index++ ] = r * cols + c;
			auiIndices[ index++ ] = (r + 1) * cols + (c + 1);
			auiIndices[ index++ ] = r * cols + (c + 1);
		}
	}

	// create and bind buffers to a vertex array object
	glGenBuffers(1, &m_VBO);	
	glGenBuffers(1, &m_IBO);
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray( m_VAO );
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);

	glBufferData(GL_ARRAY_BUFFER, (rows * cols) * sizeof(ProcGenVertex), aoVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(ProcGenVertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(ProcGenVertex), (void*)(sizeof(glm::vec4)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(ProcGenVertex), (void*)(sizeof(glm::vec4) * 2 ));
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(ProcGenVertex), (void*)(sizeof(glm::vec4) * 2 + sizeof(glm::vec2)));

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (rows - 1) * (cols - 1) * 6 *	sizeof(unsigned int), auiIndices, GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

	// we�ll do more here soon!
	delete[] aoVertices;
	delete[] auiIndices;
}

void ProcGen::CreateShaders()
{
	int success = GL_FALSE;

	unsigned int vertexShader = LoadShaderProgram(GL_VERTEX_SHADER,		"./shaderProg/Proc_Gen.vert");
	unsigned int fragmentShader = LoadShaderProgram(GL_FRAGMENT_SHADER,	"./shaderProg/Proc_Gen.frag");
	//unsigned int geometryShader = LoadShaderProgram(GL_GEOMETRY_SHADER, "./shaderProg/Proc_Gen.geom");

	m_programID = glCreateProgram();
	glAttachShader(m_programID, vertexShader);
	glAttachShader(m_programID, fragmentShader);
	//glAttachShader(m_programID, geometryShader);

	glLinkProgram(m_programID);
	glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
	if (success == GL_FALSE) 
	{
		int infoLogLength = 0;
		glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];
		glGetProgramInfoLog(m_programID, infoLogLength, 0, infoLog);
		printf("Error: Failed to link shader program!\n");
		printf("%s\n",infoLog);
		delete[] infoLog;
	}

	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);
	//glDeleteShader(geometryShader);
}

void ProcGen::Update(float _dt)
{
	if(m_pWindow != nullptr && glfwGetKey(m_pWindow, GLFW_KEY_R) == GLFW_PRESS)
	{
		GenerateNoise();
	}
}

void ProcGen::GenerateNoise()
{
	int dims = m_rows;
	float *perlin_data = new float[dims * dims];
	float scale = (1.0f / dims) * 3;
	int octaves = 6;

	for ( int x = 0 ; x < dims ; ++x)
	{
		for ( int y = 0 ; y < dims; ++y)
		{
			
			m_amplitudeCurr = m_amplitude;
			m_persistenceCurr = m_persistence;

			perlin_data[y * dims + x] = 0;
			
			for (int o = 0; o < octaves; ++o)
			{
				float freq = powf(2, (float)o);
				float perlin_sample = glm::perlin(glm::vec3((float)x, (float)y, m_seed) * scale * freq) * 0.5f + 0.5f;
				perlin_data[y * dims + x] += perlin_sample * m_amplitudeCurr;
				m_amplitudeCurr *= m_persistenceCurr;
			} 
		}
	}

	glGenTextures(1, &m_perlin_texture);
	glBindTexture(GL_TEXTURE_2D, m_perlin_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, dims, dims, 0, GL_RED, GL_FLOAT, perlin_data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	LoadTextures();
}

void ProcGen::Draw(Camera* _pCamera)
{
	glUseProgram(m_programID);
	unsigned int projectionViewUniform = glGetUniformLocation(m_programID,"ProjectionView");
	glUniformMatrix4fv(projectionViewUniform, 1, false,	glm::value_ptr(_pCamera->GetProjectionView()));


	unsigned int loc = glGetUniformLocation(m_programID, "perlin_texture");
	glUniform1i(loc, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_perlin_texture);
	
	loc = glGetUniformLocation(m_programID, "rock_texture");
	glUniform1i(loc, 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_rockTexture);

	loc = glGetUniformLocation(m_programID, "dirtGrass_texture");
	glUniform1i(loc, 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_dirtGrassTexture);

	loc = glGetUniformLocation(m_programID, "water_texture");
	glUniform1i(loc, 3);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, m_waterTexture);

	loc = glGetUniformLocation(m_programID, "snow_texture");
	glUniform1i(loc, 4);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, m_snowTexture);

	//Light stuff
		loc = glGetUniformLocation(m_programID, "diffuse");
		glUniform1i(loc, 5);//???

		unsigned int lightDirection = glGetUniformLocation(m_programID, "LightDir");
		glUniform3f(lightDirection, m_lightDir.x, m_lightDir.y, m_lightDir.z);

		//set light colour
		unsigned int lightColour = glGetUniformLocation(m_programID, "LightColour");
		glUniform3f(lightColour, m_lightCol.x, m_lightCol.y, m_lightCol.z);

		//Put it thar plz
		unsigned int ambientLight = glGetUniformLocation(m_programID, "AmbientColour");
		glUniform3f(ambientLight, m_ambientColour.x, m_ambientColour.y, m_ambientColour.z);

		unsigned int cameraPosLocation = glGetUniformLocation(m_programID, "CameraPos");
		glm::vec3 camLocation = _pCamera->GetPosition();
		glUniform3f(cameraPosLocation, camLocation.x, camLocation.y, camLocation.z);

		//set specpower
		unsigned int specPower = glGetUniformLocation(m_programID, "SpecPow");
		glUniform1f(specPower, m_specPow);

	glBindVertexArray(m_VAO);

	unsigned int indexCount = (m_rows - 1) * (m_cols - 1) * 6;
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
}

void ProcGen::LoadTextures()
{
	int imageWidth = 0, imageHeight = 0, imageFormat = 0;
	unsigned char* data;

	//Rocky Ground
	//----------------------------------------------------------------------------
		//Load diffuse map
		data = stbi_load("./data/MyTextures/rock.jpg", &imageWidth, &imageHeight, &imageFormat, STBI_default);
		//gen texture
		glGenTextures(1, &m_rockTexture);
		//bind
		glBindTexture(GL_TEXTURE_2D, m_rockTexture);
		//put in data
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		
		//freeing the data
		stbi_image_free(data);
	//----------------------------------------------------------------------------
	
	//Dirt Grass
	//----------------------------------------------------------------------------
		//Load diffuse map
		data = stbi_load("./data/MyTextures/grass.jpg", &imageWidth, &imageHeight, &imageFormat, STBI_default);

		//gen texture
		glGenTextures(1, &m_dirtGrassTexture);
		//bind
		glBindTexture(GL_TEXTURE_2D, m_dirtGrassTexture);
		//put in data
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		
		//freeing the data
		stbi_image_free(data);
	//----------------------------------------------------------------------------

	//Water Grass
	//----------------------------------------------------------------------------
		//Load diffuse map
		data = stbi_load("./data/MyTextures/water.jpg", &imageWidth, &imageHeight, &imageFormat, STBI_default);

		//gen texture
		glGenTextures(1, &m_waterTexture);
		//bind
		glBindTexture(GL_TEXTURE_2D, m_waterTexture);
		//put in data
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		//freeing the data
		stbi_image_free(data);
	//----------------------------------------------------------------------------
	
	//Snow Grass
	//----------------------------------------------------------------------------
		//Load diffuse map
		data = stbi_load("./data/MyTextures/snow.jpg", &imageWidth, &imageHeight, &imageFormat, STBI_default);

		//gen texture
		glGenTextures(1, &m_snowTexture);
		//bind
		glBindTexture(GL_TEXTURE_2D, m_snowTexture);
		//put in data
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		//freeing the data
		stbi_image_free(data);
	//----------------------------------------------------------------------------
}