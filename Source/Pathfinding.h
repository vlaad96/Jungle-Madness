#ifndef _PATHFINDING_H_
#define _PATHFINDING_H_

#include "j1Module.h"
#include "p2DynArray.h"
#include "p2List.h"
#include "p2PQueue.h"
#include "j1App.h"
#include "j1Map.h"
#include "j1Textures.h"

class Pathfinding : public j1Module
{
public:

	Pathfinding();

	~Pathfinding();

public:
	//Needed functions

	void SetMap(); //Called when the map changes
	void CreatePath(iPoint origin, iPoint dest);
	void PathBacktracking(iPoint goal);

	bool CleanUp();
	bool IsWalkable(const iPoint& pos)const; //0 if walkable and -1 if it isn't
	void DebugDraw();

	p2DynArray<iPoint>* GetLastPath();

private:

	uint width;
	uint height;

	uint* walkability = nullptr;

	p2PQueue<iPoint> frontier;
	p2List<iPoint> visited;
	p2List<iPoint> breadcrumbs;


};

#endif // _PATHFINDING_H_
