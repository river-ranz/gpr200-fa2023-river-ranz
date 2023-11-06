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
				pos.x = radius * cos(theta) * sin(phi);
				pos.y = radius * cos(phi);
				pos.z = radius * sin(theta) * sin(phi);
				ew::Vertex vert;
				vert.pos = pos;
				vert.normal = ew::Normalize(ew::Vec3(cos(theta) * sin(phi), cos(phi), sin(theta) * sin(phi)));
				vert.uv = ew::Vec2(col / (numSegments * 1.0), row / (numSegments * 1.0));
				mesh->vertices.push_back(vert);
			}
		}

		//bottom cap
		int poleStart = mesh->vertices.size() - 1 - numSegments;
		int sideStart = poleStart - numSegments - 1;
		for (int i = 0; i <= numSegments; i++) {
			mesh->indices.push_back(sideStart + i + 1);
			mesh->indices.push_back(poleStart + i);
			mesh->indices.push_back(sideStart + i);
		}

		//top cap
		poleStart = 0;
		sideStart = numSegments;
		for (int i = 0; i <= numSegments; i++) {
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

	static void topRingIndices(int numSegments, ew::MeshData* mesh) {
		int start = 0;
		int center = 0;
		for (int i = 0; i <= numSegments; i++) {
			mesh->indices.push_back(start + i);
			mesh->indices.push_back(center);
			mesh->indices.push_back(start + i + 1);
		}
	}

	static void bottomRingIndices(int numSegments, ew::MeshData* mesh) {
		int start = numSegments + 1;
		int center = 0;
		for (int i = 0; i <= numSegments; i++) {
			mesh->indices.push_back(start + i + 1);
			mesh->indices.push_back(center);
			mesh->indices.push_back(start + i);
		}
	}

	static void sideIndices(int numSegments, ew::MeshData* mesh) {
		int sideStart = 0;
		int columns = numSegments + 1;
		for (int i = 0; i < columns; i++) {
			int start = sideStart + i;
			mesh->indices.push_back(start);
			mesh->indices.push_back(start + 1);
			mesh->indices.push_back(start + columns);
			mesh->indices.push_back(start + columns + 1);
			mesh->indices.push_back(start + columns);
			mesh->indices.push_back(start + 1);
		}
	}

	static void createCylinderVert(ew::Vec3 normal, float height, int numSegments, float radius, ew::MeshData* mesh) {
		unsigned int startVertex = mesh->vertices.size();
		float size = ew::PI * radius * 2 * height;
		float thetaStep = (2 * ew::PI) / numSegments;

		for (int i = 0; i <= numSegments; i++) {
			float theta = i * thetaStep;
			ew::Vec3 pos;
			pos.x = cos(theta) * radius;
			pos.z = sin(theta) * radius;
			pos.y = height / 2;
			ew::Vertex vert;
			vert.pos = pos;
			vert.normal = ew::Normalize(ew::Vec3(cos(theta), 0, sin(theta))); //fix- caps
			vert.uv = ew::Vec2(i / (numSegments * 1.0), cos(sin(theta))); //fix- u between 0 and 2pi, v between -h/2 and h/2
			mesh->vertices.push_back(vert);
		}
	}

	ew::MeshData createCylinder(float height, float radius, int numSegments) {
		ew::MeshData mesh;
		mesh.vertices.reserve(numSegments * 4);
		mesh.indices.reserve(numSegments * 4);
		float topY = height / 2;
		float bottomY = -topY;

		ew::Vertex topCenter;
		topCenter.pos = ew::Vec3(0, topY, 0);
		mesh.vertices.push_back(topCenter);

		createCylinderVert(ew::Vec3{ +0.0f, topY, +0.0f }, height, numSegments, radius, &mesh);
		createCylinderVert(ew::Vec3{ +0.0f, bottomY, +0.0f }, -height, numSegments, radius, &mesh);
		topRingIndices(numSegments, &mesh);
		bottomRingIndices(numSegments, &mesh); //fix- connects to top center?
		sideIndices(numSegments, &mesh);

		ew::Vertex bottomCenter;
		bottomCenter.pos = ew::Vec3(0, bottomY, 0);
		mesh.vertices.push_back(bottomCenter);
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
				ew::Vec3 pos = normal * size;
				pos -= (a + b) * size;
				pos += (a * col + b * row) * size;
				pos.x += 1.5f;
				ew::Vertex vert;
				vert.pos = pos;
				vert.normal = normal;
				vert.uv = ew::Vec2(col / (subdivisions * 1.0), row / (subdivisions * 1.0));
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