#include "..\stdafx.h"

#pragma once

#include "..\Logic\Individ.h"
#include "..\Logic\Functions.h"
#include "..\Logic\Types.h"


long long int Individ::count=0;


Individ::Individ(): way(0,0) {
	gender == NEUTER;
	ID=count;
	count++;
}

Individ::Individ(Point <int> _pos, GeneticCode _dna): way(0,0) {
	ID=count;
	count++;

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
	spouse = this;
}

bool Individ::operator==(Individ i) {
	if (ID == i.ID) return true;
	else return false;
}

bool Individ::operator!=(Individ i) {
	if (ID == i.ID) return false;
	else return true;
}

void Individ::move(Individ (*(*field)[W][H]), Individ *empty) {
	Vector <double> tempPos;
	Individ *he=this;
	while (way.getLength()!=0 &&
		((*he) == *empty || (*he) == (*this)) &&
		(tempPos+way)<=(way*speed) &&
		pos.x+func::round(tempPos.x+way.x)>0 && 
		pos.x+func::round(tempPos.x+way.x)<W && 
		pos.y+func::round(tempPos.y+way.y)>0 && 
		pos.y+func::round(tempPos.y+way.y)<H) {
			he = (*field)[func::round(tempPos.x + way.x + (double) pos.x)][func::round(tempPos.y + way.y + (double) pos.y)];
			if (*he == *empty || (*he) == (*this)) {
				tempPos+=way;
				collision=false;
			} else { collision=true; break; }
	}

	if (tempPos.getLength()==0) {way.x*=(-1); way.y*=(-1);}

	pos.x+=func::round(tempPos.x);
	pos.y+=func::round(tempPos.y);
}

//�������� ��������� ����� ���� ������
//��������� � ������� �������� ���������� ������ � �������� ������� ���������
void Individ::look(Individ (*(*field)[W][H]), Individ *empty) {
	mem.clear();

	for (int i=0; i<dna.eyes.size(); i++) {
		if (dna.eyes[i].type==TRIANGLE) {
			double wayAngle = way.getDeg();
			double k1=tan(dna.eyes[i].angle + atan(dna.eyes[i].height/(dna.eyes[i].width/2)) - wayAngle), b1=0;
			double k2=tan(dna.eyes[i].angle - atan(dna.eyes[i].height/(dna.eyes[i].width/2)) - wayAngle), b2=0;
			double k3=tan(dna.eyes[i].angle - wayAngle),    b3=(dna.eyes[i].height/cos(dna.eyes[i].angle - wayAngle));

			Point <double> vert[3];
			vert[0]=func::crossLine(k1,b1,k2,b2);
			vert[1]=func::crossLine(k2,b2,k3,b3);
			vert[2]=func::crossLine(k1,b1,k3,b3);

			Vector <double> vectorR(vert[2].x, vert[2].y);
			double R = vectorR.getLength();

			Vector <double> P;
			int start_x = (pos.x-R>0) ? pos.x-R : 0;
			int start_y = (pos.y-R>0) ? pos.y-R : 0;

			for (int _x=start_x; _x<pos.x+R, _x<W; _x++) { 
				P.x=_x-pos.x;
				for (int _y=start_y; _y<pos.y+R, _y<H; _y++) {
					P.y=_y-pos.y;
					double pl1, pl2, pl3;
					pl1 = (vert[0].x - P.y)*(vert[1].y - vert[0].y)-(vert[1].x - vert[0].x)*(vert[0].y - P.x);
					pl2 = (vert[1].x - P.y)*(vert[2].y - vert[1].y)-(vert[2].x - vert[1].x)*(vert[1].y - P.x);
					pl3 = (vert[2].x - P.y)*(vert[0].y - vert[2].y)-(vert[0].x - vert[2].x)*(vert[2].y - P.x);
					if ((pl1 >= 0 && pl2 >= 0 && pl3 >= 0) || (pl1 <= 0 && pl2 <= 0 && pl3 <= 0)) {
						Point <int> absP;
						absP.x=func::round(P.x+pos.x);
						absP.y=func::round(P.y+pos.y);

						Individ *he = (*field)[absP.x][absP.y];
						if (*he != *empty && *he != *this) {
							if (he->dna.diet != dna.diet && !func::isIn(mem.enemies, he)) 
								mem.enemies.push_back(he);
							if (he->dna.diet == dna.diet && !func::isIn(mem.partners, he))
								mem.partners.push_back(he);
						}
					}
				}
			}
		} else if (dna.eyes[i].type==RADIUS) {
			double R = dna.eyes[i].height;
			Vector <double> P;
			int start_x = (pos.x-R>0) ? pos.x-R : 0;
			int start_y = (pos.y-R>0) ? pos.y-R : 0;
			for (int _x=start_x; _x<pos.x+R, _x<W; _x++) { 
				P.x=_x-pos.x;
				for (int _y=start_y; _y<pos.y+R, _y<H; _y++) {
					P.y=_y-pos.y;
					if (P.y*P.y+P.x*P.x<R*R) {
						Point <int> absP;
						absP.x=func::round(P.x+pos.x);
						absP.y=func::round(P.y+pos.y);
						Individ *he = (*field)[absP.x][absP.y];
						if (*he != *empty && *he != *this) {
							if (he->dna.diet != dna.diet && !func::isIn(mem.enemies, he)) 
								mem.enemies.push_back(he);
							if (he->dna.diet == dna.diet && !func::isIn(mem.partners, he))
								mem.partners.push_back(he);
						}
					}
				}
			}
		} else if (dna.eyes[i].type==SECTOR) {
			double R = dna.eyes[i].height;
			Vector <double> P;
			int start_x = (pos.x-R>0) ? pos.x-R : 0;
			int start_y = (pos.y-R>0) ? pos.y-R : 0;
			for (int _x=start_x; _x<pos.x+R, _x<W; _x++) { 
				P.x=_x-pos.x;
				for (int _y=start_y; _y<pos.y+R, _y<H; _y++) {
					P.y=_y-pos.y;
					if (P.y*P.y+P.x*P.x<R*R) {
						Point <int> absP;
						absP.x=func::round(P.x+pos.x);
						absP.y=func::round(P.y+pos.y);
						Individ *he = (*field)[absP.x][absP.y];
						if (*he != *empty && *he != *this) {
							if (he->dna.diet != dna.diet && !func::isIn(mem.enemies, he)) 
								mem.enemies.push_back(he);
							if (he->dna.diet == dna.diet && !func::isIn(mem.partners, he))
								mem.partners.push_back(he);
						}
					}
				}
			}
		}
	}
}


void Individ::checkWay() {
	Vector <double> cohesionEnemy, cohesionPartner, 
		separationEnemy, separationPartner,
		alignmentEnemy, alignmentPartner,
		P0;

	Vector <double> wayToEnemy;
	for(int i=0; i<mem.enemies.size(); i++)	{
		wayToEnemy.x = mem.enemies[i]->pos.x - pos.x;
		wayToEnemy.y = mem.enemies[i]->pos.y - pos.y;
		cohesionEnemy += wayToEnemy.getNorm();
		separationEnemy += wayToEnemy.getNorm()/((wayToEnemy.getLength()!=0) ? wayToEnemy.getLength() : 1);
		alignmentEnemy += mem.enemies[i]->way;
	}

	Vector <double> wayToPartner;
	for(int i=0; i<mem.partners.size(); i++) {
		wayToPartner.x = mem.partners[i]->pos.x - pos.x;
		wayToPartner.y = mem.partners[i]->pos.y - pos.y;
		cohesionPartner += wayToPartner.getNorm();
		separationPartner += wayToPartner.getNorm()/((wayToPartner.getLength()!=0) ? wayToPartner.getLength() : 1);
		alignmentPartner += mem.partners[i]->way;
	}

	if (!mem.enemies.empty() || !mem.partners.empty()) {
		Vector <double> finalWay;
		Vector <double>	wayPartners = cohesionPartner.getNorm()*dna.soc[state][cohesion_partner] + 
			separationPartner.getNorm()*dna.soc[state][separation_partner] + 
			alignmentPartner*dna.soc[state][alignment_partner];
		Vector <double> wayEnemys  = cohesionPartner.getNorm()*dna.soc[state][cohesion_enemy] + 
			separationPartner.getNorm()*dna.soc[state][separation_enemy] + 
			alignmentPartner*dna.soc[state][alignment_enemy];

		if (mem.partners.empty()) wayPartners = P0;
		if (mem.enemies.empty()) wayEnemys = P0;

		finalWay = wayPartners*dna.soc[state][partner] + wayEnemys*dna.soc[state][enemy];
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
	else if (energy == 0) 
		state = WAIT;
}


void Individ::eat() {
	if (dna.diet==AUTO && energy < dna.phis[energy_max]) {
		energy+=dna.phis[saturation];
	} else if (energy > dna.phis[energy_max]) 
		energy = dna.phis[energy_max];
}

void Individ::eat(Individ *target) {
	if (dna.diet == GETERO && target->dna.diet == AUTO && energy+dna.phis[saturation]<=dna.phis[energy_max] && target->live == true) {
		energy+=dna.phis[saturation];
		target->hp-=dna.phis[saturation];
		target->state = WAIT;
	}
	if (energy+dna.phis[saturation]>dna.phis[energy_max])
		target->checkState();
}

void Individ::beginReproduction() {
	if (state != REPRODUCT) {
		reproduction_timer = 0;
		state = REPRODUCT;
	}
}

void Individ::reproduction(Individ *target, std::vector <Individ> *cradle) {
	if (reproduction_timer < dna.phis[reproduction_time]) {
		reproduction_timer++;
	} else {
		//Individ child(pos, dna.hibridization(target->dna, AVERAGE, SOC));
		/*	if (gender == MALE) {
		target->reproduction(this, population);
		} else */
		if (gender == FEMALE) {
			Individ child(pos, dna.hibridization(target->dna, AVERAGE));
			cradle->push_back(child);
		}

		energy -= dna.phis[reproduction_cost];
		reproduction_timer = 0;
		state = WAIT;
	}
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

IndMemory <Individ*> Individ::whoIsNearby(Individ (*(*field)[W][H]), Individ *empty) {
	IndMemory <Individ*> result;
	int delta[] = {-1, 0, 1};

	for (int x=0; x<=2; x++) 
		for (int y=0; y<=2; y++) {
			if (pos.x+delta[x] >=0 && pos.y+delta[y] >=0 && pos.x+delta[x] <W && pos.y+delta[y] <H) {
				Individ *he = (*field)[pos.x+delta[x]][pos.y+delta[y]];
				if (*he != *empty && *he != *this && he->state != REPRODUCT && he->state !=EAT && he->live)
					if (he->dna.diet == dna.diet && he->gender != gender)
						result.partners.push_back(he);
					else 
						result.enemies.push_back(he);
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
void Individ::step(Individ (*(*field)[W][H]), Individ *empty, std::vector <Individ> *cradle) {
	isLive();

	if (live == true) {
		if (reproduction_timer < dna.phis[reproduction_pause] && state != REPRODUCT) 
			reproduction_timer++;

		if (state != EAT && state != REPRODUCT) {
			look(field, empty);
			checkState();
			checkWay();
			double energyCost = dna.phis[stamina]*(speed/dna.soc[state][max_speed]);
			if (energy-energyCost>=0) {
				energy -= energyCost;
				move(field, empty);
			}
		}

		if (speed+dna.phis[acceleration] <= dna.soc[state][max_speed])
			speed += dna.phis[acceleration];
		else 
			speed = dna.soc[state][max_speed];



		if (dna.diet == AUTO) {
			eat();

			if (state == HUNGRY) {
			} else if (state == MATURE) {
				if (gender == MALE) {
					
					IndMemory <Individ*> nearInd;
					nearInd = whoIsNearby(field, empty);
					
					if (!nearInd.partners.empty()) {
						std::sort(nearInd.partners.begin(), nearInd.partners.end(), isLess);
						spouse = *nearInd.partners.begin();
						spouse->spouse = this;
						beginReproduction();
						spouse->beginReproduction();
					}
				}
				//���� �������� ������� ������ ��������� ��� ����������
				//��� �� �� �������� �������� - ������������ ������� ��������.
				//� ������� �������� �������� ��������� ��������� WAIT, REPRODUCTION, EAT

			} else if (state == REPRODUCT) {
				reproduction(spouse, cradle);
			} else if (state == EAT) {

			} else if (state == WAIT) {

			}
		}

		if (dna.diet == GETERO) {
			if (state == HUNGRY) {
				IndMemory <Individ*> nearInd;
				nearInd = whoIsNearby(field, empty);

				if (!nearInd.enemies.empty()) {
					std::sort(nearInd.enemies.begin(), nearInd.enemies.end(), isLess);
					eat(*nearInd.enemies.end());
				}
			} else if (state == MATURE) {

			} else if (state == REPRODUCT) {

			} else if (state == EAT) {

			} else if (state == WAIT) {

			}
		}
	}
}