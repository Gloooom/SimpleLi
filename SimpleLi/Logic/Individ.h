
#pragma once

#include "GeneticCode.h"

enum Gender {MALE, FEMALE, NEUTER};

class Individ {
public:
	bool			live;
	GeneticCode		dna;
	int				gender;
	int				hp;
	int				energy;
	double			speed;
	Vector <double> way;
	Point <int>		pos;
	int				live_timer;
	int				reproduction_timer;
	
	int				state;
	
	long long int ID;
	long long int spouse;
private:
	bool collision;
	IndMemory <Individ*> mem;

	static long long int count;
public:
	Individ();
	Individ(Point <int> _pos, GeneticCode _gene);
	
	bool operator==(Individ i);
	bool operator!=(Individ i);

	void step(Individ (*(*field)[W][H]), std::deque <Individ> *cradle, std::map <long long int, Individ> *population);
	void move(Individ (*(*field)[W][H]));
	void look(Individ (*(*field)[W][H]));
	void checkState();
	void checkWay();
	void eat(Individ *target);
	void eat();
	void reproduction(Individ (*(*field)[W][H]), std::deque <Individ> *cradle, std::map <long long int, Individ> *population);
	void beginReproduction(long long int _spouse, std::map <long long int, Individ> *population);
	void heal();
	void isLive();
	bool isNearby(Individ *target);
	Point <int> getNearestEmpty(Individ (*(*field)[W][H]));
	IndMemory <long long int>  whoIsNearby(Individ (*(*field)[W][H]));
};

