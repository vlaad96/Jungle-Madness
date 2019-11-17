#ifndef __J1ENTITY_H__
#define __J1ENTITY_H__

#include "p2SString.h"
#include "p2Point.h"
#include "Animation.h"
#include "SDL\include\SDL.h"
#include "PugiXml\src\pugixml.hpp"

class j1EntityManager;
class Collider;

enum entity_state
{
	IDLE,
	RIGHT,
	LEFT,
	JUMPING,
	FALLING
};

enum class entity_type
{
	PLAYER,
	SNAKE,
	BAT
};

class j1Entity
{
public:

	j1Entity(const char* entityname, entity_type entitytype) : manager(NULL), Entity_Type(entitytype)
	{
		name.create(entityname);
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

	virtual bool Save(pugi::xml_node&) const
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

	virtual void OnCollision(Collider* c1, Collider* c2)
	{}



public:

	p2SString			name;
	fPoint			Position;
	fPoint          Velocity;
	float gravity = 0;

	// collider
	Collider*     Entity_Collider = nullptr;
	SDL_Rect Entity_Collider_Rect;
	float colliding_offset = 0;

	// entityinfo
	entity_type  Entity_Type;
	entity_state Entity_State;

	// art
	Animation* CurrentAnimation = nullptr;
	SDL_Texture* spritesheet = nullptr;

	j1EntityManager*	manager;
};

#endif // __J1ENTITY_H__
