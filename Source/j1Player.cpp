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

j1Player::j1Player()
{
	name.create("Player");
}

j1Player::~j1Player()
{

}

bool j1Player::Awake(pugi::xml_node& config)
{
	LOG("Loading Player Parser");
	bool ret = true;

	pugi::xml_node playernode = config.child("player");

	folder.create(config.child("folder").child_value());
	texture.create(config.child("texture").child_value());

	Idle = LoadAnimation(folder.GetString(), "Idle_Sword_Sheathed");
	Run = LoadAnimation(folder.GetString(), "Run_Sword_Sheathed");
	Jump = LoadAnimation(folder.GetString(), "Jump");
	Fall = LoadAnimation(folder.GetString(), "Fall");
	Death = LoadAnimation(folder.GetString(), "Knockout");
	Slide = LoadAnimation(folder.GetString(), "Slide");
	Wall_Slide = LoadAnimation(folder.GetString(), "Wall_Slide");

	//Load with object group maybe? lazy p o s
	Player_Collider_Rect =
	{
		config.child("collider").attribute("x").as_int(),
		config.child("collider").attribute("y").as_int(),
		config.child("collider").attribute("width").as_int(),
		config.child("collider").attribute("height").as_int()
	};

	//Player config

	Velocity.x = config.child("velocity").attribute("x").as_float();
	Velocity.y = config.child("velocity").attribute("y").as_float();
	Gravity = config.child("gravity").attribute("value").as_float();
	Jump_Force = config.child("Velocity").attribute("jump_force").as_float();
	Initial_Velocity_x = config.child("Velocity").attribute("initalVx").as_float();
	Max_Speed_y = config.child("Velocity").attribute("max_speed_y").as_float();
	Colliding_Offset = config.child("colliding_offset").attribute("value").as_float();

	CurrentAnimation = Idle;

	Death->loop = false;

	return ret;
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

	return ret;
}

bool j1Player::CleanUp()
{
	bool ret = true;
	App->tex->UnLoad(Spritesheet);

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

Animation* j1Player::LoadAnimation(const char* animationPath, const char* animationName)
{
	Animation* animation = new Animation();

	bool anim = false;

	pugi::xml_document AnimationData;
	pugi::xml_parse_result result = AnimationData.load_file(animationPath);

	if (result == NULL)
	{
		LOG("Issue loading animation");
	}

	pugi::xml_node object_group;
	for (object_group = AnimationData.child("map").child("objectgroup"); object_group; object_group = object_group.next_sibling("objectgroup"))
	{
		p2SString name = object_group.attribute("name").as_string();

		if (name == animationName)
		{
			anim = true;
			int x, y, h, w;

			for (pugi::xml_node sprite = object_group.child("object"); sprite; sprite = sprite.next_sibling("object"))
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
	{
		return animation;
	}

	else
	{
		return nullptr;
	}
}
