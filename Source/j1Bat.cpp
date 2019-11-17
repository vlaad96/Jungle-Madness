#include "j1Bat.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Collision.h"
#include "j1EntityManager.h"

j1Bat::j1Bat() : j1Entity("Bat", entity_type::BAT)
{
	
}

j1Bat::~j1Bat()
{

}

bool j1Bat::Start()
{
	LOG("Loading Bat");

	return true;
}


bool j1Bat::Update(float dt)
{
	bool ret = true;

	return ret;
}

bool j1Bat::PostUpdate(float dt)
{
	bool ret = true;

	return ret;
}

bool j1Bat::CleanUp()
{
	bool ret = true;

	return ret;
}

void j1Bat::FixedUpdate(float dt)
{
	PostUpdate(dt);
}

void j1Bat::LogicUpdate(float dt)
{
	Update(dt);

	// --- Set batpos, prevent surpassing colliders ---
	entitycoll->SetPos(position.x, position.y);

	App->coll->Update(1.0f);

	entitycoll->SetPos(position.x, position.y);
}

void j1Bat::OnCollision(Collider * c1, Collider * c2)
{
}
