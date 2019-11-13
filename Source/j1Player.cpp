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
#include "j1EntityManager.h"

j1Player::j1Player() : j1Entity("player", entity_type::PLAYER)
{
	

}

j1Player::~j1Player()
{

}

bool j1Player::Start()
{

	LOG("Loading player");

	playerinfo = manager->GetPlayerData();

	Entity_Collider_Rect = playerinfo.Player_Collider_Rect;

	Entity_Collider = App->col->AddCollider(Entity_Collider_Rect, COLLIDER_PLAYER, (j1Module*)manager);
	Velocity = playerinfo.Velocity;
	gravity = playerinfo.Gravity;
	colliding_offset = playerinfo.Colliding_Offset;

	Entity_State = IDLE;
	CurrentAnimation = playerinfo.Idle;
	Moving_Left = false;
	Moving_Right = false;

	Must_Fall = true;
	Double_Jump = false;
	Player_Colliding = false;
	Initial_Moment = true;
	First_Move = false;
	God_Mode = false;

	if (spritesheet == nullptr)
	{
		spritesheet = App->tex->Load(playerinfo.Texture.GetString());
	}

	Position.x = 0;
	Position.y = 0;
	Player_Initial_Position = Position;

	return true;
}

bool j1Player::Update(float dt)
{
	if (Dead)
	{
		if (App->scene->scene1 == true)
		{
			App->scene->SceneChange(App->scene->scenes.start->data->GetString());
			App->scene->scene1 = true;
			App->scene->scene2 = false;
		}
		else
		{
			App->scene->SceneChange(App->scene->scenes.start->next->data->GetString());
			App->scene->scene1 = false;
			App->scene->scene2 = true;
		}
		Dead = false;
	}

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

	//GODMODE
	if (God_Mode == true)
	{

		Velocity.y = 0;

		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
		{

			Velocity.y = playerinfo.Initial_Velocity_x;
			Position.y = Position.y - Velocity.y;
		}

		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		{
			Velocity.y = playerinfo.Initial_Velocity_x;
			Position.y = Position.y + Velocity.y;;
		}

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			Velocity.x = playerinfo.Initial_Velocity_x;
			Position.x = Position.x - Velocity.x;
			Was_Right = false;
		}

		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			Velocity.x = playerinfo.Initial_Velocity_x;
			Position.x = Position.x + Velocity.x;
			Was_Right = true;
		}

		CurrentAnimation = playerinfo.God;
		Entity_Collider->type = COLLIDER_NONE;

	}
	//NOT GOD MODE
	else
	{

		Entity_Collider->type = COLLIDER_PLAYER;


		if (Initial_Moment)
		{
			Entity_State = FALLING;
		}

		if (Was_Right == true)
		{
			CurrentAnimation = playerinfo.Idle;
		}

		else if (Was_Right == false)
		{
			CurrentAnimation = playerinfo.Idle;
		}

		if (Velocity.y < 0 && Entity_State == JUMPING)
		{
			Entity_State = FALLING;
		}

		if (Player_Colliding == false && Entity_State == IDLE)
		{
			Entity_State = FALLING;
		}


		//Horizontally

		if (CurrentAnimation != playerinfo.Death)
		{
			if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
			{
				if (First_Move == false)
				{
					First_Move = true;
				}

				Velocity.x = playerinfo.Initial_Velocity_x;
				Position.x = Position.x - Velocity.x;


				Moving_Left = true;
				Moving_Right = false;
				CurrentAnimation = playerinfo.Run;
				Was_Right = false;
			}

			if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
			{

				if (First_Move == false)
				{
					First_Move = true;
				}

				Velocity.x = playerinfo.Initial_Velocity_x;
				Position.x = Position.x + Velocity.x;

				Moving_Left = false;
				Moving_Right = true;
				CurrentAnimation = playerinfo.Run;
				Was_Right = true;
			}

			if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
			{

				Velocity.x = 0.0f;
				Moving_Left = true;
				Moving_Right = true;
			}

			//Vertically

			if (!Must_Fall)
			{
				if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && Player_Colliding == true)
				{

					Velocity.y = playerinfo.Jump_Force;
					Entity_State = JUMPING;
					Player_Colliding = false;

				}
			}

			if (Entity_State == JUMPING)
			{
				CurrentAnimation = playerinfo.Jump;

				Must_Fall = false;

				if (Double_Jump == true && App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && Velocity.y != playerinfo.Jump_Force)
				{
					Velocity.y = playerinfo.Jump_Force / 1.5f;
					Position.y -= Velocity.y;
					Double_Jump = false;
				}

				Velocity.y += playerinfo.Gravity / 2;
				Position.y -= Velocity.y;

			}

			if (Entity_State == FALLING && !Colliding_Roof)
			{
				Must_Fall = false;

				//CurrentAnimation = Fall;

				if (Double_Jump == true && App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && Velocity.y != playerinfo.Jump_Force)
				{
					Velocity.y = playerinfo.Jump_Force / 1.5f;
					Position.y -= Velocity.y;
					Double_Jump = false;
				}

				Velocity.y += playerinfo.Gravity / 2;
				Position.y -= Velocity.y;


			}

			if (Entity_State == FALLING && !Player_Colliding)
			{
				Must_Fall = false;

				CurrentAnimation = playerinfo.Fall;

				if (Double_Jump == true && App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && Velocity.y != playerinfo.Jump_Force)
				{
					Velocity.y = playerinfo.Jump_Force / 1.5f;
					Position.y -= Velocity.y;
					Double_Jump = false;
				}

				Velocity.y += playerinfo.Gravity / 2;
				Position.y -= Velocity.y;
			}
		}
	}

	if (Velocity.y < -playerinfo.Max_Speed_y)
	{
		Velocity.y = -playerinfo.Max_Speed_y;
	}

	//Player collider adjustment to sprites
	Entity_Collider->SetPos(Position.x/* + Player_Collider_Margin.x*/, Position.y /*+ Player_Collider_Margin.y*/);

	/*App->col->Update(1.0f);

	Player_Collider->SetPos(Position.x + 34, Position.y + 14);*/

	if (Must_Fall)
	{
		Position.y -= playerinfo.Gravity * 4.0f;
		
		CurrentAnimation = playerinfo.Fall;
	}

	if (Position.x < 0)
	{
		Position.x = 0;
		Entity_Collider->rect.x = 0;
	}

	else if (Position.x > App->map->data.width*App->map->data.tile_width)
	{
		Position.x = App->map->data.width*App->map->data.tile_width;
	}

	

	return true;
}

bool j1Player::PostUpdate(float dt)
{
	bool ret = true;

	//Calculation for Parallax

	Player_Displacement.x = Player_Initial_Position.x - Position.x;

	App->map->PX = Player_Displacement.x;

	//Player position being controlled

	if (Entity_Collider->rect.x <= playerinfo.Initial_Velocity_x)
	{
		Entity_Collider->rect.x = Position.x = playerinfo.Initial_Velocity_x;
	}

	if (Entity_Collider->rect.y + Entity_Collider->rect.h >= App->map->data.height*App->map->data.tile_height)
	{
		Entity_Collider->rect.y = App->map->data.height*App->map->data.tile_height - Entity_Collider->rect.h;
	}

	//Blitting player
	if (Was_Right == true)
	{
		App->render->Blit(spritesheet, Position.x, Position.y, &CurrentAnimation->GetCurrentFrame());
	}
	
	else
	{
		App->render->Blit(spritesheet, Position.x, Position.y, &CurrentAnimation->GetCurrentFrame(), SDL_FLIP_HORIZONTAL);
	}


	return ret;
}

void j1Player::OnCollision(Collider * c1, Collider * c2)
{
	bool lateralcollision = true;

	if (c1->rect.y + c1->rect.h == c2->rect.y)
	{
		lateralcollision = false; +c1;
	}


	float aux = c1->rect.y; //pos.y

	if (c2->type == COLLIDER_FLOOR)
	{
		Colliding_Roof = false;

		if ((Moving_Left || Moving_Right) && Must_Fall)
		{
			if (c1->rect.x + c1->rect.w >= c2->rect.x && c1->rect.x + c1->rect.w <= c2->rect.x + playerinfo.Initial_Velocity_x)
			{
				Velocity.x = 0.0f;
				c1->rect.x = c2->rect.x - c1->rect.w - playerinfo.Colliding_Offset;
			}

			if (c1->rect.x >= c2->rect.x + c2->rect.w - playerinfo.Initial_Velocity_x && c1->rect.x <= c2->rect.x + c2->rect.w)
			{
				Velocity.x = 0.0f;
				c1->rect.x = c2->rect.x + c2->rect.w + playerinfo.Colliding_Offset;
			}

			if (lateralcollision == true)
			{

				if (Moving_Left)
				{
					c1->rect.x += playerinfo.Colliding_Offset;
				}

				else
				{
					c1->rect.x -= playerinfo.Colliding_Offset;
				}

				Double_Jump = false;
				Must_Fall = true;
			}

			else
			{
				Double_Jump = true;
				Must_Fall = false;
		
			}
		}

		else
		{
			if (Entity_State != JUMPING && Entity_State != FALLING)
			{
				Velocity.y = 0.0f;
				Entity_State = IDLE;
			}

			if (Entity_State != JUMPING)
			{
				if (Moving_Right == true && Moving_Left == true)
				{
					c1->rect.y = c2->rect.y - c1->rect.h;
				}
				else
				{
					c1->rect.y = c2->rect.y - c1->rect.h;
				}
			}

			if (Moving_Right)
			{
				//stopping player if lateral collision

				if (lateralcollision)
				{

					if (c1->rect.x + c1->rect.w >= c2->rect.x && c1->rect.x + c1->rect.w <= c2->rect.x + playerinfo.Initial_Velocity_x)
					{
						Velocity.x = 0.0f;
						if (Entity_State != JUMPING)
							c1->rect.y = aux;
						c1->rect.x = c2->rect.x - c1->rect.w;
					}

					if (Entity_State == JUMPING || Entity_State == FALLING && Double_Jump)
					{
						c1->rect.x -= playerinfo.Colliding_Offset;
					}
				}

				else if (!lateralcollision && Must_Fall == false)
				{
					Entity_State = IDLE;
				}

				if ((Moving_Left || Moving_Right) && Must_Fall)
				{
					c1->rect.x = c2->rect.x + c2->rect.w - playerinfo.Colliding_Offset;
				}
			}

			//going left
			if (Moving_Left)
			{
				if (lateralcollision)
				{
					/*if (c1->rect.x >= c2->rect.x + c2->rect.w - Initial_Velocity_x && c1->rect.x <= c2->rect.x + c2->rect.w)
					{
						Velocity.x = 0.0f;
						if (State_Player != JUMPING)
							c1->rect.y = aux;
						c1->rect.x = c2->rect.x + c2->rect.w;
					}*/

					if (Entity_State == JUMPING || Entity_State == FALLING && Double_Jump)
					{
						c1->rect.x += playerinfo.Colliding_Offset;
					}
				}
				else if (!lateralcollision && Must_Fall == false)
				{
					Entity_State = IDLE;
				}

				if ((Moving_Left || Moving_Right) && Must_Fall)
				{
					c1->rect.x = c2->rect.x + c2->rect.w + playerinfo.Colliding_Offset;
				}
			}

			Double_Jump = true;
			Must_Fall = false;

		}
	}

	else if (c2->type == COLLIDER_DEADLY)
	{
		Colliding_Roof = false;
		Must_Fall = false;
		Double_Jump = false;

		if (!God_Mode)
		{
			CurrentAnimation = playerinfo.Death;

			if (CurrentAnimation->Finished())
			{
				Dead = true;
			}
		}
	}

	else if (c2->type == COLLIDER_PLATFORM)
	{
		Colliding_Roof = false;

		if ((Moving_Left || Moving_Right) && Must_Fall)
		{
			if (c1->rect.x + c1->rect.w >= c2->rect.x && c1->rect.x + c1->rect.w <= c2->rect.x + playerinfo.Initial_Velocity_x)
			{
				Velocity.x = 0.0f;
				c1->rect.x = c2->rect.x - c1->rect.w - playerinfo.Colliding_Offset;
			}

			if (c1->rect.x >= c2->rect.x + c2->rect.w - playerinfo.Initial_Velocity_x && c1->rect.x <= c2->rect.x + c2->rect.w)
			{
				Velocity.x = 0.0f;
				c1->rect.x = c2->rect.x + c2->rect.w + playerinfo.Colliding_Offset;
			}

			if ((c1->rect.y + c1->rect.h >= c2->rect.y && c1->rect.y + c1->rect.h <= c2->rect.y + (-playerinfo.Gravity * 8)))
			{
				if (Entity_State != JUMPING)
				{
					Velocity.y = 0.0f;
					Entity_State = IDLE;
				}

				c1->rect.y = c2->rect.y - c1->rect.h;
				Double_Jump = true;
				Must_Fall = false;
			}
		}

		else
		{
			if ((c1->rect.y + c1->rect.h >= c2->rect.y && c1->rect.y + c1->rect.h <= c2->rect.y + (-playerinfo.Gravity * 8)))
			{
				if (Entity_State != JUMPING)
				{
					Velocity.y = 0.0f;
					Entity_State = IDLE;
				}

				c1->rect.y = c2->rect.y - c1->rect.h;
				Double_Jump = true;
				Must_Fall = true;
			}
		}
	}

	else if (c2->type == COLLIDER_ROOF)
	{
		Colliding_Roof = true;

		if (c1->rect.y <= c2->rect.y + c2->rect.h && c1->rect.y >= c2->rect.y + c2->rect.h - playerinfo.Initial_Velocity_x + 1)
		{
			c1->rect.y = c2->rect.y + c2->rect.h + playerinfo.Colliding_Offset;
			Velocity.y = 0.0f;
			Entity_State = FALLING;
			Double_Jump = false;
			Must_Fall = true;
		}
		else
		{

			if ((Entity_State == JUMPING || Entity_State == FALLING) && Moving_Right || Moving_Left)
			{
				Double_Jump = false;
				Must_Fall = true;

			}
			if (Moving_Right)
			{
				c1->rect.x = c2->rect.x - c1->rect.w - playerinfo.Colliding_Offset;

			}
			else
			{
				c1->rect.x = c2->rect.x + c2->rect.w +playerinfo. Colliding_Offset;
			}
		}
	}

	else if (c2->type == COLLIDER_CHECKPOINT)
	{
		if (Moving_Right)
		{
			if (c1->rect.x >= c2->rect.x + c2->rect.w - playerinfo.Colliding_Offset)
			{
				App->SaveGame("save_game.xml");
			}

		}
		else
		{
			if (c1->rect.x + c1->rect.w <= c2->rect.x + playerinfo.Colliding_Offset)
			{
				App->SaveGame("save_game.xml");
			}
		}
	}


	if (First_Move)
	{
		Initial_Moment = false;
	}

	if (Initial_Moment && !First_Move)
	{
		CurrentAnimation = playerinfo.Idle;
	}



	Position.x = c1->rect.x /*- Player_Collider_Margin.x*/;
	Position.y = c1->rect.y /*- Player_Collider_Margin.y*/;

	Player_Colliding = true;
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
	App->tex->UnLoad(spritesheet);

	return ret;
}

void j1Player::FixedUpdate(float dt)
{
	PostUpdate(dt);
}

void j1Player::LogicUpdate(float dt)
{
	Update(dt);

	// --- Set player pos, prevent surpassing colliders ---
	Entity_Collider->SetPos(Position.x, Position.y);

	App->col->Update(1.0f);

	Entity_Collider->SetPos(Position.x, Position.y);
}
