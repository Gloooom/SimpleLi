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

GeneticCode GeneticCode::mutation(float coef, Mode_mutation mode, Mode_change_what mode_change) {
	GeneticCode result;
	result.diet = diet;

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
	} /*else if (mode == ONE) {
		int r = randf(0, phis.size()+0.9);
		switch(r) {
		case 0 : result.max_speed = getVariation(max_speed, coef); break;
		case 1 : result.acceleration = getVariation(acceleration,coef); break;
		case 2 : result.hp_max = getVariation(hp_max, coef); break;
		case 3 : result.energy_max = getVariation(energy_max, coef); break;
		case 4 : result.saturation = getVariation(saturation, coef); break;
		case 5 : result.stamina = getVariation(stamina, coef); break;
		case 6 : for (int i=0; i<eyes.size(); i++) {
					FOV temp_eye;
					temp_eye.angle = getVariation(eyes[i].angle, coef);
					temp_eye.height = getVariation(eyes[i].height, coef);
					temp_eye.width = getVariation(eyes[i].width, coef);
					result.eyes.push_back(temp_eye);
				 }
				 break;
		case 7 : result.fertility = getVariation(fertility,coef); break;
		case 8 : result.reproduction_time = getVariation(reproduction_time, coef); break;
		case 9 : result.reproduction_pause = getVariation(reproduction_pause, coef); break;
		case 10 : result.live_time =  getVariation(live_time, coef); break;
		case 11 : result.soc_factor = getVariation(soc_factor, coef); break;
		case 12 : result.width = getVariation(width, coef); break;
		case 13 : result.height = getVariation(height, coef); break;
		}
	}*/

	return result;
}
