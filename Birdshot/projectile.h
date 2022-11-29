//Projectile Types (Inheritance/Abstraction/Polymorphism)

#pragma once
#include "ftxuiAll.h"

class projectile {
public:
	projectile() {
	}

	virtual void Activate(bool &rockOn, bool &laserOn, bool& rcrOn) = 0;
};