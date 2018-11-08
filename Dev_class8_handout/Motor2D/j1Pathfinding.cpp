#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Pathfinding.h"

j1PathFinding::j1PathFinding() : j1Module(), map(NULL), last_path(DEFAULT_PATH_LENGTH),width(0), height(0)
{
	name.create("pathfinding");
}

// Destructor
j1PathFinding::~j1PathFinding()
{
	RELEASE_ARRAY(map);
}

// Called before quitting
bool j1PathFinding::CleanUp()
{
	LOG("Freeing pathfinding library");

	last_path.Clear();
	RELEASE_ARRAY(map);
	return true;
}

// Sets up the walkability map
void j1PathFinding::SetMap(uint width, uint height, uchar* data)
{
	this->width = width;
	this->height = height;

	RELEASE_ARRAY(map);
	map = new uchar[width*height];
	memcpy(map, data, width*height);
}

// Utility: return true if pos is inside the map boundaries
bool j1PathFinding::CheckBoundaries(const iPoint& pos) const
{
	return (pos.x >= 0 && pos.x <= (int)width &&
			pos.y >= 0 && pos.y <= (int)height);
}

// Utility: returns true is the tile is walkable
bool j1PathFinding::IsWalkable(const iPoint& pos) const
{
	uchar t = GetTileAt(pos);
	return t != INVALID_WALK_CODE && t > 0;
}

// Utility: return the walkability value of a tile
uchar j1PathFinding::GetTileAt(const iPoint& pos) const
{
	if(CheckBoundaries(pos))
		return map[(pos.y*width) + pos.x];

	return INVALID_WALK_CODE;
}

// To request all tiles involved in the last generated path
const p2DynArray<iPoint>* j1PathFinding::GetLastPath() const
{
	return &last_path;
}

// PathList ------------------------------------------------------------------------
// Looks for a node in this list and returns it's list node or NULL
// ---------------------------------------------------------------------------------
const p2List_item<PathNode>* PathList::Find(const iPoint& point) const
{
	p2List_item<PathNode>* item = list.start;
	while(item)
	{
		if(item->data.pos == point)
			return item;
		item = item->next;
	}
	return NULL;
}

// PathList ------------------------------------------------------------------------
// Returns the Pathnode with lowest score in this list or NULL if empty
// ---------------------------------------------------------------------------------
p2List_item<PathNode>* PathList::GetNodeLowestScore() const
{
	p2List_item<PathNode>* ret = NULL;
	int min = 65535;

	p2List_item<PathNode>* item = list.end;
	while(item)
	{
		if(item->data.Score() < min)
		{
			min = item->data.Score();
			ret = item;
		}
		item = item->prev;
	}
	return ret;
}

// PathNode -------------------------------------------------------------------------
// Convenient constructors
// ----------------------------------------------------------------------------------
PathNode::PathNode() : g(-1), h(-1), pos(-1, -1), parent(NULL)
{}

PathNode::PathNode(int g, int h, const iPoint& pos, const PathNode* parent) : g(g), h(h), pos(pos), parent(parent)
{}

PathNode::PathNode(const PathNode& node) : g(node.g), h(node.h), pos(node.pos), parent(node.parent)
{}

// PathNode -------------------------------------------------------------------------
// Fills a list (PathList) of all valid adjacent pathnodes
// ----------------------------------------------------------------------------------
uint PathNode::FindWalkableAdjacents(PathList& list_to_fill) const
{
	iPoint cell;
	uint before = list_to_fill.list.count();

	// north
	cell.create(pos.x, pos.y + 1);
	if(App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	// south
	cell.create(pos.x, pos.y - 1);
	if(App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	// east
	cell.create(pos.x + 1, pos.y);
	if(App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	// west
	cell.create(pos.x - 1, pos.y);
	if(App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	return list_to_fill.list.count();
}

// PathNode -------------------------------------------------------------------------
// Calculates this tile score
// ----------------------------------------------------------------------------------
int PathNode::Score() const
{
	return g + h;
}

// PathNode -------------------------------------------------------------------------
// Calculate the F for a specific destination tile
// ----------------------------------------------------------------------------------
int PathNode::CalculateF(const iPoint& destination)
{
	g = parent->g + 1;
	h = pos.DistanceTo(destination);

	return g + h;
}

// ----------------------------------------------------------------------------------
// Actual A* algorithm: return number of steps in the creation of the path or -1 ----
// ----------------------------------------------------------------------------------
int j1PathFinding::CreatePath(const iPoint& origin, const iPoint& destination)
{
	int ret = -1;
	// TODO 1: if origin or destination are not walkable, return -1
	if (!IsWalkable(origin) || !IsWalkable(destination))
	{
		ret = -1;
	}
	else
	{
		// TODO 2: Create two lists: open, close
		PathList open;
		PathList close;
		// Add the origin tile to open
		PathNode origin_tile(0, origin.DistanceNoSqrt(destination), origin, nullptr);
		open.list.add(origin_tile);
		
		// Iterate while we have tile (1 or more than 1) in the open list
		while (open.list.count() > 0)
		{
			// TODO 3: Move the lowest score cell from open list to the closed list
			p2List_item<PathNode>* current_node = open.GetNodeLowestScore();
			close.list.add(current_node->data);
			open.list.del(current_node);

			// TODO 4: If we just added the destination, we are done!
			// Backtrack to create the final path
			if (close.list.end->data.pos == destination)
			{
				// backtracking
				for (const p2List_item<PathNode>* it = close.list.end; it->data.parent != nullptr; it = close.Find(it->data.parent->pos))
				{
					last_path.PushBack(it->data.pos);
					if (it->data.parent == nullptr)
					{
						last_path.PushBack(close.list.start->data.pos);
					}
				}
				// Use the Pathnode::parent and Flip() the path when you are finish
				last_path.Flip();
				ret = last_path.Count();
			}
			else
			{
				// TODO 5: Fill a list of all adjancent nodes
				PathList neighbours;
				close.list.end->data.FindWalkableAdjacents(neighbours);

				// TODO 6: Iterate adjancent nodes:
				p2List_item<PathNode>* it = neighbours.list.start;

				while (it != nullptr)
				{
					// ignore nodes in the closed list
					if (close.Find(it->data.pos) != NULL)
					{
						// It is in the closed list, so we ignore it
						it = it->next;
						continue;
					}
					// If it is already in the open list, check if it is a better path (compare G)
					else if (open.Find(it->data.pos) != NULL)
					{
						PathNode temp_path = open.Find(it->data.pos)->data;

						// If it is a better path, Update the parent
						if (temp_path.g > it->data.g)
						{
							temp_path.parent = it->data.parent;
						}
					}
					// If it is NOT found, calculate its F and add it to the open list
					else
					{
						it->data.CalculateF(destination);
						open.list.add(it->data);
					}

					it = it->next;
				}

				neighbours.list.clear();
			}
		}
	}

	return ret;
}

