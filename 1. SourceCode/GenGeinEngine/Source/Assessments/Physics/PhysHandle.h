#pragma once
#include "PhysComps\Assets\PhysActor.h"

class PhysScene;

class PhysHandle
{
public:
	PhysHandle();
	~PhysHandle();

	void SetUpInstObjects(FBXModel* a_plane, FBXModel* a_sphere, FBXModel* a_box);
	void StartUp();
	void ShutDown();
	void Update(double a_dt);
	void Render();
private:
	struct Sweeper
	{
		Sweeper(PhysActor* a_actor, glm::vec3 a_size)
			: actor(a_actor), size(a_size)
		{}

		glm::vec3 GetMin() { return actor->GetPosition() - size; }
		glm::vec3 GetMax() { return actor->GetPosition() + size; }
		PhysActor* actor;
		glm::vec3 size;
	};

	// helper functions to keep StartUp clean
	void BuildWalling();
	void BuildPhysGeo();

	// Sweep and Clean
	//void SweepAndClean(double a_dt);
	//bool CheckBoundingBoxForOverlap(PhysActor* a_A, PhysActor* a_B);

	PhysScene* m_scene;
	//std::vector<Sweeper> m_sweepList;
};