#pragma once

#include "..\Logic\Types.h"
#include "..\Logic\Individ.h"


class Environment {
public:
	Individ empty;
	Individ *field[W][H];
	std::map <long long int, Individ> population;

private:
	std::deque <Individ> cradle;

public:
	Environment() {
		for (int i=0; i<W; i++)
			for (int j=0; j<H; j++)
				field[i][j]=&empty;
	};
	~Environment() { };
	void save(std::string path);
	void load(std::string path);

	void addIndivid(Individ ind);
	void checkDead();
	void born();
	void fill();
	void step();
};
