#include "Map.h"
#include <list>
#include "FloorGameObject.h"
#include <iostream>
#include <Windows.h>
#include <time.h>

using namespace std;

Map::Map(int _width, int _height) : width(_width), height(_height)
{
	GraphicalSymbol g2('O', 1, 2);
	floor = new FloorGameObject("floor", g2);
	for (int i = 0; i < _width; i++)
	{
		vector<Room*> tmp;
		for (int j = 0; j < _height; j++)
		{
			tmp.push_back(nullptr);
		}
		mapDesign.push_back(tmp);
	}
}
FloorGameObject* Map::getFloor() const
{
	return floor;
}
void Map::setFloor(FloorGameObject& _object)
{
	floor = &_object;
}
int Map::getWidth() const
{
	return width;
}

int Map::getHeight() const
{
	return height;
}

void Map::setRoomInMap(int _width, int _height, Room& _object)
{
	*mapDesign[_width][_height] = _object;
}
Room* Map::getRoomFromMap(int _width, int _height)
{
	return mapDesign[_width][_height];
}
void Map::addRoom(Room& _object)
{
	rooms.push_back(&_object);
	size++;
}
int Map::generateMap()
{
	int randRoom;
	int roz = width * height;
	int* chosArr = new int[roz];
	int end = 0;
	srand(time(NULL));
	int zakres = size;
	bool flag1 = false;
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			while (true) 
			{
				randRoom = rand() % zakres;
				for (int x = 0; x < end; x++)
				{
					if (chosArr[x] == randRoom)
					{
						flag1 = true;
						break;
					}
					flag1 = false;
				}
				if (flag1 == false)
				{
					break;
				}
			}
			chosArr[end] = randRoom;
			end++;
			mapDesign[i][j] = rooms[randRoom];
			cout << randRoom << endl;
		}
	}
	if (generateMapConnections() == -1)
	{
		return -1;
	}
	mergeRoomsIntoMap();
	delete chosArr;
}
struct A {
public:
	A() {}
	A(int x, int y)
	{
		array[0] = x;
		array[1] = y;
	}
	int array[2];
};


int Map::generateMapConnections()
{
	int randWidthStart = rand() % (width);
	int randheightStart = rand() % (height);
	int randWidthEnd = rand() % (width);
	int randheightEnd = rand() % (height);
	while ((randWidthStart == randWidthEnd) && (randheightStart == randheightEnd))
	{
		randWidthEnd = rand() % (width);
		randheightEnd = rand() % (height);
	}
	bool** visited = new bool* [width];
	for (int i = 0; i < width; i++)
	{
		visited[i] = new bool[height];
	}
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			visited[i][j] = false;
		}
	}
	A* list = new A[100];
	int end = 0;
	list[end] = A(randWidthStart, randheightStart);
	end++;
	int randDirection;
	int iterator = 0;

	visited[randWidthStart][randheightStart] = true;
	iterator += 1;
	while (iterator != width * height )
	{
		if (randheightStart - 1 >= 0 && visited[randWidthStart][randheightStart - 1] == false)
		{
			randheightStart = randheightStart - 1;
			visited[randWidthStart][randheightStart] = true;
			list[end] = A(randWidthStart, randheightStart);
			end++;
			iterator += 1;
			if (connect(*mapDesign[randWidthStart][randheightStart], *mapDesign[randWidthStart][randheightStart + 1], 0) == -1)
			{
				return -1;
			}
		}
		else if (randWidthStart - 1 >= 0 && visited[randWidthStart - 1][randheightStart] == false)
		{
			randWidthStart = randWidthStart - 1;
			visited[randWidthStart][randheightStart] = true;
			list[end] = A(randWidthStart, randheightStart);
			end++;
			iterator += 1;
			if (connect(*mapDesign[randWidthStart][randheightStart], *mapDesign[randWidthStart + 1][randheightStart], 1) == -1)
			{
				return -1;
			}
		}
		else if (randheightStart + 1 < height && visited[randWidthStart][randheightStart + 1] == false)
		{
			randheightStart = randheightStart + 1;
			visited[randWidthStart][randheightStart] = true;
			list[end] = A(randWidthStart, randheightStart);
			end++;
			iterator += 1;

			if (connect(*mapDesign[randWidthStart][randheightStart - 1], *mapDesign[randWidthStart][randheightStart], 2) == -1)
			{
				return -1;
			}
		}
		else if (randWidthStart + 1 < width && visited[randWidthStart + 1][randheightStart] == false)
		{
			randWidthStart = randWidthStart + 1;
			visited[randWidthStart][randheightStart] = true;
			list[end] = A(randWidthStart, randheightStart);
			end++;
			iterator += 1;

			if (connect(*mapDesign[randWidthStart - 1][randheightStart], *mapDesign[randWidthStart][randheightStart], 3) == -1)
			{
				return -1;
			}
		}
		else 
		{

			end--;

			randWidthStart = list[end].array[0];
			randheightStart = list[end].array[1];
		}
	}
	for (int i = 0; i < width; i++)
	{
		delete visited[i];
	}
	delete visited;
	delete list;
}

int Map :: createPath(Room& room, Room& room1, Room& room2, int possitionX, int possitionY)
{
	int randWidthStart = possitionX;
	int randheightStart = possitionY;

	bool** visited = new bool* [room.getWidth()];
	for (int i = 0; i < room.getWidth(); i++)
	{
		visited[i] = new bool[room.getHeight()];
	}
	for (int i = 0; i < room.getWidth(); i++)
	{
		for (int j = 0; j < room.getHeight(); j++)
		{
			visited[i][j] = false;
		}
	}
	A* list = new A[room.getWidth() * room.getHeight()];
	int end = 0;
	list[end] = A(randWidthStart, randheightStart);
	end++;
	visited[randWidthStart][randheightStart] = true;

	if (room.getHeight() > 11)
	{
		while (true)
		{
			if (randheightStart + 1 < room.getHeight() && visited[randWidthStart][randheightStart + 1] == false)
			{

				randheightStart += 1;
				visited[randWidthStart][randheightStart] = true;
				list[end] = A(randWidthStart, randheightStart);
				end++;
				room.setElementInRoom(randWidthStart, randheightStart, 0, *getFloor());
			}
			else if (randWidthStart + 1 < room.getWidth() && visited[randWidthStart + 1][randheightStart] == false)
			{

				randWidthStart += 1;
				visited[randWidthStart][randheightStart] = true;
				list[end] = A(randWidthStart, randheightStart);
				end++;
				room.setElementInRoom(randWidthStart, randheightStart, 0, *getFloor());
			}
			else if (randWidthStart - 1 >=0 && visited[randWidthStart - 1][randheightStart] == false)
			{

				randWidthStart -= 1;
				visited[randWidthStart][randheightStart] = true;
				list[end] = A(randWidthStart, randheightStart);
				end++;
				room.setElementInRoom(randWidthStart, randheightStart, 0, *getFloor());
			}

			else
			{

				if (end == 0) { return -1; }
				else
				{
					end--;
					randWidthStart = list[end].array[0];
					randheightStart = list[end].array[1];
				}
			}
			if (randheightStart > room.getHeight() / 2 && room.getHeight() > 11) 
			{
				if (randheightStart + 1 < room.getHeight())
				{	
					if (room.getRoomElement(randWidthStart, randheightStart + 1, 0)->getRepresentation().getCharSymbol() == 'O')
					{
						break;
					}
				}
				if (randWidthStart + 1 < room.getWidth())
				{
					if (room.getRoomElement(randWidthStart + 1, randheightStart, 0)->getRepresentation().getCharSymbol() == 'O')
					{
						break;
					}
				}

				if (randWidthStart - 1>=0)
				{
					if (room.getRoomElement(randWidthStart - 1, randheightStart, 0)->getRepresentation().getCharSymbol() == 'O')
					{
						break;
					}
				}
			}
		}
	}
	else
	{
		while (true)
		{
			visited[randWidthStart][randheightStart] = true;

			if (randWidthStart + 1 < room.getWidth() && visited[randWidthStart + 1][randheightStart] == false)
			{

				randWidthStart += 1;
				visited[randWidthStart][randheightStart] = true;
				list[end] = A(randWidthStart, randheightStart);
				end++;
				room.setElementInRoom(randWidthStart, randheightStart, 0, *getFloor());

			}
			else if (randheightStart + 1 < room.getHeight() && visited[randWidthStart][randheightStart + 1] == false)
			{

				randheightStart += 1;
				visited[randWidthStart][randheightStart] = true;
				list[end] = A(randWidthStart, randheightStart);
				end++;
				room.setElementInRoom(randWidthStart, randheightStart, 0, *getFloor());
			}
			else if (randheightStart - 1 <= 0 && visited[randWidthStart][randheightStart - 1] == false)
			{

				randheightStart -= 1;
				visited[randWidthStart][randheightStart] = true;
				list[end] = A(randWidthStart, randheightStart);
				end++;
				room.setElementInRoom(randWidthStart, randheightStart, 0, *getFloor());
			}
			else
			{
				if (end == 0) { return -1; }
				else
				{
					end--;
					randWidthStart = list[end].array[0];
					randheightStart = list[end].array[1];
				}
			}
			if (randWidthStart > room.getWidth() / 2 && room.getWidth() >11)
			{
				if (randheightStart - 1>=0)
				{
					if (room.getRoomElement(randWidthStart, randheightStart - 1, 0)->getRepresentation().getCharSymbol() == 'O')
					{
						break;
					}
				}
				if (randWidthStart + 1 < room.getWidth())
				{
					if (room.getRoomElement(randWidthStart + 1, randheightStart, 0)->getRepresentation().getCharSymbol() == 'O')
					{
						break;
					}
				}
				if (randheightStart + 1 < room.getHeight())
				{
					if (room.getRoomElement(randWidthStart, randheightStart + 1, 0)->getRepresentation().getCharSymbol() == 'O')
					{
						break;
					}
				}
			}
		}
	}
	if (room.getHeight() > room.getWidth())
	{

		for (int i = 0; i < room.getWidth(); i++)
		{
			for (int j = 0; j < room.getHeight(); j++)
			{
				if (j >= 10)
				{
					room2.setElementInRoom(i, j - 10, 0, *room.getRoomElement(i, j, 0));
				}
				else
				{
					room1.setElementInRoom(i, j, 0, *room.getRoomElement(i, j, 0));
				}
			}
		}
	}
	else
	{
		for (int i = 0; i < room.getWidth(); i++)
		{
			for (int j = 0; j < room.getHeight(); j++)
			{
				if (i >= 10)
				{

					room2.setElementInRoom(i - 10,j, 0, *room.getRoomElement(i,j, 0));
				}
				else
				{

					room1.setElementInRoom(i,j, 0, *room.getRoomElement(i,j ,0));
				}
			}
		}
	}
	for (int i = 0; i < room.getWidth(); i++)
	{
		delete visited[i];
	}
	delete visited;
	delete list;
}
int Map::connect(Room& room1, Room& room2, int direction)
{

	int possitionX = -1;
	int possitionY = -1;
	bool flag = false;
	if (direction == 0 || direction == 2)
	{
		for (size_t j = room1.getHeight() - 1; j >= 0; j--)
		{
			for (size_t i = 0; i < room1.getWidth(); i++)
			{
				if (room1.getRoomElement(i, j, 0)->getRepresentation().getCharSymbol() == 'O')
				{
					possitionX = i;
					possitionY = j;
					flag = true;
					break;
				}
			}
			if (flag) {
				break;
			}
		}
		Room room(10, 20, 1);
		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				room.setElementInRoom(i, j, 0, *room1.getRoomElement(i, j, 0));
			}
		}
		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				room.setElementInRoom(i, j + 10, 0, *room2.getRoomElement(i, j, 0));
			}
		}
		if (createPath(room, room1, room2, possitionX, possitionY) == -1)
		{
			return -1;
		}
	}
	else if (direction == 1 || direction == 3)
	{
		for (int i = room1.getWidth() - 1; i >= 0; i--)
		{
			for (int j = 0; j < room1.getHeight(); j++)
			{
				if (room1.getRoomElement(i, j, 0)->getRepresentation().getCharSymbol() == 'O')
				{
					possitionX = i;
					possitionY = j;
					flag = true;
					break;
				}
			}
			if (flag) {
				break;
			}
		}
		Room room(20, 10, 1);
		for (size_t i = 0; i < 10; i++)
		{
			for (size_t j = 0; j < 10; j++)
			{
				room.setElementInRoom(i, j, 0, *room1.getRoomElement(i, j, 0));
			}
		}
		for (size_t i = 0; i < 10; i++)
		{
			for (size_t j = 0; j < 10; j++)
			{
				room.setElementInRoom(i + 10, j, 0, *room2.getRoomElement(i, j, 0));
			}
		}
		if (createPath(room, room1, room2, possitionX, possitionY) == -1)
		{
			return -1;
		}
		
	}
}
//void Map::move(CreatureGameObject& _object, int x, int y) 
//{
//	auto interactObject = dynamic_cast<Colliding*>(_object.)
//	if()
//}
void Map :: mergeRoomsIntoMap()
{
	vector < vector<GameObject* >>  tmp;
	for (int i = 0; i < mapDesign.size() * 10; i++)
	{
		vector<vector<GameObject*>> tmp2;
		for (int j = 0; j < mapDesign[0].size() * 10; j++)
		{
			vector<GameObject*> tmp;
			for (int k = 0; k < mapDesign[0][0]->getDepth(); k++)
			{
				tmp.push_back(nullptr);
			}
			tmp2.push_back(tmp);
		}
		mapDesignObjects.push_back(tmp2);
	}
	for (int i = 0; i < mapDesign.size(); i++)
	{
		for (int j = 0; j < mapDesign[0].size(); j++)
		{
			for (int x = 0; x < 10; x++)
			{
				for (int y = 0; y < 10; y++)
				{
					for (int z = 0; z < mapDesign[0][0]->getDepth(); z++)
					{
						mapDesignObjects[x + (10 * i)][y + (10 * j)][z] = mapDesign[i][j]->getRoomElement(x, y, z);
					}
				}
			}
		}
	}
}
void Map :: setObjectInMap(GameObject& _object, int _width, int _height, int _depth)
{
	mapDesignObjects[_width][_height][_depth] = &_object;
}
GameObject* Map :: getObjectInMap(int _width, int _height, int _depth)
{
	return mapDesignObjects[_width][_height][_depth];
}

void Map::move(CreatureGameObject& _object, int x, int y)
{
	for (int i = 0; i < mapDesignObjects.size(); i++)
	{
		for (int j = 0; j < mapDesignObjects[i].size(); j++)
		{
			if (mapDesignObjects[i][j][3] == &_object)
			{
				auto colObj = dynamic_cast<Colliding*>(mapDesignObjects[i+x][j+y][0]);
				if (colObj != NULL)
				{
					_object.onCollide(_object);
				}
				auto inrObj = dynamic_cast<InteractionableGameObject*>(colObj);
				if (inrObj != NULL)
				{
					_object.onInteraction();
				}
				mapDesignObjects[i + x][j + y][3] = mapDesignObjects[i][j][3];
				mapDesignObjects[i][j][3] = nullptr;
				return;
			}
		}
	}
}