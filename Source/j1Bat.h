#ifndef __j1BAT_H__
#define __j1BAT_H__

#include "j1Entity.h"

struct SDL_Texture;
struct Collider;
struct PathInfo;

struct BatData {

	Animation* Move = nullptr;

	p2SString folder = nullptr;
	p2SString Texture = nullptr;

	SDL_Rect		Bat_Collider_Rect = { 0,0,0,0 };
	fPoint          Velocity = { 0,0 };
	fPoint          Velocity_Aux = { 0,0 };
	iPoint			Print_offset = { 0,0 };

	float           Gravity = 0;
	float			Initial_Velocity_x = 0;
	float			Colliding_Offset = 0;
	float			Animation_Speed = 0;

	int				Area_Of_Action = 0;
	iPoint			Reference_ID = { 0,0 };

};

class j1Bat :public j1Entity
{
public:

	j1Bat();
	~j1Bat();

	bool Start();
	bool Update(float dt);
	bool PostUpdate(float dt);
	bool CleanUp();

	// Called each loop iteration
	void FixedUpdate(float dt);

	// Called each logic iteration
	void LogicUpdate(float dt);

	void OnCollision(Collider* c1, Collider* c2);

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	//Pathfinding functions here

public:
	bool dead = false;
	bool going_right = false;
	bool going_left = false;
	bool going_up = false;
	bool going_down = false;

	bool batcolliding = false;

	BatData BatInfo;

	//Pathfinding stuff here

	PathInfo* path_info = nullptr;
}

#endif // __j1BAT_H__
