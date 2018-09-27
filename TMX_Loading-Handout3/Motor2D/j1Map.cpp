#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Map.h"
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

	return ret;
}

void j1Map::Draw()
{
	if(map_loaded == false)
		return;

	// TODO 6: Iterate all tilesets and draw all their 
	// images in 0,0 (you should have only one tileset for now)

}

// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map");

	// TODO 2: Make sure you clean up any memory allocated
	// from tilesets / map
	if (tileset_list.start != nullptr)
		tileset_list.clear();

	tileset_info.name.Clear();

	map_file.reset();

	return true;
}

// Load new map
bool j1Map::Load(const char* file_name)
{
	bool ret = true;
	p2SString tmp("%s%s", folder.GetString(), file_name);

	pugi::xml_parse_result result = map_file.load_file(tmp.GetString());

	if(result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}

	if(ret == true)
	{
		// TODO 3: Create and call a private function to load and fill
		// all your map data
		if (LoadMap())
		{ 
			LOG("Successfully parsed map XML file: %s", file_name);
			LOG("width: %i", map_info.width);
			LOG("height: %i", map_info.height);
			LOG("tile_width: %i", map_info.tilewidth);
			LOG("tile_height: %i", map_info.tileheight);
			
			map_loaded = ret;
		}
	}

	// TODO 4: Create and call a private function to load a tileset
	// remember to support more any number of tilesets!

	if(map_loaded == true)
	{
		// TODO 5: LOG all the data loaded
		// iterate all tilesets and LOG everything
		if (LoadTilesets())
		{
			p2List_item<Tileset_info>* item;
			item = tileset_list.start;

			while (item != NULL)
			{
				LOG("Tileset ----");
				LOG("name: %s", item->data.name.GetString());
				LOG("firstgid: %i", item->data.first_gid);
				LOG("tile_width: %i", item->data.tile_width);
				LOG("tile_height: %i", item->data.tile_height);
				LOG("spacing: %i", item->data.spacing);
				LOG("margin: %i", item->data.margin);

				item = item->next;
			}

			tilesets_loaded = ret;
		}
	}

	tmp.Clear();

	return ret;
}

bool j1Map::LoadMap()
{
	bool ret = true;

	pugi::xml_node map = map_file.child("map");

	// Checking orientation as a string
	p2SString temp_orientation = map.attribute("orientation").as_string();
	if (temp_orientation == "orthogonal")
		map_info.orientation = ORIENTATION::ORTHOGONAL;
	if (temp_orientation == "isometric")
		map_info.orientation = ORIENTATION::ISOMETRIC;
	if (temp_orientation == "isometric_stepped")
		map_info.orientation = ORIENTATION::ISOMETRIC_STEPPED;
	if (temp_orientation == "hexagonal_stepped")
		map_info.orientation = ORIENTATION::HEXAGONAL_STEPPED;

	// Checking render order as a string
	p2SString temp_render_order = map.attribute("renderorder").as_string();
	if (temp_render_order == "right-down")
		map_info.render_order = RENDER_ORDER::RIGHT_DOWN;
	if (temp_render_order == "right-up")
		map_info.render_order = RENDER_ORDER::RIGHT_UP;
	if (temp_render_order == "left-down")
		map_info.render_order = RENDER_ORDER::LEFT_DOWN;
	if (temp_render_order == "left-up")
		map_info.render_order = RENDER_ORDER::LEFT_UP;

	map_info.width = map.attribute("width").as_int();
	map_info.height = map.attribute("height").as_int();
	map_info.tilewidth = map.attribute("tilewidth").as_int();
	map_info.tileheight = map.attribute("tileheight").as_int();
	map_info.nextobjectid = map.attribute("nextobjectid").as_int();

	temp_orientation.Clear();
	temp_render_order.Clear();

	return ret;
}

bool j1Map::LoadTilesets()
{
	bool ret = true;

	for (pugi::xml_node tileset = map_file.child("map").child("tileset"); tileset; tileset = tileset.next_sibling("tileset")) {

		tileset_info.first_gid = tileset.attribute("firstgid").as_uint();

		p2SString temp_name("%s%s%s", folder.GetString(), tileset.attribute("name").as_string(), ".png");

		tileset_info.name = temp_name;

		tileset_info.tile_width = tileset.attribute("tilewidth").as_uint();
		tileset_info.tile_height = tileset.attribute("tileheight").as_uint();
		tileset_info.spacing = tileset.attribute("spacing").as_uint();
		tileset_info.margin = tileset.attribute("margin").as_uint();

		tileset_list.add(tileset_info);

		temp_name.Clear();
	}

	return ret;
}