#pragma once

#include "Individ.h"


class Environment {
private:
	int _width;
	int _height;
public:
	long long int stepCount;
	Individ_Auto empty;
	Array <Individ_Proto*> field;
	std::map <long long int, Individ_Proto*> population;
public:
	float mutation_maxDelta;
	int	  mutation_mutGenCount;
	float mutation_eyeAddChance;
	float mutation_eyeMutationChance;
	float mutation_radEyeMutationChance;

	void setMutation(float maxDelta, int mutGenCount, float eyeAddChance, float eyeMutationChance, 
								  float radEyeMutationChance);
private:
	std::deque <Individ_Proto*> cradle;

public:
	Environment(int width, int height):
	field(width, height) {
		_width = width;
		_height = height;
		empty.dna.color = 0xFFFFFFFF;
		mutation_maxDelta = 0;
		mutation_mutGenCount = 1;
		mutation_eyeAddChance = 0;
		mutation_eyeMutationChance = 0;
		mutation_radEyeMutationChance = 0;

		stepCount = 0;
		for (int i=0; i<field.getW(); i++)
			for (int j=0; j<field.getH(); j++)
				field(i,j ) = &empty;
	};
	~Environment() { };
	void save(std::string path);
	void load(std::string path);

	void addIndivid(Individ_Proto *ind);
	void checkDead();
	void born();
	void fill();
	void step();
	void clear();

	int W() {return _width;};
	int H() {return _height;};
};
