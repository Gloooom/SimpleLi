
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
	
	bool operator==(Individ *i);
	bool operator!=(Individ *i);

protected:
	void		checkState();
	void		reproduction(Array <Individ*> *field, std::deque <Individ*> *cradle, std::map <long long int, Individ*> *population);
	void		beginReproduction(long long int _spouseID, std::map <long long int, Individ*> *population);
	bool		isNearby(Individ *target);
	void		isLive();
	void		heal();
	void		calcMove(Array <Individ*> *field);
	
	Point		<int>				getNearestEmpty(Array <Individ*> *field);
	IndMemory	<long long int>		whoIsNearby(Array <Individ*> *field);
public:
	void		look(Array <Individ*> *field);
	void		look(std::map <long long int, Individ*> *population);
	void		checkWay();
	void		move(Array <Individ*> *field);

	virtual void step(Array <Individ*> *field, std::deque <Individ*> *cradle, std::map <long long int, Individ*> *population) = 0;
};

class Individ_Auto : public Individ {
public:
	Individ_Auto();
	Individ_Auto(Vector <int> _pos, GeneticCode _dna);
	
	void eat();
	void step(Array <Individ*> *field, std::deque <Individ*> *cradle, std::map <long long int, Individ*> *population);
};

//class Individ_Getero : public Individ {
//public:
//	void eat(long long int targetID, std::map <long long int, Individ> *population);
//	void step(Array <Individ*> *field, std::deque <Individ> *cradle, std::map <long long int, Individ> *population);
//};