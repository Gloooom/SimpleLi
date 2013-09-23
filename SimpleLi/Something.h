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
//
//void rotate(float aplha) {
//  alpha := ???;
//  xa := 0;
//  ya := 0;
//  xb := w-1;
//  yb := h-1;
//  s := sin(alpha);
//  c := cos(alpha);
//  xc := (xb-xa) div 2;
//  yc := (xb-xa) div 2;
//  x := xa-xc;
//  y := ya-yc;
//  x1 := round(x*cos(alpha) - y*sin(alpha));
//  y1 := round(x*sin(alpha) + y*cos(alpha));
//  x := xa-xc;
//  y := yb-yc;
//  x2 := round(x*cos(alpha) - y*sin(alpha));
//  y2 := round(x*sin(alpha) + y*cos(alpha));
//  x := xb-xc;
//  y := ya-yc;
//  x3 := round(x*cos(alpha) - y*sin(alpha));
//  y3 := round(x*sin(alpha) + y*cos(alpha));
//  x := xb-xc;
//  y := yb-yc;
//  x4 := round(x*cos(alpha) - y*sin(alpha));
//  y4 := round(x*sin(alpha) + y*cos(alpha));
//  dl := Min4(x1,x2,x3,x4);
//  dt := Min4(y1,y2,y3,y4);
//  dw := Max4(x1,x2,x3,x4) - dl;
//  dh := Max4(y1,y2,y3,y4) - dt;
//  for i := 0 to dh-1 do
//    for j := 0 to dw-1 do begin
//      x := j + dl;
//      y := i + dt;
//      sx :=  x*c + y*s + xc;
//      sy := -x*s + y*c + yc;
//      if (sx<xa)or(sx>xb)or(sy<ya)or(sy>yb) then begin
//        dest[j,i].r := 0;
//        dest[j,i].g := 0;
//        dest[j,i].b := 0;
//      end else begin
//        sl := Min(Max(floor(sx),0),w-1);
//        sr := Min(Max(sl+1,0),w-1);
//        st := Min(Max(floor(sy),0),h-1);
//        sb := Min(Max(st+1,0),h-1);
//        klt := 1/(sqrt(sqr(sl-sx)+sqr(st-sy))+epsilon);
//        klb := 1/(sqrt(sqr(sl-sx)+sqr(sb-sy))+epsilon);
//        krt := 1/(sqrt(sqr(sr-sx)+sqr(st-sy))+epsilon);
//        krb := 1/(sqrt(sqr(sr-sx)+sqr(sb-sy))+epsilon);
//        ksum := 1/(klt + klb + krt + krb);
//        klt := klt*ksum;
//        klb := klb*ksum;
//        krt := krt*ksum;
//        krb := krb*ksum;
//        with dest[j,i] do begin
//          r := r + klt*source[sl,st].r
//                 + klb*source[sl,sb].r
//                 + krt*source[sr,st].r
//                 + krb*source[sr,sb].r;
//          g := g + klt*source[sl,st].g
//                 + klb*source[sl,sb].g
//                 + krt*source[sr,st].g
//                 + krb*source[sr,sb].g;
//          b := b + klt*source[sl,st].b
//                 + klb*source[sl,sb].b
//                 + krt*source[sr,st].b
//                 + krb*source[sr,sb].b;
//        end;
//      end;
//    end;
//}


			//Cell c = quadArr[p->second.pos.x + p->second.pos.y*env->W()];
			//float x1, y1, x2, y2;
			//x1 = (c.getQuad()->v[0].x +  c.getQuad()->v[2].x)/2;
			//y1 = (c.getQuad()->v[0].y +  c.getQuad()->v[2].y)/2;
			//x2 = x1 + p->second.way.x*20;
			//y2 = y1 + p->second.way.y*20;
			//hge->Gfx_RenderLine(x1, y1, x2, y2, 0xAA00AA00);