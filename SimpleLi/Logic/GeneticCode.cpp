#include "..\stdafx.h"

#include "Functions.h"
#include "GeneticCode.h"



using namespace func;

GeneticCode GeneticCode::hibridization(GeneticCode person, Mode_hibrid mode) {
	GeneticCode result;
	result.diet = diet;
	result.eyes = person.eyes;

	if (mode==HALF_TO_HALF) {
		for (int i=0; i<end_of_phis; i++)
			result.phis[i] = (randBool()) ? phis[i] : person.phis[i];
		for (int i=0; i<end_of_status; i++)
			for (int j=0; j<end_of_soc; j++)
				result.soc[i][j] = (randBool()) ? soc[i][j] : person.soc[i][j];
	} else if (mode==AVERAGE) {
		for (int i=0; i<end_of_phis; i++)
			result.phis[i] = (phis[i] + person.phis[i])/2;
		for (int i=0; i<end_of_status; i++)
			for (int j=0; j<end_of_soc; j++)
				result.soc[i][j] = (soc[i][j] + person.soc[i][j])/2;
	}

	return result;
}


//GeneticCode GeneticCode::hibridization(GeneticCode person, Mode_hibrid mode, Mode_change_what mode_change = SOC) {
//	GeneticCode result;
//	result.diet = diet;
//
//	if (mode==HALF_TO_HALF) {
//		if (mode_change == PHIS || mode_change == SOC_PHIS) 
//			for (int i=0; i<phis.size(); i++)
//				result.phis[i] = (randBool()) ? phis[i] : person.phis[i];
//		if (mode_change == SOC || mode_change == SOC_PHIS) 
//			for (int i=0; i<soc.size(); i++)
//				for (int j=0; i<soc[i].size(); j++)
//					result.soc[i][j] = (randBool()) ? soc[i][j] : person.soc[i][j];
//	} else if (mode==AVERAGE) {
//		if (mode_change == PHIS || mode_change == SOC_PHIS) 
//			for (int i=0; i<phis.size(); i++)
//				result.phis[i] = (phis[i] + person.phis[i])/2;
//		if (mode_change == SOC || mode_change == SOC_PHIS) {
//			for (int i=0; i<end_of_status; i++)
//				for (int j=0; j<end_of_soc; j++)
//					result.soc[i][j] = (soc[i][j] + person.soc[i][j])/2;
//		}
//	}
//
//	return result;
//}

GeneticCode GeneticCode::mutation(float coef, Mode_mutation mode) {
	GeneticCode result;
	result.diet = diet;
	result.eyes = eyes;
	for (int i=0; i<end_of_phis; i++)
			result.phis[i] = phis[i];

	if (mode == ALL) {
		for (int i=0; i<phis.size(); i++)
			result.phis[i] = getVariation(phis[i], coef);
		for (int i=0; i<eyes.size(); i++) {
			FOV temp_eye;
			temp_eye.angle = getVariation(eyes[i].angle, coef);
			temp_eye.height = getVariation(eyes[i].height, coef);
			temp_eye.width = getVariation(eyes[i].width, coef);
			result.eyes.push_back(temp_eye);
		}
	} else if (mode == HALF) {
		for (int i=0; i<phis.size(); i++)
			result.phis[i] = (randBool()) ? getVariation(phis[i], coef) : phis[i];
		for (int i=0; i<eyes.size(); i++) {
			FOV temp_eye;
			temp_eye.angle = (randBool()) ? getVariation(eyes[i].angle, coef) : eyes[i].angle;
			temp_eye.height = (randBool()) ? getVariation(eyes[i].height, coef): eyes[i].height;
			temp_eye.width = (randBool()) ? getVariation(eyes[i].width, coef) : eyes[i].width;
			result.eyes.push_back(temp_eye);
		}
	} else if (mode == ONE) {
		result.soc[func::round(func::randf(0, end_of_status-1))]
		[func::round(func::randf(0, end_of_soc-1))]
		= getVariation(
			soc[func::round(func::randf(0, end_of_status-1))]
		[func::round(func::randf(0, end_of_soc-1))], coef);
	}
	return result;
}
