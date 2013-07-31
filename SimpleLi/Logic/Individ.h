
#pragma once


#include "..\Logic\Types.h"
#include "..\Logic\GeneticCode.h"
#include "..\Logic\Functions.h"
#include "..\Types_gui.h"

enum Gender {MALE, FEMALE, NEUTER};

class Individ {
public:
	bool			live;
	GeneticCode		dna;
	Gender			gender;
	int				hp;
	int				energy;
	double			speed;
	Vector <double> way;
	Point <int>		pos;
	int				live_timer;
	int				reproduction_timer;
	
	Individ_status	state;

	long long int	ID;
private:
	Individ *spouse;
	bool collision;
	IndMemory <Individ*> mem;

	static long long int count;
public:
	Individ();
	Individ(Point <int> _pos, GeneticCode _gene);
	
	bool operator==(Individ i);
	bool operator!=(Individ i);

	void step(Individ (*(*field)[W][H]), std::vector <Individ> *cradle);
	void move(Individ (*(*field)[W][H]));
	void look(Individ (*(*field)[W][H]));
	void checkState();
	void checkWay();
	void eat(Individ *target);
	void eat();
	void reproduction(Individ *target, std::vector <Individ> *population) ;
	void beginReproduction();
	void heal();
	void isLive();
	bool isNearby(Individ *target);
	IndMemory <Individ*>  whoIsNearby(Individ (*(*field)[W][H]));
};

