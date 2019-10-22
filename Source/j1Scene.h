#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"

struct SDL_Texture;
class Collider;

class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	bool SceneChange(const char* scene);

public:

	fPoint camera_displacement;
	iPoint CamScene1;
private:
	p2List<p2SString*> scenes;
	p2SString firstscene;
	bool scene1 = true;
	bool scene2 = false;
	bool scene1Loaded = false;
	bool scene2Loaded = false;
	p2SString map_name;
	Collider* colliderfloor = nullptr;
};

#endif // __j1SCENE_H__