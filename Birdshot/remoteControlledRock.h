#pragma once
#include "projectile.h"
#include "ftxuiAll.h"

using namespace ftxui;

class remoteControlledRock : public projectile {
public:
	remoteControlledRock() {
	}

	void Activate(bool& rockOn, bool& laserOn, bool& rcrOn) override {
		rockOn = false;
		laserOn = false;
		rcrOn = true;
	}
};