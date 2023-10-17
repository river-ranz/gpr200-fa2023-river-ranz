#pragma once
#include "../ew/ewMath/mat4.h"
#include "../ew/ewMath/vec3.h"
#include "../ew/ewMath/ewMath.h"
#include "transformations.h"

namespace riversLibrary {
	struct Camera {
		ew::Vec3 position;
		ew::Vec3 target;
		float fov;
		float aspectRatio;
		float nearPlane;
		float farPlane;
		bool orthographic;
		float orthoSize;
		ew::Mat4 ViewMatrix();
		ew::Mat4 ProjectionMatrix();
	};

	ew::Mat4 Camera::ViewMatrix() {
		return LookAt(position, target, (0, 1, 0));
	}

	ew::Mat4 Camera::ProjectionMatrix() {
		if (orthographic) {
			return Orthographic(orthoSize, aspectRatio, nearPlane, farPlane);
		}
		return Perspective(fov, aspectRatio, nearPlane, farPlane);
	}


	struct CameraControls {
		double prevMouseX, prevMouseY;
		float yaw = 0, pitch = 0; //degrees
		float mouseSensitivity = 0.1f;
		bool firstMouse = true;
		float moveSpeed = 5.0f;
	};
}