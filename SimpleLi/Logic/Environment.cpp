#include "Environment.h"

#pragma once


Environment::Environment(int width, int height):
	field(width, height) {
		_width = width;
		_height = height;
		empty.dna.color = 0xFFFFFFFF;
		mutation_maxDelta = 0;
		mutation_mutGenCount = 1;
		mutation_eyeAddChance = 0;
		mutation_eyeMutationChance = 0;
		mutation_radEyeMutationChance = 0;

		stepCount = 0;
		for (int i=0; i<field.getW(); i++)
			for (int j=0; j<field.getH(); j++)
				field(i,j ) = &empty;

		//Реализация генетических ограничений//
	};

void saveVal(std::ofstream *out, UnionVal *val) {
	for (int i = 0; i<8; i++) {
		*out << val->by[i];
	}
	for (int i = 0; i<8; i++) {
		val->by[i] = 0x00;
	}
}

void loadVal(std::ifstream *in, UnionVal *val) {
	for (int i = 0; i<8; i++) {
		val->by[i] = 0x00;
	}
	for (int i = 0; i<8; i++) {
		val->by[i] = in->get();
	}
}

void Environment::save(std::string path) {
	std::ofstream outf(path, std::ios::binary);
	outf.clear();
	
	UnionVal val;

	val.li = stepCount;
	saveVal(&outf, &val);

	val.i = population.size();
	saveVal(&outf, &val);

	std::map <long long int, Individ_Proto*> ::iterator p = population.begin();
	while (p != population.end()) {
		//dna
		val.dw = p->second->dna.color;
		saveVal(&outf, &val);
		val.i = p->second->dna.diet;
		saveVal(&outf, &val);
		val.i = p->second->dna.eyes.size();
		saveVal(&outf, &val);
		for (int i = 0; i<p->second->dna.eyes.size(); i++) {
			val.f = p->second->dna.eyes[i].getAngle();
			saveVal(&outf, &val);
			val.f = p->second->dna.eyes[i].getHeight();
			saveVal(&outf, &val);
			val.f = p->second->dna.eyes[i].getWidth();
			saveVal(&outf, &val);
		}
		saveVal(&outf, &val);
		for (int i = 0; i<end_of_phis; i++) {
			val.f = p->second->dna.phis[i];
			saveVal(&outf, &val);
		}
		for (int i=0; i<end_of_status; i++)
			for (int j=0; j<end_of_soc; j++) {
				val.f = p->second->dna.soc[i][j];
				saveVal(&outf, &val);
			}
		//individ
		val.f = p->second->energy;
		saveVal(&outf, &val);
		val.f = p->second->hp;
		saveVal(&outf, &val);
		val.f = p->second->way.x;
		saveVal(&outf, &val);
		val.f = p->second->way.y;
		saveVal(&outf, &val);
		val.i = p->second->pos.x;
		saveVal(&outf, &val);
		val.i = p->second->pos.y;
		saveVal(&outf, &val);
		val.i = p->second->gender;
		saveVal(&outf, &val);
		val.li = p->second->live_timer;
		saveVal(&outf, &val);
		val.i = p->second->live;
		saveVal(&outf, &val);
		val.i = p->second->state;
		saveVal(&outf, &val);
		val.li = p->second->ID;
		saveVal(&outf, &val);
		val.d = p->second->speed;
		saveVal(&outf, &val);
		val.li = p->second->reproduction_timer;
		saveVal(&outf, &val);
		val.i = p->second->spouseID;
		saveVal(&outf, &val);
		p++;
	}
	
	outf.close();
}

void Environment::load(std::string path) {
	clear();

	std::ifstream inf(path, std::ios::binary);
	UnionVal val;
	if(inf.is_open()) {

	loadVal(&inf, &val);
	stepCount = val.li;

	loadVal(&inf, &val);
	for (int p = val.i; p>0; p--) {
		loadVal(&inf, &val);
		Individ_Proto *ind = CreateIndivid(val.i);
		loadVal(&inf, &val);
		ind->dna.color = val.dw;
		loadVal(&inf, &val);
		for (int i = val.i; i>0; i--) {
			FOV_Tri tempEye;
			loadVal(&inf, &val);
			tempEye.setAngle(val.f);
			loadVal(&inf, &val);
			tempEye.setHeight(val.f);
			loadVal(&inf, &val);
			tempEye.setWidth(val.f);
			tempEye.calculatPolygon();
			ind->dna.eyes.push_back(tempEye);
		}
		loadVal(&inf, &val);
		for (int i = 0; i<end_of_phis; i++) {
			loadVal(&inf, &val);
			ind->dna.phis[i] = val.f;
		}
		for (int i=0; i<end_of_status; i++)
			for (int j=0; j<end_of_soc; j++) {
				loadVal(&inf, &val);
				ind->dna.soc[i][j] = val.f;
			}
		//individ
		loadVal(&inf, &val);
		ind->energy = val.f;
		loadVal(&inf, &val);
		ind->hp = val.f;
		loadVal(&inf, &val);
		ind->way.x = val.f;
		loadVal(&inf, &val);
		ind->way.y = val.f;
		loadVal(&inf, &val);
		ind->pos.x = val.i;
		loadVal(&inf, &val);
		ind->pos.y  = val.i;
		loadVal(&inf, &val);
		ind->gender = val.i;
		loadVal(&inf, &val);
		ind->live_timer = val.li;
		loadVal(&inf, &val);
		ind->live = val.i;
		loadVal(&inf, &val);
		ind->state = val.i;
		loadVal(&inf, &val);
		ind->ID = val.li;
		loadVal(&inf, &val);
		ind->speed = val.d;
		loadVal(&inf, &val);
		ind->reproduction_timer = val.li;
		loadVal(&inf, &val);
		ind->spouseID = val.i;

		population[ind->ID] = ind;
		inf.clear();
	}
	}
	inf.close();
}

void Environment::fill() {
	field.fill(&empty);

	std::map <long long int, Individ_Proto*> ::iterator p = population.begin();
	while (p != population.end()) {
		field(p->second->pos.x, p->second->pos.y) = p->second;
		p++;
	}
}

void Environment::checkDead() {
	std::map <long long int, Individ_Proto*> ::iterator p = population.begin();
	while (p != population.end()) {
		if (p->second->live == false) {
			delete p->second;
			p = population.erase(p);
		} else 
			p++;
	}
}

void Environment::born() {
	std::deque <Individ_Proto*> ::iterator c = cradle.begin();
	while (c != cradle.end()) {
		(*c)->dna = (*c)->dna.mutation(mutation_maxDelta, mutation_mutGenCount, mutation_eyeAddChance, 
			mutation_eyeMutationChance, mutation_radEyeMutationChance);
		population[(*c)->ID] = *c;
		c++;
	}
	cradle.clear();
}

void Environment::addIndivid(Individ_Proto *ind) {
	ind->env = this;
	cradle.push_back(ind);
}

void Environment::step() {
	if (stepCount % 100 == 0 && stepCount != 0)
		save("temp.bin");
	stepCount++;
	checkDead();
	born();
	
	fill();

	std::map <long long int, Individ_Proto*> ::iterator p = population.begin();
	while (p != population.end()) {
		p->second->step();
		p++;
	}
	
	fill();
}

void Environment::setMutation(float maxDelta, int mutGenCount, float eyeAddChance, float eyeMutationChance, 
								  float radEyeMutationChance) {
	mutation_maxDelta = maxDelta;
	mutation_eyeAddChance = eyeAddChance;
	mutation_eyeMutationChance = eyeMutationChance;
	mutation_radEyeMutationChance = radEyeMutationChance;
	mutation_mutGenCount = mutGenCount;
}

void Environment::clear() {
	stepCount = 0;
	std::map <long long int, Individ_Proto*> ::iterator p = population.begin();
	while (p != population.end()) {
		delete p->second;
		p++;
	}

	population.clear();
	cradle.clear();
}