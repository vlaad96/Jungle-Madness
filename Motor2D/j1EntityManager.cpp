#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1EntityManager.h"

j1EntityManager::j1EntityManager() : j1Module()
{
	name.create("entities");
}

// Destructor
j1EntityManager::~j1EntityManager()
{}

bool j1EntityManager::Awake(pugi::xml_node& config)
{
	bool ret = true;
	return ret;
}

// Called before the first frame
bool j1EntityManager::Start()
{
	LOG("start j1EntityManager");
	bool ret = true;
	return ret;
}

bool j1EntityManager::PreUpdate()
{
	return true;
}

bool j1EntityManager::Update(float dt)
{
	return true;
}

bool j1EntityManager::PostUpdate(float dt)
{
	return true;
}

bool j1EntityManager::CleanUp()
{
	return true;
}

bool j1EntityManager::Load(pugi::xml_node &file)
{
	return true;
}

bool j1EntityManager::Save(pugi::xml_node &file) const
{
	return true;
}