#include "procGen.h"

namespace riversLibrary {
	static void createSphereFace(ew::Vec3 normal, int numSegments, float radius, ew::MeshData* mesh) {
		unsigned int startVertex = mesh->vertices.size();
		float thetaStep = (2 * ew::PI) / numSegments;
		float phiStep = ew::PI / numSegments;

		for (int row = 0; row <= numSegments; row++) {
			float phi = row * phiStep;
			for (int col = 0; col <= numSegments; col++) {
				float theta = col * thetaStep;
				ew::Vec3 pos;
				pos.x = radius * cos(theta) * sin(phi) - 1.5f;
				pos.y = radius * cos(phi);
				pos.z = radius * sin(theta) * sin(phi);
				ew::Vertex vert;
				vert.pos = pos;
				vert.normal = normal;
				vert.uv = ew::Vec2(col / numSegments, row / numSegments); //fix
				mesh->vertices.push_back(vert);
			}
		}

		int poleStart = 0;
		int sideStart = numSegments + 1;
		for (int i = 0; i < numSegments; i++) {
			mesh->indices.push_back(sideStart + i);
			mesh->indices.push_back(poleStart + i);
			mesh->indices.push_back(sideStart + i + 1);
		}

		poleStart = numSegments; //fix
		sideStart = numSegments + 1;
		for (int i = 0; i < numSegments; i++) {
			mesh->indices.push_back(sideStart + i);
			mesh->indices.push_back(poleStart + i);
			mesh->indices.push_back(sideStart + i + 1);
		}

		int columns = numSegments + 1;
		for (int row = 1; row < numSegments - 1; row++) {
			for (int col = 0; col < numSegments; col++) {
				int start = row * columns + col;
				mesh->indices.push_back(start);
				mesh->indices.push_back(start + 1);
				mesh->indices.push_back(start + columns);
				mesh->indices.push_back(start + 1);
				mesh->indices.push_back(start + columns + 1);
				mesh->indices.push_back(start + columns);
			}
		}
	}

	ew::MeshData createSphere(float radius, int numSegments) {
		ew::MeshData mesh;
		mesh.vertices.reserve(numSegments * 2);
		mesh.indices.reserve(numSegments * 2);
		createSphereFace(ew::Vec3{ +0.0, +1.0, +0.0 }, numSegments, radius, &mesh);
		return mesh;
	}

	static void createCylinderFace(ew::Vec3 normal, float height, int numSegments, float radius, ew::MeshData* mesh) {
		unsigned int startVertex = mesh->vertices.size();
		float size = ew::PI * radius * 2 * height;
		float thetaStep = (2 * ew::PI) / numSegments;

		for (int i = 0; i <= numSegments; i++) {
			float theta = i * thetaStep;
			ew::Vec3 pos;
			pos.x = cos(theta) * radius - 3.0f;
			pos.z = sin(theta) * radius;
			pos.y = height / 2;
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

		int sideStart = 0;
		int columns = numSegments + 1;
		for (int i = 0; i < columns; i++) {
			int start = sideStart + i;
			mesh->indices.push_back(start);
			mesh->indices.push_back(start + 1);
			mesh->indices.push_back(start + columns);
		}
	}

	ew::MeshData createCylinder(float height, float radius, int numSegments) {
		ew::MeshData mesh;
		mesh.vertices.reserve(numSegments * 2);
		mesh.indices.reserve(numSegments * 2);
		float topY = height / 2;
		float bottomY = -topY;
		createCylinderFace(ew::Vec3{ +0.0f, topY, +0.0f }, height, numSegments, radius, &mesh);
		createCylinderFace(ew::Vec3{ +0.0f, topY, +0.0f }, height, numSegments, radius, &mesh);
		createCylinderFace(ew::Vec3{ +0.0f, bottomY, +0.0f }, height, numSegments, radius, &mesh);
		createCylinderFace(ew::Vec3{ +0.0f, bottomY, +0.0f }, height, numSegments, radius, &mesh);
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
				vert.uv = ew::Vec2(col / subdivisions, row / subdivisions); //fix
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