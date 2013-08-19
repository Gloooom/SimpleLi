#include "..\stdafx.h"

#include "..\Logic\Environment.h"

#pragma once

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
	checkDead();
	born();
	fill();
	std::map <long long int, Individ> ::iterator p = population.begin();
	while (p != population.end()) {
		p->second.step(&field, &cradle, &population);
		p++;
	}
}
