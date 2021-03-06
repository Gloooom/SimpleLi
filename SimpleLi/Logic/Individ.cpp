#include "inclusion.h"

#pragma once

#include "Individ.h"

long long int Individ::count=1;


Individ::Individ(): way(0,0) {
	gender = NEUTER;
	ID = 0;
	live = false;
}

Individ::Individ(Vector <int> _pos, GeneticCode _dna) {
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
	spouseID = ID;
}

bool Individ::operator==(Individ i) {
	if (ID == i.ID) return true;
	else return false;
}

bool Individ::operator!=(Individ i) {
	if (ID == i.ID) return false;
	else return true;
}

//���������� ���������. ������ �����.
void Individ::move(Array <Individ*> *field) {
	Vector <double> tempPos(0, 0);
	Individ *he=this;
	while (way.getLength()!=0 &&
		(he->ID == 0 || (*he) == (*this)) &&
		(tempPos+way)<=(way*speed) &&
		func::round(pos.x+tempPos.x+way.x)>=0 && 
		func::round(pos.x+tempPos.x+way.x)<field->getW() && 
		func::round(pos.y+tempPos.y+way.y)>=0 && 
		func::round(pos.y+tempPos.y+way.y)<field->getH()) {
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

	pos += tempPos.round();
	if (pos.x >=field->getW()) pos.x = field->getW()-1;
	if (pos.x <=0) pos.x = 0;
	if (pos.y >=field->getH()) pos.y = field->getH()-1;
	if (pos.y <=0) pos.y = 0;
}

//�������� ��������� ����� ���� ������
//��������� � ������� �������� ���������� ������ � �������� ������� ���������
//��������� ����������. � ���������� ����������� ������� ������� ��������� � ��� �� �� ��������� ������� �����������
//� �� ��� ������: ��������� � ������ �� ���� ��� ���� ������.
void Individ::look(Array <Individ*> *field) {
	mem.clear();
	double wayAngle = way.getDeg();
	for (int i=0; i<dna.eyes.size(); i++) {

		std::vector <Vector <int> > eyeVectors = dna.eyes[i].getVectors(wayAngle);

		std::vector <Vector <int> > ::iterator p = eyeVectors.begin();
		while (p != eyeVectors.end()) {
			Vector <int> absP;
			absP = *p + pos;
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

void Individ::look(std::map <long long int, Individ> *population) {
	//���� ������� �����. ������-�� ����� ��������� ������������ ��������� ���������, �� ����� ����� ���� ������� �������
	mem.clear();
	double wayAngle = way.getDeg();

	std::vector <FOV_Tri> ::iterator e = dna.eyes.begin();
	while (e != dna.eyes.end()) {
		std::map <long long int, Individ> ::iterator p = population->begin();
		while (p != population->end()) {
			if (e->checkHit(p->second.pos.toDouble(), pos.toDouble(), wayAngle)) {
				Individ *he = &p->second;
				if (he->ID != 0 && *he != *this) {
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

void Individ::checkWay() {
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

void Individ::eat(long long int targetID, std::map <long long int, Individ> *population) {
	if (dna.diet == GETERO 
		&& energy+dna.phis[saturation]<=dna.phis[energy_max] 
		&& (*population)[targetID].live == true
		) {
		energy+=dna.phis[saturation];
		(*population)[targetID].hp-=dna.phis[saturation];
		(*population)[targetID].state = STOP;
	}
	if (energy+dna.phis[saturation]>dna.phis[energy_max])
		(*population)[targetID].checkState();
}

void Individ::beginReproduction(long long int _spouseID, std::map <long long int, Individ> *population) {
	if (state != REPRODUCT && (*population)[_spouseID].state != REPRODUCT) {
		mem.clear();
		(*population)[_spouseID].mem.clear();
		reproduction_timer = 0;
		(*population)[_spouseID].reproduction_timer = 0;
		state = REPRODUCT;
		(*population)[_spouseID].state = REPRODUCT;
		spouseID = _spouseID;
		(*population)[_spouseID].spouseID = ID;
	}
}

void Individ::reproduction(Array <Individ*> *field, std::deque <Individ> *cradle, std::map <long long int, Individ> *population) {
	if (population->find(spouseID) != population->end()) {
		if (reproduction_timer < dna.phis[reproduction_time]) {
			reproduction_timer++;
		} else {
			if (gender == FEMALE) {
				cradle->push_back(
					Individ(
					getNearestEmpty(field), 
					dna.hibridization((*population)[spouseID].dna, AVERAGE))
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
				absP = P + pos;
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
	Vector <int> R = target->pos - pos;
	R.x = abs(R.x);
	R.y = abs(R.y);
	if (R.x>1 || R.y>1) return false;
	else return true;
}

IndMemory <long long int> Individ::whoIsNearby(Array <Individ*> *field) {
	IndMemory <long long int> result;
	
	int delta[] = {-1, 0, 1};
	Point <int> selectCell;

	for (int x=0; x<=2; x++) 
		for (int y=0; y<=2; y++) {
			selectCell.x = pos.x+delta[x];
			selectCell.y = pos.y+delta[y];
			if (field->isValid(selectCell)) {
				Individ *he = (*field)(selectCell);
				if (he->ID != 0 && he->ID != ID && he->live
					&& he->state != REPRODUCT
					&& he->state != STOP) {
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

void Individ::step(Array <Individ*> *field, std::deque <Individ> *cradle, std::map <long long int, Individ> *population) {
	isLive();

	if (live == true) {
		if (reproduction_timer < dna.phis[reproduction_pause] && state != REPRODUCT) 
			reproduction_timer++;

		if (state != REPRODUCT && state != STOP) {
			look(field);
			checkState();
			checkWay();
			double energyCost;
			//�������. ���������.
			if (dna.soc[state][max_speed] != 0 && dna.phis[stamina] != 0) 
				energyCost = (speed/dna.soc[state][max_speed])/dna.phis[stamina];
			else
				energyCost = dna.phis[energy_max];

			if (energy-energyCost>=0) {
				energy -= energyCost;
				move(field);
			}

			if (speed + dna.phis[acceleration] <= dna.soc[state][max_speed])
				speed += dna.phis[acceleration];
			else 
				speed = dna.soc[state][max_speed];
		}

		if (dna.diet == AUTO) {
			eat();

			if (state == HUNGRY) {

			} else if (state == MATURE && gender == MALE) {
				IndMemory <long long int> nearInd;
				nearInd = whoIsNearby(field);
				if (!nearInd.partners.empty()) {
					//�������� ������� ������ ���������� ����� ��� ����������� �����-�� ����� ���������
					beginReproduction(*nearInd.partners.begin(), population);
				}
				//��� �� �� �������� �������� - ������������ ������� ��������.
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