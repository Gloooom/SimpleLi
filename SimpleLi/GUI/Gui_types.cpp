#include "Gui_types.h"

double Dot(hgeVector hgev1, hgeVector hgev2) {
	if (hgev1.Length()==0 || hgev2.Length()==0) return 0;
	return acos((hgev1.x*hgev2.x + hgev1.y*hgev2.y)/sqrt((hgev1.x*hgev1.x+hgev1.y*hgev1.y)*(hgev2.x*hgev2.x+hgev2.y*hgev2.y)));
}