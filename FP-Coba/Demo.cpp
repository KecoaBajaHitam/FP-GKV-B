#include "Demo.h"

Demo::Demo() {
}

Demo::~Demo() {
}

void Demo::Init() {
	// build and compile our shader program
	// ------------------------------------
	shaderProgram = BuildShader("vertexShader.vert", "fragmentShader.frag", nullptr);

	//Godzilla
	BuildCubeG();
	BuildCubeKepala();
	BuildCubeTop();
	BuildHandR();
	BuildHandL();
	BuildKakiR();
	BuildKakiL();
	BuildEkor();

	//Tank
	BuildCubeT();
	BuildCubeKepalaT();
	BuildLaras();

	//Gedung
	BuildCubeGedung();

	//Pohon
	BuildBatang();
	BuildDahan1();
	BuildDahan2();
	BuildDahan3();
	BuildDahan4();

	BuildPlane();

	InitCamera();
}

void Demo::DeInit() {
	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void Demo::ProcessInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		pindahY += 10 * GetDeltaTime();
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		pindahY -= 10 * GetDeltaTime();
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		pindahX += 10 * GetDeltaTime();
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		pindahX -= 10 * GetDeltaTime();
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		pindahZ += 10 * GetDeltaTime();
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		pindahZ -= 10 * GetDeltaTime();
	}
	// zoom camera
	// -----------
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		if (fovy < 90) {
			fovy += 0.0001f;
		}
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		if (fovy > 0) {
			fovy -= 0.0001f;
		}
	}

	// update camera movement 
	// -------------
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
		MoveCamera(CAMERA_SPEED);
	}
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
		MoveCamera(-CAMERA_SPEED);
	}

	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
		StrafeCamera(-CAMERA_SPEED);
	}

	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
		StrafeCamera(CAMERA_SPEED);
	}

	// update camera rotation
	// ----------------------
	double mouseX, mouseY;
	double midX = screenWidth / 2;
	double midY = screenHeight / 2;
	float angleY = 0.0f;
	float angleZ = 0.0f;

	// Get mouse position
	glfwGetCursorPos(window, &mouseX, &mouseY);
	if ((mouseX == midX) && (mouseY == midY)) {
		return;
	}

	// Set mouse position
	glfwSetCursorPos(window, midX, midY);

	// Get the direction from the mouse cursor, set a resonable maneuvering speed
	angleY = (float)((midX - mouseX)) / 1000;
	angleZ = (float)((midY - mouseY)) / 1000;

	// The higher the value is the faster the camera looks around.
	viewCamY += angleZ * 2;

	// limit the rotation around the x-axis
	if ((viewCamY - posCamY) > 8) {
		viewCamY = posCamY + 8;
	}
	if ((viewCamY - posCamY) < -8) {
		viewCamY = posCamY - 8;
	}
	RotateCamera(-angleY);

}

void Demo::Update(double deltaTime) {
	//angle += (float) ((deltaTime * 1.5f) / 1000);

}

void Demo::Render() {
	glViewport(0, 0, this->screenWidth, this->screenHeight);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_DEPTH_TEST);

	// Pass perspective projection matrix
	glm::mat4 projection = glm::perspective(fovy, (GLfloat)this->screenWidth / (GLfloat)this->screenHeight, 0.1f, 100.0f);
	GLint projLoc = glGetUniformLocation(this->shaderProgram, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// LookAt camera (position, target/direction, up)
	glm::mat4 view = glm::lookAt(glm::vec3(posCamX, posCamY, posCamZ), glm::vec3(viewCamX, viewCamY, viewCamZ), glm::vec3(upCamX, upCamY, upCamZ));

	//glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 15), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	GLint viewLoc = glGetUniformLocation(this->shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	//Godzilla
	DrawCubeG();
	DrawCubeKepala();
	DrawCubeTop();
	DrawHandL();
	DrawHandR();
	DrawKakiR();
	DrawKakiL();
	DrawEkor();

	//Tank
	DrawCubeT();
	DrawCubeKepalaT();
	DrawLaras();

	//Gedung
	DrawCubeGedung();

	//Pohon
	DrawBatang();
	DrawDahan1();
	DrawDahan2();
	DrawDahan3();
	DrawDahan4();

	DrawPlane();

	glDisable(GL_DEPTH_TEST);
}
//Godzilla
void Demo::BuildCubeG() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("BadanGodzilla.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// front
		-0.5, -0.5, 0.5, 0, 0,  // 0
		0.5, -0.5, 0.5, 0.3, 0.3,   // 1
		0.5,  0.5, 0.5, 0.3, 0.3,   // 2
		-0.5,  0.5, 0.5, 0.3, 0.3,  // 3

		// right
		0.5,  0.5,  0.5, 0, 0,  // 4
		0.5,  0.5, -0.5, 0.3, 0.3,  // 5
		0.5, -0.5, -0.5, 0.3, 0.3,  // 6
		0.5, -0.5,  0.5, 0.3, 0.3,  // 7

		// back
		-0.5, -0.5, -0.5, 0, 0, // 8 
		0.5,  -0.5, -0.5, 0.3, 0.3, // 9
		0.5,   0.5, -0.5, 0.3, 0.3, // 10
		-0.5,  0.5, -0.5, 0.3, 0.3, // 11

		// left
		-0.5, -0.5, -0.5, 0, 0, // 12
		-0.5, -0.5,  0.5, 0.3, 0.3, // 13
		-0.5,  0.5,  0.5, 0.3, 0.3, // 14
		-0.5,  0.5, -0.5, 0.3, 0.3, // 15

		// upper
		0.5, 0.5,  0.5, 0, 0,   // 16
		-0.5, 0.5,  0.5, 0.3, 0.3,  // 17
		-0.5, 0.5, -0.5, 0.3, 0.3,  // 18
		0.5, 0.5, -0.5, 0.3, 0.3,   // 19

		// bottom
		-0.5, -0.5, -0.5, 0, 0, // 20
		0.5, -0.5, -0.5, 0.3, 0.3,  // 21
		0.5, -0.5,  0.5, 0.3, 0.3,  // 22
		-0.5, -0.5,  0.5, 0.3, 0.3, // 23
	};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22   // bottom
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawCubeG()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(pindahX, pindahY, pindahZ));

	model = glm::rotate(model, 0.0f, glm::vec3(0, 1, 0));

	model = glm::scale(model, glm::vec3(2, 4, 3));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildHandR() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &textureArmR);
	glBindTexture(GL_TEXTURE_2D, textureArmR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("KepalaGodzilla1.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// front
-1.5, 1, 1, 0, 0,  // 0
-0.5, 1, 1, 0.3, 0.3,   // 1
-0.5,  2, 1, 0.3, 0.3,   // 2
-1.5,  2, 1, 0.3, 0.3,  // 3

// right
-0.5,  2,  1, 0, 0,  // 4
-0.5,  2, 0, 0.3, 0.3,  // 5
-0.5, 1, 0, 0.3, 0.3,  // 6
-0.5, 1,  1, 0.3, 0.3,  // 7

// back
-1.5, 1, 0, 0, 0, // 8 
-0.5,  1, 0, 0.3, 0.3, // 9
-0.5,   2, 0, 0.3, 0.3, // 10
-1.5,  2, 0, 0.3, 0.3, // 11

// left
-1.5, 1, 0, 0, 0, // 12
-1.5, 1,  1, 0.3, 0.3, // 13
-1.5,  2,  1, 0.3, 0.3, // 14
-1.5,  2, 0, 0.3, 0.3, // 15

// upper
-0.5, 2,  1, 0, 0,   // 16
-1.5, 2,  1, 0.3, 0.3,  // 17
-1.5, 2, 0, 0.3, 0.3,  // 18
-0.5, 2, 0, 0.3, 0.3,   // 19

// bottom
-1.5, 1, 0, 0, 0, // 20
-0.5, 1, 0, 0.3, 0.3,  // 21
-0.5, 1,  1, 0.3, 0.3,  // 22
-1.5, 1,  1, 0.3, 0.3, // 23
};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22   // bottom
	};

	glGenVertexArrays(1, &armR);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(armR);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawHandR()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, armR);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(armR); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(pindahX, pindahY, pindahZ));

	model = glm::rotate(model, 0.0f, glm::vec3(0, 1, 0));

	model = glm::scale(model, glm::vec3(1, 0.7, 2.5));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildHandL() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &textureArmL);
	glBindTexture(GL_TEXTURE_2D, textureArmL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("KepalaGodzilla1.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// front
0.5, 1, 1, 0, 0,  // 0
1.5, 1, 1, 0.3, 0.3,   // 1
1.5,  2, 1, 0.3, 0.3,   // 2
0.5,  2, 1, 0.3, 0.3,  // 3

// right
1.5,  2,  1, 0, 0,  // 4
1.5,  2, 0, 0.3, 0.3,  // 5
1.5, 1, 0, 0.3, 0.3,  // 6
1.5, 1,  1, 0.3, 0.3,  // 7

// back
0.5, 1, 0, 0, 0, // 8 
1.5,  1, 0, 0.3, 0.3, // 9
1.5,   2, 0, 0.3, 0.3, // 10
0.5,  2, 0, 0.3, 0.3, // 11

// left
0.5, 1, 0, 0, 0, // 12
0.5, 1,  1, 0.3, 0.3, // 13
0.5,  2,  1, 0.3, 0.3, // 14
0.5,  2, 0, 0.3, 0.3, // 15

// upper
1.5, 2,  1, 0, 0,   // 16
0.5, 2,  1, 0.3, 0.3,  // 17
0.5, 2, 0, 0.3, 0.3,  // 18
1.5, 2, 0, 0.3, 0.3,   // 19

// bottom
0.5, 1, 0, 0, 0, // 20
1.5, 1, 0, 0.3, 0.3,  // 21
1.5, 1,  1, 0.3, 0.3,  // 22
0.5, 1,  1, 0.3, 0.3, // 23
};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22   // bottom
	};

	glGenVertexArrays(1, &armL);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(armL);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawHandL()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, armL);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(armL); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(pindahX, pindahY, pindahZ));

	model = glm::rotate(model, 0.0f, glm::vec3(0, 1, 0));

	model = glm::scale(model, glm::vec3(1, 0.7, 2.5));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}
void Demo::BuildCubeKepala() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &textureAtas);
	glBindTexture(GL_TEXTURE_2D, textureAtas);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("KepalaGodzilla1.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// front
		-0.5, 4, 0.7, 0, 0,  // 0
		0.5, 4, 0.7, 1, 0,   // 1
		0.5,  5, 0.7, 1, 1,   // 2
		-0.5,  5, 0.7, 0, 1,  // 3

		// right
		0.5,  5,  0.7, 0, 0,  // 4
		0.5,  5, -0.3, 0.3, 0.3,  // 5
		0.5, 4, -0.3, 0.3, 0.3,  // 6
		0.5, 4,  0.7, 0, 0.3,  // 7

		// back
		-0.5, 4, -0.3, 0, 0, // 8 
		0.5,  4, -0.3, 0.3, 0.3, // 9
		0.5,   5, -0.3, 0.3, 0.3, // 10
		-0.5,  5, -0.3, 0.3, 0.3, // 11

		// left
		-0.5, 4, -0.3, 0, 0, // 12
		-0.5, 4,  0.7, 0.3, 0.3, // 13
		-0.5,  5,  0.7, 0.3, 0.3, // 14
		-0.5,  5, -0.3, 0.3, 0.3, // 15

		// upper
		0.5, 5,  0.7, 0, 0,   // 16
		-0.5, 5,  0.7, 0.3, 0.3,  // 17
		-0.5, 5, -0.3, 0.3, 0.3,  // 18
		0.5, 5, -0.3, 0.3, 0.3,   // 19

		// bottom
		-0.5, 4, -0.3, 0.3, 0.3, // 20
		0.5, 4, -0.3, 0.3, 0.3,  // 21
		0.5, 4,  0.7, 0.3, 0.3,  // 22
		-0.5, 4,  0.7, 0.3, 0.3, // 23
	};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22   // bottom
	};

	glGenVertexArrays(1, &atas);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(atas);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawCubeKepala()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureAtas);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(atas); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(pindahX, pindahY, pindahZ));

	model = glm::rotate(model, 0.0f, glm::vec3(0, 1, 0));

	model = glm::scale(model, glm::vec3(1, 0.5, 3));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}


void Demo::BuildCubeTop() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &textureTop);
	glBindTexture(GL_TEXTURE_2D, textureTop);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("KepalaGodzilla2.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);


	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
float vertices[] = {
// format position, tex coords
// front
-0.5, 5, 0.7, 0, 0,  // 0
0.5, 5, 0.7, 1, 0,   // 1
0.5,  6, 0.7, 1, 1,   // 2
-0.5,  6, 0.7, 0, 1,  // 3

// right
0.5,  6,  0.7, 0, 0,  // 4
0.5,  6, -0.3, 0.3, 0.3,  // 5
0.5, 5, -0.3, 0.3, 0.3,  // 6
0.5, 5,  0.7, 0, 0.3,  // 7

// back
-0.5, 5, -0.3, 0, 0, // 8 
0.5,  5, -0.3, 0.3, 0.3, // 9
0.5,   6, -0.3, 0.3, 0.3, // 10
-0.5,  6, -0.3, 0.3, 0.3, // 11

// left
-0.5, 5, -0.3, 0, 0, // 12
-0.5, 5,  0.7, 0.3, 0.3, // 13
-0.5,  6,  0.7, 0.3, 0.3, // 14
-0.5,  6, -0.3, 0.3, 0.3, // 15

// upper
0.5, 6,  0.7, 0, 0,   // 16
-0.5, 6,  0.7, 0.3, 0.3,  // 17
-0.5, 6, -0.3, 0.3, 0.3,  // 18
0.5, 6, -0.3, 0.3, 0.3,   // 19

// bottom
-0.5, 5, -0.3, 0.3, 0.3, // 20
0.5, 5, -0.3, 0.3, 0.3,  // 21
0.5, 5,  0.7, 0.3, 0.3,  // 22
-0.5, 5,  0.7, 0.3, 0.3, // 23
};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22   // bottom
	};

	glGenVertexArrays(1, &top);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(top);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


}

void Demo::DrawCubeTop()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureTop);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(top); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(pindahX, pindahY, pindahZ));

	model = glm::rotate(model, 0.0f, glm::vec3(0, 1, 0));

	model = glm::scale(model, glm::vec3(0.7, 0.5, 1.5));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);


}

void Demo::BuildKakiR() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &textureKakiR);
	glBindTexture(GL_TEXTURE_2D, textureKakiR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("KepalaGodzilla1.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// front
	-1, -2.5, 0.7, 0, 0,  // 0
	0, -2.5, 0.7, 0.3, 0.3,   // 1
	0,  -1.5, 0.7, 0.3, 0.3,   // 2
	-1,  -1.5, 0.7, 0.3, 0.3,  // 3

	// right
	0,  -1.5,  0.7, 0, 0,  // 4
	0,  -1.5, -0.3, 0.3, 0.3,  // 5
	0, -2.5, -0.3, 0.3, 0.3,  // 6
	0, -2.5,  0.7, 0.3, 0.3,  // 7

	// back
	-1, -2.5, -0.3, 0, 0, // 8 
	0,  -2.5, -0.3, 0.3, 0.3, // 9
	0,   -1.5, -0.3, 0.3, 0.3, // 10
	-1,  -1.5, -0.3, 0.3, 0.3, // 11

	// left
	-1, -2.5, -0.3, 0, 0, // 12
	-1, -2.5,  0.7, 0.3, 0.3, // 13
	-1,  -1.5,  0.7, 0.3, 0.3, // 14
	-1,  -1.5, -0.3, 0.3, 0.3, // 15

	// upper
	0, -1.5,  0.7, 0, 0,   // 16
	-1, -1.5,  0.7, 0.3, 0.3,  // 17
	-1, -1.5, -0.3, 0.3, 0.3,  // 18
	0, -1.5, -0.3, 0.3, 0.3,   // 19

	// bottom
	-1, -2.5, -0.3, 0, 0, // 20
	0, -2.5, -0.3, 0.3, 0.3,  // 21
	0, -2.5,  0.7, 0.3, 0.3,  // 22
	-1, -2.5,  0.7, 0.3, 0.3, // 23
	};
	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22   // bottom
	};

	glGenVertexArrays(1, &kakiR);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(kakiR);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawKakiR()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureKakiR);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(kakiR); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(pindahX, pindahY, pindahZ));

	model = glm::rotate(model, 0.0f, glm::vec3(0, 1, 0));

	model = glm::scale(model, glm::vec3(1, 1, 3));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildKakiL() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &textureKakiL);
	glBindTexture(GL_TEXTURE_2D, textureKakiL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("KepalaGodzilla1.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// front
0, -2.5, 0.7, 0, 0,  // 0
1, -2.5, 0.7, 0.3, 0.3,   // 1
1,  -1.5, 0.7, 0.3, 0.3,   // 2
0,  -1.5, 0.7, 0.3, 0.3,  // 3

// right
1,  -1.5,  0.7, 0, 0,  // 4
1,  -1.5, -0.3, 0.3, 0.3,  // 5
1, -2.5, -0.3, 0.3, 0.3,  // 6
1, -2.5,  0.7, 0.3, 0.3,  // 7

// back
0, -2.5, -0.3, 0, 0, // 8 
1,  -2.5, -0.3, 0.3, 0.3, // 9
1,   -1.5, -0.3, 0.3, 0.3, // 10
0,  -1.5, -0.3, 0.3, 0.3, // 11

// left
0, -2.5, -0.3, 0, 0, // 12
0, -2.5,  0.7, 0.3, 0.3, // 13
0,  -1.5,  0.7, 0.3, 0.3, // 14
0,  -1.5, -0.3, 0.3, 0.3, // 15

// upper
1, -1.5,  0.7, 0, 0,   // 16
0, -1.5,  0.7, 0.3, 0.3,  // 17
0, -1.5, -0.3, 0.3, 0.3,  // 18
1, -1.5, -0.3, 0.3, 0.3,   // 19

// bottom
0, -2.5, -0.3, 0, 0, // 20
1, -2.5, -0.3, 0.3, 0.3,  // 21
1, -2.5,  0.7, 0.3, 0.3,  // 22
0, -2.5,  0.7, 0.3, 0.3, // 23
	};
	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22   // bottom
	};

	glGenVertexArrays(1, &kakiL);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(kakiL);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawKakiL()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureKakiL);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(kakiL); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(pindahX, pindahY, pindahZ));

	model = glm::rotate(model, 0.0f, glm::vec3(0, 1, 0));

	model = glm::scale(model, glm::vec3(1, 1, 3));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildEkor() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &textureEkor);
	glBindTexture(GL_TEXTURE_2D, textureEkor);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("BadanGodzilla.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// front
-0.5, -2, 0, 0, 0,  // 0
0.5, -2, 0, 0.3, 0.3,   // 1
0.5,  -1, 0, 0.3, 0.3,   // 2
-0.5,  -1, 0, 0.3, 0.3,  // 3

// right
0.5,  -1,  0, 0, 0,  // 4
0.5,  -1, -1, 0.3, 0.3,  // 5
0.5, -2, -1, 0.3, 0.3,  // 6
0.5, -2,  0, 0.3, 0.3,  // 7

// back
-0.5, -2, -1, 0, 0, // 8 
0.5,  -2, -1, 0.3, 0.3, // 9
0.5,   -1, -1, 0.3, 0.3, // 10
-0.5,  -1, -1, 0.3, 0.3, // 11

// left
-0.5, -2, -1, 0, 0, // 12
-0.5, -2,  0, 0.3, 0.3, // 13
-0.5,  -1,  0, 0.3, 0.3, // 14
-0.5,  -1, -1, 0.3, 0.3, // 15

// upper
0.5, -1,  0, 0, 0,   // 16
-0.5, -1,  0, 0.3, 0.3,  // 17
-0.5, -1, -1, 0.3, 0.3,  // 18
0.5, -1, -1, 0.3, 0.3,   // 19

// bottom
-0.5, -2, -1, 0, 0, // 20
0.5, -2, -1, 0.3, 0.3,  // 21
0.5, -2,  0, 0.3, 0.3,  // 22
-0.5, -2,  0, 0.3, 0.3, // 23
};
	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22   // bottom
	};

	glGenVertexArrays(1, &ekor);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(ekor);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawEkor()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureEkor);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(ekor); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(pindahX, pindahY, pindahZ));

	model = glm::rotate(model, 0.0f, glm::vec3(0, 1, 0));

	model = glm::scale(model, glm::vec3(1, 1, 5));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

//Tank

void Demo::BuildCubeT() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &textureTank);
	glBindTexture(GL_TEXTURE_2D, textureTank);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("TankBawah1.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// front
-0.5, -2.5, -2, 0, 0,  // 0
0.5, -2.5, -2, 0.3, 0.3,   // 1
0.5,  -1.5, -2, 0.3, 0.3,   // 2
-0.5,  -1.5, -2, 0.3, 0.3,  // 3

// right
0.5,  -1.5,  -2, 0, 0,  // 4
0.5,  -1.5, -3, 0.3, 0.3,  // 5
0.5, -2.5, -3, 0.3, 0.3,  // 6
0.5, -2.5,  -2, 0.3, 0.3,  // 7

// back
-0.5, -2.5, -3, 0, 0, // 8 
0.5,  -2.5, -3, 0.3, 0.3, // 9
0.5,  -1.5, -3, 0.3, 0.3, // 10
-0.5, -1.5, -3, 0.3, 0.3, // 11

// left
-0.5, -2.5, -3, 0, 0, // 12
-0.5, -2.5,  -2, 0.3, 0.3, // 13
-0.5, -1.5,  -2, 0.3, 0.3, // 14
-0.5, -1.5, -3, 0.3, 0.3, // 15

// upper
0.5,  -1.5,  -2, 0, 0,   // 16
-0.5, -1.5,  -2, 0.3, 0.3,  // 17
-0.5, -1.5, -3, 0.3, 0.3,  // 18
0.5,  -1.5, -3, 0.3, 0.3,   // 19

// bottom
-0.5, -2.5, -3, 0, 0, // 20
0.5, -2.5, -3, 0.3, 0.3,  // 21
0.5, -2.5,  -2, 0.3, 0.3,  // 22
-0.5, -2.5,  -2, 0.3, 0.3, // 23
};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22   // bottom
	};

	glGenVertexArrays(1, &tank);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(tank);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawCubeT()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureTank);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(tank); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(pindahX, pindahY, pindahZ));

	model = glm::rotate(model, 0.0f, glm::vec3(0, 1, 0));

	model = glm::scale(model, glm::vec3(3, 1, 4));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildCubeKepalaT() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &textureATank);
	glBindTexture(GL_TEXTURE_2D, textureATank);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("TankBawah.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// front
-0.5, -1.5, -6.5, 0, 0,  // 0
0.5, -1.5, -6.5, 1, 0,   // 1
0.5,  -0.5, -6.5, 1, 1,   // 2
-0.5,  -0.5, -6.5, 0, 1,  // 3

// right
0.5,  -0.5,  -6.5, 0, 0,  // 4
0.5,  -0.5, -7.5, 0.3, 0.3,  // 5
0.5, -1.5, -7.5, 0.3, 0.3,  // 6
0.5, -1.5,  -6.5, 0, 0.3,  // 7

// back
-0.5, -1.5, -7.5, 0, 0, // 8 
0.5,  -1.5, -7.5, 0.3, 0.3, // 9
0.5,   -0.5, -7.5, 0.3, 0.3, // 10
-0.5,  -0.5, -7.5, 0.3, 0.3, // 11

// left
-0.5, -1.5, -7.5, 0, 0, // 12
-0.5, -1.5,  -6.5, 0.3, 0.3, // 13
-0.5,  -0.5,  -6.5, 0.3, 0.3, // 14
-0.5,  -0.5, -7.5, 0.3, 0.3, // 15

// upper
0.5, -0.5,  -6.5, 0, 0,   // 16
-0.5, -0.5,  -6.5, 0.3, 0.3,  // 17
-0.5, -0.5, -7.5, 0.3, 0.3,  // 18
0.5, -0.5, -7.5, 0.3, 0.3,   // 19

// bottom
-0.5, -1.5, -7.5, 0.3, 0.3, // 20
0.5, -1.5, -7.5, 0.3, 0.3,  // 21
0.5, -1.5,  -6.5, 0.3, 0.3,  // 22
-0.5, -1.5,  -6.5, 0.3, 0.3, // 23
};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22   // bottom
	};

	glGenVertexArrays(1, &atank);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(atank);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawCubeKepalaT()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureATank);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(atank); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(pindahX, pindahY, pindahZ));

	model = glm::rotate(model, 0.0f, glm::vec3(0, 1, 0));

	model = glm::scale(model, glm::vec3(1.5, 1, 1.5));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildLaras() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &textureLaras);
	glBindTexture(GL_TEXTURE_2D, textureLaras);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("LarasTank.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// front
-0.5, -2, -2.5, 0, 0,  // 0
0.5, -2, -2.5, 1, 0,   // 1
0.5,  -1, -2.5, 1, 1,   // 2
-0.5,  -1, -2.5, 0, 1,  // 3

// right
0.5,  -1,  -2.5, 0, 0,  // 4
0.5,  -1, -3.5, 0.3, 0.3,  // 5
0.5, -2, -3.5, 0.3, 0.3,  // 6
0.5, -2,  -2.5, 0, 0.3,  // 7

// back
-0.5, -2, -3.5, 0, 0, // 8 
0.5,  -2, -3.5, 0.3, 0.3, // 9
0.5,   -1, -3.5, 0.3, 0.3, // 10
-0.5,  -1, -3.5, 0.3, 0.3, // 11

// left
-0.5, -2, -3.5, 0, 0, // 12
-0.5, -2,  -2.5, 0.3, 0.3, // 13
-0.5,  -1,  -2.5, 0.3, 0.3, // 14
-0.5,  -1, -3.5, 0.3, 0.3, // 15

// upper
0.5, -1,  -2.5, 0, 0,   // 16
-0.5, -1,  -2.5, 0.3, 0.3,  // 17
-0.5, -1, -3.5, 0.3, 0.3,  // 18
0.5, -1, -3.5, 0.3, 0.3,   // 19

// bottom
-0.5, -2, -3.5, 0.3, 0.3, // 20
0.5, -2, -3.5, 0.3, 0.3,  // 21
0.5, -2,  -2.5, 0.3, 0.3,  // 22
-0.5, -2,  -2.5, 0.3, 0.3, // 23
	};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22   // bottom
	};

	glGenVertexArrays(1, &laras);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(laras);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawLaras()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureLaras);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(laras); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(pindahX, pindahY, pindahZ));

	model = glm::rotate(model, 0.0f, glm::vec3(0, 1, 0));

	model = glm::scale(model, glm::vec3(0.5, 0.7, 3));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

//Gedung

void Demo::BuildCubeGedung() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &textureGedung);
	glBindTexture(GL_TEXTURE_2D, textureGedung);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("Gedung.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// front
-2.5, -0.5, 2, 0, 0,  // 0
-1.5, -0.5, 2, 0.3, 0.3,   // 1
-1.5,  0.5, 2, 0.3, 0.3,   // 2
-2.5,  0.5, 2, 0.3, 0.3,  // 3

// right
-1.5,  0.5,  2, 0, 0,  // 4
-1.5,  0.5, 1, 0.3, 0.3,  // 5
-1.5, -0.5, 1, 0.3, 0.3,  // 6
-1.5, -0.5,  2, 0.3, 0.3,  // 7

// back
-2.5, -0.5, 1, 0, 0, // 8 
-1.5,  -0.5, 1, 0.3, 0.3, // 9
-1.5,   0.5, 1, 0.3, 0.3, // 10
-2.5,  0.5, 1, 0.3, 0.3, // 11

// left
-2.5, -0.5, 1, 0, 0, // 12
-2.5, -0.5,  2, 0.3, 0.3, // 13
-2.5,  0.5,  2, 0.3, 0.3, // 14
-2.5,  0.5, 1, 0.3, 0.3, // 15

// upper
-1.5, 0.5,  2, 0, 0,   // 16
-2.5, 0.5,  2, 0.3, 0.3,  // 17
-2.5, 0.5, 1, 0.3, 0.3,  // 18
-1.5, 0.5, 1, 0.3, 0.3,   // 19

// bottom
-2.5, -0.5, 1, 0, 0, // 20
-1.5, -0.5, 1, 0.3, 0.3,  // 21
-1.5, -0.5,  2, 0.3, 0.3,  // 22
-2.5, -0.5,  2, 0.3, 0.3, // 23
};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22   // bottom
	};

	glGenVertexArrays(1, &gedung);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(gedung);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawCubeGedung()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureGedung);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(gedung); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(pindahX, pindahY, pindahZ));

	model = glm::rotate(model, 0.0f, glm::vec3(0, 1, 0));

	model = glm::scale(model, glm::vec3(3, 5, 4));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

//Pohon

void Demo::BuildBatang() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &textureBatang);
	glBindTexture(GL_TEXTURE_2D, textureBatang);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("TankBawah1.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// front
-0.5, -0.5, 10.5, 0, 0,  // 0
0.5, -0.5, 10.5, 0.3, 0.3,   // 1
0.5,  0.5, 10.5, 0.3, 0.3,   // 2
-0.5,  0.5, 10.5, 0.3, 0.3,  // 3

// right
0.5,  0.5,  10.5, 0, 0,  // 4
0.5,  0.5, 9.5, 0.3, 0.3,  // 5
0.5, -0.5, 9.5, 0.3, 0.3,  // 6
0.5, -0.5,  10.5, 0.3, 0.3,  // 7

// back
-0.5, -0.5, 9.5, 0, 0, // 8 
0.5,  -0.5, 9.5, 0.3, 0.3, // 9
0.5,   0.5, 9.5, 0.3, 0.3, // 10
-0.5,  0.5, 9.5, 0.3, 0.3, // 11

// left
-0.5, -0.5, 9.5, 0, 0, // 12
-0.5, -0.5,  10.5, 0.3, 0.3, // 13
-0.5,  0.5,  10.5, 0.3, 0.3, // 14
-0.5,  0.5, 9.5, 0.3, 0.3, // 15

// upper
0.5, 0.5,  10.5, 0, 0,   // 16
-0.5, 0.5,  10.5, 0.3, 0.3,  // 17
-0.5, 0.5, 9.5, 0.3, 0.3,  // 18
0.5, 0.5, 9.5, 0.3, 0.3,   // 19

// bottom
-0.5, -0.5, 9.5, 0, 0, // 20
0.5, -0.5, 9.5, 0.3, 0.3,  // 21
0.5, -0.5,  10.5, 0.3, 0.3,  // 22
-0.5, -0.5,  10.5, 0.3, 0.3, // 23
};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22   // bottom
	};

	glGenVertexArrays(1, &batang);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(batang);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawBatang()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureBatang);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(batang); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(pindahX, pindahY, pindahZ));

	model = glm::rotate(model, 0.0f, glm::vec3(0, 1, 0));

	model = glm::scale(model, glm::vec3(1, 5, 1));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildDahan1() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &textureDahan1);
	glBindTexture(GL_TEXTURE_2D, textureDahan1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("Dahan4.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// front
-0.5, 1, 6, 0, 0,  // 0
0.5, 1, 6, 1, 0,   // 1
0.5,  2, 6, 1, 1,   // 2
-0.5,  2, 6, 0, 1,  // 3

// right
0.5,  2,  6, 0, 0,  // 4
0.5,  2, 5, 0.3, 0.3,  // 5
0.5, 1, 5, 0.3, 0.3,  // 6
0.5, 1,  6, 0, 0.3,  // 7

// back
-0.5, 1, 5, 0, 0, // 8 
0.5,  1, 5, 0.3, 0.3, // 9
0.5,   2, 5, 0.3, 0.3, // 10
-0.5,  2, 5, 0.3, 0.3, // 11

// left
-0.5, 1, 5, 0, 0, // 12
-0.5, 1,  6, 0.3, 0.3, // 13
-0.5,  2,  6, 0.3, 0.3, // 14
-0.5,  2, 5, 0.3, 0.3, // 15

// upper
0.5, 2,  6, 0, 0,   // 16
-0.5, 2,  6, 0.3, 0.3,  // 17
-0.5, 2, 5, 0.3, 0.3,  // 18
0.5, 2, 5, 0.3, 0.3,   // 19

// bottom
-0.5, 1, 5, 0.3, 0.3, // 20
0.5, 1, 5, 0.3, 0.3,  // 21
0.5, 1,  6, 0.3, 0.3,  // 22
-0.5, 1,  6, 0.3, 0.3, // 23
};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22   // bottom
	};

	glGenVertexArrays(1, &dahan1);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(dahan1);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawDahan1()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureDahan1);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(dahan1); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(pindahX, pindahY, pindahZ));

	model = glm::rotate(model, 0.0f, glm::vec3(0, 1, 0));

	model = glm::scale(model, glm::vec3(2, 2, 2));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildDahan2() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &textureDahan2);
	glBindTexture(GL_TEXTURE_2D, textureDahan2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("Dahan4.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// front
-0.5, 1, 5.5, 0, 0,  // 0
0.5, 1, 5.5, 1, 0,   // 1
0.5,  2, 5.5, 1, 1,   // 2
-0.5,  2, 5.5, 0, 1,  // 3

// right
0.5,  2,  5.5, 0, 0,  // 4
0.5,  2, 4.5, 0.3, 0.3,  // 5
0.5, 1, 4.5, 0.3, 0.3,  // 6
0.5, 1,  5.5, 0, 0.3,  // 7

// back
-0.5, 1, 4.5, 0, 0, // 8 
0.5,  1, 4.5, 0.3, 0.3, // 9
0.5,   2, 4.5, 0.3, 0.3, // 10
-0.5,  2, 4.5, 0.3, 0.3, // 11

// left
-0.5, 1, 4.5, 0, 0, // 12
-0.5, 1,  5.5, 0.3, 0.3, // 13
-0.5,  2,  5.5, 0.3, 0.3, // 14
-0.5,  2, 4.5, 0.3, 0.3, // 15

// upper
0.5, 2,  5.5, 0, 0,   // 16
-0.5, 2,  5.5, 0.3, 0.3,  // 17
-0.5, 2, 4.5, 0.3, 0.3,  // 18
0.5, 2, 4.5, 0.3, 0.3,   // 19

// bottom
-0.5, 1, 4.5, 0.3, 0.3, // 20
0.5, 1, 4.5, 0.3, 0.3,  // 21
0.5, 1,  5.5, 0.3, 0.3,  // 22
-0.5, 1,  5.5, 0.3, 0.3, // 23
};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22   // bottom
	};

	glGenVertexArrays(1, &dahan2);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(dahan2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawDahan2()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureDahan2);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(dahan2); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(pindahX, pindahY, pindahZ));

	model = glm::rotate(model, 0.0f, glm::vec3(0, 1, 0));

	model = glm::scale(model, glm::vec3(1.5, 3, 2));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildDahan3() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &textureDahan3);
	glBindTexture(GL_TEXTURE_2D, textureDahan3);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("Dahan4.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// front
-0.5, 0.5, 4.8, 0, 0,  // 0
0.5, 0.5, 4.8, 1, 0,   // 1
0.5,  1.5, 4.8, 1, 1,   // 2
-0.5,  1.5, 4.8, 0, 1,  // 3

// right
0.5,  1.5,  4.8, 0, 0,  // 4
0.5,  1.5, 3.8, 0.3, 0.3,  // 5
0.5, 0.5, 3.8, 0.3, 0.3,  // 6
0.5, 0.5,  4.8, 0, 0.3,  // 7

// back
-0.5, 0.5, 3.8, 0, 0, // 8 
0.5,  0.5, 3.8, 0.3, 0.3, // 9
0.5,   1.5, 3.8, 0.3, 0.3, // 10
-0.5,  1.5, 3.8, 0.3, 0.3, // 11

// left
-0.5, 0.5, 3.8, 0, 0, // 12
-0.5, 0.5,  4.8, 0.3, 0.3, // 13
-0.5,  1.5,  4.8, 0.3, 0.3, // 14
-0.5,  1.5, 3.8, 0.3, 0.3, // 15

// upper
0.5, 1.5,  4.8, 0, 0,   // 16
-0.5, 1.5,  4.8, 0.3, 0.3,  // 17
-0.5, 1.5, 3.8, 0.3, 0.3,  // 18
0.5, 1.5, 3.8, 0.3, 0.3,   // 19

// bottom
-0.5, 0.5, 3.8, 0.3, 0.3, // 20
0.5, 0.5, 3.8, 0.3, 0.3,  // 21
0.5, 0.5,  4.8, 0.3, 0.3,  // 22
-0.5, 0.5,  4.8, 0.3, 0.3, // 23
};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22   // bottom
	};

	glGenVertexArrays(1, &dahan3);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(dahan3);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawDahan3()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureDahan3);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(dahan3); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(pindahX, pindahY, pindahZ));

	model = glm::rotate(model, 0.0f, glm::vec3(0, 1, 0));

	model = glm::scale(model, glm::vec3(2, 2, 2));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildDahan4() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &textureDahan4);
	glBindTexture(GL_TEXTURE_2D, textureDahan4);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("Dahan4.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// front
-0.5, 0.5, 5, 0, 0,  // 0
0.5, 0.5, 5, 1, 0,   // 1
0.5,  1.5, 5, 1, 1,   // 2
-0.5,  1.5, 5, 0, 1,  // 3

// right
0.5,  1.5,  5, 0, 0,  // 4
0.5,  1.5, 4, 0.3, 0.3,  // 5
0.5, 0.5, 4, 0.3, 0.3,  // 6
0.5, 0.5,  5, 0, 0.3,  // 7

// back
-0.5, 0.5, 4, 0, 0, // 8 
0.5,  0.5, 4, 0.3, 0.3, // 9
0.5,   1.5, 4, 0.3, 0.3, // 10
-0.5,  1.5, 4, 0.3, 0.3, // 11

// left
-0.5, 0.5, 4, 0, 0, // 12
-0.5, 0.5,  5, 0.3, 0.3, // 13
-0.5,  1.5,  5, 0.3, 0.3, // 14
-0.5,  1.5, 4, 0.3, 0.3, // 15

// upper
0.5, 1.5,  5, 0, 0,   // 16
-0.5, 1.5,  5, 0.3, 0.3,  // 17
-0.5, 1.5, 4, 0.3, 0.3,  // 18
0.5, 1.5, 4, 0.3, 0.3,   // 19

// bottom
-0.5, 0.5, 4, 0.3, 0.3, // 20
0.5, 0.5, 4, 0.3, 0.3,  // 21
0.5, 0.5,  5, 0.3, 0.3,  // 22
-0.5, 0.5,  5, 0.3, 0.3, // 23
};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22   // bottom
	};

	glGenVertexArrays(1, &dahan4);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(dahan4);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawDahan4()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureDahan4);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(dahan4); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(pindahX, pindahY, pindahZ));

	model = glm::rotate(model, 0.0f, glm::vec3(0, 1, 0));

	model = glm::scale(model, glm::vec3(3, 2.5, 2));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}


void Demo::BuildPlane()
{
	// Load and create a texture 
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height;
	unsigned char* image = SOIL_load_image("grass.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Build geometry
	GLfloat vertices[] = {
		// format position, tex coords
		// bottom
		-50.0, -0.5, -50.0,  0,  0,
		 50.0, -0.5, -50.0, 50,  0,
		 50.0, -0.5,  50.0, 50, 50,
		-50.0, -0.5,  50.0,  0, 50,
		//-50.0, -0.5, -50.0,  0,  0,
		//50.0, -0.5, -50.0, 1, 0,
		//50.0, -0.5,  50.0, 1, 1,
		//-50.0, -0.5,  50.0,  0, 1,


	};

	GLuint indices[] = { 0,  2,  1,  0,  3,  2 };

	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);
	glGenBuffers(1, &EBO2);

	glBindVertexArray(VAO2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // Unbind VAO
}

void Demo::DrawPlane()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 1);

	glBindVertexArray(VAO2); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::InitCamera()
{
	posCamX = 0.0f;
	posCamY = 1.0f;
	posCamZ = 8.0f;
	viewCamX = 0.0f;
	viewCamY = 1.0f;
	viewCamZ = 0.0f;
	upCamX = 0.0f;
	upCamY = 1.0f;
	upCamZ = 0.0f;
	CAMERA_SPEED = 0.001f;
	fovy = 45.0f;
	glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Demo::MoveCamera(float speed)
{
	float x = viewCamX - posCamX;
	float z = viewCamZ - posCamZ;
	// forward positive cameraspeed and backward negative -cameraspeed.
	posCamX = posCamX + x * speed;
	posCamZ = posCamZ + z * speed;
	viewCamX = viewCamX + x * speed;
	viewCamZ = viewCamZ + z * speed;
}

void Demo::StrafeCamera(float speed)
{
	float x = viewCamX - posCamX;
	float z = viewCamZ - posCamZ;
	float orthoX = -z;
	float orthoZ = x;

	// left positive cameraspeed and right negative -cameraspeed.
	posCamX = posCamX + orthoX * speed;
	posCamZ = posCamZ + orthoZ * speed;
	viewCamX = viewCamX + orthoX * speed;
	viewCamZ = viewCamZ + orthoZ * speed;
}

void Demo::RotateCamera(float speed)
{
	float x = viewCamX - posCamX;
	float z = viewCamZ - posCamZ;
	viewCamZ = (float)(posCamZ + glm::sin(speed) * x + glm::cos(speed) * z);
	viewCamX = (float)(posCamX + glm::cos(speed) * x - glm::sin(speed) * z);
}

int main(int argc, char** argv) {
	RenderEngine &app = Demo();
	app.Start("Final Project Kelompok Cucaracha", 800, 600, false, false);
}