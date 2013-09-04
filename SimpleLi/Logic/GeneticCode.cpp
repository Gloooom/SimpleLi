
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

GeneticCode GeneticCode::mutation(float maxDelta, float eyeAddChance, float eyeMutationChance, 
								  float radEyeMutationChance, int mutation_mode) {
	GeneticCode result;
	result.diet = diet;
	result.eyes = eyes;

	for (int i=0; i<end_of_phis; i++)
		result.phis[i] = phis[i];
	for (int i=0; i<end_of_status; i++)
		for (int j=0; j<end_of_soc; j++)
			result.soc[i][j] = soc[i][j];

	if (mutation_mode == ALL) {
	} else if (mutation_mode == HALF) {
	} else if (mutation_mode == ONE) {
		int selectStatus, selectSoc;
		selectStatus = randi(0, end_of_status-1);
		selectSoc = randi(0, end_of_soc-1);
		result.soc[selectStatus][selectSoc] = getVariation(soc[selectStatus][selectSoc], maxDelta);
		
		if (randPercent(eyeAddChance)) result.eyes.push_back(FOV_Tri(randf(0, 6.28), randf(0,50), randf(0,50)));
		if (randPercent(eyeMutationChance)) {
			int selectEye = randi(0, eyes.size()-1);
			result.eyes[selectEye] = eyes[selectEye].mutation(maxDelta, mutation_mode);
		}
		if (randPercent(radEyeMutationChance)) result.radialEye = radialEye.mutation(maxDelta, mutation_mode);
	}
	return result;
}
