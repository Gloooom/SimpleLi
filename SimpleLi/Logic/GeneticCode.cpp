
#include "GeneticCode.h"

using namespace func;

GeneticCode GeneticCode::hibridization(GeneticCode person, Mode_hibrid mode) {
	GeneticCode result;
	result.diet = diet;

	for(int i = 0; i<eyes.size(); i++) {

	}
	if (mode==HALF_TO_HALF) {
		for (int i=0; i<end_of_phis; i++)
			result.phis[i] = (randBool()) ? phis[i] : person.phis[i];
		for (int i=0; i<end_of_status; i++)
			for (int j=0; j<end_of_soc; j++)
				result.soc[i][j] = (randBool()) ? soc[i][j] : person.soc[i][j];
	} else if (mode==AVERAGE) {
		for (int i=0; 
			i < ((eyes.size()<person.eyes.size()) ? eyes.size() : person.eyes.size()); 
			i++) 
			result.eyes.push_back(eyes[i].hibrid(person.eyes[i]));
		for (int i=0; i<end_of_phis; i++)
			result.phis[i] = (phis[i] + person.phis[i])/2;
		for (int i=0; i<end_of_status; i++)
			for (int j=0; j<end_of_soc; j++)
				result.soc[i][j] = (soc[i][j] + person.soc[i][j])/2;
	}

	return result;
}


GeneticCode GeneticCode::mutation(float coef, Mode_mutation mode) {
	GeneticCode result;
	result.diet = diet;
	result.eyes = eyes;
	for (int i=0; i<end_of_phis; i++)
		result.phis[i] = phis[i];
	for (int i=0; i<end_of_status; i++)
		for (int j=0; j<end_of_soc; j++)
			result.soc[i][j] = soc[i][j];

	if (mode == ALL) {
	} else if (mode == HALF) {
	} else if (mode == ONE) {
		int seed = func::randi(0, 1);
		if (seed == 0)
			result.soc[randi(0, end_of_status-1)][randi(0, end_of_soc-1)]
		= getVariation(soc[randf(0, end_of_status-1)][randf(0, end_of_soc-1)], coef);
		if (seed == 1)
			if (randi(0, 20)==0) eyes.push_back(FOV_Tri(randf(0, 6.28), randf(0,50), randf(0,50)));
			eyes[randi(0, eyes.size()-1)].mutation(coef, mode);
		if (randi(0, 20)==0) radialEye.mutation(coef, mode);
	}
	return result;
}
