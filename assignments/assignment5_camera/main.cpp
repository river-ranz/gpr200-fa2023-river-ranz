#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <ew/shader.h>
#include <ew/procGen.h>
#include <ew/transform.h>
#include <riv/camera.h>

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void moveCamera(GLFWwindow* window, riversLibrary::Camera* camera, riversLibrary::CameraControls* controls);

//Projection will account for aspect ratio!
const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

const int NUM_CUBES = 4;
ew::Transform cubeTransforms[NUM_CUBES];

int main() {
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Camera", NULL, NULL);
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

	//Enable back face culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//Depth testing - required for depth sorting!
	glEnable(GL_DEPTH_TEST);

	ew::Shader shader("assets/vertexShader.vert", "assets/fragmentShader.frag");
	
	//Cube mesh
	ew::Mesh cubeMesh(ew::createCube(0.5f));

	//Cube positions
	for (size_t i = 0; i < NUM_CUBES; i++)
	{
		cubeTransforms[i].position.x = i % (NUM_CUBES / 2) - 0.5;
		cubeTransforms[i].position.y = i / (NUM_CUBES / 2) - 0.5;
	}

	ew::Vec3 position = (0.0f, 0.0f, 5.0f);
	ew::Vec3 target = (0.0f, 0.0f, 0.0f);
	float fov = 60.0f;
	float orthoSize = 6;
	bool ortho = false;

	riversLibrary::Camera camera;
	camera.position = position;
	camera.target = target;
	camera.fov = fov;
	camera.orthoSize = orthoSize;
	camera.nearPlane = 0.1;
	camera.farPlane = 100;
	camera.orthographic = ortho;
	camera.aspectRatio = (float)SCREEN_WIDTH / SCREEN_HEIGHT;

	riversLibrary::CameraControls cameraControls;

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		moveCamera(window, &camera, &cameraControls);
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		//Clear both color buffer AND depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Set uniforms
		shader.use();
		shader.setMat4("_View", camera.ViewMatrix());
		shader.setMat4("_Projection", camera.ProjectionMatrix());

		//TODO: Set model matrix uniform
		for (size_t i = 0; i < NUM_CUBES; i++)
		{
			//Construct model matrix
			shader.setMat4("_Model", cubeTransforms[i].getModelMatrix());
			cubeMesh.draw();
		}

		//Render UI
		{
			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Settings");
			//ImGui::Text("Cubes");
			//for (size_t i = 0; i < NUM_CUBES; i++)
			//{
			//	ImGui::PushID(i);
			//	if (ImGui::CollapsingHeader("Transform")) {
			//		ImGui::DragFloat3("Position", &cubeTransforms[i].position.x, 0.05f);
			//		ImGui::DragFloat3("Rotation", &cubeTransforms[i].rotation.x, 1.0f);
			//		ImGui::DragFloat3("Scale", &cubeTransforms[i].scale.x, 0.05f);
			//	}
			//	ImGui::PopID();
			//}
			ImGui::Text("Camera");
			ImGui::DragFloat3("Position", &position.x, 0.05f);
			ImGui::DragFloat3("Target", &target.x, 0.05f);
			ImGui::DragFloat("FOV", &fov);
			ImGui::DragFloat("Orthographic Height", &orthoSize);
			ImGui::Checkbox("Orthographic", &ortho);
			ImGui::End();
			
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void moveCamera(GLFWwindow* window, riversLibrary::Camera* camera, riversLibrary::CameraControls* controls) {
	if (!glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2)) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		controls->firstMouse = true;
		return;
	}

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	double mouseX, mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);

	if (controls->firstMouse) {
		controls->firstMouse = false;
		controls->prevMouseX = mouseX;
		controls->prevMouseY = mouseY;
	}

	float yawRad = controls->yaw * (3.14159 / 180);
	float pitchRad = controls->pitch * (3.14159 / 180);

	ew::Vec3 forward = (cos(yawRad) * cos(pitchRad), sin(pitchRad), sin(yawRad) * cos(pitchRad));
	ew::Vec3 right = ew::Normalize(ew::Cross(forward, (0, 1, 0)));
	ew::Vec3 up = ew::Normalize(ew::Cross(right, forward));

//TODO: Get mouse position delta for this frame


	camera->target = camera->position + forward;

	controls->yaw += (mouseX - controls->prevMouseX) * controls->mouseSensitivity;
	controls->pitch -= (mouseY - controls->prevMouseY) * controls->mouseSensitivity;
//TODO: Clamp pitch between -89 and 89 degrees
	if (controls->pitch > 89) {

	}
	else if (controls->pitch < -89) {

	}

	controls->prevMouseX = mouseX;
	controls->prevMouseY = mouseY;
}