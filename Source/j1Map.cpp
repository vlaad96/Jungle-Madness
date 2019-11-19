#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "j1Collision.h"
#include "j1Window.h"
#include <math.h>

j1Map::j1Map() : j1Module(), map_loaded(false)
{
	name.create("map");
}

// Destructor
j1Map::~j1Map()
{}

// Called before render is available
bool j1Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;

	folder.create(config.child("folder").child_value());
	//colliders XML
	RedCol = config.child("collision").attribute("red").as_int();
	BlueCol = config.child("collision").attribute("blue").as_int();
	PinkCol = config.child("collision").attribute("pink").as_int();
	GreenCol = config.child("collision").attribute("green").as_int();

	return ret;
}

void j1Map::Draw(MapData &data)
{

	 if (map_loaded == false)
			return;

		ImageLayer* image;
		int parallax;

		for (int x = 0; x < data.images.count(); ++x)
		{
			image = data.images.At(x)->data;

			parallax = (image->image_offset_x - PX) / image->speedi;

			App->render->Blit(data.images[x]->texture,
				parallax,
				image->image_offset_y,
				&image->GetImageRect());
		}

	
	MapLayer* layer;


	for (uint l = 0; l < data.layers.count(); l++)
	{
		layer = data.layers.At(l)->data;

		if (layer->properties_lay.GetProperties("Draw").operator==("0"))
			continue;

		for (int y = 0; y < data.height; ++y)
		{
			for (int x = 0; x < data.width; ++x)
			{
				int tile_id = layer->Get(x, y);
				if (tile_id > 0)
				{
					TileSet* tileset = TileId(tile_id, data);

					if (tileset != nullptr)
					{
						SDL_Rect r = tileset->GetTileRect(tile_id);

						iPoint pos = MapToWorld(x, y, data);


						if ((pos.x + data.tile_width)*App->win->GetScale() >= -App->render->camera.x && pos.x <= -App->render->camera.x + App->render->camera.w
							&& (pos.y + data.tile_height)*App->win->GetScale() >= -App->render->camera.y && pos.y <= -App->render->camera.y + App->render->camera.h)
						{
							App->render->Blit(tileset->texture, pos.x, pos.y, &r);
						}
					}
				}
			}
		}
	}



	// TODO 10(old): Complete the draw function
}

iPoint j1Map::MapToWorld(int x, int y, MapData& Data) const
{
	iPoint ret(0, 0);
	// TODO 8(old): Create a method that translates x,y coordinates from map positions to world positions

	if (data.type == MapTypes::MAPTYPE_ORTHOGONAL)
	{
		ret.x = x * data.tile_width;
		ret.y = y * data.tile_height;
	}

	else if (data.type == MapTypes::MAPTYPE_ISOMETRIC)
	{
		ret.x = (x - y) * (data.tile_width / 2);
		ret.y = (x + y) * (data.tile_height / 2);
	}


	// TODO 1: Add isometric map to world coordinates
	return ret;
}


iPoint j1Map::WorldToMap(int x, int y, MapData& Data) const
{
	iPoint ret(0, 0);
	// TODO 2: Add orthographic world to map coordinates

	if (data.type == MapTypes::MAPTYPE_ORTHOGONAL)
	{
		ret.x = x / data.tile_width;
		ret.y = y / data.tile_height;
	}

	else if (data.type == MapTypes::MAPTYPE_ISOMETRIC)
	{

		y -= 16;
		ret.x = ((x / (data.tile_width / 2) + (y / (data.tile_height / 2))) / 2);
		ret.y = ((y / (data.tile_height / 2) - (x / (data.tile_width / 2))) / 2);

	}


	// TODO 3: Add the case for isometric maps to WorldToMap
	return ret;
}

SDL_Rect TileSet::GetTileRect(int id) const
{
	int relative_id = id - firstgid;

	SDL_Rect rect = { 0, 0, 0, 0 };

	// TODO 7(old): Create a method that receives a tile id and returns it's Rect

	rect.w = tile_width;
	rect.h = tile_height;

	rect.x = margin + ((rect.w + spacing) * (relative_id % num_tiles_width));
	rect.y = margin + ((rect.h + spacing) * (relative_id / num_tiles_width));


	return rect;
}

SDL_Rect ImageLayer::GetImageRect() const
{
	SDL_Rect rect;

	rect.w = width;
	rect.h = height;

	rect.x = 0;
	rect.y = 0;

	return rect;
}

// Called before quitting
bool j1Map::CleanUp()
{
	MapData* MapDataItem = &data;
	for (int i = 0; i < numberStages; ++i)
	{
		LOG("Unloading map");

		// Remove all tilesets
		p2List_item<TileSet*>* item;
		item = MapDataItem->tilesets.start;

		while (item != NULL)
		{
			RELEASE(item->data);
			item = item->next;
		}
		MapDataItem->tilesets.clear();

		// Remove all layers
		p2List_item<MapLayer*>* item2;
		item2 = MapDataItem->layers.start;

		p2List_item<p2SString*>* item2_properties;


		while (item2 != NULL)
		{
			item2_properties = item2->data->properties_lay.name.start;

			while (item2_properties != NULL)
			{

				RELEASE(item2_properties->data);
				if (item2_properties == item2->data->properties_lay.name.end)
					break;
				item2_properties = item2_properties->next;
			}
			item2->data->properties_lay.name.clear();

			item2_properties = item2->data->properties_lay.value.start;

			while (item2_properties != NULL)
			{
				RELEASE(item2_properties->data);
				if (item2_properties == item2->data->properties_lay.value.end)
					break;
				item2_properties = item2_properties->next;
			}
			item2->data->properties_lay.value.clear();

			RELEASE(item2->data);
			item2 = item2->next;
		}
		MapDataItem->layers.clear();

		// Remove all image layers
		p2List_item<ImageLayer*>* item3;
		item3 = MapDataItem->images.start;

		p2List_item<p2SString*>* item3_properties;


		while (item3 != NULL)
		{
			item3_properties = item3->data->properties_img.name.start;

			while (item3_properties != NULL)
			{

				RELEASE(item3_properties->data);
				if (item3_properties == item3->data->properties_img.name.end)
					break;
				item3_properties = item3_properties->next;
			}
			item3->data->properties_img.name.clear();

			item3_properties = item3->data->properties_img.value.start;

			while (item3_properties != NULL)
			{
				RELEASE(item3_properties->data);
				if (item3_properties == item3->data->properties_img.value.end)
					break;
				item3_properties = item3_properties->next;
			}
			item3->data->properties_img.value.clear();

			RELEASE(item3->data);
			item3 = item3->next;
		}
		MapDataItem->images.clear();

		// Clean up the pugui tree
		map_file.reset();

		MapDataItem = &data2;
	}

	return true;
}

// Load new map
bool j1Map::Load(const char* file_name, MapData& dataRef)
{
	bool ret = true;
	p2SString tmp("%s%s", folder.GetString(), file_name);

	pugi::xml_parse_result result = map_file.load_file(tmp.GetString());

	if (result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}

	// Load general info ----------------------------------------------
	if (ret == true)
	{
		ret = LoadMap(dataRef);
	}

	// Load all tilesets info ----------------------------------------------
	pugi::xml_node tileset;
	for (tileset = map_file.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		if (ret == true)
		{
			ret = LoadTilesetDetails(tileset, set);
		}

		if (ret == true)
		{
			ret = LoadTilesetImage(tileset, set);
		}

		dataRef.tilesets.add(set);
	}

	// Load layer info ----------------------------------------------
	pugi::xml_node layer;
	for (layer = map_file.child("map").child("layer"); layer && ret; layer = layer.next_sibling("layer"))
	{
		MapLayer* lay = new MapLayer();

		ret = LoadLayer(layer, lay);

		if (ret == true)
			dataRef.layers.add(lay);
	}

	// Load Image Layer info----------------------------------------------
		pugi::xml_node imagelayer;
	for (imagelayer = map_file.child("map").child("imagelayer"); imagelayer && ret; imagelayer = imagelayer.next_sibling("imagelayer"))
	{
		ImageLayer* imageList = new ImageLayer();

		ret = LoadImageLayer(imagelayer, imageList);

		if (ret == true)
			dataRef.images.add(imageList);
	}

	if (ret == true)
	{
		LOG("Successfully parsed map XML file: %s", file_name);
		LOG("width: %d height: %d", dataRef.width, dataRef.height);
		LOG("tile_width: %d tile_height: %d", dataRef.tile_width, dataRef.tile_height);

		p2List_item<TileSet*>* item = dataRef.tilesets.start;
		while (item != NULL)
		{
			TileSet* s = item->data;
			LOG("Tileset ----");
			LOG("name: %s firstgid: %d", s->name.GetString(), s->firstgid);
			LOG("tile width: %d tile height: %d", s->tile_width, s->tile_height);
			LOG("spacing: %d margin: %d", s->spacing, s->margin);
			item = item->next;
		}

		p2List_item<MapLayer*>* item_layer = dataRef.layers.start;
		while (item_layer != NULL)
		{
			MapLayer* l = item_layer->data;
			LOG("Layer ----");
			LOG("name: %s", l->name.GetString());
			LOG("tile width: %d tile height: %d", l->width, l->height);
			item_layer = item_layer->next;
		}

		p2List_item<ImageLayer*>* item_image = dataRef.images.start;
		while (item_image != NULL)
		{
			ImageLayer* I = item_image->data;
			LOG("Image ---");
			LOG("name: %s", I->name.GetString());
			LOG("image width: %d image height: %d", I->width, I->height);
			item_image = item_image->next;
		}
	}

	map_loaded = ret;

	return ret;
}

// Load map general properties
bool j1Map::LoadMap(MapData& data)
{
	bool ret = true;
	pugi::xml_node map = map_file.child("map");

	if (map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		data.width = map.attribute("width").as_int();
		data.height = map.attribute("height").as_int();
		data.tile_width = map.attribute("tilewidth").as_int();
		data.tile_height = map.attribute("tileheight").as_int();

		p2SString bg_color(map.attribute("backgroundcolor").as_string());

		data.background_color.r = 0;
		data.background_color.g = 0;
		data.background_color.b = 0;
		data.background_color.a = 0;

		if (bg_color.Length() > 0)
		{
			p2SString red, green, blue;
			bg_color.SubString(1, 2, red);
			bg_color.SubString(3, 4, green);
			bg_color.SubString(5, 6, blue);

			int v = 0;

			sscanf_s(red.GetString(), "%x", &v);
			if (v >= 0 && v <= 255) data.background_color.r = v;

			sscanf_s(green.GetString(), "%x", &v);
			if (v >= 0 && v <= 255) data.background_color.g = v;

			sscanf_s(blue.GetString(), "%x", &v);
			if (v >= 0 && v <= 255) data.background_color.b = v;
		}

		p2SString orientation(map.attribute("orientation").as_string());

		if (orientation == "orthogonal")
		{
			data.type = MAPTYPE_ORTHOGONAL;
		}
		else if (orientation == "isometric")
		{
			data.type = MAPTYPE_ISOMETRIC;
		}
		else if (orientation == "staggered")
		{
			data.type = MAPTYPE_STAGGERED;
		}
		else
		{
			data.type = MAPTYPE_UNKNOWN;
		}

		p2SString obj(map.child("objectgroup").attribute("name").as_string());
		
		if (obj == "FinishPoint")
		{
			data.FinishPoint.x = map.child("objectgroup").child("object").attribute("x").as_int();
			data.FinishPoint.y = map.child("objectgroup").child("object").attribute("y").as_int() + map.child("objectgroup").child("object").attribute("height").as_int();
		}

		p2SString obj2(map.child("objectgroup").next_sibling("objectgroup").attribute("name").as_string());
		
		if (obj2 == "StartPoint")
		{
			data.StartPoint.x = map.child("objectgroup").next_sibling("objectgroup").child("object").attribute("x").as_int();
			data.StartPoint.y = map.child("objectgroup").next_sibling("objectgroup").child("object").attribute("y").as_int();
		}
	}

	return ret;
}

bool j1Map::LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	set->name.create(tileset_node.attribute("name").as_string());
	set->firstgid = tileset_node.attribute("firstgid").as_int();
	set->tile_width = tileset_node.attribute("tilewidth").as_int();
	set->tile_height = tileset_node.attribute("tileheight").as_int();
	set->margin = tileset_node.attribute("margin").as_int();
	set->spacing = tileset_node.attribute("spacing").as_int();
	pugi::xml_node offset = tileset_node.child("tileoffset");

	if (offset != NULL)
	{
		set->offset_x = offset.attribute("x").as_int();
		set->offset_y = offset.attribute("y").as_int();
	}
	else
	{
		set->offset_x = 0;
		set->offset_y = 0;
	}

	return ret;
}

bool j1Map::LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = tileset_node.child("image");

	if (image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		set->texture = App->tex->Load(PATH(folder.GetString(), image.attribute("source").as_string()));
		int w, h;
		SDL_QueryTexture(set->texture, NULL, NULL, &w, &h);
		set->tex_width = image.attribute("width").as_int();

		if (set->tex_width <= 0)
		{
			set->tex_width = w;
		}

		set->tex_height = image.attribute("height").as_int();

		if (set->tex_height <= 0)
		{
			set->tex_height = h;
		}

		set->num_tiles_width = set->tex_width / set->tile_width;
		set->num_tiles_height = set->tex_height / set->tile_height;
	}

	return ret;
}

bool j1Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
	bool ret = true;

	layer->name = node.attribute("name").as_string();
	layer->width = node.attribute("width").as_int();
	layer->height = node.attribute("height").as_int();
	layer->properties_lay.LoadProperties(node);

	pugi::xml_node layer_data = node.child("data");


	if (layer_data == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'layer/data' tag.");
		ret = false;
		RELEASE(layer);
	}
	else
	{
		layer->data = new uint[layer->width*layer->height];
		memset(layer->data, 0, layer->width*layer->height);

		int i = 0;
		for (pugi::xml_node tile = layer_data.child("tile"); tile; tile = tile.next_sibling("tile"))
		{
			layer->data[i++] = tile.attribute("gid").as_int(0);
		}
	}

	return ret;
}

bool j1Map::LoadImageLayer(pugi::xml_node& node, ImageLayer* imagelayer)//parallax
{
	bool ret = true;

	imagelayer->name = node.attribute("name").as_string();
	imagelayer->width = node.child("image").attribute("width").as_int();
	imagelayer->height = node.child("image").attribute("height").as_int();
	imagelayer->texture = App->tex->Load(PATH(folder.GetString(), node.child("image").attribute("source").as_string()));
	imagelayer->properties_img.LoadProperties(node);
	

	if (node.attribute("offsetx").as_int() != NULL)
	{
		imagelayer->image_offset_x = node.attribute("offsetx").as_float();
	}

	if (node.attribute("offsety").as_int() != NULL)
	{
		imagelayer->image_offset_y = node.attribute("offsety").as_float();
	}

	imagelayer->speed = imagelayer->properties_img.GetProperties("Speed"); //Gets the parallax speed value for every image layer
	
	if (imagelayer->speed != NULL)
	{
		imagelayer->speedi = atoi(imagelayer->speed.GetString());
	}

	return ret;
}

bool j1Map::MapCollisions(MapData& data)
{
	bool ret = true;

	MapLayer* layer;

	for (uint l = 0; l < data.layers.count(); l++)
	{
		layer = data.layers.At(l)->data;

		if (layer->properties_lay.GetProperties("Draw").operator==("0"))
		{
			for (int y = 0; y < data.height; ++y)
			{
				for (int x = 0; x < data.width; ++x)
				{
					int tile_id = layer->Get(x, y);

					if (tile_id > 0)
					{
						TileSet* tileset = TileId(tile_id, data);

						if (tile_id > tileset->firstgid)
						{

							iPoint pos = MapToWorld(x, y, data);

							if (tile_id == RedCol)
							{
								App->col->AddCollider({ pos.x,pos.y,data.tile_width,data.tile_height }, COLLIDER_FLOOR, this);
							}
							else if (tile_id == BlueCol)
							{
								App->col->AddCollider({ pos.x,pos.y,data.tile_width,data.tile_height }, COLLIDER_DEADLY, this);
							}
							else if (tile_id == PinkCol)
							{
								App->col->AddCollider({ pos.x,pos.y,data.tile_width,data.tile_height }, COLLIDER_PLATFORM, this);
							}
							else if (tile_id == GreenCol)
							{
								App->col->AddCollider({ pos.x,pos.y,data.tile_width,data.tile_height }, COLLIDER_CHECKPOINT, this);
							}
						}
					}
				}
			}
		}
	}



	return ret;
}

TileSet* j1Map::TileId(int id, MapData& data) const
{
	//method so we pick the right Tileset based on a tile id

	TileSet* tileset;

	int j = data.tilesets.count();

	for (int i = 0; i < j; ++i)
	{
		tileset = data.tilesets.At(i)->data;

		if (tileset == NULL)
		{
			LOG("Problem in tileset get");
		}

		if (id >= tileset->firstgid)
		{
			if (data.tilesets.At(i + 1) != NULL)
			{

				if (id <= data.tilesets.At(i + 1)->data->firstgid)
				{
					if (tileset == NULL)
					{
						LOG("Problem in tileset get");
					}
					return tileset;
				}
				else
					continue;
			}
			return tileset;
		}
	}

	LOG("No tileset matches tile_id");
}

bool Properties::LoadProperties(pugi::xml_node & node)
{
	// node has to be layer, map, tileset
	bool ret = true;

	pugi::xml_node property = node.child("properties").child("property");

	while (property != NULL)
	{
		p2SString* tmp = new p2SString();
		p2SString* tmp2 = new p2SString();

		tmp->operator=(property.attribute("name").as_string());
		this->name.add(tmp);
		tmp2->operator=(property.attribute("value").as_string());
		this->value.add(tmp2);

		property = property.next_sibling();
	}

	// fill the custom properties from an xml_node

	return ret;
}

p2SString Properties::GetProperties(const char * request)
{
	p2SString tmp;
	bool requestfound = false;
	pugi::xml_parse_result result;


	for (int i = 0; i < name.count(); ++i)
	{
		tmp.operator= (name.At(i)->data->GetString());
		if (tmp.operator==(request) == true)
		{
			requestfound = true;
			tmp = value.At(i)->data->GetString();
			break;
		}

	}

	if (requestfound == false)
		tmp = result.description();


	return tmp;
}
