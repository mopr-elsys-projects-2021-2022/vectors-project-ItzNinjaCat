#pragma once
// your declarations (and certain types of definitions) here


#include "Point.h"

struct Line {
	double A, B, C;
	Line(double A = 0, double B = 0, double C = 0) : A(A), B(B), C(C) {}
	Line(Point p1, Point p2) : A((p1.y - p2.y) / (p1.x - p2.x)), B(-1), C(p1.y - ((p1.y - p2.y) / (p1.x - p2.x)) * p1.x){
		if(p1.y == p2.y){
			A = 0;
			B = -1;
			C = p1.y;
		}
		else if(p1.x == p2.x){
			A = -1;
			B = 0;
			C = p1.x;
		}
	}
	friend ostream& operator<<(ostream& os, const Line& l) {
	os << "(" << l.A << ", " << l.B << ", " << l.C << ")";

	return os;
}
};
