#pragma once

#include "Ball.h"

class Field {
	Ball ball;
	Point endPoints[4];

public:
	Field();
	Field(Point origin, double width, double height, Ball ball);
	Field(Point endPoints[4], Ball ball);
	int corner_hit_check(Line ball_l, Point corners[], Point& cmp_p1, Point& cmp_p2);
	int collision(Line ball_l, Line walls[4], Point& cmp_p1, Point& cmp_p2);
	Point calculated_new_point(const Point& p1, const Point& p2, const double& power);
	Point generate_intersect_point(const Line& l1,const Line& l2);
	double calcualte_distance(const Point& p1, const Point& p2);
	void hit(Point target, double power);

	friend ostream& operator<<(ostream& os, const Field& f);
	friend void ballCase(Field& f);

};

void simpleCase(Field& f);
void complexCase(Field& f);

