

#include "stdafx.h"
//
//double Dot(hgeVector hgev1, hgeVector hgev2) {
//	Vector <double> v1(hgev1.x, hgev1.y), v2(hgev2.x, hgev2.y);
//	return v1.getDot(v2);
//}
//
//bool HandleKeys(int key) {
//	Point <float> tempP;
//
//	switch(key) {
//		case 0:
//			return false;
//
//		case HGEK_ESCAPE:
//			return true;
//
//		case HGEK_TAB:
//			break;
//		
//		case HGEK_RBUTTON:
//			tempP.x=state.mp.x/(Field[0][0].width+border);
//			tempP.y=state.mp.y/(Field[0][0].height+border);
//			selectInd=env.field[(int)tempP.x][(int)tempP.y];
//			//addIndivid(tempP, GETERO);
//			break;
//
//		case HGEK_LBUTTON:
//			tempP.x=state.mp.x/(Field[0][0].width+border);
//			tempP.y=state.mp.y/(Field[0][0].height+border);
//			selectInd=env.field[(int)tempP.x][(int)tempP.y];
//			//addIndivid(tempP, AUTO);
//			break;
//
//		case HGEK_SPACE:
//			play=!play;
//			break;
//
//		case HGEK_UP:
//			break;
//		case HGEK_DOWN:
//			break;
//		case HGEK_RIGHT:
//			break;
//		case HGEK_LEFT:
//			break;
//
//
//		case HGEK_1:
//			break;
//		case HGEK_2:
//			break;
//		case HGEK_3:
//			break;
//		case HGEK_4:
//			break;
//		case HGEK_5:
//			break;
//		case HGEK_6:
//			break;
//		case HGEK_7:
//			break;
//		case HGEK_8:
//			break;
//		case HGEK_9: 
//			break;
//	}
//
//	return false;
//}
//
//bool DoCommands(int id) {
//	switch(id) {
//		case 0:	return false;
//		case CMD_EXIT:  return true;
//		case CMD_HELP:  break;
//		
//	}
//
//	return false;
//}