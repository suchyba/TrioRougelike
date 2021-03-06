#pragma once
#include "InteractionalbeGameObject.h"

class EndGameObject : public InteractionableGameObject
{
public:
	EndGameObject(string Name, GraphicalSymbol Symbol);
	// Odziedziczono za pośrednictwem elementu InteractionableGameObject
	virtual void onInteraction() override;
	virtual GameObject* clone() const override;
};

