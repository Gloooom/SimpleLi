
#pragma once


#include "GeneticCode.h"

enum Gender {MALE, FEMALE, NEUTER};


class Environment;

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
	
	Environment *env;

private:
	static long long int count;

public:
	Individ_Proto();
	Individ_Proto(Vector <int> _pos, GeneticCode _dna);
	
	bool operator==(Individ_Proto *i);
	bool operator!=(Individ_Proto *i);

public:
	void reproduction();
	void beginReproduction(long long int _spouseID);
	
	bool		isNearby(long long int target);
	void		isLive();
	void		heal();
	void		calcMove();
	
	Point		<int>				getNearestEmpty();
	IndMemory	<long long int>		whoIsNearby();
public:
	void		look(Array <Individ_Proto*> *field);
	void		look(std::map <long long int, Individ_Proto*> *population);
	void		checkWay();
	void		checkState();
	bool		move();

	virtual void step() = 0;
};

class Individ_Empty : public Individ_Proto {
public:
	Individ_Empty() : Individ_Proto() {};
	Individ_Empty(GeneticCode _dna) : Individ_Proto() { dna = _dna; };

	void step() {};
};

class Individ_Auto : public Individ_Proto {
public:
	Individ_Auto() : Individ_Proto() {};
	Individ_Auto(Vector <int> _pos, GeneticCode _dna) : Individ_Proto(_pos, _dna) {};
	
	void eat();
	void step();
};

class Individ_Getero : public Individ_Proto {
private:
	long long int victimID;
public:
	Individ_Getero() : Individ_Proto() {};
	Individ_Getero(Vector <int> _pos, GeneticCode _dna) : Individ_Proto(_pos, _dna) {};

	void beginEating(long long int _victimID);
	void eat();
	void step();
};


Individ_Proto *CreateIndivid(Vector <int> _pos, GeneticCode _dna);
Individ_Proto *CreateIndivid(int _diet);














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
	Environment(int width, int height);
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