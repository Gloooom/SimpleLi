
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
