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
	}

	if (scenes.start->data->GetString() == NULL)
	{
		ret = false;
	}

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	bool ret = true;

	//Loading scenes
	ret = App->map->Load(scenes.start->data->GetString());
	firstscene = scenes.start->data->GetString();

	if (firstscene == "Map_Beta.tmx")
	{
		//load different music samples
	}
	else
	{
		//
	}

	//collider test
	
	if (colliderfloor == nullptr)
		colliderfloor = App->col->AddCollider({ 0, 730, 1024, 40 }, COLLIDER_FLOOR, this);
	else
		colliderfloor->SetPos(0, 0);


	if (!ret)
	{
		ret = false;
	}

	return ret;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_U) == KEY_DOWN)
	{
		App->audio->ChangeVolume_music(10);
		LOG("volume up");
	}

	if (App->input->GetKey(SDL_SCANCODE_T) == KEY_DOWN)
	{
		App->audio->ChangeVolume_music(-10);
		LOG("volume down");
	}

	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		SceneChange(scenes.start->data->GetString());


	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
		SceneChange(scenes.start->next->data->GetString());



	if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		App->LoadGame("save_game.xml");

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		App->SaveGame("save_game.xml");

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
	
	App->map->Draw();

	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint map_coordinates = App->map->WorldToMap(x - App->render->camera.x, y - App->render->camera.y);
	p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d Tile:%d,%d",
		App->map->data.width, App->map->data.height,
		App->map->data.tile_width, App->map->data.tile_height,
		App->map->data.tilesets.count(),
		map_coordinates.x, map_coordinates.y);

	App->win->SetTitle(title.GetString());
	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
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

	if (colliderfloor != nullptr)
		colliderfloor = nullptr;

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
	App->map->CleanUp();
	App->map->Load(scene);

	return ret;
}