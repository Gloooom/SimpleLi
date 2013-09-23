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
	FOV_Tri(float ang, float wdt, float hgt):
		_angle(ang),
		_height(hgt),
		_width(wdt) {calculatPolygon();};

	float getAngle()	{return _angle;};
	float getHeight()	{return _height;};
	float getWidth()	{return _width;};
	std::vector < Vector <double> >  getPolygon() {
		std::vector < Vector <double> > v;
		v.push_back(polygon[0]);
		v.push_back(polygon[1]);
		v.push_back(polygon[2]);
		return v;
	};

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
		if (_height > 0 && _width > 0) {
			double cost = cos(-_angle);
			double sint = sin(-_angle);
			polygon[0].x = 0;
			polygon[0].y = 0;
			polygon[1].x = _height * sint - 0.5 * _width * cost;
			polygon[1].y = _height * cost + 0.5 * _width * sint; 
			polygon[2].x = _height * sint + 0.5 * _width * cost;
			polygon[2].y = _height * cost - 0.5 * _width * sint;
		} else {
			polygon[0].x =
			polygon[0].y = 
			polygon[1].x = 
			polygon[1].y = 
			polygon[2].x =
			polygon[2].y = 0;
		}
	};

	std::vector <Vector <int> > getVectors(double rotateAngle = 0) {
		std::vector <Vector <int> > result;
		if (_height > 0 && _width > 0) {
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
		} else {
			result.push_back(Vector <int> (0, 0));
		}
		return result;
	};

	FOV_Tri mutation(float coef) {
		FOV_Tri result;
			result._angle = getVariation(_angle, coef);
			result._height = getVariation(_height, coef, false, false);
			result._width = getVariation(_width, coef, false, false); 
		result.calculatPolygon();
		return result;
	};

	FOV_Tri hibrid(FOV_Tri eye) {
		FOV_Tri result;
		if (eye._angle > M_PI) eye._angle-=M_PI*2;
		if (eye._angle < -M_PI) eye._angle+=M_PI*2;
		result._angle = (_angle + eye._angle)/2;
		result._height = (_height + eye._height)/2;
		result._width = (_width + eye._width)/2;
		result.calculatPolygon();
		return result;
	};

	bool checkHit(Vector <double> point, Vector <double> offset, double rotateAngle = 0) {
		//нужна оптимизация.
		if (_height == 0 || _width == 0) return false; 
		double sint = sin(rotateAngle);
		double cost = cos(rotateAngle);

		Vector <double> vert[3];
		vert[0]=polygon[0];
		vert[1]=polygon[1];
		vert[2]=polygon[2];

		vert[1].multiplying(cost, -sint, sint, cost);
		vert[2].multiplying(cost, -sint, sint, cost);
		
		vert[0]+=offset;
		vert[1]+=offset;
		vert[2]+=offset;

		double pl1, pl2, pl3;
		pl1 = (vert[0].x - point.y)*(vert[1].y - vert[0].y)-(vert[1].x - vert[0].x)*(vert[0].y - point.x);
		pl2 = (vert[1].x - point.y)*(vert[2].y - vert[1].y)-(vert[2].x - vert[1].x)*(vert[1].y - point.x);
		pl3 = (vert[2].x - point.y)*(vert[0].y - vert[2].y)-(vert[0].x - vert[2].x)*(vert[2].y - point.x);
		if ((pl1 >= 0 && pl2 >= 0 && pl3 >= 0) || (pl1 <= 0 && pl2 <= 0 && pl3 <= 0)) {
			return true;
		} else {
			return false;
		}
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

	FOV_Rad mutation(float coef) {
		FOV_Rad result;
		getVariation(_height, coef, false);
		return result;
	};
	FOV_Rad hibrid(FOV_Rad eye) {
		FOV_Rad result;
		result._height = (_height + eye._height)/2;
		return result;
	};
};