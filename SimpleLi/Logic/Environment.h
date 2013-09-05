#pragma once

#include "Individ.h"


class Environment {
private:
	int _width;
	int _height;
public:
	long long int stepCount;
	Individ empty;
	Array <Individ*> field;
	std::map <long long int, Individ> population;
public:
	void setMutation(float maxDelta, float eyeAddChance, float eyeMutationChance, 
								  float radEyeMutationChance, Mode_mutation mode);
	float mutation_maxDelta;
	float mutation_eyeAddChance;
	float mutation_eyeMutationChance;
	float mutation_radEyeMutationChance;
	int mutation_mode;
private:
	std::deque <Individ> cradle;

public:
	Environment(int width, int height):
	field(width, height) {
		_width = width;
		_height = height;
		empty.dna.color = 0xFFFFFFFF;
		mutation_maxDelta = 0;
		mutation_eyeAddChance = 0;
		mutation_eyeMutationChance = 0;
		mutation_radEyeMutationChance = 0;
		mutation_mode = ONE;

		stepCount = 0;
		for (int i=0; i<field.getW(); i++)
			for (int j=0; j<field.getH(); j++)
				field(i,j ) = &empty;
	};
	~Environment() { };
	void save(std::string path);
	void load(std::string path);

	void addIndivid(Individ ind);
	void checkDead();
	void born();
	void fill();
	void step();

	int W() {return _width;};
	int H() {return _height;};
};
