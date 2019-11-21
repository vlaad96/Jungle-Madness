#include "Troll.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Collision.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Window.h"
#include "j1Audio.h"
#include "j1EntityManager.h"
#include "Player.h"
#include "Pathfinding.h"
#include "p2Log.h"


Troll::Troll() :Entity("Troll", entityTypes::TROLL) {}

Troll::~Troll() {}

bool Troll::Start()
{
	LOG("Loading Troll");

	trollData = manager->GetTroll();

	entColliderRect = trollData.trollRect;
	colliderOffset = trollData.colliderOffset;
	entCol = App->col->AddCollider(entColliderRect, COLLIDER_TYPE::COLLIDER_TROLL, (j1Module*)manager);

	currentAnim = trollData.idleRight;
	trollData.walkRight->speed = trollData.animSpeed;
	trollData.walkLeft->speed = trollData.animSpeed;

	gravity = trollData.gravity;

	pos.x = NULL;
	pos.y = NULL;

	entState = FALLING;

	rightDirection = true;
	leftDirection = false;
	willFall = true;
	dead = false;

	if (sprites == nullptr)
		sprites = App->tex->Load(trollData.texture.GetString());

	id = App->entities->entityId;

	isActive = true;

	return true;
}

bool Troll::Update(float dt)
{
	willFall = true;
	entState = FALLING;

	return true;
}

bool Troll::PostUpdate(float dt)
{
	
	return true;
}

void Troll::OnCollision(Collider* col1, Collider* col2)
{
	
}

bool Troll::ResetVars()
{
	pathfindSize = 0;

	return true;
}

bool Troll::CreatePath(const iPoint dest)
{
	return true;
}

bool Troll::Pathfind(float dt)
{
	return true;
}

void Troll::UpdateMov(float dt)
{

}

bool Troll::Load(pugi::xml_node &config)
{
	if (entId == trollData.id.x)
	{
		pos.x = config.child("Entity4").child("Troll").attribute("value").as_float();
		pos.y = config.child("Entity4").child("Troll").attribute("value").as_float();
		isActive = config.child("Entity4").child("active").attribute("value").as_bool();
	}
	else if (entId == trollData.id.y)
	{
		pos.x = config.child("Entity5").child("Troll").attribute("value").as_float();
		pos.y = config.child("Entity5").child("Troll").attribute("value").as_float();
		isActive = config.child("Entity5").child("active").attribute("value").as_bool();
	}
}

bool Troll::Save(pugi::xml_node &config)const
{
	if (entId == trollData.id.x)
	{
		config.append_child("Entity4").append_child("Troll").append_attribute("value") = pos.x;
		config.append_child("Entity4").append_child("Troll").append_attribute("value") = pos.x;
		config.append_child("Entity4").append_child("Troll").append_attribute("value") = pos.x;
	}
}