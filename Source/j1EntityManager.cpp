#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1EntityManager.h"
#include "j1Entity.h"

j1EntityManager::j1EntityManager() : j1Module(), logic_updates_per_second(DEFAULT_LPS), accumulated_time(0.0f)
{
	name.create("entities");
}

// Destructor
j1EntityManager::~j1EntityManager()
{}


// Called before render is available
bool j1EntityManager::Awake(pugi::xml_node& config)
{
	LOG("Setting up Entity manager");
	bool ret = true;
	logic_updates_per_second = DEFAULT_LPS;
	update_ms_cycle = 1.0f / (float)logic_updates_per_second;
	
	LOG("Loading Player Parser");
	pugi::xml_node playernode = config.child("player");

	playerinfo.folder.create(playernode.child("folder").child_value());
	playerinfo.Texture.create(playernode.child("texture").child_value());

	//animations
	playerinfo.Idle = LoadAnimation(playerinfo.folder.GetString(), "Idle_Sword_Sheathed");
	playerinfo.Run = LoadAnimation(playerinfo.folder.GetString(), "Run_Sword_Sheathed");
	playerinfo.Jump = LoadAnimation(playerinfo.folder.GetString(), "Jump");
	playerinfo.Fall = LoadAnimation(playerinfo.folder.GetString(), "Fall");
	playerinfo.Death = LoadAnimation(playerinfo.folder.GetString(), "Knockout");
	playerinfo.Slide = LoadAnimation(playerinfo.folder.GetString(), "Slide");
	playerinfo.Wall_Slide = LoadAnimation(playerinfo.folder.GetString(), "Wall_Slide");
	playerinfo.God = LoadAnimation(playerinfo.folder.GetString(), "God_Mode");

	//COLLIDER PLAYER
	
	int x = playernode.child("collider").attribute("x").as_int();
	int y = playernode.child("collider").attribute("y").as_int();
	int w = playernode.child("collider").attribute("width").as_int();
	int h = playernode.child("collider").attribute("height").as_int();
	playerinfo.Player_Collider_Rect = { x,y,w,h};

	//Player config

	playerinfo.Velocity.x = playernode.child("velocity").attribute("x").as_float();
	playerinfo.Velocity.y = playernode.child("velocity").attribute("y").as_float();
	playerinfo.Gravity = playernode.child("gravity").attribute("value").as_float();
	playerinfo.Jump_Force = playernode.child("velocity").attribute("jump_force").as_float();
	playerinfo.Initial_Velocity_x = playernode.child("velocity").attribute("initalVx").as_float();
	playerinfo.Max_Speed_y = playernode.child("velocity").attribute("max_speed_y").as_float();
	playerinfo.Colliding_Offset = playernode.child("colliding_offset").attribute("value").as_float();


	

	playerinfo.Idle->speed = 0.15f;
	playerinfo.Run->speed = 0.15f;
	playerinfo.God->speed = 0.15f;
	playerinfo.Jump->speed = 0.60f;
	playerinfo.Fall->speed = 0.15f;
	playerinfo.Death->speed = 0.15f;

	

	playerinfo.Death->loop = false;

	return ret;
}

// Called before the first frame
bool j1EntityManager::Start()
{
	LOG("start j1EntityManager");
	bool ret = true;
	return ret;
}

// Called each loop iteration
bool j1EntityManager::PreUpdate()
{
	logic = false;
	return true;
}

bool j1EntityManager::Update(float dt)
{
	accumulated_time += dt;

	if (accumulated_time >= update_ms_cycle)
	{
		logic = true;
	}

	EntityUpdate(dt);

	if (logic == true)
	{
		accumulated_time = accumulated_time - update_ms_cycle;
	}

	return true;
}

void j1EntityManager::EntityUpdate(float dt)
{
	p2List_item <j1Entity*> *entity = entities.start;

	while (entity != NULL)
	{

		/*if (do_logic == true)
		{*/
		entity->data->LogicUpdate(dt);
		//}

		entity = entity->next;
	}

	/*while (entity != NULL)
	{
		entity->data->FixedUpdate(dt);

		if (logic == true)
		{
			entity->data->LogicUpdate(accumulated_time);
		}

		entity = entity->next;
	}*/
}

bool j1EntityManager::PostUpdate(float dt)
{
	p2List_item <j1Entity*> *entity = entities.start;

	while (entity != NULL)
	{
		entity->data->FixedUpdate(dt);

		entity = entity->next;
	}


	return true;
}

// Called before quitting
bool j1EntityManager::CleanUp()
{
	LOG("cleanup j1EntityManager");
	// release all entities
	p2List_item<j1Entity*>* entity = entities.start;

	while (entity != NULL)
	{
		entity->data->CleanUp();
		RELEASE(entity->data);
		entity = entity->next;
	}

	entities.clear();
	return true;
}

j1Entity* const j1EntityManager::EntityCreation(const char* entname, entity_type entitytype)
{
	j1Entity* entity = nullptr;

	switch (entitytype)
	{
	case entity_type::PLAYER:
		entity = new j1Player();
		break;
	}

	entity->Init(this);
	entity->Start();
	entities.add(entity);
	return(entity);
}

void j1EntityManager::KillEntity(j1Entity* entity)
{
	p2List_item <j1Entity*> *entity_item = entities.At(entities.find(entity));

	entities.del(entity_item);
}

void j1EntityManager::OnCollision(Collider * c1, Collider * c2)
{
	p2List_item <j1Entity*> *entity = entities.start;

	while (entity != NULL)
	{
		if (entity->data->Entity_Collider == c1)
		{
			entity->data->OnCollision(c1, c2);
			break;
		}
		entity = entity->next;
	}
}

bool j1EntityManager::Load(pugi::xml_node &file)
{
	p2List_item <j1Entity*> *entity = entities.start;

	while (entity != NULL)
	{
		entity->data->Load(file);

		entity = entity->next;
	}


	return true;
}

bool j1EntityManager::Save(pugi::xml_node &file) const
{
	p2List_item <j1Entity*> *entity = entities.start;

	while (entity != NULL)
	{
		entity->data->Save(file);

		entity = entity->next;
	}

	return true;
}

Animation* j1EntityManager::LoadAnimation(const char* animationPath, const char* animationName) {

	Animation* animation = new Animation();

	bool anim = false;

	pugi::xml_document animationDocument;
	pugi::xml_parse_result result = animationDocument.load_file(animationPath);


	if (result == NULL)
	{
		LOG("Issue loading animation");
	}

	pugi::xml_node objgroup;
	for (objgroup = animationDocument.child("map").child("objectgroup"); objgroup; objgroup = objgroup.next_sibling("objectgroup"))
	{
		p2SString name = objgroup.attribute("name").as_string();
		if (name == animationName)
		{
			anim = true;
			int x, y, h, w;

			for (pugi::xml_node sprite = objgroup.child("object"); sprite; sprite = sprite.next_sibling("object"))
			{
				x = sprite.attribute("x").as_int();
				y = sprite.attribute("y").as_int();
				w = sprite.attribute("width").as_int();
				h = sprite.attribute("height").as_int();

				animation->PushBack({ x, y, w, h });
			}

		}
	}
	if (anim = true)
		return animation;
	else
		return nullptr;

}