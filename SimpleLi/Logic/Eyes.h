using namespace func;

class FOV_Tri {
private:
	float _angle;
	float _height;
	float _width;
	Point <double> polygon[3];
public:
	FOV_Tri():
		_angle(0),
		_height(0),
		_width(0) {};
	FOV_Tri(float ang, float hgt, float wdt):
		_angle(ang),
		_height(hgt),
		_width(wdt) {calculatPolygon();};

	float getAngle()	{return _angle;};
	float getHeight()	{return _height;};
	float getWidth()	{return _width;};
	Point <double> *getPolygon() {return polygon;};

	void setAngle(float _a)	{
		_angle = _a;
		calculatPolygon();
	};
	void setHeight(float _h) {
		_height = _h;
		calculatPolygon();
	};
	void setWidth(float _w)	{
		_width = _w;
		calculatPolygon();
	};

	void calculatPolygon() {
		double cost = cos(-_angle);
		double sint = sin(-_angle);
		polygon[0].x = 0;
		polygon[0].y = 0;
		polygon[1].x = _height * sint - 0.5 * _width * cost;
		polygon[1].y = _height * cost + 0.5 * _width * sint; 
		polygon[2].x = _height * sint + 0.5 * _width * cost;
		polygon[2].y = _height * cost - 0.5 * _width * sint;
	};

	std::vector <Vector <int> > getVectors(double rotateAngle) {
		std::vector <Vector <int> > result;
		double sint = sin(rotateAngle);
		double cost = cos(rotateAngle);

		Vector <double> vert[3];
		vert[0]=polygon[0];
		vert[1]=polygon[1];
		vert[2]=polygon[2];

		vert[1].multiplying(cost, -sint, sint, cost);
		vert[2].multiplying(cost, -sint, sint, cost);

		double min_x = getMin(vert[0].x, getMin(vert[1].x, vert[2].x));
		double min_y = getMin(vert[0].y, getMin(vert[1].y, vert[2].y));
		Point <int> start;
		Point <int> end;

		Vector <double> dt(min_x, min_y);
		vert[0]-=dt;
		vert[1]-=dt;
		vert[2]-=dt;
		
		int max_x = getMax(vert[0].x, getMax(vert[1].x, vert[2].x));
		int max_y = getMax(vert[0].y, getMax(vert[1].y, vert[2].y));

		for (int x=0; x<=max_y; x++) {
			for (int y=0; y<=max_x; y++) {
				double pl1, pl2, pl3;
				pl1 = (vert[0].x - y)*(vert[1].y - vert[0].y)-(vert[1].x - vert[0].x)*(vert[0].y - x);
				pl2 = (vert[1].x - y)*(vert[2].y - vert[1].y)-(vert[2].x - vert[1].x)*(vert[1].y - x);
				pl3 = (vert[2].x - y)*(vert[0].y - vert[2].y)-(vert[0].x - vert[2].x)*(vert[2].y - x);
				if ((pl1 >= 0 && pl2 >= 0 && pl3 >= 0) || (pl1 <= 0 && pl2 <= 0 && pl3 <= 0)) {
					result.push_back(Vector <int> (x+dt.y, y+dt.x));
				}
			}
		}
		return result;
	};

	FOV_Tri mutation(float coef, int mutation_mode) {
		FOV_Tri result;
		if (mutation_mode == ONE) {
			int seed = randi(0, 2);
			switch(seed) {
			case 0: result._angle = getVariation(_angle, coef); break;
			case 1: result._height = getVariation(_height, coef); break;
			case 2: result._width = getVariation(_width, coef); break;
			}
		} else if (mutation_mode == ALL || mutation_mode == HALF) {
			result._angle = getVariation(_angle, coef);
			result._height = getVariation(_height, coef);
			result._width = getVariation(_width, coef); 
		}
		result.calculatPolygon();
		return result;
	};
	FOV_Tri hibrid(FOV_Tri eye) {
		//Конечно всё очень круто, но если у одной особи глаз имеет 1 градус, а у другой 359, то
		//в итоге у потомка будет глаз на жопе.
		FOV_Tri result;
		result._angle = (_angle + eye._angle)/2;
		result._height = (_height + eye._height)/2;
		result._width = (_width + eye._width)/2;
		result.calculatPolygon();
		return result;
	};
};

class FOV_Rad {
private:
	float _height;
public:
	FOV_Rad(): 
	  _height(0) {};
	FOV_Rad(float hgt):
		_height(hgt)
	{ };

	float angle()	{return 0;};
	float height()	{return _height;};
	float width()	{return 0;};

	void setHeight(float hgt) {_height = hgt;};

	FOV_Rad mutation(float coef, int mutation_mode) {
		FOV_Rad result;
		getVariation(_height, coef);
		return result;
	};
	FOV_Rad hibrid(FOV_Rad eye) {
		FOV_Rad result;
		result._height = (_height + eye._height)/2;
		return result;
	};
};