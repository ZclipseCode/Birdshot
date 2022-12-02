#pragma once
#include <string>

class highscore {
private:
	std::string name;
	std::string score;

public:
	highscore(std::string name, std::string score) {
		this->name = name;
		this->score = score;
	};

	void SetName(std::string name) {
		this->name = name;
	}

	void SetScore(std::string score) {
		this->score = score;
	}

	std::string CompleteScore() {
		return name + " - " + score;
	}
};