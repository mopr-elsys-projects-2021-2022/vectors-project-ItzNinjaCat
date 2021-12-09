#pragma once
#include "Point.h"

struct Line {
	double A, B;
	Line(double A = 0, double B = 0) : A(A), B(B) {}
	Line(Point p1, Point p2) : A((p1.y - p2.y) / (p1.x - p2.x)), B(p1.y - ((p1.y - p2.y) / (p1.x - p2.x)) * p1.x) {}
};
