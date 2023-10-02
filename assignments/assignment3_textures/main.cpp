#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <ew/shader.h>
#include <riv/texture.h>

struct Vertex {
	float x, y, z;
	float u, v;
};

unsigned int createVAO(Vertex* vertexData, int numVertices, unsigned short* indicesData, int numIndices);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

Vertex vertices[4] = {
	{-1.0, -1.0, 0.0, 0.0, 0.0},
	{1.0, -1.0, 0.0, 1.0, 0.0},
	{1.0, 1.0, 0.0, 1.0, 1.0},
	{-1.0, 1.0, 0.0, 0.0, 1.0}
};
unsigned short indices[6] = {
	0, 1, 2,
	2, 3, 0
};

int wrapType[4] = { GL_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER, GL_MIRRORED_REPEAT};
const char* WRAP_MODE_TYPE[4] = { "Repeat", "Clamp to Edge", "Clamp to Border", "Mirrored Repeat"};
int currentItem = 0;
int wrapMode = wrapType[currentItem];
int prevMode = wrapMode;

float* tilingFloat = new float;

int main() {
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Textures", NULL, NULL);
	if (window == NULL) {
		printf("GLFW failed to create window");
		return 1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	if (!gladLoadGL(glfwGetProcAddress)) {
		printf("GLAD Failed to load GL headers");
		return 1;
	}

	//Initialize ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	ew::Shader backgroundShader("assets/background.vert", "assets/background.frag");
	unsigned int textureA = loadTexture("assets/wood-background.png", wrapMode, GL_LINEAR);
	unsigned int textureB = loadTexture("assets/horror.png", wrapMode, GL_LINEAR);
	unsigned int textureC = loadTexture("assets/overlay-texture.png", wrapMode, GL_LINEAR);

	ew::Shader characterShader("assets/character.vert", "assets/character.frag");
	unsigned int characterTexture = loadTexture("assets/pixel-cat.png", GL_REPEAT, GL_NEAREST);


	unsigned int quadVAO = createVAO(vertices, 4, indices, 6);
	*tilingFloat = 0.5f;

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glBindVertexArray(quadVAO);

		int wrapMode = wrapType[currentItem];
		if (prevMode != wrapMode) {
			textureA = loadTexture("assets/wood-background.png", wrapMode, GL_LINEAR);
			textureB = loadTexture("assets/horror.png", wrapMode, GL_LINEAR);
			textureC = loadTexture("assets/overlay-texture.png", wrapMode, GL_LINEAR);
			prevMode = wrapMode;
		}

		float time = (float)glfwGetTime();

		//Set uniforms
		backgroundShader.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureA);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textureB);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, textureC);
		backgroundShader.setInt("_WoodTexture", 0);
		backgroundShader.setInt("_ScarySmile", 1);
		backgroundShader.setInt("_OverlayTexture", 2);
		backgroundShader.setFloat("_Time", time);
		backgroundShader.setFloat("_Tiling", *tilingFloat);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL);

		characterShader.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, characterTexture);
		characterShader.setInt("_CatTexture", 0);
		characterShader.setFloat("_Time", time);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//Render UI
		{
			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Settings");
			ImGui::ListBox("Wrap Mode", &currentItem, WRAP_MODE_TYPE, IM_ARRAYSIZE(WRAP_MODE_TYPE));
			ImGui::DragFloat("Tiling", tilingFloat, 0.01f);
			ImGui::End();

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}

unsigned int createVAO(Vertex* vertexData, int numVertices, unsigned short* indicesData, int numIndices) {
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//Vertex Buffer Object 
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*numVertices, vertexData, GL_STATIC_DRAW);

	//Element Buffer Object
	unsigned int ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * numIndices, indicesData, GL_STATIC_DRAW);

	//Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex,x));
	glEnableVertexAttribArray(0);

	//UV attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(offsetof(Vertex, u)));
	glEnableVertexAttribArray(1);

	return vao;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

