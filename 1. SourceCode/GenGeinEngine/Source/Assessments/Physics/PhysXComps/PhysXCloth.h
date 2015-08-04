#pragma once
#include <PxPhysicsAPI.h>
#include <cloth\PxCloth.h>

class PhysXCloth
{
public:
	PhysXCloth();
	~PhysXCloth();

	void SetUp(physx::PxPhysics* a_physics, physx::PxScene* a_scene, const physx::PxVec3 a_position,
		const unsigned int a_row, const unsigned int a_col, const float a_springSize);

	void Shutdown();
	
	void Update(const double a_dt);
	void Render();

private:

	void CreateVertexInfo( const unsigned int a_row,
		const unsigned int a_col, const float a_size, 
		const physx::PxVec3 a_position);

	void CreateIndexInfo(unsigned int* a_indicies, 
		const unsigned int a_row, const unsigned int a_col);
	
	void CreateCloth(const physx::PxVec3& a_position,
		unsigned int& a_vertexCount, unsigned int& a_indexCount,
		const physx::PxVec3* a_vertices,
		unsigned int* a_indices);

	physx::PxCloth* m_cloth;

	unsigned int* m_shaderID;
	
	unsigned int m_clothIndexCount;
	unsigned int m_clothVertexCount;
	physx::PxVec3* m_clothPositions;

	unsigned int m_clothVBO,m_clothTextureVBO, m_clothIBO;

	physx::PxPhysics* m_pPhysicsRef;
};

