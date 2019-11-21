#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1EntityManager.h"
#include "Entity.h"
#include "Player.h"


j1EntityManager::j1EntityManager() : j1Module()
{
	name.create("entities");
}

j1EntityManager::~j1EntityManager()
{

}

bool j1EntityManager::Awake(pugi::xml_node& config)
{

	LOG("Setting up Entity manager");
	bool ret = true;

	//Player
	LOG("Loading player parser");

	pugi::xml_node playerNode = config.child("player");

	playerData.folder.create(playerNode.child("folder").child_value());
	playerData.texture.create(playerNode.child("texture").child_value());

	playerData.Idle = LoadAnimation(playerData.folder.GetString(), "Idle");
	playerData.Run = LoadAnimation(playerData.folder.GetString(), "Run");
	playerData.Jump = LoadAnimation(playerData.folder.GetString(), "Jump");
	playerData.Fall = LoadAnimation(playerData.folder.GetString(), "Fall");
	playerData.Slide = LoadAnimation(playerData.folder.GetString(), "Slide");
	playerData.Wall_Slide = LoadAnimation(playerData.folder.GetString(), "Wall_Slide");
	playerData.Death = LoadAnimation(playerData.folder.GetString(), "Death");
	playerData.God = LoadAnimation(playerData.folder.GetString(), "God_Mode");

	int x = playerNode.child("collider").attribute("x").as_int();
	int y = playerNode.child("collider").attribute("y").as_int();
	int w = playerNode.child("collider").attribute("width").as_int();
	int h = playerNode.child("collider").attribute("height").as_int();
	playerData.Player_Collider_Rect = { x,y,w,h };

	//Player stats and variables
	playerData.Velocity.x = playerNode.child("velocity").attribute("x").as_float();
	playerData.Velocity.y = playerNode.child("velocity").attribute("y").as_float();

	playerData.Jump_Force = playerNode.child("velocity").attribute("jump_force").as_float();

	playerData.Gravity = playerNode.child("gravity").attribute("value").as_float();

	playerData.Idle->speed = 10.0f;
	playerData.Run->speed = 10.0f;
	playerData.Jump->speed = 10.0f;
	playerData.Slide->speed = 10.0f;
	playerData.Fall->speed = 10.0f;
	playerData.Death->speed = 10.0f;
	playerData.God->speed = 10.0f;

	playerData.Death->loop = false;

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
	
	return true;
}

bool j1EntityManager::Update(float dt)
{

	return true;
}

void j1EntityManager::UpdateEntity(float dt)
{
	p2List_item <Entity*> *entity = entities.start;

	while (entity !=NULL)
	{
		entity->data->LogicUpdate(dt);

		entity = entity->next;
	}
}

bool j1EntityManager::PostUpdate(float dt)
{
	p2List_item <Entity*> *entity = entities.start;

	while (entity != NULL)
	{
		entity->data->FixedUpdate(dt);

		entity = entity->next;
	}

	return true;
}

bool j1EntityManager::CleanUp()
{
	LOG("Cleanup EntityManager");

	// release all entities
	p2List_item<Entity*>* entity = entities.start;

	while (entity != NULL)
	{
		entity->data->CleanUp();
		RELEASE(entity->data);
		entity = entity->next;
	}

	entities.clear();
	return true;
}

bool j1EntityManager::Load(pugi::xml_node &file)
{
	p2List_item <Entity*> *entity = entities.start;

	while (entity != NULL)
	{
		entity->data->Load(file);

		entity = entity->next;
	}


	return true;
}

bool j1EntityManager::Save(pugi::xml_node &file) const
{
	p2List_item <Entity*> *entity = entities.start;

	while (entity != NULL)
	{
		entity->data->Save(file);

		entity = entity->next;
	}

	return true;
}

Entity* const j1EntityManager::CreateEntity(const char* name, entityTypes type)
{
	Entity* entity = nullptr;

	switch (type)
	{
	case PLAYER:
		entity = new Player();
		break;
	/*case GROUND_ENEMY:
		break;
	case FLYING_ENEMY:
		break;
	case ENDGAME:
		break;
	default:
		break;*/
	}

	entityID++;
	entity->Init(this);
	entity->Start();
	entities.add(entity);
	return(entity);

}

void j1EntityManager::DeleteEntity(Entity* entity)
{
	p2List_item <Entity*> *entity_item = entities.At(entities.find(entity));

	entities.del(entity_item);
	entityID--;

}

void j1EntityManager::OnCollision(Collider * c1, Collider * c2)
{
	p2List_item <Entity*> *entity = entities.start;

	while (entity != NULL)
	{
		if (entity->data->entCol == c1)
		{
			entity->data->OnCollision(c1, c2);
			break;
		}

		entity = entity->next;
	}
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