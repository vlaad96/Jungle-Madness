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

}

void Pathfinding::PathBacktracking(iPoint goal)
{

}

void Pathfinding::SetMap()
{

}

bool Pathfinding::CleanUp()
{
	return true;
}

p2DynArray<iPoint>* Pathfinding::GetLastPath()
{

}

bool Pathfinding::IsWalkable(const iPoint& pos)const
{

}

void Pathfinding::DebugDraw()
{

}