#include "Field.h"
#include <cmath>

Field::Field() {}

Field::Field(Point origin, double width, double height, Ball ball) {
	endPoints[0] = Point(origin.x, origin.y);
	endPoints[1] = Point(origin.x + width, origin.y);
	endPoints[2] = Point(origin.x + width, origin.y + height);
	endPoints[3] = Point(origin.x, origin.y + height);

	this->ball = ball;
	this->startingPoint = ball.center;
}

Field::Field(Point endPoints[4], Ball ball) {
	for (int i = 0; i < 4; ++i) {
		this->endPoints[i] = endPoints[i];
	}

	this->ball = ball;
	this->startingPoint = ball.center;
}


double area(const Point& p1, const Point& p2, const Point& p3)
{
    return abs((p1.x * (p2.y - p3.y) + p2.x * (p3.y - p1.y) + p3.x * (p1.y - p2.y)) / 2.0);
}
  
bool check(Point corners[],const Point& p)
{
    double A = area(corners[0], corners[1], corners[2]) ;
	double B = area(corners[0], corners[3], corners[2]);
  	A += B;
  	
    double A1 = area(p, corners[0], corners[1]);
    double A2 = area(p, corners[1], corners[2]);
    double A3 = area(p, corners[2], corners[3]);
    double A4 = area(p, corners[0], corners[3]);

    return (A == A1 + A2 + A3 + A4);
}

Point Field::generate_intersect_point(const Line& l1,const Line& l2){
	Point tmp;
	double x = (l1.B * l2.C - l2.B * l1.C) / (l1.A * l2.B - l2.A * l1.B);
	double y = (l1.C * l2.A - l2.C * l1.A) / (l1.A * l2.B - l2.A * l1.B);
	if(x == -0){
		x = 0;
	}
	if(y == -0){
		y = 0;
	}
	tmp = Point(x, y);

	return tmp;
}

double Field::calculate_distance(const Point& p1, const Point& p2){
	return sqrt(pow((p2.x - p1.x), 2) + pow((p2.y - p1.y), 2));
}

int Field::corner_hit_check(Line ball_l, Point corners[], Point& p1, Point& p2){
	Point save_point;
	int corner_index = -1;
	for(int i = 0;i < 4; i++){
		double a = ((corners[i].x - p1.x) / (p2.x - p1.x));
		double b = ((corners[i].y - p1.y) / (p2.y - p1.y));
		double scale = 0.000001;
		//cout << "before "<< endl;
		//cout << a << " " << b << endl;
    	a = floor(a / scale + 0.5) * scale;
    	b = floor(b / scale + 0.5) * scale;
    	//cout << "after "<< endl;
    	//cout << a << " " << b << endl;
		if((a == b) and ( a < 1)){
			if( a < 0){

				ball.center.x = p1.x;
				ball.center.y = p1.y;
				return -1;
			}
			corner_index = i;
			save_point.x = corners[i].x;
			save_point.y = corners[i].y;
		}
	}
	return corner_index;
}

int Field::collision(Line ball_l, Line walls[4], Point& p1, Point& p2){
	Point tmp;
	Point save_point;
	int bounce_index = -1;
	for(int i = 0; i < 4; i++){
		tmp = generate_intersect_point(ball_l, walls[i]);
		double a = ((tmp.x - p1.x) / (p2.x - p1.x));
		double b = ((tmp.y - p1.y) / (p2.y - p1.y));
		double scale = 0.000001;
    	a = floor(a / scale + 0.5) * scale;
    	b = floor(b / scale + 0.5) * scale;
		if((a == b) and ( a < 1 and a > 0)){
			if(bounce_index != -1){
				if(calculate_distance(tmp, p2) < calculate_distance(save_point, p2)){
					bounce_index = i;
					save_point.x = tmp.x;
					save_point.y = tmp.y;
					}
			}	
			else{
				bounce_index = i;
				save_point.x = tmp.x;
				save_point.y = tmp.y;
			}

		}
	}
	return bounce_index;

}

Line Field::generate_perpendicular_line(const Line& line, const Point& p){
	Line perp_line;
	if(line.A == 0){
		perp_line = Line(1, 0, -p.x);
	}
	else if(line.B == 0){
		perp_line = Line(0 , -1, p.y);
	}
	else{
		perp_line = Line(-1 / line.A, -1, p.y - (-1 / line.A * p.x));
	}
	return perp_line;
}

Point Field::calculated_new_point(const Point& p1, const Point& p2, const double& power){
	double new_x = p1.x - ((p1.x - p2.x) * power);
	double new_y = p1.y - ((p1.y - p2.y) * power);
	Point new_p(new_x, new_y);
	return new_p;
}

void Field::hit(Point target, double power) {
	if(power < 1 or power > 10){
		throw "Incorrect power";
		return;
	}
	if(!check(endPoints, ball.center)){
		throw "Ball outside table";
	}
	Point new_p;
	new_p = calculated_new_point(ball.center, target, power);
	Line ball_line;
	ball_line = Line(ball.center, new_p);
	Line rectangle[4];
	Point endPoints_cpy[4];
	rectangle[0] = Line(endPoints[0], endPoints[1]);
	rectangle[1] = Line(endPoints[1], endPoints[2]);
	rectangle[2] = Line(endPoints[2], endPoints[3]);
	rectangle[3] = Line(endPoints[3], endPoints[0]);
	if(ball.diameter){
		Point smaller_endPoints[4];
		smaller_endPoints[0].x = endPoints[0].x - ((ball.diameter / 2 )* (endPoints[0].x - endPoints[1].x) / calculate_distance(endPoints[0], endPoints[1]));
		smaller_endPoints[0].y = endPoints[0].y - ((ball.diameter / 2 )* (endPoints[0].y - endPoints[1].y) / calculate_distance(endPoints[0], endPoints[1]));
		smaller_endPoints[1].x = endPoints[1].x - ((ball.diameter / 2 )* (endPoints[1].x - endPoints[2].x) / calculate_distance(endPoints[1], endPoints[2]));
		smaller_endPoints[1].y = endPoints[1].y - ((ball.diameter / 2 )* (endPoints[1].y - endPoints[2].y) / calculate_distance(endPoints[1], endPoints[2]));
		smaller_endPoints[2].x = endPoints[2].x - ((ball.diameter / 2 )* (endPoints[2].x - endPoints[3].x) / calculate_distance(endPoints[2], endPoints[3]));
		smaller_endPoints[2].y = endPoints[2].y - ((ball.diameter / 2 )* (endPoints[2].y - endPoints[3].y) / calculate_distance(endPoints[2], endPoints[3]));
		smaller_endPoints[3].x = endPoints[3].x - ((ball.diameter / 2 )* (endPoints[3].x - endPoints[0].x) / calculate_distance(endPoints[3], endPoints[0]));
		smaller_endPoints[3].y = endPoints[3].y - ((ball.diameter / 2 )* (endPoints[3].y - endPoints[0].y) / calculate_distance(endPoints[3], endPoints[0]));
		
		rectangle[0] = generate_perpendicular_line(rectangle[1], smaller_endPoints[1]);
		rectangle[1] = generate_perpendicular_line(rectangle[2], smaller_endPoints[2]);
		rectangle[2] = generate_perpendicular_line(rectangle[3], smaller_endPoints[3]);
		rectangle[3] = generate_perpendicular_line(rectangle[0], smaller_endPoints[0]);
		
		smaller_endPoints[1] = generate_intersect_point(rectangle[0], rectangle[1]);
		smaller_endPoints[2] = generate_intersect_point(rectangle[1], rectangle[2]);
		smaller_endPoints[3] = generate_intersect_point(rectangle[2], rectangle[3]);
		smaller_endPoints[0] = generate_intersect_point(rectangle[3], rectangle[0]);
		
		endPoints_cpy[0] = Point(smaller_endPoints[0].x, smaller_endPoints[0].y);
		endPoints_cpy[1] = Point(smaller_endPoints[1].x, smaller_endPoints[1].y);
		endPoints_cpy[2] = Point(smaller_endPoints[2].x, smaller_endPoints[2].y);
		endPoints_cpy[3] = Point(smaller_endPoints[3].x, smaller_endPoints[3].y);
	}
	else{
		endPoints_cpy[0] = Point(endPoints[0].x, endPoints[0].y);
		endPoints_cpy[1] = Point(endPoints[1].x, endPoints[1].y);
		endPoints_cpy[2] = Point(endPoints[2].x, endPoints[2].y);
		endPoints_cpy[3] = Point(endPoints[3].x, endPoints[3].y);
	}

	int corner_case = corner_hit_check(ball_line, endPoints, new_p, ball.center);
	int bounce_index = collision(ball_line, rectangle, new_p, ball.center);
	while(bounce_index != -1 or corner_case != -1){
		if(corner_case != -1){
			ball.center.x = startingPoint.x;
			ball.center.y = startingPoint.y;
			return;
		}
		else{
			int next_index = bounce_index + 1;
			if(bounce_index == 3){
			 next_index = 0;
			} 
			if(endPoints_cpy[bounce_index].y == 0 and endPoints_cpy[next_index].y == 0){
				ball.center.x = new_p.x;
				ball.center.y = 0 - new_p.y;
			}
			else if(endPoints_cpy[bounce_index].x == 0 and endPoints_cpy[next_index].x == 0){
				ball.center.y = new_p.y;
				ball.center.x = 0 - new_p.x;
			}
			else{
				Line bounce_line_perp;
				Point new_point_on_border;
				bounce_line_perp = generate_perpendicular_line(rectangle[bounce_index], new_p);
				new_point_on_border = generate_intersect_point(bounce_line_perp, rectangle[bounce_index]);		

				Point after_bounce(new_point_on_border.x * 2 - new_p.x, new_point_on_border.y * 2 - new_p.y);
				ball.center.x = after_bounce.x;
				ball.center.y = after_bounce.y;
			}
			Point intersect;
			intersect = generate_intersect_point(ball_line, rectangle[bounce_index]);
			ball_line = Line(intersect, ball.center);
			bounce_index = collision(ball_line, rectangle, ball.center, intersect);
			corner_case = corner_hit_check(ball_line, endPoints, ball.center, intersect);
		}
		new_p.x = ball.center.x;
		new_p.y = ball.center.y;
	}
	ball.center.x = new_p.x;
	ball.center.y = new_p.y;
}

ostream& operator<<(ostream& os, const Field& f) {
	os << "Field points:" << endl;
	for(int i = 0; i < 4; i++)
		os << "  " << f.endPoints[i] << endl;
	os << f.ball;

	return os;
}

void simpleCase(Field& f) {
	Point origin;
	double width, height;
	Ball ball;
	cout << "Point of origin of field: ";
	cin >> origin;
	cout << "Width and height of field: ";
	cin >> width >> height;
	cin >> ball;
	f = Field(origin, width, height, ball);	
}

void complexCase(Field& f) {
	Point endPoints[4];
	Ball ball;
	cout << "Field points: " << endl;
	for (int i = 0; i < 4; ++i) {
		cout << "  " << i+1 << ") ";
		cin >> endPoints[i];
	}
	cin >> ball;
	f = Field(endPoints, ball);
}

void ballCase(Field& f) {
	Ball ball;
	cin >> ball;
	f = Field(f.endPoints, ball);
}
