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

