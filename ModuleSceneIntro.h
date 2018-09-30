#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "p2List.h"
#include "Timer.h"
#include "MathGeoLib/MathGeoLib.h"

#include <list>

#define ROAD_HEIGHT 9
#define ROAD_WIDTH 15
#define INVISIBLE_WALL 12
#define MAP_SCALE 2

using namespace std;

class Primitive;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();
	//void OnCollision(PhysBody3D* body1, PhysBody3D* body2);

	bool Save(rapidjson::Document& document, rapidjson::FileWriteStream& os);

	bool Load(rapidjson::Document& document);

	void Draw();

public:

	Sphere S_Test_A;
	Sphere S_Test_B;
	AABB   AABB_Test_A;
	AABB   AABB_Test_B;

	list<Primitive*> ShapesToDraw;

};
