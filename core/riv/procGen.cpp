#include "procGen.h"

namespace riversLibrary {
	static void createSphereFace(ew::Vec3 normal, int numSegments, float radius, ew::MeshData* mesh) {
		unsigned int startVertex = mesh->vertices.size();
		ew::Vec3 a = ew::Vec3(normal.z, normal.x, normal.y);
		ew::Vec3 b = ew::Cross(normal, a);
		float thetaStep = (2 * ew::PI) / numSegments;
		float phiStep = ew::PI / numSegments;

		for (int row = 0; row <= numSegments; row++) {
			float phi = row * phiStep;
			for (int col = 0; col <= numSegments; col++) {
				float theta = col * thetaStep;
			}
		}
	}

	ew::MeshData createSphere(float radius, int numSegments) {
		ew::MeshData mesh;

		return mesh;
	}

	static void createCylinderFace(ew::Vec3 normal, float height, int numSegments, float radius, ew::MeshData* mesh) {
		unsigned int startVertex = mesh->vertices.size();
		ew::Vec3 a = ew::Vec3(normal.z, normal.x, normal.y);
		ew::Vec3 b = ew::Cross(normal, a);
		float size = ew::PI * radius * 2 * height;
		float thetaStep = (2 * ew::PI) / numSegments;

		for (int i = 0; i <= numSegments; i++) {
			float theta = i * thetaStep;
			ew::Vec3 pos = normal * size;
			pos -= (a + b) * size;
			pos += (a * (cos(theta) * radius) + b * (sin(theta) * radius));
			ew::Vertex vert;
			vert.pos = pos;
			vert.normal = normal;
			vert.uv = ew::Vec2(cos(theta) * radius, sin(theta) * radius);
			mesh->vertices.push_back(vert);
		}

		for (int i = 0; i <= numSegments; i++) {
			int start = 0 - numSegments;
			int center = 0;
			mesh->indices.push_back(start + i);
			mesh->indices.push_back(center);
			mesh->indices.push_back(start + i + 1);
		}
	}

	ew::MeshData createCylinder(float height, float radius, int numSegments) {
		ew::MeshData mesh;
		mesh.vertices.reserve(numSegments);
		mesh.indices.reserve(numSegments);
		float topY = height / 2;
		float bottomY = -topY;
		createCylinderFace(ew::Vec3{ +0.0f, +topY, +0.0f }, height, numSegments, radius, &mesh);
		createCylinderFace(ew::Vec3{ +0.0f, +1.0, +0.0f }, height, numSegments, radius, &mesh);
		createCylinderFace(ew::Vec3{ +0.0f, -1.0, +0.0f }, height, numSegments, radius, &mesh);
		createCylinderFace(ew::Vec3{ +0.0f, +bottomY, +0.0f }, height, numSegments, radius, &mesh);
		return mesh;
	}

	static void createPlaneFace(ew::Vec3 normal, float size, int subdivisions, ew::MeshData* mesh) {
		unsigned int startVertex = mesh->vertices.size();
		ew::Vec3 a = ew::Vec3(normal.z, normal.x, normal.y);
		ew::Vec3 b = ew::Cross(normal, a);
		float width = size / subdivisions;
		float height = size / width;

		for (int row = 0; row <= subdivisions; row++) {
			for (int col = 0; col <= subdivisions; col++) {
				ew::Vec3 pos = normal * size * -0.5f;
				pos -= (a + b) * size * -1.5f;
				pos += (a * col + b * row) * size;
				ew::Vertex vert;
				vert.pos = pos;
				vert.normal = normal;
				vert.uv = ew::Vec2(col / subdivisions, row / subdivisions); //fix!!!
				mesh->vertices.push_back(vert);
			}
		}

		for (int row = 0; row < subdivisions; row++) {
			for (int col = 0; col < subdivisions; col++) {
				int start = row * (subdivisions + 1) + col;
				mesh->indices.push_back(start);
				mesh->indices.push_back(start + 1);
				mesh->indices.push_back(start + (subdivisions + 1) + 1);
				mesh->indices.push_back(start);
				mesh->indices.push_back(start + (subdivisions + 1) + 1);
				mesh->indices.push_back(start + (subdivisions + 1));
			}
		}
	}

	ew::MeshData createPlane(float size, int subdivisions) {
		ew::MeshData mesh;
		mesh.vertices.reserve(subdivisions * 2);
		mesh.indices.reserve(subdivisions * 2);
		createPlaneFace(ew::Vec3{ +0.0f, +1.0f, +0.0f }, size, subdivisions, &mesh);
		return mesh;
	}
}