
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
	Vector <int>	pos;
	int				live_timer;
	int				reproduction_timer;
	
	int				state;
	
	long long int	ID;
	long long int	spouseID;
private:
	bool			collision;
	IndMemory <Individ*> mem;

	static long long int count;
public:
	Individ();
	Individ(Vector <int> _pos, GeneticCode _dna);
	
	bool operator==(Individ i);
	bool operator!=(Individ i);

	void step(Array <Individ*> *field, std::deque <Individ> *cradle, std::map <long long int, Individ> *population);
	void move(Array <Individ*> *field);
	void look(Array <Individ*> *field);
	void look(std::map <long long int, Individ> *population);
	void checkState();
	void checkWay();
	void eat(long long int targetID, std::map <long long int, Individ> *population);
	void eat();
	void reproduction(Array <Individ*> *field, std::deque <Individ> *cradle, std::map <long long int, Individ> *population);
	void beginReproduction(long long int _spouseID, std::map <long long int, Individ> *population);
	void heal();
	void isLive();
	bool isNearby(Individ *target);
	Point <int> getNearestEmpty(Array <Individ*> *field);
	IndMemory <long long int>  whoIsNearby(Array <Individ*> *field);
};

