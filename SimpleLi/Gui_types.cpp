
#include "Logic\Individ.h"
#include "Logic\Environment.h"
#include "Logic\Types.h"
#include "Gui_types.h"

double Dot(hgeVector hgev1, hgeVector hgev2) {
	Vector <double> v1(hgev1.x, hgev1.y), v2(hgev2.x, hgev2.y);
	return v1.getDot(v2);
}