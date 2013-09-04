#include "inclusion.h"

#pragma once

#include "Individ.h"

long long int Individ::count=0;


Individ::Individ(): way(0,0) {
	gender = NEUTER;
	ID = count;
	count++;
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

void Individ::move(Individ (*(*field)[W][H])) {
	Vector <double> tempPos(0, 0);
	Individ *he=this;
	while (way.getLength()!=0 &&
		(he->ID == 0 || (*he) == (*this)) &&
		(tempPos+way)<=(way*speed) &&
		pos.x+func::round(tempPos.x+way.x)>=0 && 
		pos.x+func::round(tempPos.x+way.x)<W && 
		pos.y+func::round(tempPos.y+way.y)>=0 && 
		pos.y+func::round(tempPos.y+way.y)<H) {
			he = (*field)[func::round(tempPos.x + way.x + (double) pos.x)][func::round(tempPos.y + way.y + (double) pos.y)];
			if (he->ID == 0 || (*he) == (*this)) {
				tempPos+=way;
				collision=false;
			} else { collision=true; break; }
	}

	if (tempPos.getLength()==0) {way.x*=(-1); way.y*=(-1);}

	pos.x+=func::round(tempPos.x);
	pos.y+=func::round(tempPos.y);
	if (pos.x >=W) pos.x = W-1;
	if (pos.x <=0) pos.x = 0;
	if (pos.y >=H) pos.y = H-1;
	if (pos.y <=0) pos.y = 0;
}

//�������� ��������� ����� ���� ������
//��������� � ������� �������� ���������� ������ � �������� ������� ���������
//��������� ����������. � ���������� ����������� ������� ������� ��������� � ��� �� �� ��������� ������� �����������
//� �� ��� ������: ��������� � ������ �� ���� ��� ���� ������.
void Individ::look(Individ (*(*field)[W][H])) {
	mem.clear();

	if (dna.radialEye.height() > 0) {
		double R = dna.radialEye.height();
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
					if (he->ID != 0 && *he != *this) {
						if (he->dna.diet != dna.diet && !func::isIn(mem.enemies, he)) 
							mem.enemies.push_back(he);
						if (he->dna.diet == dna.diet && !func::isIn(mem.partners, he))
							mem.partners.push_back(he);
					}
				}
			}
		}
	}

	for (int i=0; i<dna.eyes.size(); i++) {
		if (dna.eyes[i].height() > 0 && dna.eyes[i].width() > 0) {
			double wayAngle = way.getDeg();
			double k1=tan(dna.eyes[i].angle() + atan(dna.eyes[i].height()/(dna.eyes[i].width()/2)) - wayAngle), b1=0;
			double k2=tan(dna.eyes[i].angle() - atan(dna.eyes[i].height()/(dna.eyes[i].width()/2)) - wayAngle), b2=0;
			double k3=tan(dna.eyes[i].angle() - wayAngle),    b3=(dna.eyes[i].height()/cos(dna.eyes[i].angle() - wayAngle));

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
						if (he->ID != 0 && *he != *this) {
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

void Individ::reproduction(Individ (*(*field)[W][H]), std::deque <Individ> *cradle, std::map <long long int, Individ> *population) {
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


Point <int> Individ::getNearestEmpty(Individ (*(*field)[W][H])) {
	double R = 10;
	std::vector <Vector <int> > emptyNear;
	Vector <int> P;

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

IndMemory <long long int> Individ::whoIsNearby(Individ (*(*field)[W][H])) {
	IndMemory <long long int> result;
	
	int delta[] = {-1, 0, 1};

	for (int x=0; x<=2; x++) 
		for (int y=0; y<=2; y++) {
			if (pos.x+delta[x] >=0 && pos.y+delta[y] >=0 
				&& pos.x+delta[x] <W && pos.y+delta[y] <H) {
				Individ *he = (*field)[pos.x+delta[x]][pos.y+delta[y]];
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


//��������� �������������� ������ �� ������� (���������, �����������, �����, �����)
//����������� � ���������� ����� ������������� ������������
void Individ::step(Individ (*(*field)[W][H]), std::deque <Individ> *cradle, std::map <long long int, Individ> *population) {
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
					if (!mem.empty()) {
						IndMemory <long long int> nearInd;
						nearInd = whoIsNearby(field);

						if (!nearInd.partners.empty()) {
							//�������� ������� ������ ���������� ����� ��� ����������� �����-�� ����� ��������
							beginReproduction(*nearInd.partners.begin(), population);
						} 
					}
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