#include "j1Snake.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Collision.h"
#include "j1EntityManager.h"

j1Snake::j1Snake() : j1Entity("Snake", entity_type::SNAKE)
{
}

j1Snake::~j1Snake()
{
}

bool j1Snake::Start()
{
	LOG("Loading Snake");

	return true;
}

bool j1Snake::Update(float dt)
{
	return false;
}

bool j1Snake::PostUpdate(float dt)
{
	return false;
}

bool j1Snake::CleanUp()
{
	return false;
}

void j1Snake::FixedUpdate(float dt)
{
}

void j1Snake::LogicUpdate(float dt)
{
}

void j1Snake::OnCollision(Collider * c1, Collider * c2)
{
}

bool j1Snake::Load(pugi::xml_node &)
{
	return false;
}

bool j1Snake::Save(pugi::xml_node &) const
{
	return false;
}
