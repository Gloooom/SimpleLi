//// Logic.cpp: определяет тчку входа для консольного приложения.
////
//
//#include "..\stdafx.h"
//
//
//int b=1;
//float c=3;
//
////Point <int> curPos(10,10);
////Переработать концепцию. Представить организмы как клетки на поле и сделать всё ахеренно квадратным. 
////Проработать логику поведение  квадратов.
////Увеличить колличество ячеек.
//
//
//int _tmain(int argc, _TCHAR* argv[])
//{
//
//	FOV eye1;
//	eye1.height=10;
//	eye1.width=10;
//	eye1.angle=0;
//
//	FOV eye2;
//	eye2.height=5;
//	eye2.width=5;
//	eye2.angle=0;
//
//	FOV eye3;
//	eye3.height=15;
//	eye3.width=4;
//	eye3.angle=M_PI + M_PI_4;
//
//	GeneticCode g;
//	g.genes[acceleration] = 1;
//	g.genes[max_speed] = 3;
//	g.genes[hp_max] = 10; 
//	g.genes[energy_max] = 30;
//	g.genes[saturation] = 2;
//	g.genes[stamina] = 2;
//	g.genes[fertility] = 4;
//	g.genes[live_time] = 30;
//	g.genes[width] =  1;
//	g.genes[height] = 1;
//	g.genes[reproduction_time] = 5; 
//	g.genes[reproduction_pause] = 20;
//	g.genes[soc_partner_factor] = 1;
//	g.genes[soc_enemy_factor] = 1;
//
//	g.eyes.push_back(eye1);
//
//	Point <int> p1, p2, p3;
//	p1.x=10; p2.x=2; p3.x=15;
//	p1.y=10; p2.y=5; p3.y=10;
//
//	empty = new Individ();
//
//	Environment env;
//
//	Individ ind(p1, g);
//	env.population.push_back(ind);
//
//	while (1) {
//		
//		
//		env.fill();
//		
//		
//
//		for (int _x=0; _x<W; _x++) 
//			for (int _y=0; _y<H; _y++) 
//				if (env.field[_x][_y]==empty) 
//					printField[_x][_y]='-';
//				else 
//					printField[_x][_y]='&';
//
//		env.population[0].look(&env.field, printField, empty);
//		env.population[0].checkWay(printField);
//		printField[env.population[0].pos.x][env.population[0].pos.y]='@';
//		env.population[0].move(&env.field, empty);
//
//
//		//cki = System::Console::ReadKey(true);
//		//switch(cki.Key) 
//		//{
//		//case System::ConsoleKey::UpArrow : curPos.y--; printField[(int)curPos.x][(int)curPos.y]='?'; break;
//		//case System::ConsoleKey::DownArrow : curPos.y++; printField[(int)curPos.x][(int)curPos.y]='?'; break;
//		//case System::ConsoleKey::RightArrow : curPos.x++; printField[(int)curPos.x][(int)curPos.y]='?'; break;
//		//case System::ConsoleKey::LeftArrow : curPos.x--; printField[(int)curPos.x][(int)curPos.y]='?'; break;
//		//case System::ConsoleKey::Enter : 
//		//	if ((*(env.field[(int)curPos.x][(int)curPos.y]))==(*empty)) {
//		//		env.population.push_back(Individ(curPos, g));
//		//	} else {
//		//		//env.population.erase(((*(env.field[(int)curPos.x][(int)curPos.y])).it));
//		//	}; break;
//		//}
//
//		//System::Console::Clear();
//
//
//
//	}
//}
//
