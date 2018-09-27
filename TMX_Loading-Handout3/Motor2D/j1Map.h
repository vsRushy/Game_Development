#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"

// TODO 2: Create a struct to hold information for a TileSet
// Ignore Terrain Types and Tile Types for now, but we want the image!
// ----------------------------------------------------
struct Tileset_info
{
	uint first_gid = 0;
	p2SString name = "";
	uint tile_width = 0;
	uint tile_height = 0;
	uint spacing = 0;
	uint margin = 0;
};

// TODO 1: Create a struct needed to hold the information to Map node
enum class ORIENTATION
{
	ERROR_ = -1,
	ORTHOGONAL,
	ISOMETRIC,
	ISOMETRIC_STEPPED,
	HEXAGONAL_STEPPED
};

enum class RENDER_ORDER
{
	ERROR_ = -1,
	RIGHT_DOWN,
	RIGHT_UP,
	LEFT_DOWN,
	LEFT_UP
};

struct Map_info
{
	ORIENTATION orientation = ORIENTATION::ERROR_;
	RENDER_ORDER render_order = RENDER_ORDER::ERROR_;
	uint width = 0;
	uint height = 0;
	uint tile_width = 0;
	uint tile_height = 0;
	uint next_object_id = 0;
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
	void Draw();

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(const char* path);

private:
	// TODO 3
	bool LoadMap();
	// TODO 4
	bool LoadTileset();

public:

	// TODO 1: Add your struct for map info as public for now
	Map_info map_info;
	// TODO 4
	Tileset_info tileset_info;
	// We need to support any number of tilesets, so we can create a list for it
	p2List<Tileset_info> tilesetList;

private:

	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded;

	// TODO 6
	SDL_Texture** tile;
};

#endif // __j1MAP_H__