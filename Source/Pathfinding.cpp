#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "PathFinding.h"
#include "j1Map.h"
#include "j1Scene.h"

Pathfinding::Pathfinding() : j1Module()
{
	name.create("pathfinding");
}

Pathfinding::~Pathfinding()
{
	
}

void Pathfinding::CreatePath(iPoint origin, iPoint dest)
{
	//reset everything before starting
	lastPath.Clear();
	frontier.Clear();
	visited.clear();
	breadcrumbs.clear();

	if (IsWalkable(origin) && IsWalkable(dest))
	{
		iPoint curr = origin;
		frontier.Push(curr, curr.DistanceManhattan(dest));

		for (uint i = 0; i != width * height; ++i)
		{
			frontier.Pop(curr);
			if (curr == dest)
			{
				PathBacktracking(curr);

				break;
			}

			iPoint neighbours[8];
			neighbours[0].create(curr.x + 1, curr.y + 0);
			neighbours[1].create(curr.x + 1, curr.y + 1);
			neighbours[2].create(curr.x + 1, curr.y - 1);
			neighbours[3].create(curr.x + 0, curr.y + 1);
			neighbours[4].create(curr.x + 0, curr.y - 1);
			neighbours[5].create(curr.x - 1, curr.y - 1);
			neighbours[6].create(curr.x - 1, curr.y + 0);
			neighbours[7].create(curr.x - 1, curr.y + 1);

			for (uint i = 0; i < 8; ++i)
			{
				if (IsWalkable(neighbours[i]) && visited.find(neighbours[i]) == -1)
				{
					int newCost = neighbours[i].DistanceTo(dest);
					frontier.Push(neighbours[i], newCost);
					visited.add(neighbours[i]);
					breadcrumbs.add(curr);
				}
			}
		}
	}
}

void Pathfinding::PathBacktracking(iPoint goal)
{
	lastPath.PushBack(goal);

	int lastValue = visited.find(goal) + 1;

	for (uint i = visited.find(goal); i < lastValue && i >= 0; i = visited.find(breadcrumbs.At(i)->data))
	{
		lastPath.PushBack(breadcrumbs.At(i)->data);

		if (i < lastValue)
			lastValue = i;
	}

	lastPath.Flip();
}

void Pathfinding::SetMap()
{
	walkability = App->map->data.layers.At(1)->data->data;
	width = App->map->data.width;
	height = App->map->data.width;

}

bool Pathfinding::CleanUp()
{
	lastPath.Clear();
	return true;
}

p2DynArray<iPoint>* Pathfinding::GetLastPath()
{
	return &lastPath;
}

bool Pathfinding::IsWalkable(const iPoint& pos)const
{
	bool boundaries = (pos.x >= 0 && pos.x <= (int)width && pos.y >= 0 && pos.y <= (int)height);
	if (!boundaries)
		return false;

	bool walkable = (walkability[(pos.y * width) + pos.x] != 11 && walkability[(pos.y * width) + pos.x] != 12);
	if (!walkable) 
		return false;

	return true;

}

void Pathfinding::DebugDraw()
{

	for (uint i = 0; i < lastPath.Count(); ++i)
	{
		iPoint pos(lastPath.At(i)->x, lastPath.At(i)->y);

		App->map->MapToWorld(pos.x, pos.y, App->map->data); //to check

	}

}