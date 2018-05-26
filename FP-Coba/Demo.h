#pragma once
#include "RenderEngine.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <SOIL/SOIL.h>

class Demo :
	public RenderEngine
{
public:
	Demo();
	~Demo();
private:
	GLuint shaderProgram, VBO, VAO, EBO, texture, VBO2, VAO2, EBO2,
		texture2, textureAtas, atas, textureTop, top, textureKakiR, kakiR, 
		textureKakiL, kakiL, textureArmR, armR, textureArmL, armL, textureEkor, ekor, textureBawah, bawah, //Godzilla
		textureTank, tank, textureATank, atank, textureLaras, laras, //Tank 
		textureBatang, batang, textureDahan1, dahan1, textureDahan2, dahan2, textureDahan3, dahan3, textureDahan4, dahan4, //Pohon
		textureGedung, gedung; //gedung
	float viewCamX, viewCamY, viewCamZ, upCamX, upCamY, upCamZ, posCamX, posCamY, 
		posCamZ, CAMERA_SPEED, fovy;

	float angle = 0;
	double pindahX = 0, pindahY = 0, pindahZ = 0;

	virtual void Init();
	virtual void DeInit();
	virtual void Update(double deltaTime);
	virtual void Render();
	virtual void ProcessInput(GLFWwindow *window);
	
	//Godzilla
	void BuildCubeG();
	void BuildCubeTop();
	void BuildCubeKepala();
	void BuildHandR();
	void BuildHandL();
	void BuildKakiR();
	void BuildKakiL();
	void BuildEkor();
	void BuildPlane();

	void DrawCubeG();
	void DrawCubeTop();
	void DrawCubeKepala();
	void DrawHandR();
	void DrawHandL();
	void DrawKakiR();
	void DrawKakiL();
	void DrawEkor();
	void DrawPlane();

	//Tank

	void BuildCubeT();
	void BuildLaras();
	void BuildCubeKepalaT();
	
	void DrawCubeT();
	void DrawCubeKepalaT();
	void DrawLaras();


	//Gedung

	void BuildCubeGedung();

	void DrawCubeGedung();

	//Pohon
	void BuildBatang();
	void BuildDahan1();
	void BuildDahan2();
	void BuildDahan3();
	void BuildDahan4();

	void DrawBatang();
	void DrawDahan1();
	void DrawDahan2();
	void DrawDahan3();
	void DrawDahan4();

	void MoveCamera(float speed);
	void StrafeCamera(float speed);
	void RotateCamera(float speed);
	void InitCamera();
};

