#include <glm\glm.hpp>
#include <PxPhysics.h>
#include <PxPhysicsAPI.h>
#include <cloth\PxCloth.h>

#include <extensions\PxExtensionsAPI.h>
#include "Engine\Core\ShaderHandler.h"
#include "Engine\Renderer\TextureHandler.h"

#include "PhysXCloth.h"

using namespace physx;

PhysXCloth::PhysXCloth()
{
}

PhysXCloth::~PhysXCloth()
{

}

void PhysXCloth::SetUp(PxPhysics* a_physics, PxScene* a_scene, const PxVec3 a_position,
	const unsigned int a_rows, const unsigned int a_cols, const float a_springSize)
{
	m_pPhysicsRef = a_physics;

	ShaderHandler::LoadShaderProgram("ClothPhysX",
		"Data/Shaders/Physics/Cloth.vert",
		"Data/Shaders/Physics/Cloth.frag");
	m_shaderID = &ShaderHandler::GetShader("ClothPhysX");

	TextureHandler::LoadTexture("ClothPhysX", "clothDiffuseMap", "Data/Textures/logo.jpg");
	
	// Set cloth properties
	float springSize = 0.2f;

	// The position will represent the top middle vertex
	PxVec3 clothPosition = PxVec3(0, 40, 0);

	m_clothIndexCount = (a_rows - 1) * (a_cols - 1) * 2 * 3;
	m_clothVertexCount = a_rows * a_cols;

	CreateVertexInfo(a_rows, a_cols, springSize, clothPosition);

	unsigned int* indices = new unsigned int[m_clothIndexCount];
	CreateIndexInfo(indices,a_rows, a_cols);

	CreateCloth(clothPosition, m_clothVertexCount, m_clothIndexCount, m_clothPositions, indices);
	a_scene->addActor(*m_cloth);

	delete[] indices;
}

void PhysXCloth::Shutdown(){

}
	
void PhysXCloth::Update(const double a_dt)
{

}

void PhysXCloth::Render()
{
	// bind shader and transforms, along with texture
	glUseProgram(*m_shaderID);
	
	TextureHandler::RenderAllTextures();
	
	// update vertex positions from the cloth
	glBindBuffer(GL_ARRAY_BUFFER, m_clothVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_clothVertexCount * sizeof(PxVec3),
		m_clothPositions);

	// bind and draw the cloth
	glBindVertexArray(m_clothIBO);
	glDrawElements(GL_TRIANGLES, m_clothIndexCount, GL_UNSIGNED_INT, 0);
}

// Private Functions:

void PhysXCloth::CreateVertexInfo(
	const unsigned int a_row, const unsigned int a_col,
	const float a_size, const PxVec3 a_position)
{
	// Shifting grid positions for looks
	float halfWidth = a_row * a_size * 0.5f;

	PxVec2* clothTexCoords = new PxVec2[m_clothVertexCount];
	m_clothPositions = new PxVec3[m_clothVertexCount];

	for (unsigned int r = 0; r < a_row; ++r)
	{
		for (unsigned int c = 0; c < a_col; ++c)
		{
			m_clothPositions[r * a_col + c].x = a_position.x + a_size * c;
			m_clothPositions[r * a_col + c].y = a_position.y;
			m_clothPositions[r * a_col + c].z = a_position.z + a_size * r
				- halfWidth;
			clothTexCoords[r * a_col + c].x = 1.0f - r / (a_row - 1.0f);
			clothTexCoords[r * a_col + c].y = 1.0f - c / (a_col - 1.0f);
		}
	}

	// Create and bind buffers to a vertex array object
	glGenBuffers(1, &m_clothVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_clothVBO);
	glBufferData(GL_ARRAY_BUFFER, m_clothVertexCount * sizeof(PxVec3), m_clothPositions, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(PxVec3), (void*)0);

	glGenBuffers(1, &m_clothTextureVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_clothTextureVBO);
	glBufferData(GL_ARRAY_BUFFER, m_clothVertexCount * sizeof(PxVec2), clothTexCoords, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(PxVec2), (void*)0);

	delete[] clothTexCoords;
}

void PhysXCloth::CreateIndexInfo(unsigned int *a_indicies, 
	const unsigned int a_row, const unsigned int a_col)
{
	// Set up indices for a grid
	m_clothIndexCount = (a_row - 1) * (a_col - 1) * 2 * 3;
	
	for (unsigned int r = 0; r < (a_row - 1); ++r)
	{
		for (unsigned int c = 0; c < (a_col - 1); ++c)
		{
			// Indices for the 4 quad corner vertices
			unsigned int i0 = r * a_col + c;
			unsigned int i1 = i0 + 1;
			unsigned int i2 = i0 + a_col;
			unsigned int i3 = i2 + 1;
			// Every second quad changes the triangle order
			if ((c + r) % 2)
			{
				*a_indicies++ = i0; *a_indicies++ = i2; *a_indicies++ = i1;
				*a_indicies++ = i1; *a_indicies++ = i2; *a_indicies++ = i3;
			}
			else
			{
				*a_indicies++ = i0; *a_indicies++ = i2; *a_indicies++ = i3;
				*a_indicies++ = i0; *a_indicies++ = i3; *a_indicies++ = i1;
			}
		}
	}

	// Gen and bind data to Index buffer object
	glGenBuffers(1, &m_clothIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_clothIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_clothIndexCount * sizeof(uint), a_indicies, GL_STATIC_DRAW);
}

void PhysXCloth::CreateCloth(const PxVec3& a_position,
	unsigned int& a_vertexCount, unsigned int& a_indexCount,
	const PxVec3* a_vertices,
	unsigned int* a_indices)
{
	// set up the cloth description
	PxClothMeshDesc clothDesc;
	clothDesc.setToDefault();
	clothDesc.points.count = a_vertexCount;
	clothDesc.triangles.count = a_indexCount / 3;
	clothDesc.points.stride = sizeof(glm::vec3);
	clothDesc.triangles.stride = sizeof(unsigned int) * 3;
	clothDesc.points.data = a_vertices;
	clothDesc.triangles.data = a_indices;

	// cook the geometry into fabric

	PxClothFabric* fabric = PxClothFabricCreate(*m_pPhysicsRef, clothDesc, PxVec3(0, 9.87f, 0));
	// set up the particles for each vertex
	PxClothParticle* particles = new PxClothParticle[a_vertexCount];
	for (unsigned int i = 0; i < a_vertexCount; ++i)
	{
		particles[i].pos = PxVec3(a_vertices[i].x, a_vertices[i].y, a_vertices[i].z);
		// set weights (0 means static)
		if (a_vertices[i].x == a_position.x)
			particles[i].invWeight = 0;
		else
			particles[i].invWeight = 1.f;
	}
	// create the cloth then setup the spring properties
	m_cloth = m_pPhysicsRef->createCloth(PxTransform(a_position), *fabric, particles, PxClothFlags());

	// we need to set some solver configurations
	if (m_cloth != nullptr)
	{
		m_cloth->setSolverFrequency(240.0f);
		m_cloth->setStretchConfig(PxClothFabricPhaseType::eVERTICAL, PxClothStretchConfig(1.0f));
		m_cloth->setStretchConfig(PxClothFabricPhaseType::eHORIZONTAL, PxClothStretchConfig(0.9f));
		m_cloth->setStretchConfig(PxClothFabricPhaseType::eSHEARING, PxClothStretchConfig(0.75f));
		m_cloth->setStretchConfig(PxClothFabricPhaseType::eBENDING, PxClothStretchConfig(0.5f));

		PxClothStretchConfig stretchConfig;
		stretchConfig.stiffness = 0.8f;
		stretchConfig.stiffnessMultiplier = 0.5f;
		stretchConfig.compressionLimit = 0.6f;
		stretchConfig.stretchLimit = 1.2f;
		m_cloth->setStretchConfig(PxClothFabricPhaseType::eVERTICAL, stretchConfig);

		// Two spheres located on the x-axis
		PxClothCollisionSphere spheres[2] =
		{
			PxClothCollisionSphere(PxVec3(-1.0f, 0.0f, 0.0f), 0.5f),
			PxClothCollisionSphere(PxVec3(1.0f, 0.0f, 0.0f), 0.25f)
		};

		m_cloth->setCollisionSpheres(spheres, 2);
		m_cloth->addCollisionCapsule(0, 1);

		m_cloth->addCollisionPlane(PxClothCollisionPlane(PxVec3(0.0f, 1.0f, 0.0f), 0.0f));
		m_cloth->addCollisionConvex(1 << 0); // Convex references the first plane
	}
	delete[] particles;
}