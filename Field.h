#pragma once

#include "Ball.h"

class Field {
	Ball ball;
	Point startingPoint;
	Point endPoints[4];

public:
	Field();
	Field(Point origin, double width, double height, Ball ball);
	Field(Point endPoints[4], Ball ball);
	
	double area(const Point& p1, const Point& p2, const Point& p3);
	bool check_if_point_is_inside_rect(Point corners[],const Point& p);
	bool ratio_check(Point endp[]);
	bool rectangle_check(Point endp[], Line walls[]);
	
	int corner_hit_check(Line ball_l, Point corners[], Point& cmp_p1, Point& cmp_p2);
	int collision(Line ball_l, Line walls[4], Point& cmp_p1, Point& cmp_p2);
	
	Point calculated_new_point(const Point& p1, const Point& p2, const double& power);
	Point intersection_point(const Line& l1,const Line& l2);
	Line perpendicular_line(const Line& line, const Point& p);
	double calculate_distance(const Point& p1, const Point& p2);
	
	void hit(Point target, double power);

	friend ostream& operator<<(ostream& os, const Field& f);
	friend void ballCase(Field& f);

};

void simpleCase(Field& f);
void complexCase(Field& f);
void ballCase(Field& f);
