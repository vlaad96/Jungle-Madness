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
	//entCol = App->col->AddCollider(entColliderRect, COLLIDER_TYPE::CO)

	return true;
}