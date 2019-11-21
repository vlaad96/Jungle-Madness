#ifndef _ENTITY_H_
#define _ENTITY_H_


#include "p2SString.h"
#include "p2Point.h"
#include "SDL\include\SDL.h"
#include "Animation.h"
#include "PugiXml\src\pugixml.hpp"
#include "p2DynArray.h"

class j1EntityManager;
struct Collider;

enum entityStates
{
	IDLE = 0,
	LEFT,
	RIGHT,
	JUMPING,
	FALLING,
	FLYING,
};

enum entityTypes
{
	PLAYER,
	TROLL,
	FLY,
	ENDGAME
};

class Entity
{
public:

	Entity(const char* name, entityTypes type) : manager(NULL), entType(type)
	{
		entName.create(name);
	}

	void Init(j1EntityManager* manager)
	{
		this->manager = manager;
	}

	virtual bool Start()
	{
		return true;
	}

	virtual bool Load(pugi::xml_node&)
	{
		return true;
	}

	virtual bool Save(pugi::xml_node&)
	{
		return true;
	}

	// Called each loop iteration
	virtual void FixedUpdate(float dt)
	{}

	// Called each logic iteration
	virtual void LogicUpdate(float dt)
	{}

	// Called before quitting
	virtual bool CleanUp()
	{
		return true;
	}

	virtual void OnCollision(Collider* col1, Collider* col2) {}

public:

	j1EntityManager * manager = nullptr;

public:

	//Entities
	p2SString		entName;
	entityTypes		entType;
	entityStates	entState;

	//Position and speed of entities
	fPoint pos = { 0,0 };
	fPoint vel = { 0,0 };

	//Colliders
	Collider* entCol = nullptr;
	SDL_Rect entColliderRect{ 0,0,0,0 };
	float colliderOffset;

	//Gravity
	float gravity = 0;

	//Sprites and animations
	SDL_Texture* sprites = nullptr;
	Animation* currentAnim = nullptr;

	//Is the entity active?
	bool isActive = false;


};
#endif // !_ENTITY_H_

