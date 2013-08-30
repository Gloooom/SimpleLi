#include "..\stdafx.h"

#include "..\Logic\Environment.h"

#pragma once

void saveVal(std::ofstream *out, UnionVal val) {
	for (int i = 0; i<8; i++) {
		*out << val.by[i];
	}
}

void loadVal(std::ifstream *in, UnionVal *val) {
	for (int i = 0; i<8; i++) {
		*in >> val->by[i];
	}
}

void Environment::save(std::string path) {
	std::ofstream outf(path, std::ios::binary);
	UnionVal val;
	std::map <long long int, Individ> ::iterator p = population.begin();
	while (p != population.end()) {
		//dna
		val.dw = p->second.dna.color;
		saveVal(&outf, val);
		val.i = p->second.dna.diet;
		saveVal(&outf, val);
		val.i = p->second.dna.eyes.size();
		saveVal(&outf, val);
		for (int i = 0; i<p->second.dna.eyes.size(); i++) {
			val.f = p->second.dna.eyes[i].angle();
			saveVal(&outf, val);
			val.f = p->second.dna.eyes[i].height();
			saveVal(&outf, val);
			val.f = p->second.dna.eyes[i].width();
			saveVal(&outf, val);
		}
		val.f = p->second.dna.radialEye.height();
			saveVal(&outf, val);
		for (int i = 0; i<end_of_phis; i++) {
			val.f = p->second.dna.phis[i];
			saveVal(&outf, val);
		}
		for (int i=0; i<end_of_status; i++)
			for (int j=0; j<end_of_soc; j++)
				val.f = p->second.dna.soc[i][j];
		//individ
		val.f = p->second.energy;
		saveVal(&outf, val);
		val.f = p->second.hp;
		saveVal(&outf, val);
		val.f = p->second.way.x;
		saveVal(&outf, val);
		val.f = p->second.way.y;
		saveVal(&outf, val);
		val.i = p->second.pos.x;
		saveVal(&outf, val);
		val.i = p->second.pos.y;
		saveVal(&outf, val);
		val.i = p->second.gender;
		saveVal(&outf, val);
		val.li = p->second.live_timer;
		saveVal(&outf, val);
		val.i = p->second.live;
		saveVal(&outf, val);
		val.i = p->second.state;
		saveVal(&outf, val);
		val.li = p->second.ID;
		saveVal(&outf, val);
		val.d = p->second.speed;
		saveVal(&outf, val);
		val.li = p->second.reproduction_timer;
		saveVal(&outf, val);
		val.i = p->second.spouse;
		saveVal(&outf, val);
		p++;
	}
	
	outf.close();
}

void Environment::load(std::string path) {
	std::ifstream inf(path, std::ios::binary);
	UnionVal val;
	
}


void Environment::fill() {
	for (int i=0; i<W; i++)
			for (int j=0; j<H; j++)
				field[i][j]=&empty;

	std::map <long long int, Individ> ::iterator p = population.begin();
	while (p != population.end()) {
		field[p->second.pos.x][p->second.pos.y]=&(p->second);
		p++;
	}
}

void Environment::checkDead() {
	std::map <long long int, Individ> ::iterator p = population.begin();
	while (p != population.end()) {
		if (p->second.live == false) 
			p = population.erase(p);
		else 
			p++;
	}
}

void Environment::born() {
	std::deque <Individ> ::iterator c = cradle.begin();
	while (c != cradle.end()) {
		population[c->ID] = *c;
		c++;
	}
	cradle.clear();
}

void Environment::addIndivid(Individ ind) {
	cradle.push_back(ind);
}

void Environment::step() {
	stepCount++;
	checkDead();
	born();
	fill();
	std::map <long long int, Individ> ::iterator p = population.begin();
	while (p != population.end()) {
		p->second.step(&field, &cradle, &population);
		p++;
	}
}
