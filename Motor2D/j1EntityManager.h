#ifndef __J1ENTITYMANAGER_H__
#define __J1ENTITYMANAGER_H__

#include "p2List.h"
#include "j1Module.h"
#include "j1Entity.h"
#include "j1Player.h"

class j1Entity;

class j1EntityManager : public j1Module
{
public:

	j1EntityManager();

	// Destructor
	virtual ~j1EntityManager();

	// Called before render is available
	bool Awake(pugi::xml_node&); // spritesheets and animations

	// Called before the first frame
	bool Start(); // textures

	// Called each loop iteration
	bool PreUpdate();
	bool Update(float dt);

	bool PostUpdate(float dt);

	// Called before quitting
	bool CleanUp();

	// --- Save & Load ---
	bool Load(pugi::xml_node&);

	bool Save(pugi::xml_node&) const;

private:

};

#endif // __J1ENTITYMANAGER_H__
