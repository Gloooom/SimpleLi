#include "inclusion.h"

#pragma once

#include "Environment.h"


Individ_Proto *CreateIndivid(Vector <int> _pos, GeneticCode _dna) {
	if (_dna.diet == AUTO) return new Individ_Auto(_pos, _dna);
	if (_dna.diet == GETERO) return new Individ_Getero(_pos, _dna);
}; 

Individ_Proto *CreateIndivid(int _diet) {
	if (_diet == AUTO) return new Individ_Auto();
	if (_diet == GETERO) return new Individ_Getero();
};


long long int Individ_Proto::count=1;


Individ_Proto::Individ_Proto(): way(0,0) {
	gender = NEUTER;
	ID = 0;
	live = false;
}

Individ_Proto::Individ_Proto(Vector <int> _pos, GeneticCode _dna) {
	ID = count;
	count++;

	way.fromDeg(func::randf(0, M_PI*2));
	gender = (func::randBool()) ? MALE : FEMALE;
	collision=false;
	live = true;
	pos =_pos;
	speed = 0;
	dna = _dna;
	live_timer = 0;
	state = WAIT;
	reproduction_timer = 0;
	hp = _dna.phis[hp_max];
	energy = _dna.phis[energy_max];
	spouseID = ID;
}

bool Individ_Proto::operator==(Individ_Proto *i) {
	if (ID == i->ID) return true;
	else return false;
}

bool Individ_Proto::operator!=(Individ_Proto *i) {
	if (ID == i->ID) return false;
	else return true;
}

//Переписать полностью. Полное говно.
bool Individ_Proto::move() {
	Vector <int> oldPos = pos;

	Vector <double> tempPos(0, 0);
	Individ_Proto *he=this;
	while (way.getLength()!=0 &&
		(he->ID == 0 || *he == this) &&
		(tempPos+way)<=(way*speed) &&
		func::round(pos.x+tempPos.x+way.x)>=0 && 
		func::round(pos.x+tempPos.x+way.x)<env->W() && 
		func::round(pos.y+tempPos.y+way.y)>=0 && 
		func::round(pos.y+tempPos.y+way.y)<env->H()) {
			he = env->field(
				func::round(tempPos.x + way.x + (double) pos.x), 
				func::round(tempPos.y + way.y + (double) pos.y)
				);
			if (he->ID == 0 || *he == this) {
				tempPos+=way;
				collision=false;
			} else { break; }
	}

	if (tempPos.getLength()==0) { way.x*=(-1); way.y*=(-1); }

	pos += tempPos.round();
	if (pos.x >=env->W()) pos.x = env->W()-1;
	if (pos.x <=0) pos.x = 0;
	if (pos.y >=env->H()) pos.y = env->H()-1;
	if (pos.y <=0) pos.y = 0;

	if (oldPos == pos) return false;
	else return true;
}

//дописать обработку глаза типа сектор
//вычислять с помощью перевода координаты ячейки в полярную систему координат
//Полностью переписать. В промежутке формировать матрицу области видимости и уже из неё вычислять видимых индивидумов
//А не как сейчас: добавлять в массив по пять раз один объект.
void Individ_Proto::look(Array <Individ_Proto*> *field) {
	mem.clear();
	double wayAngle = way.getDeg();
	for (int i=0; i<dna.eyes.size(); i++) {

		std::vector <Vector <int> > eyeVectors = dna.eyes[i].getVectors(wayAngle);

		std::vector <Vector <int> > ::iterator p = eyeVectors.begin();
		while (p != eyeVectors.end()) {
			Vector <int> absP;
			absP = *p + pos;
			if (absP.x >= 0 && absP.x < field->getW() && absP.y >= 0 && absP.y < field->getH()) {
				Individ_Proto *he = (*field)(absP);
				if (he->ID != 0 && *he != this) {
					if (he->dna.diet != dna.diet && !func::isIn(mem.enemies, he)) 
						mem.enemies.push_back(he);
					if (he->dna.diet == dna.diet && !func::isIn(mem.partners, he))
						mem.partners.push_back(he);
				}
			}
			p++;
		} 
	}
}

void Individ_Proto::look(std::map <long long int, Individ_Proto*> *population) {
	//Пока слишком криво. Почему-то точки попадания определяются абсолютно правильно, но особи ведут себя слишком странно
	mem.clear();
	double wayAngle = way.getDeg();

	std::vector <FOV_Tri> ::iterator e = dna.eyes.begin();
	while (e != dna.eyes.end()) {
		std::map <long long int, Individ_Proto*> ::iterator p = population->begin();
		while (p != population->end()) {
			if (e->checkHit(p->second->pos.toDouble(), pos.toDouble(), wayAngle)) {
				Individ_Proto *he = p->second;
				if (he->ID != 0 && *he != this) {
					if (he->dna.diet != dna.diet && !func::isIn(mem.enemies, he)) 
						mem.enemies.push_back(he);
					if (he->dna.diet == dna.diet && !func::isIn(mem.partners, he))
						mem.partners.push_back(he);
				}
			}
			p++;
		}
		e++;
	}
}

void Individ_Proto::checkWay() {
	Vector <double> enemyCohesion, partnerCohesion, 
		enemySeparation, partnerSeparation,
		enemyAlignment, partnerAlignment,
		spouseAttraction,
		P0;

	Vector <double> enemiesImpact;
	for(int i=0; i<mem.enemies.size(); i++)	{
		enemiesImpact = (mem.enemies[i]->pos - pos).toDouble();
		enemyCohesion += enemiesImpact.getNorm();
		enemySeparation -= enemiesImpact.getNorm()/((enemiesImpact.getLength()!=0) ? enemiesImpact.getLength() : 1);
		enemyAlignment += mem.enemies[i]->way;
	}

	Vector <double> partnersImpact;
	for(int i=0; i<mem.partners.size(); i++) {
		partnersImpact = (mem.partners[i]->pos - pos).toDouble();
		partnerCohesion += partnersImpact.getNorm();
		partnerSeparation -= partnersImpact.getNorm()/((partnersImpact.getLength()!=0) ? partnersImpact.getLength() : 1);
		partnerAlignment += mem.partners[i]->way;
	}

	Vector <double> libidoImpact;
	for(int i=0; i<mem.partners.size(); i++) {
		if (mem.partners[i]->gender != gender) {
			libidoImpact = (mem.partners[i]->pos - pos).toDouble();
			spouseAttraction += libidoImpact.getNorm();
		}
	}

	if (!mem.enemies.empty() || !mem.partners.empty()) {
		Vector <double> finalWay;
		Vector <double>	wayPartners = partnerCohesion.getNorm()*dna.soc[state][cohesion_partner] + 
			partnerSeparation.getNorm()*dna.soc[state][separation_partner] + 
			partnerAlignment*dna.soc[state][alignment_partner];
		Vector <double> wayEnemies  = partnerCohesion.getNorm()*dna.soc[state][cohesion_enemy] + 
			partnerSeparation.getNorm()*dna.soc[state][separation_enemy] + 
			partnerAlignment*dna.soc[state][alignment_enemy];

		if (mem.partners.empty()) wayPartners = P0;
		if (mem.enemies.empty()) wayEnemies = P0;

		finalWay = wayPartners*dna.soc[state][partner] + wayEnemies*dna.soc[state][enemy] + spouseAttraction*dna.soc[state][libido];
		way = finalWay.getNorm();

	} else {
		way.fromDeg(way.getDeg() + func::randf((-1)*dna.soc[state][rand_way], dna.soc[state][rand_way]));
	}
}

void Individ_Proto::checkState() {
	if (reproduction_timer >= dna.phis[reproduction_pause] && energy >= dna.phis[energy_mature]) 
		state = MATURE;
	else if (energy < dna.phis[energy_hungry]) 
		state = HUNGRY;
	else  
		state = WAIT;
}

void Individ_Proto::beginReproduction(long long int _spouseID) {
	if (state != REPRODUCT && env->population[_spouseID]->state != REPRODUCT) {
		mem.clear();
		env->population[_spouseID]->mem.clear();
		reproduction_timer = 0;
		env->population[_spouseID]->reproduction_timer = 0;
		state = REPRODUCT;
		env->population[_spouseID]->state = REPRODUCT;
		spouseID = _spouseID;
		env->population[_spouseID]->spouseID = ID;
	}
}


//void Individ_Proto::reproduction(Array <Individ_Proto*> *field, 
//	std::deque <Individ_Proto*> *cradle, 
//	std::map <long long int, Individ_Proto*> *population) {
void Individ_Proto::reproduction() {
		if (env->population.find(spouseID) != env->population.end()) {
			if (reproduction_timer < dna.phis[reproduction_time]) {
				reproduction_timer++;
			} else {
				if (gender == FEMALE) {
					env->addIndivid(
						CreateIndivid(
							getNearestEmpty(), 
							dna.hibridization(env->population[spouseID]->dna, AVERAGE))
						);
				}
				energy -= dna.phis[reproduction_cost];
				reproduction_timer = 0;
				state = WAIT;
			}
		} else {
			reproduction_timer = 0;
			state = WAIT;
			spouseID = ID;
		}
}


Point <int> Individ_Proto::getNearestEmpty() {
	double R = 10;
	std::vector <Vector <int> > emptyNear;
	Vector <int> P;

	int start_x = (pos.x-R>0) ? pos.x-R : 0;
	int start_y = (pos.y-R>0) ? pos.y-R : 0;
	for (int _x=start_x; _x<pos.x+R, _x<env->W(); _x++) { 
		P.x=_x-pos.x;
		for (int _y=start_y; _y<pos.y+R, _y<env->H(); _y++) {
			P.y=_y-pos.y;
			if (P.y*P.y+P.x*P.x<R*R) {
				Point <int> absP;
				absP = P + pos;
				Individ_Proto *he = env->field(absP);
				if (he->ID == 0) 
					emptyNear.push_back(P);
			}
		}
	}
	std::sort(emptyNear.begin(), emptyNear.end());

	Point <int> nearestEmptyPoint((*emptyNear.begin()) + pos);
	return nearestEmptyPoint;
}

void Individ_Proto::heal() {
	speed = 0;
	energy--;
	hp++;
}

bool Individ_Proto::isNearby(long long int target) {
	Vector <int> R = env->population[target]->pos - pos;
	R.x = abs(R.x);
	R.y = abs(R.y);
	if (R.x>1 || R.y>1) return false;
	else return true;
}

IndMemory <long long int> Individ_Proto::whoIsNearby() {
	IndMemory <long long int> result;

	int delta[] = {-1, 0, 1};
	Point <int> selectCell;

	for (int x=0; x<=2; x++) 
		for (int y=0; y<=2; y++) {
			selectCell.x = pos.x+delta[x];
			selectCell.y = pos.y+delta[y];
			if (env->field.isValid(selectCell)) {
				Individ_Proto *he = env->field(selectCell);
				if (he->ID != 0 && he->ID != ID && he->live
					&& he->state != REPRODUCT) {
						if (he->dna.diet == dna.diet && he->gender != gender && he->state == MATURE)
							result.partners.push_back(he->ID);
						if (he->dna.diet != dna.diet)
							result.enemies.push_back(he->ID);
				}
			}
		}

		return result;
}

void Individ_Proto::isLive() {
	if (live_timer < dna.phis[live_time]) live_timer++;
	else live = false;
	if (hp==0) live = false;
}

bool isLess(Individ_Proto *first, Individ_Proto *second) {
	if (first->energy < second->energy) return true;
	else return false;
}

void Individ_Proto::calcMove() {
	//Тут надо поработать над логикой. Пока всё очень плохо.
	look(&env->field);
	checkWay();
	float energyCost;

	energyCost = dna.phis[consumption] * speed;

	if (energy-energyCost>=0) {
		if (move())
			energy -= energyCost;
	}

	if (speed + dna.phis[acceleration] <= dna.soc[state][max_speed])
		speed += dna.phis[acceleration];
	else 
		speed = dna.soc[state][max_speed];
}

void Individ_Auto::eat() {
	if (energy + dna.phis[saturation] <= dna.phis[energy_max]) {
		energy += dna.phis[saturation];
	} else {
		energy = dna.phis[energy_max];
	}
}

void Individ_Auto::step() {
	isLive();
	if (live) {
		if (reproduction_timer < dna.phis[reproduction_pause] && state != REPRODUCT) 
			reproduction_timer++;

		if (state == HUNGRY) {
			checkState();
			calcMove();
			eat();
		} else if (state == MATURE) {
			checkState();
			calcMove();
			eat();
			if (gender == MALE) {
				IndMemory <long long int> nearInd;
				nearInd = whoIsNearby();
				if (!nearInd.partners.empty()) {
					//написать функцию поиска сильнейшей особи или осуществить какой-то отбор партнёров
					beginReproduction(*nearInd.partners.begin());
				}
			}
		} else if (state == WAIT) {
			checkState();
			calcMove();
			eat();
		} else if (state == REPRODUCT) {
			reproduction();
		} else if (state == STOP) {
		} else if (state == EAT) {
		}
	}
}

void Individ_Getero::eat() {
	if (env->population.find(victimID) != env->population.end()) {
		if (energy+dna.phis[saturation] <= dna.phis[energy_max]) {
			energy+=dna.phis[saturation];
			env->population[victimID]->hp--;
			env->population[victimID]->isLive();
		} else {
			checkState();
			env->population[victimID]->checkState();
			victimID = 0;
		} 
	} else {
		checkState();
		victimID = 0;
	}
}

void Individ_Getero::beginEating(long long int _victimID) {
	if (state != EAT && env->population[_victimID]->state != REPRODUCT) {
		mem.clear();
		env->population[_victimID]->mem.clear();
		state = EAT;
		env->population[_victimID]->state = STOP;
		victimID = _victimID;
	}
}

void Individ_Getero::step() {
	isLive();
	if (live) {
		if (reproduction_timer < dna.phis[reproduction_pause] && state != REPRODUCT) 
			reproduction_timer++;

		if (state == HUNGRY) {
			checkState();
			calcMove();

			IndMemory <long long int> nearVictim;
			nearVictim = whoIsNearby();
			if (!nearVictim.enemies.empty()) {
				//написать функцию отбора
				beginEating(*nearVictim.enemies.begin());
			}
		} else if (state == MATURE) {
			checkState();
			calcMove();

			if (gender == MALE) {
				IndMemory <long long int> nearInd;
				nearInd = whoIsNearby();
				if (!nearInd.partners.empty()) {
					//написать функцию поиска сильнейшей особи или осуществить какой-то отбор партнёров
					beginReproduction(*nearInd.partners.begin());
				}
			}
		} else if (state == WAIT) {
			checkState();
			calcMove();
		} else if (state == REPRODUCT) {
			reproduction();
		} else if (state == STOP) {
		} else if (state == EAT) {
			eat();
		}
	}
}




