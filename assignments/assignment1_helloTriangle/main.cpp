// FILE WAS EDITED BY ACCIDENT!!! NOT THE FILE FOR ASSIGNMENT 2!!!

#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>

#include <riv/shader.h>

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

float vertices[21] = {
	-0.5, -0.5, 0.0, 1.0, 0.0, 0.0, 1.0,
	 0.5, -0.5, 0.0, 0.0, 1.0, 0.0, 1.0,
	 0.0,  0.5, 0.0, 0.0, 0.0, 1.0, 1.0
};

const char* vertexShaderSource = R"(
	#version 450
	layout(location = 0) in vec3 vPos;
	layout(location = 1) in vec4 vColor;
	out vec4 Color;
	uniform float _Time;
	void main(){
		Color = vColor;
		vec3 offset = vec3(0, sin(vPos.x + _Time), 0) * 0.5;
		gl_Position = vec4(vPos + offset, 1.0);
	}
)";

const char* fragmentShaderSource = R"(
	#version 450
	out vec4 FragColor;
	in vec4 Color;
	uniform float _Time;
	void main(){
		FragColor = Color * abs(sin(_Time));
	}
)";

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

	if (!gladLoadGL(glfwGetProcAddress)) {
		printf("GLAD Failed to load GL headers");
		return 1;
	}

	riversLibrary::Shader shader("assets/vertexShader.vert", "assets/fragmentShader.frag");

	unsigned int vao = createVAO(vertices, 3);

	//render loop
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//glUseProgram(shader);

		//draw calls
		glBindVertexArray(vao);

		//current time for the frame
		//float time = (float)glfwGetTime();
		//int timeLocation = glGetUniformLocation(shader, "_Time");
		//glUniform1f(timeLocation, time);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}


unsigned int createVAO(float* vertexData, int numVertices) {
	//vao = vertex array object
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//vbo = vertex buffer object
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 7 * numVertices, vertexData, GL_STATIC_DRAW);

	//position attribute
	glVertexAttribPointer(0, numVertices, GL_FLOAT, GL_FALSE, sizeof(float) * 7, (const void*)0);
	glEnableVertexAttribArray(0);

	//color attribute
	glVertexAttribPointer(1, numVertices + 1, GL_FLOAT, GL_FALSE, sizeof(float) * 7, (const void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);

	return vao;
}
