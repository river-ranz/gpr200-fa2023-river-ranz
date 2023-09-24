#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <riv/shader.h>

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

struct Vertex {
	float x, y, z;
	float u, v;
};

unsigned int createVAO(Vertex* vertexData, int numVertices, unsigned int* indicesData, int numIndices);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);

Vertex vertices[4] = {
	{ -0.5, -0.5, 0.0, 0.0, 0.0 },
	{ -0.5, 0.5, 0.0, 0.0, 1.0 },
	{ 0.5, 0.5, 0.0, 1.0, 1.0 },
	{ 0.5, -0.5, 0.0, 1.0, 0.0 }
};

unsigned int indices[6] = {
	0, 1, 2,
	0, 2, 3
};

float skyDayTop[3] = { 0.5, 0.3, 0.8 };
float skyDayBottom[3] = { 1.0, 0.2, 0.0 };
float skyNightTop[3] = { 0.0, 0.0, 1.0 };
float skyNightBottom[3] = { 0.4, 0.1, 0.5 };
float sunYellow[3] = { 0.85, 0.65, 0.0 };
float sunOrange[3] = { 1.0, 0.4, 0.0 };
float hillColor[3] = { 0.1, 0.1, 0.1 };
float* radius = new float;
float* speed = new float;
float* resolution = new float[2];
bool showImGUIDemoWindow = true;

int main() {
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello Triangle", NULL, NULL);
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

	riversLibrary::Shader shader("assets/vertexShader.vert", "assets/fragmentShader.frag");
	shader.use();

	unsigned int vao = createVAO(vertices, 4, indices, 6);
	glBindVertexArray(vao);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//info for uniforms & ImGUI window
		float time = (float)glfwGetTime();
		*radius = 0.22f;
		*speed = abs(sin(time));
		*resolution = SCREEN_WIDTH;
		*(resolution + 1) = SCREEN_HEIGHT;

		//uniforms
		shader.setFloat("_Time", time);
		shader.setFloat("_SunRadius", 0.22);
		shader.setFloat("_SunSpeed", abs(sin(time)));
		shader.setVec2("_Resolution", SCREEN_WIDTH, SCREEN_HEIGHT);
		shader.setVec3("_SkyDayColorTop", skyDayTop[0], skyDayTop[1], skyDayTop[2]);
		shader.setVec3("_SkyDayColorBottom", skyDayBottom[0], skyDayBottom[1], skyDayBottom[2]);
		shader.setVec3("_SkyNightColorTop", skyNightTop[0], skyNightTop[1], skyNightTop[2]);
		shader.setVec3("_SkyNightColorBottom", skyNightBottom[0], skyNightBottom[1], skyNightBottom[2]);
		shader.setVec3("_SunYellow", sunYellow[0], sunYellow[1], sunYellow[2]);
		shader.setVec3("_SunOrange", sunOrange[0], sunOrange[1], sunOrange[2]);
		shader.setVec3("_HillColor", hillColor[0], hillColor[1], hillColor[2]);
		

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

		//Render UI
		{
			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Settings");
			ImGui::Checkbox("Show Demo Window", &showImGUIDemoWindow);
			ImGui::ColorEdit3("Sky Top Color", skyDayTop);
			ImGui::ColorEdit3("Sky Bottom Color", skyDayBottom);
			ImGui::ColorEdit3("Night Sky Top Color", skyNightTop);
			ImGui::ColorEdit3("Night Sky Bottom Color", skyNightBottom);
			ImGui::ColorEdit3("Sun Day Color", sunYellow);
			ImGui::ColorEdit3("Sun Night Color", sunOrange);
			ImGui::ColorEdit3("Hill Color", hillColor);
			ImGui::DragFloat("Sun Radius", radius);
			ImGui::DragFloat("Speed", speed);
			ImGui::DragFloat2("Resolution", resolution);
			ImGui::End();
			if (showImGUIDemoWindow) {
				ImGui::ShowDemoWindow(&showImGUIDemoWindow);
			}

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}


unsigned int createVAO(Vertex* vertexData, int numVertices, unsigned int* indicesData, int numIndices) {
	//vao = vertex array object
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//vbo = vertex buffer object
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * numVertices * 3, vertexData, GL_STATIC_DRAW);

	unsigned int ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numIndices, indicesData, GL_STATIC_DRAW);

	//position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, x));
	glEnableVertexAttribArray(0);

	//uv
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(offsetof(Vertex, u)));
	glEnableVertexAttribArray(1);

	return vao;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

