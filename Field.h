#pragma once

#include "Ball.h"

class Field {
	Ball ball;
	Point endPoints[4];

public:
	Field();
	Field(Point origin, double width, double height, Ball ball);
	Field(Point endPoints[4], Ball ball);
	int collision(Line ball_l, Line walls[4], Point cmp_p);
	void hit(Point target, double power);

	friend ostream& operator<<(ostream& os, const Field& f);
	friend void ballCase(Field& f);

};

void simpleCase(Field& f);
void complexCase(Field& f);

