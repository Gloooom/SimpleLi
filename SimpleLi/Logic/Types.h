#include "inclusion.h"

#pragma once

//#define W 100
//#define H 100
#define POP_A 50
#define POP_G 0


union UnionVal {
	DWORD dw;
	BYTE by[8];
	double d;
	long long int li;
	float f;
	int i;
	bool b;
};

enum Mode_hibrid {HALF_TO_HALF, AVERAGE};
enum Mode_change_what {SOC, SOC_PHIS, PHIS};
enum Mode_mutation {ONE, ALL, HALF};
enum Mode_feeding {AUTO, GETERO};
enum Type_eye {RADIUS, TRIANGLE, SECTOR};

enum Individ_status {HUNGRY, MATURE, REPRODUCT, WAIT, end_of_status};

template <typename Type> class IndMemory {
public:
	std::vector <Type> enemies;
	std::vector <Type> partners;
	void clear() {
		enemies.clear();
		partners.clear();
	};
	void operator=(IndMemory m) { 
		enemies = m.enemies;
		partners = m.partners;
	};
	bool empty() {
		if (enemies.empty() && partners.empty()) return true;
		else return false;
	};
};






