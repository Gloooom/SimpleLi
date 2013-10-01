
#pragma once

#include "GeneticCode.h"

enum Gender {MALE, FEMALE, NEUTER};


class Individ_Proto {
public:
	bool			live;
	GeneticCode		dna;
	int				gender;
	int				hp;
	float			energy;
	double			speed;
	Vector <double> way;
	Vector <int>	pos;
	int				live_timer;
	int				reproduction_timer;
	
	int				state;
	
	long long int	ID;
	long long int	spouseID;
public:
	bool			collision;
	IndMemory <Individ_Proto*> mem;

	static long long int count;
public:
	Individ_Proto();
	Individ_Proto(Vector <int> _pos, GeneticCode _dna);
	
	bool operator==(Individ_Proto *i);
	bool operator!=(Individ_Proto *i);

public:
	void		reproduction(Array <Individ_Proto*> *field, std::deque <Individ_Proto*> *cradle, std::map <long long int, Individ_Proto*> *population);
	void		beginReproduction(long long int _spouseID, std::map <long long int, Individ_Proto*> *population);
	bool		isNearby(Individ_Proto *target);
	void		isLive();
	void		heal();
	void		calcMove(Array <Individ_Proto*> *field);
	
	Point		<int>				getNearestEmpty(Array <Individ_Proto*> *field);
	IndMemory	<long long int>		whoIsNearby(Array <Individ_Proto*> *field);
public:
	void		look(Array <Individ_Proto*> *field);
	void		look(std::map <long long int, Individ_Proto*> *population);
	void		checkWay();
	void		checkState();
	bool		move(Array <Individ_Proto*> *field);

	virtual void step(Array <Individ_Proto*> *field, std::deque <Individ_Proto*> *cradle, std::map <long long int, Individ_Proto*> *population) = 0;
};

class Individ_Empty : public Individ_Proto {
public:
	Individ_Empty() : Individ_Proto() {};
	Individ_Empty(GeneticCode _dna) : Individ_Proto() { dna = _dna; };

	void step(Array <Individ_Proto*> *field, std::deque <Individ_Proto*> *cradle, std::map <long long int, Individ_Proto*> *population) {};
};

class Individ_Auto : public Individ_Proto {
public:
	Individ_Auto() : Individ_Proto() {};
	Individ_Auto(Vector <int> _pos, GeneticCode _dna) : Individ_Proto(_pos, _dna) {};
	
	void eat();
	void step(Array <Individ_Proto*> *field, std::deque <Individ_Proto*> *cradle, std::map <long long int, Individ_Proto*> *population);
};

class Individ_Getero : public Individ_Proto {
private:
	long long int victimID;
public:
	Individ_Getero() : Individ_Proto() {};
	Individ_Getero(Vector <int> _pos, GeneticCode _dna) : Individ_Proto(_pos, _dna) {};

	void beginEating(long long int _victimID, std::map <long long int, Individ_Proto*> *population);
	void eat(std::map <long long int, Individ_Proto*> *population);
	void step(Array <Individ_Proto*> *field, std::deque <Individ_Proto*> *cradle, std::map <long long int, Individ_Proto*> *population);
};


Individ_Proto *CreateIndivid(Vector <int> _pos, GeneticCode _dna);
Individ_Proto *CreateIndivid(int _diet);