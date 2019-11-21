#ifndef _J1ENTITYMANAGER_H_
#define _J1ENTITYMANAGER_H_

#include "p2List.h"
#include "j1Module.h"
#include "Entity.h"
#include "Player.h"

class Entity;

struct PathfindingInfo
{
	iPoint startingPos = { 0,0 };
	iPoint endPos = { 0,0 };
	iPoint* path = nullptr;

	PathfindingInfo();
	~PathfindingInfo();

};

class j1EntityManager :public j1Module
{
public:

	j1EntityManager();
	virtual ~j1EntityManager();

	// --- Called before render is available ---
	bool Awake(pugi::xml_node&);

	// --- Called before the first frame ---
	bool Start();

	// --- Called each loop iteration ---
	bool PreUpdate();
	bool Update(float dt);
	void UpdateEntity(float dt);
	bool PostUpdate(float dt);

	// --- Called before quitting ---
	bool CleanUp();

	//Entities
	Entity* const CreateEntity(const char* name, entityTypes type);
	void DeleteEntity(Entity* entity);
	void OnCollision(Collider* col1, Collider* col2);
	Animation* LoadAnimation(const char* animPath, const char* animName);

	//Save and Load
	bool Save(pugi::xml_node&)const;
	bool Load(pugi::xml_node&);

public:

	p2List <Entity*> entities;
	
	int entityID = 0;

private:

	//Info for every entity
	PlayerInfo playerData;
	
};

#endif // !_J1ENTITYMANAGER_H_