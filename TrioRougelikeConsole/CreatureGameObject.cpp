#include "CreatureGameObject.h"
#include "ItemGameObject.h"
#include "Bars.h"
#include <Windows.h>

CreatureGameObject::CreatureGameObject(int hp, int armor, int exp, int activeItemsCount, string Name, GraphicalSymbol Symbol) :
	healthPoints(hp), baseArmor(armor), baseHealthPoints(hp), experience(exp),
	DynamicGameObject(Name, Symbol)
{
	//	DEBUG
	cout << getTag() << "Created CreatureGameObject (hp=" << hp << ", armor=" << armor << ", actItems=" << activeItemsCount << ")" << endl;
	//	END DEBUG

	for (int i = 0; i < activeItemsCount; ++i)
		activeInventory.push_back(NULL);
}

CreatureGameObject::CreatureGameObject(int hp, int armor, int exp, int activeItemsCount, string Name, GraphicalSymbol Symbol, const vector<ItemGameObject*>& itemList) :
	healthPoints(hp), baseArmor(armor), baseHealthPoints(hp), experience(exp),
	DynamicGameObject(Name, Symbol)
{
	//	DEBUG
	cout << getTag() << "Created CreatureGameObject (hp=" << hp << ", armor=" << armor << ", actItems=" << activeItemsCount << ") with items: ";
	for (auto item : itemList)
		cout << item->getName() << "|";
	cout << endl;
	//	END DEBUG

	activeInventory = vector<ItemGameObject*>(itemList);

	for (int i = 0; i < activeItemsCount - itemList.size(); ++i)
		activeInventory.push_back(NULL);
}

void CreatureGameObject::onHit(int dmg)
{
	int bonusArmor = 0;
	for (ItemGameObject* item : activeInventory)
	{
		if (item)
			bonusArmor += item->getArmor();
	}
	int damage = dmg - (baseArmor + bonusArmor);

	for (ItemGameObject* item : activeInventory)
	{
		if (item)
			damage = item->onDamege(damage, *this);
	}
	cout << getTag() << "Taking " << damage << " damage (tried: " << dmg << ")" << endl;

	if (damage > 0)
	{
		healthPoints -= damage;
	}
	cout << getTag() << healthPoints << "/" << baseHealthPoints << endl;

	showHealthBar(healthPoints, baseHealthPoints);

	

	if (healthPoints <= 0)
	{
		onDeath();
		return;
	}
}

void CreatureGameObject::onDeath()
{
	alive = false;
	activeEffects.clear();

	cout << getTag() << "Died" << endl;
}

void CreatureGameObject::onAttack(CreatureGameObject& opponent)
{
	int damage = 0;

	for (ItemGameObject* item : activeInventory)
		if (item)
			damage += item->getMinDamage() + (rand() % (item->getMaxDamage() - item->getMinDamage() + 1));

	cout << getTag() << "Attacking " << opponent.getName() << " for " << damage << " damage" << endl;


	for (int i = 0; i < activeInventory.size(); ++i)
		if (activeInventory[i])
			activeInventory[i]->onAttack(opponent);

	opponent.onHit(damage);
}

void CreatureGameObject::onRefresh()
{
	for (auto effect : activeEffects)
	{
		if (alive)
			effect->onRefresh(*this);
	}
}

void CreatureGameObject::onInteraction()
{
}

void CreatureGameObject::onCollide(GameObject& sender)
{

}

int CreatureGameObject::getHealthPoints() const
{
	return healthPoints;
}

int CreatureGameObject::getBaseHealthPoints() const
{
	return baseHealthPoints;
}

int CreatureGameObject::getBaseArmor() const
{
	return baseArmor;
}

ItemGameObject* CreatureGameObject::getItemFromSlot(int slot) const
{
	ItemGameObject* item = NULL;
	if (slot < activeInventory.size())
		item = activeInventory[slot];
	return item;
}

bool CreatureGameObject::isAlive() const
{
	return alive;
}

int CreatureGameObject::getExperience() const
{
	return experience;
}

void CreatureGameObject::addEffect(const EffectGameObject& effect)
{
	for (auto eff : activeEffects)
		if (eff->getName() == effect.getName())
			return;

	activeEffects.push_back((EffectGameObject*)effect.clone());
	cout << getTag() << "Effect applied on this creature: " << effect.getName() << endl;
	activeEffects.back()->onRefresh(*this);
}

void CreatureGameObject::removeEffect(string Name)
{
	for (int i = 0; i < activeEffects.size(); ++i)
	{
		if (activeEffects[i]->getName() == Name)
		{
			activeEffects.erase(activeEffects.begin() + i);
			cout << getTag() << "Effect " << Name << " has been removed from creature." << endl;
		}
	}
}

bool CreatureGameObject::equipItem(ItemGameObject* item, int slot)
{
	if (slot < activeInventory.size())
	{
		if (activeInventory[slot] == NULL)
		{
			cout << "[" << name << "] Equipping " << item->getName() << " in slot " << slot << endl;
			activeInventory[slot] = (ItemGameObject*)item->clone();
			item->onEquipping(*this);
		}
	}
	return false;
}

void CreatureGameObject::directDmg(int dmg)
{
	healthPoints -= dmg;
	if (healthPoints > baseHealthPoints)
		healthPoints = baseHealthPoints;
	cout << getTag() << "Taking direct damage: " << dmg << endl;
	cout << getTag() << healthPoints << "/" << baseHealthPoints << endl;

	showHealthBar(healthPoints, baseHealthPoints);

	

	if (healthPoints <= 0)
	{
		onDeath();
	}
}

CreatureGameObject::~CreatureGameObject()
{
	for (ItemGameObject* item : activeInventory)
		if (item)
			delete item;
	for (EffectGameObject* effect : activeEffects)
		if (effect)
			delete effect;
}
