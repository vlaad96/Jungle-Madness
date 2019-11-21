#ifndef __j1Player_H__
#define __j1Player_H__

#include "j1Module.h"
#include "p2Point.h"
#include "Animation.h"

struct SDL_Texture;
struct Collider;

struct PlayerInfo
{
	//Animations
	Animation* CurrentAnimation = nullptr;
	Animation* Idle = nullptr;
	Animation* Run = nullptr;
	Animation* Jump = nullptr;
	Animation* Fall = nullptr;
	Animation* Slide = nullptr;
	Animation* Wall_Slide = nullptr;
	Animation* Death = nullptr;
	Animation* God = nullptr;

	SDL_Texture* Spritesheet = nullptr;

	p2SString folder;
	p2SString texture;

	//Movement and collision
	SDL_Rect Player_Collider_Rect;
	fPoint Velocity = { 0.0f,0.0f };

	float Gravity;
	float Jump_Force;

};

enum Player_State
{
	IDLE,
	RIGHT,
	LEFT,
	JUMPING,
	FALLING,
};

class j1Player : public Entity
{
public:
	j1Player();
	~j1Player();

	bool Awake(pugi::xml_node &config);

	bool Start();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	void OnCollision(Collider* c1, Collider* c2);

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	Animation* LoadAnimation(const char* animationPath, const char* animationName);
	SDL_Rect LoadColliderRect(const char* colliderPath, const char* colliderName);
	//void Jump();
	//void Slide();
	//void WallSlide();

public:

	//struct
	PlayerInfo playerData;

	SDL_Texture * Graphics = nullptr;
	Collider* Player_Collider = nullptr;

	fPoint Player_Initial_Position;
	fPoint Position;

	fPoint Player_Collider_Margin = { 34, 14 };
	fPoint Player_Displacement;

	
	float Initial_Velocity_x;
	float Max_Speed_y;
	float Colliding_Offset;

	Player_State State_Player;

	bool Player_Colliding;
	bool Colliding_Roof;

	bool Double_Jump;
	bool Must_Fall;

	bool Moving_Right;
	bool Moving_Left;

	bool Was_Right = true;
	bool Dead = false;

	bool Initial_Moment;
	bool First_Move;

	bool God_Mode = false;


private:
	
};

#endif // __j1Player_H__


