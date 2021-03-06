#include "EnemyGameObject.h"
#include "Game.h"
#include <time.h>

EnemyGameObject::EnemyGameObject(int hp, int armor, int exp, int activeItemsCount, string Name, GraphicalSymbol Symbol) : 
    CreatureGameObject(hp, armor, exp, activeItemsCount, Name, Symbol)
{
 //   cout << getTag() << "Creating EnemyGameObject";
}

EnemyGameObject::EnemyGameObject(int hp, int armor, int exp, int activeItemsCount, string Name, GraphicalSymbol Symbol, const vector<ItemGameObject*>& itemList):
    CreatureGameObject(hp, armor, exp, activeItemsCount, Name, Symbol, itemList)
{
  //  cout << getTag() << "Creating EnemyGameObject with items" << endl;;
}

void EnemyGameObject::onRefresh()
{
    srand(time(NULL));
    int x = -1 + rand() % 3, y = -1 + rand() % 3;
    Game::getMap()->move(*this, x, y);

    CreatureGameObject::onRefresh();
}

GameObject* EnemyGameObject::clone() const
{
    return new EnemyGameObject(*this);
}

EnemyGameObject::~EnemyGameObject()
{
}
