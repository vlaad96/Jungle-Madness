#ifndef __J1ENTITYMANAGER_H__
#define __J1ENTITYMANAGER_H__

#include "p2List.h"
#include "j1Module.h"
#include "j1Entity.h"
#include "j1Player.h"
#include "j1Bat.h"
#include "j1Snake.h"

#define DEFAULT_LPS 60//(LOGIC PER SEC)

class j1Entity;

class j1EntityManager : public j1Module
{
public:

	j1EntityManager();

	// Destructor
	virtual ~j1EntityManager();

	// Called before render is available
	bool Awake(pugi::xml_node& config); 

	// Called before the first frame
	bool Start(); 

	// Called each loop iteration
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate(float dt);

	// Called before quitting
	bool CleanUp();

	j1Entity * const EntityCreation(const char* entname, entity_type entitytype);
	void EntityUpdate(float dt);
	void KillEntity(j1Entity* entity);

	void OnCollision(Collider* c1, Collider* c2);
	
	Animation* LoadAnimation(const char* animationPath, const char* animationName);
	
	PlayerData& GetPlayerData() { return playerinfo; }//player info
	BatData& GetBatData() { return batinfo; }
	SnakeData& GetSlimeData() { return snakeinfo; }


	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

public:

	p2List <j1Entity*>	entities;
	bool				logic;
	int					logic_updates_per_second;
	float				update_ms_cycle;
	float				accumulated_time;

	int					entityID = 0;

private:
	PlayerData playerinfo;
	BatData batinfo;
	SnakeData snakeinfo;
};

#endif // __J1ENTITYMANAGER_H__
