#include "procGen.h"

namespace riversLibrary {
	ew::MeshData createSphere(float radius, int numSegments) {
		ew::MeshData mesh;

		return mesh;
	}

	ew::MeshData createCylinder(float height, float radius, int numSegments) {
		ew::MeshData mesh;

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
				vert.uv = ew::Vec2(col % 2, row / 2); //fix uv
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
				mesh->indices.push_back(start + (subdivisions + 1));
				mesh->indices.push_back(start + (subdivisions + 1) + 1);
			}
		}
	}

	ew::MeshData createPlane(float size, int subdivisions) {
		ew::MeshData mesh;
		mesh.vertices.reserve(subdivisions * 2);
		mesh.indices.reserve(subdivisions * 2);
		createPlaneFace(ew::Vec3{ +0.0f, +1.0f,+0.0f }, size, subdivisions, &mesh);
		return mesh;
	}
}