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

	std::string GetName() {
		return name;
	}

	std::string GetScoreStr() {
		return score;
	}

	int GetScoreInt() {
		return std::stoi(score);
	}

	std::string CompleteScore() {
		return name + " - " + score;
	}
};