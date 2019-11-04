#include "j1PathFinding.h"
#include "p2Log.h"

j1PathFinding::j1PathFinding() : j1Module()
{
	name.create("pathfinding");
}

bool j1PathFinding::CleanUp()
{
	LOG("Freeing pathfinding");

	return true;
}