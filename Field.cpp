#include "Field.h"

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

int Field::collision(Line ball_l, Line walls[4], Point cmp_p){
	Point perp_point_tmp;
	int bounce_index = -1;
	for(int i = 0; i < 4; i++){
		perp_point_tmp = Point((ball_l.B - walls[i].B) / (walls[i].A - ball_l.A), (ball_l.A * (ball_l.B - walls[i].B) / (walls[i].A - ball_l.A)) + ball_l.B);
		if((cmp_p.x < perp_point_tmp.x and perp_point_tmp.x < ball.center.x) and (cmp_p.y < perp_point_tmp.y and perp_point_tmp.y < ball.center.y)){
			bounce_index = i;
		}
		else if((cmp_p.x > perp_point_tmp.x and perp_point_tmp.x > ball.center.x) and (cmp_p.y > perp_point_tmp.y and perp_point_tmp.y > ball.center.y)) {
			bounce_index = i;
		}
	}
	return bounce_index;

}

void Field::hit(Point target, double power) {
	double new_x;
	double new_y;
	Point new_p;
	Line ball_line;
	Line rectangle[4];
	rectangle[0] = Line(endPoints[0], endPoints[1]);
	rectangle[1] = Line(endPoints[1], endPoints[2]);
	rectangle[2] = Line(endPoints[2], endPoints[3]);
	rectangle[3] = Line(endPoints[3], endPoints[0]);
	do{
	double new_x = ball.center.x - ((ball.center.x - target.x) * power);
	double new_y = ball.center.y - ((ball.center.y - target.y) * power);
	new_p = Point(new_x, new_y);
	ball_line = Line(ball.center, target);
	//pseudo code
	//if point outside wall 1-4
	/*Point perp_point_tmp;
	int bounce_index = -1;
	for(int i = 0; i < 4; i++){
		perp_point_tmp = Point((ball_line.B - rectangle[i].B) / (rectangle[i].A - ball_line.A), (ball_line.A * (ball_line.B - rectangle[i].B) / (rectangle[i].A - ball_line.A)) + ball_line.B);
		if((new_p.x < perp_point_tmp.x and perp_point_tmp.x < ball.center.x) and (new_p.y < perp_point_tmp.y and perp_point_tmp.y < ball.center.y)){
			bounce_index = i;
		}
		else if((new_p.x > perp_point_tmp.x and perp_point_tmp.x > ball.center.x) and (new_p.y > perp_point_tmp.y and perp_point_tmp.y > ball.center.y)) {
			bounce_index = i;
		}
	}*/
	int bounce_index = collision(ball_line, rectangle, new_p);
	if(bounce_index != -1){
		int next_index = bounce_index + 1;
		if(bounce_index == 3){
		 next_index = 0;
		}
		if(endPoints[bounce_index].y == 0 && endPoints[next_index].y == 0){
			ball.center.x = new_p.x;
			ball.center.y = 0 - new_p.y;
			continue;
		}
		else if(endPoints[bounce_index].x == 0 && endPoints[next_index].x == 0){
			ball.center.y = new_p.y;
			ball.center.x = 0 - new_p.x;
			continue;
		}
		Line bounce_line_perp(-1 / rectangle[bounce_index].A, new_p.y - (-1 / rectangle[bounce_index].A * new_p.x));
		double border_point = (rectangle[bounce_index].B - bounce_line_perp.B) / (bounce_line_perp.A - rectangle[bounce_index].A);
		Point new_point_on_border(border_point, (rectangle[bounce_index].A * border_point) + rectangle[bounce_index].B);
		Point after_bounce(new_point_on_border.x * 2 - new_p.x, new_point_on_border.y * 2 - new_p.y);
		ball.center.x = after_bounce.x;
		ball.center.y = after_bounce.y;
		continue;
	}
	ball.center.x = new_p.x;
	ball.center.y = new_p.y;
	return;
	}while(collision(ball_line, rectangle, new_p) != -1);
		//double border_eq_a1 = (endPoints[0].y - endPoints[1].y)/(endPoints[0].x - endPoints[1].x);
		//double border_eq_b1 = endPoints[0].y - (border_eq_a1 * endPoints[0].x);
		//double perpendicular_eq_a = -1 / border_eq_a1;
		//double perpendicular_eq_b = new_p.y - (perpendicular_eq_a * new_p.x);
		//double border_point = (border_eq_b1 - perpendicular_eq_b) / (perpendicular_eq_a - border_eq_a1);
		//Point new_point_on_border(border_point, (border_eq_a1 * border_point) + border_eq_b1);
		//Point after_bounce(new_point_on_border.x * 2 - new_p.x, new_point_on_border.y * 2 - new_p.y);
		//if any of the walls lie on the x/y axis case
	

		/*
		double border_eq_a2 = (endPoints[1].y - endPoints[2].y)/(endPoints[1].x - endPoints[2].x);
		double border_eq_b2 = endPoints[1].y - (border_eq_a2 * endPoints[1].x);
		double border_eq_a3 = (endPoints[2].y - endPoints[3].y)/(endPoints[2].x - endPoints[3].x);
		double border_eq_b3 = endPoints[2].y - (border_eq_a3 * endPoints[2].x);
		double border_eq_a4 = (endPoints[3].y - endPoints[0].y)/(endPoints[3].x - endPoints[0].x);
		double border_eq_b4 = endPoints[3].y - (border_eq_a4 * endPoints[3].x);
		double a = (ball.center.y - new_y) / (ball.center.x - new_x);
		double b = ball.center.y - (a * ball.center.x);
		Point border_intersect_1((border_eq_b1 - b) / (a - border_eq_a1), (a * (border_eq_b1 - b) / (a - border_eq_a1)) + b);
		//case first wall bounce
		if((ball.center.x < border_intersect_1.x && border_intersect_1.x < new_x && ball.center.y < border_intersect_1.y && border_intersect_1.y < new_y) || (ball.center.x > border_intersect_1.x && border_intersect_1.x > new_x  && ball.center.y > border_intersect_1.y && border_intersect_1.y > new_y)){
			// -a* new_x - b = new_y
		}
	*/
	

	
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
