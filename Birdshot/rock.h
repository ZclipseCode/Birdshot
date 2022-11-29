#pragma once
#include "projectile.h"
#include "ftxuiAll.h"

using namespace ftxui;

class rock : public projectile {
public:
	rock() {
	}

	void Activate(bool &rockOn, bool &laserOn, bool& rcrOn) override{
		rockOn = true;
		laserOn = false;
		rcrOn = false;
	}
};