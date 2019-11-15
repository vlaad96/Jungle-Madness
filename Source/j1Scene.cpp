#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Collision.h"
#include "j1Player.h"
#include "j1EntityManager.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	for (pugi::xml_node stage = config.child("scene_name"); stage; stage = stage.next_sibling("scene_name"))
	{
		p2SString* SceneName = new p2SString;

		SceneName->create(stage.attribute("name").as_string());
		scenes.add(SceneName);

		App->map->numberStages++;
	}

	if (scenes.start->data->GetString() == NULL)
	{
		ret = false;
		LOG("scenelist is null");
	}

	CamScene1.x = config.child("firstcamera").attribute("x").as_int();
	CamScene1.y = config.child("firstcamera").attribute("y").as_int();
	CamScene2.x = config.child("secondcamera").attribute("x").as_int();
	CamScene2.y = config.child("secondcamera").attribute("y").as_int();

	

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	bool ret = true;

	player = (j1Player*)App->entities->EntityCreation("player", entity_type::PLAYER);
	//Loading both scenes(maps/levels)

	p2List_item<p2SString*>* sceneListItem;
	sceneListItem = scenes.start;
	
	ret = App->map->Load(sceneListItem->data->GetString(), App->map->data);
	
	if (ret == false)
	{
		LOG("issue loading first scene");
		ret = false;
	}

	sceneListItem = scenes.start->next;

	ret = App->map->Load(sceneListItem->data->GetString(), App->map->data2);

	if (ret == false)
	{
		LOG("issue loading second scene");
		ret = false;
	}

	//Loading positions and music
	firstscene = scenes.start->data->GetString();

	if (firstscene == "Map_Beta.tmx")
	{

		App->render->camera.x = CamScene1.x;
		App->render->camera.y = CamScene1.y;

		//Player position Loaded from map
		player->Position.x = App->map->data.StartPoint.x;
		player->Position.y = App->map->data.StartPoint.y;
		
		scene1 = true;
		scene2 = false;

		currentscene = scenes.start->data->GetString();
		//load different music samples
		/*p2SString SceneMusic("%s%s", App->audio->musicfolder.GetString(), App->audio->songs.start->data->GetString());
		App->audio->PlayMusic(SceneMusic.GetString());*/
	}
	//else if(currentscene == "Map_alpha.tmx")
	//{

	//	App->render->camera.x = CamScene2.x;
	//	App->render->camera.y = CamScene2.y;

	//	App->player->Position.x = App->map->data2.StartPoint.x;
	//	App->player->Position.y = App->map->data2.StartPoint.y;
	//	//Player position Loaded from map
	//	/*App->player->Player_Initial_Position.x = App->map->data2.StartPoint.x;
	//	App->player->Player_Initial_Position.y = App->map->data2.FinishPoint.y;
	//	App->player->Player_Initial_Position.x = App->player->Position.x;
	//	App->player->Player_Initial_Position.y = App->player->Position.y;*/

	//	//load different music samples
	//	/*p2SString SceneMusic("%s%s", App->audio->musicfolder.GetString(), App->audio->songs.start->next->data->GetString());
	//	App->audio->PlayMusic(SceneMusic.GetString());*/
	//}

	
	p2SString SceneMusic("%s%s", App->audio->musicfolder.GetString(), App->audio->songs.start->data->GetString());
	App->audio->PlayMusic(SceneMusic.GetString());

	//colliders from tiled
	App->map->MapCollisions(App->map->data);

	return ret;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	//TODO: Win condition

	if (scene1 && (player->Position.x >= App->map->data.FinishPoint.x))
	{
		currentscene = scenes.start->next->data->GetString();
		SceneChange(scenes.start->next->data->GetString());
		scene1 = false;
		scene2 = true;
	}


	else if (scene2 && (player->Position.x >= App->map->data2.FinishPoint.x))
	{
		currentscene = scenes.start->data->GetString();
		SceneChange(scenes.start->data->GetString());
		scene1 = true;
		scene2 = false;
	}

	//camera X axis
	App->render->camera.x = (-player->Position.x*App->win->GetScale() - player->Entity_Collider->rect.w/2  + App->render->camera.w /2);

	if (-App->render->camera.x <= player->playerinfo.Initial_Velocity_x)
	{
		App->render->camera.x = -player->playerinfo.Initial_Velocity_x;
	}

	if (-App->render->camera.x + App->render->camera.w >= App->map->data.width*App->map->data.tile_width*App->win->GetScale())
	{
		App->render->camera.x = -App->map->data.width*App->map->data.tile_width*App->win->GetScale() + App->render->camera.w;
	}

	// camera up y axis
	if (player->Position.y*App->win->GetScale() <= -App->render->camera.y + App->render->camera.h / 6)
	{
		if (App->render->camera.y + (-player->gravity * 8) < 0)
			App->render->camera.y += (-player->gravity * 8);
	}

	//camera down y axis
	

	if (player->Position.y*App->win->GetScale() > -App->render->camera.y + App->render->camera.h - App->render->camera.h / 6)
	{
		App->render->camera.y -= -(player->gravity * 8) + 150;
	}


	if (-App->render->camera.y + App->render->camera.h > App->map->data.height*App->map->data.tile_height*App->win->GetScale())
	{
		App->render->camera.y = (-App->map->data.height*App->map->data.tile_height*App->win->GetScale() + App->render->camera.h);
	}
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{

	//SCENE INTERACTION

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN && scene1 == false)//FIRST
	{
	

		currentscene = scenes.start->data->GetString();
		SceneChange(scenes.start->data->GetString());
		scene1 = true;
		scene2 = false;
		

	}

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN && scene2 == false)//SECOND
	{
		currentscene = scenes.start->next->data->GetString();
		SceneChange(scenes.start->next->data->GetString());
		scene1 = false;
		scene2 = true;
		
	}

	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)//BEGINING OF CURRENT SCENE
	{
		if (player->God_Mode == true)
			player->God_Mode = false;

		if (scene1)
		{
			currentscene = scenes.start->data->GetString();
			SceneChange(scenes.start->data->GetString());
			scene1 = true;
			scene2 = false;
		}
		else if (scene2)
		{
			currentscene = scenes.start->next->data->GetString();
			SceneChange(scenes.start->next->data->GetString());
			scene1 = false;
			scene2 = true;
		}
	}
//---------------------------------------

//SAVE & LOAD SCENE

	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->SaveGame("save_game.xml");

	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
	{
		bool result = App->LoadGame("save_game.xml");
	}

//-----------------------------------------
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y += 10;

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y -= 10;

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x += 10;

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x -= 10;

	//check utility of this when doing player
	camera_displacement.x = App->render->camera_initial_pos.x - App->render->camera.x;
	
	//App->map->Draw(App->map->data);

	int x, y;
	App->input->GetMousePosition(x, y);

	if (scene1 == true)
	{
		App->map->Draw(App->map->data);

		iPoint map_coordinates = App->map->WorldToMap(x - App->render->camera.x, y - App->render->camera.y, App->map->data);
		p2SString title("Jungle Madness Map:%dx%d Tiles:%dx%d Tilesets:%d Tile:%d,%d",
			App->map->data.width, App->map->data.height,
			App->map->data.tile_width, App->map->data.tile_height,
			App->map->data.tilesets.count(),
			map_coordinates.x, map_coordinates.y);

		//App->win->SetTitle(title.GetString());
	}
	else
	{
		App->map->Draw(App->map->data2);


		iPoint map_coordinates = App->map->WorldToMap(x - App->render->camera.x, y - App->render->camera.y, App->map->data2);
		p2SString title("Jungle Madness Map:%dx%d Tiles:%dx%d Tilesets:%d Tile:%d,%d",
			App->map->data2.width, App->map->data2.height,
			App->map->data2.tile_width, App->map->data2.tile_height,
			App->map->data2.tilesets.count(),
			map_coordinates.x, map_coordinates.y);

		//App->win->SetTitle(title.GetString());
	}

	
	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate(float dt)
{
	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");


	//cleaning scenes list
	p2List_item<p2SString*>* item;
	item = scenes.start;

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	scenes.clear();

	return true;
}

bool j1Scene::SceneChange(const char* scene) {
	bool ret = true;

	//THIS WAS CAUSING MEMORY LEAKS LIKE A BOSS
	//App->map->CleanUp();
	//App->map->Load(scene);

	player->Initial_Moment = true;
	player->First_Move = false;

	App->col->CleanUp();
	player->Entity_Collider = App->col->AddCollider(player->Entity_Collider_Rect, COLLIDER_PLAYER, App->entities);

	if (currentscene == scenes.start->data->GetString())
	{
		App->map->MapCollisions(App->map->data);

		//TODO: Initial position
		player->Position.x = App->map->data.StartPoint.x;
		player->Position.y = App->map->data.StartPoint.y;

		App->render->camera.x = CamScene1.x;
		App->render->camera.y = CamScene1.y;

		p2SString stageMusic("%s%s", App->audio->musicfolder.GetString(), App->audio->songs.start->data->GetString());
		App->audio->PlayMusic(stageMusic.GetString());

		player->Entity_State = FALLING;
	}
	else if (currentscene == scenes.start->next->data->GetString()) 
	{
		App->map->MapCollisions(App->map->data2);
		
		//TODO: Initial position
		player->Position.x = App->map->data2.StartPoint.x;
		player->Position.y = App->map->data2.StartPoint.y;

		App->render->camera.x = CamScene2.x;
		App->render->camera.y = CamScene2.y;

		p2SString stageMusic("%s%s", App->audio->musicfolder.GetString(), App->audio->songs.start->next->data->GetString());
		App->audio->PlayMusic(stageMusic.GetString());

		player->Entity_State = FALLING;
	}

	return ret;
}

bool j1Scene::Save(pugi::xml_node &config) const
{
	bool ret = true;

	config.append_child("scene1").append_attribute("value") = scene1;
	config.append_child("scene2").append_attribute("value") = scene2;

	return ret;
}

bool j1Scene::Load(pugi::xml_node &config)
{

	bool ret = true;
	int x = player->Position.x;
	int y = player->Position.y;

	scene1Loaded = config.child("scene1").attribute("value").as_bool();
	scene2Loaded= config.child("scene2").attribute("value").as_bool();

	if (scene1)
	{


		if (scene2Loaded)
		{
			currentscene = scenes.start->next->data->GetString();
			SceneChange(scenes.start->next->data->GetString());
			scene2 = true;
			scene1 = false;
			player->Position.x = x;
			player->Position.y = y;
		}

		else
		{
			currentscene = scenes.start->data->GetString();
			SceneChange(scenes.start->data->GetString());
			scene1 = true;
			scene2 = false;
			player->Position.x = x;
			player->Position.y = y;

		}

	}

	else if (scene2)
	{

		if (scene1Loaded)
		{
			currentscene = scenes.start->data->GetString();
			SceneChange(scenes.start->data->GetString());
			scene1 = true;
			scene2 = false;
			player->Position.x = x;
			player->Position.y = y;

		}

		else
		{
			currentscene = scenes.start->next->data->GetString();
			SceneChange(scenes.start->next->data->GetString());
			scene1 = false;
			scene2 = true;
			player->Position.x = x;
			player->Position.y = y;
		}
	}

	return ret;
}