	//заготовка дл€ метода look индивида.
	//кака€-то странан€ херн€ с поворотом шаблона. ¬озможно дело в округлении точек при повороте. ј возможно и нет.
	//ƒобавить красивые затухающие окошки об каких-то событи€х в центре экрана. Ѕольше кава€, да!

	//Point <int> pos(35, 35);
	//std::vector <Vector <double> > mem;

	//FOV_Tri eee(0, (GetWinSliderValue(winManager, WIN_EDIT_EYE, "height_s")), 
	//				(GetWinSliderValue(winManager, WIN_EDIT_EYE, "width_s")));
	////FOV_Tri eee((GetWinSliderValue(winManager, WIN_EDIT_EYE, "angle_s")/100)*M_PI*2, 20, 30);
	//double wayAngle = 0;//(GetWinSliderValue(winManager, WIN_EDIT_EYE, "height_s")/100)*M_PI*2;
	//		double sint = sin(-wayAngle);
	//		double cost = cos(-wayAngle);

	//		Vector <double> vert[3];
	//		vert[0]=eee.getPolygon()[0];
	//		vert[1]=eee.getPolygon()[1];
	//		vert[2]=eee.getPolygon()[2];

	//		vert[1].multiplying(cost, -sint, sint, cost);
	//		vert[2].multiplying(cost, -sint, sint, cost);

	//		Vector <double> vectorR(vert[2].x, vert[2].y);
	//		double R = vectorR.getLength();


	//Vector <double> P;
	//int start_x = (pos.x-R>0) ? pos.x-R : 0;
	//int start_y = (pos.y-R>0) ? pos.y-R : 0;

	//for (int _x=start_x; _x<pos.x+R, _x<70; _x++) { 
	//	P.x=_x-pos.x;
	//	for (int _y=start_y; _y<pos.y+R, _y<70; _y++) {
	//		P.y=_y-pos.y;
	//		double pl1, pl2, pl3;
	//		pl1 = (vert[0].x - P.y)*(vert[1].y - vert[0].y)-(vert[1].x - vert[0].x)*(vert[0].y - P.x);
	//		pl2 = (vert[1].x - P.y)*(vert[2].y - vert[1].y)-(vert[2].x - vert[1].x)*(vert[1].y - P.x);
	//		pl3 = (vert[2].x - P.y)*(vert[0].y - vert[2].y)-(vert[0].x - vert[2].x)*(vert[2].y - P.x);
	//		if ((pl1 >= 0 && pl2 >= 0 && pl3 >= 0) || (pl1 <= 0 && pl2 <= 0 && pl3 <= 0)) {
	//			Point <int> absP;
	//			absP.x=func::round(P.x+pos.x);
	//			absP.y=func::round(P.y+pos.y);

	//			mem.push_back(Vector <double> (P.x, P.y));
	//		}
	//	}
	//}

	//double angle = (GetWinSliderValue(winManager, WIN_EDIT_EYE, "angle_s")/100)*M_PI*2;
	//double tgt = tan(angle/2);
	//double sintt = sin(angle);
	//double costt = cos(angle);

	//std::vector <Vector <double> > ::iterator m = mem.begin();
	//while (m != mem.end()) {
	//	Vector <int> g(m->x, m->y);

	//	//g.multiplying(costt, -sintt, sintt, costt);
	//	//g.multiplying(1, -tgt, 0, 1);
	//	//g.multiplying(1, 0, sintt, 1);
	//	//g.multiplying(1, -tgt, 0, 1);

	//	g.x+=35;
	//	g.y+=35;
	//	
	//	if (g.x >= 0 && g.y >= 0 && g.x < 70 && g.y < 70) 
	//		setQuadColor(&(winManager->getWin(WIN_EDIT_EYE)->graphic[g.x + g.y*70]), 0xFFFF0000);
	//	m++;
	//}