
#include "GeneticCode.h"

using namespace func;

GeneticCode GeneticCode::hibridization(GeneticCode person, Mode_hibrid mode) {
	GeneticCode result;
	result.diet = diet;

	if (mode==HALF_TO_HALF) {
		for (int i=0; i<end_of_phis; i++)
			result.phis[i] = (randBool()) ? phis[i] : person.phis[i];
		for (int i=0; i<end_of_status; i++)
			for (int j=0; j<end_of_soc; j++)
				result.soc[i][j] = (randBool()) ? soc[i][j] : person.soc[i][j];
	} else if (mode==AVERAGE) {
		if (eyes.size()<person.eyes.size()) {
			for (int i=0; i < eyes.size(); i++)
				result.eyes.push_back(eyes[i].hibrid(person.eyes[i]));
			for (int i=eyes.size(); i < person.eyes.size(); i++)
				result.eyes.push_back(person.eyes[i]);

		} else if (person.eyes.size()<eyes.size()) {
			for (int i=0; i < person.eyes.size(); i++)
				result.eyes.push_back(eyes[i].hibrid(person.eyes[i]));
			for (int i=person.eyes.size(); i < eyes.size(); i++)
				result.eyes.push_back(eyes[i]);

		} else if (person.eyes.size() == eyes.size()) {
			for (int i=0; i < eyes.size(); i++)
				result.eyes.push_back(eyes[i].hibrid(person.eyes[i]));
		}

		for (int i=0; i<end_of_phis; i++)
			result.phis[i] = (phis[i] + person.phis[i])/2;
		for (int i=0; i<end_of_status; i++)
			for (int j=0; j<end_of_soc; j++)
				result.soc[i][j] = (soc[i][j] + person.soc[i][j])/2;
	}

	return result;
}

GeneticCode GeneticCode::mutation(float maxDelta, int mutGenCount, float eyeAddChance, float eyeMutationChance, 
								  float radEyeMutationChance) {
	GeneticCode result;
	result.diet = diet;
	result.eyes = eyes;

	for (int i=0; i<end_of_phis; i++)
		result.phis[i] = phis[i];
	for (int i=0; i<end_of_status; i++) 
		for (int j=0; j<end_of_soc; j++)
			result.soc[i][j] = soc[i][j];
	

	for (int i = 0; i<mutGenCount; i++) {
		int selectStatus, selectSoc;
		selectStatus = randi(0, end_of_status-1);
		selectSoc = randi(0, end_of_soc-1);
		if (selectSoc != 0 && selectSoc != 1)
			result.soc[selectStatus][selectSoc] = getVariation(soc[selectStatus][selectSoc], maxDelta);
		else 
			result.soc[selectStatus][selectSoc] = getVariation(soc[selectStatus][selectSoc], maxDelta, false);
	}
	
	if (randPercentBool(eyeMutationChance)) {
		int selectEye = randi(0, eyes.size()-1);
		result.eyes[selectEye] = eyes[selectEye].mutation(maxDelta);
	}
	if (randPercentBool(eyeAddChance)) result.eyes.push_back(FOV_Tri(randf(-M_PI, M_PI), randf(0,50), randf(0,50)));
	if (randPercentBool(radEyeMutationChance)) result.radialEye = radialEye.mutation(maxDelta);

	return result;
}


void GeneticCode::randomize() {
	eyes.clear();
	for(int i = func::randi(1, 3); i>=0; i--)
		eyes.push_back(FOV_Tri(func::randf(-M_PI, M_PI), func::randi(5, 40), func::randi(5, 40)));
	for (int i=0; i<end_of_status; i++) {
		soc[i][max_speed] = func::randf(0.5, 5);
		soc[i][libido] = func::randf(-10, 10);
		soc[i][rand_way] = func::randf(0, M_PI*2);
		soc[i][partner] = func::randf(-10, 10);
		soc[i][cohesion_partner] = func::randf(-10, 10);
		soc[i][separation_partner] = func::randf(-10, 10);
		soc[i][alignment_partner] = func::randf(-10, 10);
		soc[i][enemy] = func::randf(-10, 10);
		soc[i][cohesion_enemy] = func::randf(-10, 10);
		soc[i][separation_enemy] = func::randf(-10, 10);
		soc[i][alignment_enemy] = func::randf(-10, 10);
	}
}