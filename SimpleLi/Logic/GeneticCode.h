
#pragma once

#include "Functions.h"

enum Phis { 
	acceleration,
	hp_max,
	saturation, 
	consumption, 
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

class GenesConfines {
private:
	static GenesConfines *_self;
	GenesConfines() {
		phisConf.resize(end_of_phis, Interval(0, 0));
		socConf.resize(end_of_soc, Interval(0, 10));
		eyesConf.resize(3, Interval(0, 10));

		eyesConf[0].start = -M_PI;
		eyesConf[0].end = M_PI;

		socConf[max_speed].start = 0;
		socConf[max_speed].end = 5;
		socConf[rand_way].start = 0;
		socConf[rand_way].end = M_PI;
	};
	~GenesConfines();
public:
	static std::vector <Interval> phisConf;
	static std::vector <Interval> socConf;

	static std::vector <Interval> eyesConf;

	static GenesConfines *instance() {
		if (_self == NULL) {
			_self = new GenesConfines();
		}
		return _self;
	};
};


#include "Eyes.h"

class GeneticCode {
public:
	std::vector <float> phis;
	std::vector <std::vector <float> > soc;

	DWORD color;
	std::vector <FOV_Tri> eyes;
	int diet;
private:
	GenesConfines *config;
public:
	GeneticCode() {
		color=0xFF000000;

		std::vector <float> phis_temp;
		phis_temp.resize(end_of_phis, 0);
		phis = phis_temp;

		std::vector <float> one_of_soc;
		one_of_soc.resize(end_of_soc, 0);
		soc.resize(end_of_status, one_of_soc);

		config = GenesConfines::instance();
	};

	GeneticCode hibridization(GeneticCode person, Mode_hibrid mode);
	GeneticCode mutation(float maxDelta, int mutGenCount, float eyeAddChance, float eyeMutationChance, 
						 float radEyeMutationChance);

	void randomize();
	bool save(std::string path);
	bool load(std::string path);

private:
	void mutationGen(int selectStatus, int selectSoc, float maxDelta);
};
