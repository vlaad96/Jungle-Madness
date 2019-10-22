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
	Run = LoadAnimation(folder.GetString(), "Run_Sword_Sheathed");
	Jump = LoadAnimation(folder.GetString(), "Jump");
	Fall = LoadAnimation(folder.GetString(), "Fall");
	Death = LoadAnimation(folder.GetString(), "Knockout");
	Slide = LoadAnimation(folder.GetString(), "Slide");
	Wall_Slide = LoadAnimation(folder.GetString(), "Wall_Slide");

	////Load with object group 
	//Player_Collider_Rect = LoadColliderRect(folder.GetString(), "Collider_Player_Idle");
	//{
	//	config.child("collider").attribute("x").as_int(),
	//	config.child("collider").attribute("y").as_int(),
	//	config.child("collider").attribute("width").as_int(),
	//	config.child("collider").attribute("height").as_int()
	//}

	//COLLIDER
	int x = config.child("collider").attribute("x").as_int();
	int y = config.child("collider").attribute("y").as_int();
	int w = config.child("collider").attribute("width").as_int();
	int h = config.child("collider").attribute("height").as_int();
	Player_Collider_Rect = {x, y, w, h};


	//Player config

	Velocity.x = config.child("velocity").attribute("x").as_float();
	Velocity.y = config.child("velocity").attribute("y").as_float();
	Gravity = config.child("gravity").attribute("value").as_float();
	Jump_Force = config.child("velocity").attribute("jump_force").as_float();
	Initial_Velocity_x = config.child("velocity").attribute("initalVx").as_float();
	Max_Speed_y = config.child("velocity").attribute("max_speed_y").as_float();
	Colliding_Offset = config.child("colliding_offset").attribute("value").as_float();

	//Load it from config at least wtf
	Player_Initial_Position.x = 150;
	Player_Initial_Position.y = 600;
	Position.x = Player_Initial_Position.x;
	Position.y = Player_Initial_Position.y;

	Idle->speed = 0.15f;
	Run->speed = 0.15f;

	CurrentAnimation = Idle;

	Death->loop = false;

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

	if (Initial_Moment)
	{
		State_Player = FALLING;
	}

	if (Was_Right == true)
		CurrentAnimation = Idle;
	else if (Was_Right == false)
		CurrentAnimation = Idle;

	//Horizontally

	if (State_Player != DEAD)
	{
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			if (First_Move == false)
			{
				First_Move = true;
			}

			Velocity.x = Initial_Velocity_x;
			Position.x = Position.x - Velocity.x;

			Moving_Left = true;
			Moving_Right = false;
			CurrentAnimation = Run;
			Was_Right = false;
		}


		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{

			if (First_Move == false)
			{
				First_Move = true;
			}

			Velocity.x = Initial_Velocity_x;
			Position.x = Position.x + Velocity.x;

			Moving_Left = true;
			Moving_Right = false;
			CurrentAnimation = Run;
			Was_Right = true;
		}

		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{

			Velocity.x = 0.0f;
			Moving_Left = true;
			Moving_Right = true;
		}
		//testing camera up & down purposes
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT )
		{

			Velocity.y = Initial_Velocity_x;
			Position.y = Position.y - Velocity.y;
			
		}

		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		{

			Velocity.y = Initial_Velocity_x;
			Position.y = Position.y + Velocity.y;

		}

	}


	Player_Collider->SetPos(Position.x + 40, Position.y +5);
	return true;
}

bool j1Player::PostUpdate()
{
	bool ret = true;

	//Calculation for Parallax

	Player_Displacement.x = Player_Initial_Position.x - Position.x;

	App->map->PX = Player_Displacement.x;

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
	{
		return animation;
	}

	else
	{
		return nullptr;
	}

}

SDL_Rect j1Player::LoadColliderRect(const char* colliderPath, const char* colliderName)
{
	SDL_Rect colliderRect;

	bool rect = false;

	pugi::xml_document colliderDocument;
	pugi::xml_parse_result result = colliderDocument.load_file(colliderPath);

	if (result == NULL)
	{
		LOG("Issue loading Collider Rect");
	}
	
	pugi::xml_node objgroup;
	for (objgroup = colliderDocument.child("map").child("objectgroup"); objgroup; objgroup = objgroup.next_sibling("objectgroup"))
	{
		p2SString name = objgroup.attribute("name").as_string();
		if (name == colliderName)
		{
			rect = true;
			int x, y, h, w;

			for (pugi::xml_node sprite = objgroup.child("object"); sprite; sprite = sprite.next_sibling("object"))
			{
				x = sprite.attribute("x").as_int();
				y = sprite.attribute("y").as_int();
				w = sprite.attribute("width").as_int();
				h = sprite.attribute("height").as_int();

				colliderRect = { x, y, w, h };
			}

		}
	}

	if (rect = true)
	{
		return colliderRect;
	}

	else
	{
		return { 0, 0, 10, 10 };
	}

}

bool j1Player::CleanUp()
{
	bool ret = true;
	App->tex->UnLoad(Spritesheet);

	return ret;
}