#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"

// ----------------------------------------------------

struct Properties
{
	p2List <p2SString*> name;

	p2List <p2SString*> value;

	bool LoadProperties(pugi::xml_node& node);

	p2SString GetProperties(const char * request);

	~Properties()
	{
	}

};

struct ImageLayer
{
	SDL_Rect GetImageRect()const;

	p2SString			name;
	int					width;
	int					height;
	float				image_offset_x, image_offset_y = 0.0f;
	SDL_Texture*		texture;

	p2SString			speed;
	int					speedi = 0;

	Properties			properties_img;
};

// ----------------------------------------------------
struct MapLayer
{
	p2SString	name;
	int			width;
	int			height;
	uint*		data;

	MapLayer() : data(NULL)
	{}

	~MapLayer()
	{
		RELEASE(data);
	}

	// TODO 6 (old): Short function to get the value of x,y
	inline uint Get(int x, int y) const
	{
		return data[(y*width) + x];
	}

	Properties	properties_lay;
};

// ----------------------------------------------------
struct TileSet
{
	SDL_Rect GetTileRect(int id) const;

	p2SString			name;
	int					firstgid;
	int					margin;
	int					spacing;
	int					tile_width;
	int					tile_height;
	SDL_Texture*		texture;
	int					tex_width;
	int					tex_height;
	int					num_tiles_width;
	int					num_tiles_height;
	int					offset_x;
	int					offset_y;
};

enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};
// ----------------------------------------------------
struct MapData
{
	int					width;
	int					height;
	int					tile_width;
	int					tile_height;
	SDL_Color			background_color;
	MapTypes			type;
	p2List<TileSet*>	tilesets;
	p2List<MapLayer*>	layers;
	p2List<ImageLayer*>	images;
    
	
};

// ----------------------------------------------------
class j1Map : public j1Module
{
public:

	j1Map();

	// Destructor
	virtual ~j1Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called each loop iteration
	void Draw(MapData& data);

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(const char* path, MapData& data);

	// Coordinate translation methods
	iPoint MapToWorld(int x, int y, MapData& Data) const;
	iPoint WorldToMap(int x, int y, MapData& Data) const;
	bool MapCollisions(MapData& data);


private:

	bool LoadMap(MapData& data);
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadLayer(pugi::xml_node& node, MapLayer* layer);
	bool LoadImageLayer(pugi::xml_node& node, ImageLayer* imagelayer);

	TileSet* TileId(int id, MapData& mapdata) const;

public:

	MapData				data;
	MapData				data2;
	int					PX = 0;
	int					numberStages = 0;

private:

	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded;
	//colliders
	int RedCol, BlueCol, PinkCol, GreenCol;
	
};

#endif // __j1MAP_H__
