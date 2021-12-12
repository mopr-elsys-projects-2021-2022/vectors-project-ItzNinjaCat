#include "Field.h"
#include <cmath>
Field::Field() {}

Field::Field(Point origin, double width, double height, Ball ball) {
	endPoints[0] = Point(origin.x, origin.y);
	endPoints[1] = Point(origin.x + width, origin.y);
	endPoints[2] = Point(origin.x + width, origin.y + height);
	endPoints[3] = Point(origin.x, origin.y + height);

	this->ball = ball;
}

Field::Field(Point endPoints[4], Ball ball) {
	for (int i = 0; i < 4; ++i) {
		this->endPoints[i] = endPoints[i];
	}

	this->ball = ball;
}

Point generate_intersect_point(Line& l1, Line& l2){
	Point perp_point_tmp;
	//possible issue
	double x = (l1.B * l2.C - l2.B * l1.C) / (l1.A * l2.B - l2.A * l1.B);
	double y = (l1.C * l2.A - l2.C * l1.A) / (l1.A * l2.B - l2.A * l1.B);
	if(x == -0){
		x = 0;
	}
	if(y == -0){
		y = 0;
	}
	perp_point_tmp = Point(x, y);
		/*cout << "l1: "<< endl;
		cout << l1.A << endl;
		cout << l1.B << endl;
		cout << l1.C << endl;
		cout << "l2: "<< endl;
		cout << l2.A << endl;
		cout << l2.B << endl;
		cout << l2.C << endl;*/
	return perp_point_tmp;
}

double calcualte_distance(const Point& p1, const Point& p2){
	return sqrt(pow((p2.x - p1.x), 2) + pow((p2.y - p1.y), 2));
}

//broken still
int Field::collision(Line ball_l, Line walls[4], Point& cmp_p1, Point& cmp_p2){
	Point perp_point_tmp;
	int bounce_index = -1;
	Point save_intersect_point;
	for(int i = 0; i < 4; i++){
		perp_point_tmp = generate_intersect_point(ball_l, walls[i]);
		//cout << "Point of intersection with wall with index: " << i << " is: " << perp_point_tmp << endl;
		//if(walls[i].A == 0 and walls[i].B == 0){
			/*if(i != 3){
				perp_point_tmp = generate_intersect_point(ball_l, walls[i]);
				if(endPoints[i].x == 0 and endPoints[i + 1].x == 0){
					perp_point_tmp = Point(0, ball_l.B);
				}
				else if(endPoints[i].y == 0 and endPoints[i + 1].y == 0){
					perp_point_tmp = Point(-ball_l.B / ball_l.A, 0);
				}
			}
			else{
				perp_point_tmp = generate_intersect_point(ball_l, walls[i]);
			
				if(endPoints[i].x == 0 and endPoints[0].x == 0){
					perp_point_tmp = Point(0, ball_l.B);
				}
				else if(endPoints[i].y == 0 and endPoints[0].y == 0){
					perp_point_tmp = Point(-ball_l.B / ball_l.A, 0);
				}
			}*/
		/*else{
			perp_point_tmp = Point((ball_l.B - walls[i].B) / (walls[i].A - ball_l.A), (ball_l.A * (ball_l.B - walls[i].B) / (walls[i].A - ball_l.A)) + ball_l.B);
		}*/
		//expected index 3 gives index but still ends up with the right point kinda?
		// this shit returns the wrong line cuz it instersects with both
		//need to add some way of checking for the first line intersected not the fist found intersection
		if(((perp_point_tmp.x - cmp_p1.x) / (cmp_p2.x - cmp_p1.x)) == ((perp_point_tmp.y - cmp_p1.y) / (cmp_p2.y - cmp_p1.y)) 
		and ((perp_point_tmp.x - cmp_p1.x) / (cmp_p2.x - cmp_p1.x)) < 1 and ((perp_point_tmp.x - cmp_p1.x) / (cmp_p2.x - cmp_p1.x)) > 0){
			if(bounce_index != -1){
				if(calcualte_distance(perp_point_tmp, cmp_p2) < calcualte_distance(save_intersect_point, cmp_p2) ){
					bounce_index = i;
					save_intersect_point.x = perp_point_tmp.x;
					save_intersect_point.y = perp_point_tmp.y;
					//cout << perp_point_tmp << endl;
					//cout << "here at index : " << i << endl;
					}
			}	
			else{	//check if disntace between first detected intersection is smaller than the second
				bounce_index = i;
				save_intersect_point.x = perp_point_tmp.x;
				save_intersect_point.y = perp_point_tmp.y;
			}
			
			//break;
		}
		/*cout << "index : " << i << " currnet poit dist: " << calcualte_distance(perp_point_tmp, cmp_p2) << " saved poit dist: " << calcualte_distance(save_intersect_point, cmp_p2) << endl;
		cout << "comparason point 1 :" << cmp_p1 << endl;
		cout << "comparason point 2 :" << cmp_p2 << endl;
		cout << "itnercection in collision func :" << save_intersect_point<< endl;*/
		cout << "index is :"<< i << " alpha? :" <<((perp_point_tmp.x - cmp_p1.x) / (cmp_p2.x - cmp_p1.x)) << endl;
	}
	return bounce_index;

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
	Point new_p;
	Line ball_line;
	Line rectangle[4];
	rectangle[0] = Line(endPoints[0], endPoints[1]);
	rectangle[1] = Line(endPoints[1], endPoints[2]);
	rectangle[2] = Line(endPoints[2], endPoints[3]);
	rectangle[3] = Line(endPoints[3], endPoints[0]);
	new_p = calculated_new_point(ball.center, target, power);
	ball_line = Line(ball.center, new_p);
	int bounce_index = collision(ball_line, rectangle, new_p, ball.center);
	while(bounce_index != -1){
//	if(bounce_index != -1){
		int next_index = bounce_index + 1;
		if(bounce_index == 3){
		 next_index = 0;
		} 
		if(endPoints[bounce_index].y == 0 && endPoints[next_index].y == 0){
			ball.center.x = new_p.x;
			ball.center.y = 0 - new_p.y;
		}
		else if(endPoints[bounce_index].x == 0 && endPoints[next_index].x == 0){
			ball.center.y = new_p.y;
			ball.center.x = 0 - new_p.x;
			//cout << "here " << new_p<< endl;
		}
		else{
			Line bounce_line_perp;
			Point new_point_on_border;
			if(rectangle[bounce_index].A == 0){
				/*if(endPoints[bounce_index].y == 0){
					bounce_line_perp = Line(endPoints[bounce_index].x, 0);
					new_point_on_border = Point(bounce_line_perp.C, 0);
				}
				if(endPoints[bounce_index].x == 0){
					bounce_line_perp = Line(0, endPoints[bounce_index].y);
					new_point_on_border = Point(0, bounce_line_perp.C);
				}*/
				bounce_line_perp = Line(1, 0, -new_p.x);
				new_point_on_border = generate_intersect_point(bounce_line_perp, rectangle[bounce_index]);
				
			}
			else{
				bounce_line_perp = Line(-1 / rectangle[bounce_index].A, -1,new_p.y - (-1 / rectangle[bounce_index].A * new_p.x));
				/*double border_point = (rectangle[bounce_index].C - bounce_line_perp.C) / (bounce_line_perp.A - rectangle[bounce_index].A);
				new_point_on_border = Point(border_point, (rectangle[bounce_index].A * border_point) + rectangle[bounce_index].C);*/
				//ISSUE
				new_point_on_border = generate_intersect_point(bounce_line_perp, rectangle[bounce_index]);
			}
			cout << "bounce line!!  "<< bounce_line_perp.A << " "<< bounce_line_perp.B<< " "  << bounce_line_perp.C  <<endl;
			cout << "new_p_on_border  " << new_point_on_border<< endl;
			cout << "new_p "<< new_p<< endl;
			Point after_bounce(new_point_on_border.x * 2 - new_p.x, new_point_on_border.y * 2 - new_p.y);
			cout << "bounce l point: " << after_bounce << endl;
			ball.center.x = after_bounce.x;
			ball.center.y = after_bounce.y;
		}
		Point intersect;
		intersect = generate_intersect_point(ball_line, rectangle[bounce_index]);
		/*if(bounce_index != 3){
			cout << "\n" << endPoints[bounce_index] << endl;
			cout  << endPoints[bounce_index+1] << endl;
			intersect = generate_intersect_point(ball_line, rectangle[bounce_index]);
		}
		else{
			cout << "\n" <<endPoints[bounce_index] << endl;
			cout << endPoints[0] << endl;
			intersect = generate_intersect_point(ball_line, rectangle[bounce_index]);
		}*/
		/*cout << ball_line.A << endl;
		cout << ball_line.B << endl;
		cout << ball_line.C << endl;
		cout << new_p << endl;
		cout << intersect << endl;*/
		//cout << "itersect p:  "<< intersect<< endl;
		//cout << "changing ball line!!!"<< endl;//ISSUE
		ball_line = Line(intersect, ball.center);
		//cout << ball_line.A << " "<<ball_line.B<< " "<<ball_line.C<< endl;
		//cout << "Point after bounce : " <<ball.center<< endl;
		bounce_index = collision(ball_line, rectangle, ball.center, intersect);
		//cout<< "bounce_index : "<< bounce_index << endl;
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
