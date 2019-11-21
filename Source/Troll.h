#ifndef _TROLL_H_
#define _TROLL_H_

#include "Entity.h"

struct SDL_Texture;
struct Collider;
struct PathfindingInfo;

struct TrollInfo {
		
	Animation*	idleRight = nullptr;
	Animation*	idleLeft = nullptr;
	Animation*	walkRight = nullptr;
	Animation*	walkLeft = nullptr;
	Animation*	attackRight = nullptr;
	Animation*	attackLeft = nullptr;
	Animation*	deathRight = nullptr;
	Animation*	deathLeft = nullptr;

	p2SString	folder = nullptr;
	p2SString	texture = nullptr;

	SDL_Rect	trollRect = { 0,0,0,0 };
	fPoint		Velocity = { 0.0f,0.0f };
	fPoint		auxVelocity{ 0.0f,0.0f };
	iPoint		offset = { 0,0 };

	float		gravity = 0;
	float		initialSpeed = 0;
	float		colliderOffset = 0;
	float		animSpeed = 0;

	int			actionArea = 0;
	iPoint		id = { 0,0 };

};

class Troll :public Entity
{
public:

	Troll();
	~Troll();

	bool Start();
	bool Update(float dt);
	bool PostUpdate(float dt);
	bool CleanUp();

	// Called each loop iteration
	void FixedUpdate(float dt);

	// Called each logic iteration
	void LogicUpdate(float dt);

	void OnCollision(Collider* col1, Collider* col2);

	//Save and Load
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	//Pathfinding
	bool CreatePath(const iPoint dest);
	bool Pathfind(float dt);
	void UpdateMov(float dt);
	bool ResetVars();

public:

	PathfindingInfo * pathInfo = nullptr;
	
	const	p2DynArray <iPoint> *lastPath = nullptr;
	uint	pathfindID = 0;
	uint	pathfindSize = 0;

public:

	bool	dead = false;
	bool	rightDirection = false;
	bool	leftDirection = false;
	bool	willFall = false;
	bool	trollColliding = false;

	TrollInfo trollData;

};
#endif // !_TROLL_H_

