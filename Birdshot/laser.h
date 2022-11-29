#pragma once
#include "projectile.h"
#include "ftxuiAll.h"

using namespace ftxui;

class laser : public projectile {
public:
	laser() {
	}

	void Activate(bool &rockOn, bool &laserOn, bool& rcrOn) override {
		rockOn = false;
		laserOn = true;
		rcrOn = false;
	}
};