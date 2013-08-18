#include "..\stdafx.h"

#include "..\Logic\Environment.h"

#pragma once

void Environment::fill() {
	for (int i=0; i<W; i++)
			for (int j=0; j<H; j++)
				field[i][j]=&empty;

	std::vector <Individ> ::iterator p = population.begin();
	while (p != population.end()) {
		field[(*p).pos.x][(*p).pos.y]=&(*p);
		p++;
	}
}

void Environment::checkDead() {
	std::vector <Individ> ::iterator p = population.begin();
	while (p != population.end()) {
		if ((*p).live == false) 
			p = population.erase(p);
		else 
			p++;
	}
}

void Environment::born() {
	std::vector <Individ> ::iterator c = cradle.begin();
	while (c != cradle.end()) {
		population.push_back(*c);
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
	std::vector <Individ> ::iterator p = population.begin();
	while (p != population.end()) {
		(*p).step(&field, &cradle);
		p++;
	}
}
