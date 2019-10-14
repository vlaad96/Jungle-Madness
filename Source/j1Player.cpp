#include "j1Player.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Collision.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Window.h"

bool j1Player::Awake(pugi::xml_node& config) {

	LOG("Loading Player Parser");
	bool ret = true;

	folder.create(config.child("folder").child_value());
	texture.create(config.child("texture").child_value());
	//animations
	Idle = LoadAnimation(folder.GetString(), "Idle_Sword_Sheathed");

	int x = config.child("collider").attribute("x").as_int();
	int y = config.child("collider").attribute("y").as_int();
	int w = config.child("collider").attribute("width").as_int();
	int h = config.child("collider").attribute("height").as_int();
	Player_Collider_Rect = { x,y,w,h };



	Position.x = 50;
	Position.y = 650;



	CurrentAnimation = Idle;



	return ret;
}

j1Player::j1Player()
{
	name.create("player");

}

j1Player::~j1Player()
{

}

bool j1Player::Start()
{

	LOG("Loading player");

	Player_Collider = App->col->AddCollider(Player_Collider_Rect, COLLIDER_PLAYER, this);

	State_Player = IDLE;

	if (Spritesheet == nullptr)
	{
		Spritesheet = App->tex->Load(texture.GetString());
	}

	return true;
}

bool j1Player::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		if (God_Mode == false)
		{
			God_Mode = true;
		}
		else
		{
			God_Mode = false;
		}
	}

	if (InitialMoment)
	{
		State_Player = FALLING;
	}

	if (Was_Right) {}

	return true;
}

bool j1Player::PostUpdate()
{
	bool ret = true;

	//Blitting player
	App->render->Blit(Spritesheet, Position.x, Position.y, &CurrentAnimation->GetCurrentFrame());

	return ret;
}

void j1Player::OnCollision(Collider * c1, Collider * c2)
{

}

bool j1Player::Load(pugi::xml_node &config)
{

	bool ret = true;

	Position.x = config.child("Playerx").attribute("value").as_float();
	Position.y = config.child("Playery").attribute("value").as_float();

	return ret;
}

bool j1Player::Save(pugi::xml_node &config) const
{
	config.append_child("Playerx").append_attribute("value") = Position.x;
	config.append_child("Playery").append_attribute("value") = Position.y;

	return true;
}

Animation* j1Player::LoadAnimation(const char* animationPath, const char* animationName) {

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

bool j1Player::CleanUp()
{
	bool ret = true;
	App->tex->UnLoad(Spritesheet);

	return ret;
}