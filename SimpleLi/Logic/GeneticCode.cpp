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
	}
	return result;
}
