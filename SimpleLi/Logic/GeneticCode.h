
#pragma once

#include "Functions.h"
#include "Eyes.h"

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
	libido,
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

	DWORD color;
	FOV_Rad radialEye;
	std::vector <FOV_Tri> eyes;
	int diet;
public:
	GeneticCode() {
		color=0xFF000000;

		phis.resize(end_of_phis, 0);

		std::vector <float> one_of_soc;
		one_of_soc.resize(end_of_soc, 0);
		soc.resize(end_of_status, one_of_soc);
	};

	GeneticCode hibridization(GeneticCode person, Mode_hibrid mode);
	GeneticCode mutation(float maxDelta, float eyeAddChance, float eyeMutationChance, 
						 float radEyeMutationChance, int mutation_mode);
	bool save(std::string path);
	bool load(std::string path);
};
