#include "..\stdafx.h"

#pragma once

#include "..\Logic\Types.h"
#include "..\HGE\hgecolor.h"

enum Phis { 
	acceleration,
	hp_max,
	saturation, 
	stamina, 
	fertility,
	live_time,

	energy_max, 
	energy_mature,
	energy_hungry,

	reproduction_cost,
	reproduction_time,
	reproduction_pause,

	end_of_phis
};

enum Soc {
	max_speed,
	rand_way,
	partner,
	cohesion_partner,
	separation_partner,
	alignment_partner,
	enemy,
	cohesion_enemy,
	separation_enemy,
	alignment_enemy,

	end_of_soc
};

class GeneticCode {
public:
	std::vector <float> phis;
	std::vector <std::vector <float> > soc;

	//факторы поведения

	DWORD color;
	std::vector <FOV> eyes;
	Mode_feeding diet;
public:
	GeneticCode() {
		color=0xFFFFFFFF;

		phis.resize(Phis::end_of_phis, 0);

		std::vector <float> one_of_soc;
		one_of_soc.resize(Soc::end_of_soc, 0);
		soc.resize(Individ_status::end_of_status, one_of_soc);
	};

	GeneticCode hibridization(GeneticCode person, Mode_hibrid mode);
	GeneticCode mutation(float coef, Mode_mutation mode, Mode_change_what mode_change);
	//void operator=(GeneticCode person);
	bool save(std::string path);
	bool load(std::string path);
};
