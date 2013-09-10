#include "inclusion.h"

#pragma once

#include "Individ.h"

long long int Individ::count=1;


Individ::Individ(): way(0,0) {
	gender = NEUTER;
	ID = 0;
}

Individ::Individ(Point <int> _pos, GeneticCode _dna) {
	ID=count;
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
	spouse = ID;
}

bool Individ::operator==(Individ i) {
	if (ID == i.ID) return true;
	else return false;
}

bool Individ::operator!=(Individ i) {
	if (ID == i.ID) return false;
	else return true;
}

void Individ::move(Array <Individ*> *field) {
	Vector <double> tempPos(0, 0);
	Individ *he=this;
	while (way.getLength()!=0 &&
		(he->ID == 0 || (*he) == (*this)) &&
		(tempPos+way)<=(way*speed) &&
		pos.x+func::round(tempPos.x+way.x)>=0 && 
		pos.x+func::round(tempPos.x+way.x)<field->getW() && 
		pos.y+func::round(tempPos.y+way.y)>=0 && 
		pos.y+func::round(tempPos.y+way.y)<field->getH()) {
			he = (*field)(
				func::round(tempPos.x + way.x + (double) pos.x), 
				func::round(tempPos.y + way.y + (double) pos.y)
				);
			if (he->ID == 0 || (*he) == (*this)) {
				tempPos+=way;
				collision=false;
			} else { break; }
	}

	if (tempPos.getLength()==0) { way.x*=(-1); way.y*=(-1); }

	pos.x+=func::round(tempPos.x);
	pos.y+=func::round(tempPos.y);
	if (pos.x >=field->getW()) pos.x = field->getW()-1;
	if (pos.x <=0) pos.x = 0;
	if (pos.y >=field->getH()) pos.y = field->getH()-1;
	if (pos.y <=0) pos.y = 0;
}

//дописать обработку глаза типа сектор
//вычислять с помощью перевода координаты ячейки в полярную систему координат
//Полностью переписать. В промежутке формировать матрицу области видимости и уже из неё вычислять видимых индивидумов
//А не как сейчас: добавлять в массив по пять раз один объект.
void Individ::look(Array <Individ*> *field) {
	mem.clear();
	for (int i=0; i<dna.eyes.size(); i++) {
		double wayAngle = way.getDeg();

		std::vector <Vector <int> > eyeVectors = dna.eyes[i].getVectors(wayAngle);

		std::vector <Vector <int> > ::iterator p = eyeVectors.begin();
		while (p != eyeVectors.end()) {
			Vector <int> absP;
			absP.x = p->x + pos.x;
			absP.y = p->y + pos.y;
			if (absP.x >= 0 && absP.x < field->getW() && absP.y >= 0 && absP.y < field->getH()) {
				Individ *he = (*field)(absP.x, absP.y);
				if (he->ID != 0 && *he != *this) {
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

void Individ::checkWay() {
	Vector <double> enemyCohesion, partnerCohesion, 
		enemySeparation, partnerSeparation,
		enemyAlignment, partnerAlignment,
		spouseAttraction,
		P0;

	Vector <double> enemiesImpact;
	for(int i=0; i<mem.enemies.size(); i++)	{
		enemiesImpact.x = mem.enemies[i]->pos.x - pos.x;
		enemiesImpact.y = mem.enemies[i]->pos.y - pos.y;
		enemyCohesion += enemiesImpact.getNorm();
		enemySeparation += enemiesImpact.getNorm()/((enemiesImpact.getLength()!=0) ? enemiesImpact.getLength() : 1);
		enemyAlignment += mem.enemies[i]->way;
	}

	Vector <double> partnersImpact;
	for(int i=0; i<mem.partners.size(); i++) {
		partnersImpact.x = mem.partners[i]->pos.x - pos.x;
		partnersImpact.y = mem.partners[i]->pos.y - pos.y;
		partnerCohesion += partnersImpact.getNorm();
		partnerSeparation += partnersImpact.getNorm()/((partnersImpact.getLength()!=0) ? partnersImpact.getLength() : 1);
		partnerAlignment += mem.partners[i]->way;
	}

	Vector <double> libidoImpact;
	for(int i=0; i<mem.partners.size(); i++) {
		if (mem.partners[i]->gender != gender) {
			libidoImpact.x = mem.partners[i]->pos.x - pos.x;
			libidoImpact.y = mem.partners[i]->pos.y - pos.y;
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

void Individ::checkState() {
	if (reproduction_timer == dna.phis[reproduction_pause] && energy >= dna.phis[reproduction_cost]) 
		state = MATURE;
	else if (energy < dna.phis[energy_hungry]) 
		state = HUNGRY;
	else  
		state = WAIT;
}

void Individ::eat() {
	if (dna.diet==AUTO && energy < dna.phis[energy_max]) {
		energy+=dna.phis[saturation];
	} else if (energy > dna.phis[energy_max]) 
		energy = dna.phis[energy_max];
}

void Individ::eat(Individ *target) {
	if (dna.diet == GETERO && target->dna.diet == AUTO 
		&& energy+dna.phis[saturation]<=dna.phis[energy_max] 
		&& target->live == true
		) {
		energy+=dna.phis[saturation];
		target->hp-=dna.phis[saturation];
		target->state = WAIT;
	}
	if (energy+dna.phis[saturation]>dna.phis[energy_max])
		target->checkState();
}

void Individ::beginReproduction(long long int _spouse, std::map <long long int, Individ> *population) {
	if (state != REPRODUCT && (*population)[_spouse].state != REPRODUCT) {
		reproduction_timer = 0;
		(*population)[_spouse].reproduction_timer = 0;
		state = REPRODUCT;
		(*population)[_spouse].state = REPRODUCT;
		spouse = _spouse;
		(*population)[_spouse].spouse = ID;
	}
}

void Individ::reproduction(Array <Individ*> *field, std::deque <Individ> *cradle, std::map <long long int, Individ> *population) {
	if (population->find(spouse) != population->end()) {
		if (reproduction_timer < dna.phis[reproduction_time]) {
			reproduction_timer++;
		} else {
			if (gender == FEMALE) {
				cradle->push_back(
					Individ(
					getNearestEmpty(field), 
					dna.hibridization((*population)[spouse].dna, AVERAGE))
					);
			}
			energy -= dna.phis[reproduction_cost];
			reproduction_timer = 0;
			state = WAIT;
		}
	} else {
		reproduction_timer = 0;
		state = WAIT;
	}
}


Point <int> Individ::getNearestEmpty(Array <Individ*> *field) {
	double R = 10;
	std::vector <Vector <int> > emptyNear;
	Vector <int> P;

	int start_x = (pos.x-R>0) ? pos.x-R : 0;
	int start_y = (pos.y-R>0) ? pos.y-R : 0;
	for (int _x=start_x; _x<pos.x+R, _x<field->getW(); _x++) { 
		P.x=_x-pos.x;
		for (int _y=start_y; _y<pos.y+R, _y<field->getH(); _y++) {
			P.y=_y-pos.y;
			if (P.y*P.y+P.x*P.x<R*R) {
				Point <int> absP;
				absP.x=func::round(P.x+pos.x);
				absP.y=func::round(P.y+pos.y);
				Individ *he = (*field)(absP.x, absP.y);
				if (he->ID == 0) 
					emptyNear.push_back(P);
			}
		}
	}
	std::sort(emptyNear.begin(), emptyNear.end());

	Point <int> nearestEmptyPoint((*emptyNear.begin()) + pos);
	return nearestEmptyPoint;
}

void Individ::heal() {
	speed = 0;
	energy--;
	hp++;
}

bool Individ::isNearby(Individ* target) {
	Vector <int> R(target->pos.x-pos.x, target->pos.y-pos.y);
	R.x = abs(R.x);
	R.y = abs(R.y);
	if (R.x>1 || R.y>1) return false;
	else return true;
}

IndMemory <long long int> Individ::whoIsNearby(Array <Individ*> *field) {
	IndMemory <long long int> result;
	
	int delta[] = {-1, 0, 1};

	for (int x=0; x<=2; x++) 
		for (int y=0; y<=2; y++) {
			if (pos.x+delta[x] >=0 && pos.y+delta[y] >=0 
				&& pos.x+delta[x] <field->getW() && pos.y+delta[y] <field->getH()) {
				Individ *he = (*field)(pos.x+delta[x], pos.y+delta[y]);
				if (he->ID != 0 && he->ID != ID && he->state != REPRODUCT && he->live) {
					if (he->dna.diet == dna.diet && he->gender != gender && he->state == MATURE)
						result.partners.push_back(he->ID);
					if (he->dna.diet != dna.diet)
						result.enemies.push_back(he->ID);
				}
			}
		}

	return result;
}

void Individ::isLive() {
	if (live_timer < dna.phis[live_time]) live_timer++;
	else live = false;
	if (hp==0) live = false;
}

bool isLess(Individ *first, Individ *second) {
	if (first->energy < second->energy) return true;
	else return false;
}


//раскидать индивидуальные методы по классам (автотрофы, гетеротрофы, самка, самец)
//реализовать в дальнейшем через множественное наследование
void Individ::step(Array <Individ*> *field, std::deque <Individ> *cradle, std::map <long long int, Individ> *population) {
	isLive();

	if (live == true) {
		if (reproduction_timer < dna.phis[reproduction_pause] && state != REPRODUCT) 
			reproduction_timer++;

		if (state != REPRODUCT) {
			look(field);
			checkState();
			checkWay();
			double energyCost = dna.phis[stamina]*(speed/dna.soc[state][max_speed]);
			if (energy-energyCost>=0) {
				energy -= energyCost;
				move(field);
			}
			if (speed+dna.phis[acceleration] <= dna.soc[state][max_speed])
				speed += dna.phis[acceleration];
			else 
				speed = dna.soc[state][max_speed];
		}

		if (dna.diet == AUTO) {
			eat();

			if (state == HUNGRY) {
			} else if (state == MATURE) {
				if (gender == MALE) {
					if (!mem.empty()) {
						IndMemory <long long int> nearInd;
						nearInd = whoIsNearby(field);

						if (!nearInd.partners.empty()) {
							//написать функцию поиска сильнейшей особи или осуществить какой-то отбор партнёров
							beginReproduction(*nearInd.partners.begin(), population);
						} 
					}
				}
				//что бы всё нормаьно работало - перелапатить функцию движения.
			} else if (state == REPRODUCT) {
				reproduction(field, cradle, population);
			} else if (state == WAIT) {

			}
		}

		if (dna.diet == GETERO) {
			if (state == HUNGRY) {
				//IndMemory <long long int> nearInd;
				//nearInd = whoIsNearby(field);
				//if (!nearInd.enemies.empty()) {
					//std::sort(nearInd.enemies.begin(), nearInd.enemies.end(), isLess);
					//eat(nearInd.enemies.end());
				//}
			} else if (state == MATURE) {

			} else if (state == REPRODUCT) {

			} else if (state == WAIT) {

			}
		}
	}
}