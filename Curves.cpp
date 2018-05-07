#include <iostream>
#include <gl/glut.h>
#include <math.h>
#include <vector>

using namespace std;

//Point class for taking the points
class Point {
public:
	Point(){}

	float x, y;
	bool isOrigin = false;
	void setxy(float x2, float y2)
	{
		x = x2; y = y2;
	}
	//operator overloading for '=' sign
// 	const Point & operator=(const Point &rPoint)
// 	{
// 		x = rPoint.x;
// 		y = rPoint.y;
// 		return *this;
// 	}

};

int pointNumber = 4;

int SCREEN_WIDTH = 640;
int SCREEN_HEIGHT = 480;
int points = 0;
int clicks = 2;
float TriangleSize = 12.0f;
float width = 2.0f;
float vertexRadius = 2.0f;
bool mouseDown = false;

Point abc[20];
Point mousePoints[20];

vector<Point> mouseVertices[20] = {};
static vector<Point> drawVertices;

int factorial(int n)
{
	if (n <= 1)
		return(1);
	else
		n = n*factorial(n - 1);
	return n;
}

float binomial_coff(float n, float k)
{
	float ans;
	ans = factorial(n) / (factorial(k)*factorial(n - k));
	return ans;
}


void drawDot(int x, int y) {
	glBegin(GL_POINTS);
	glColor3f(0.0, 0.0, 1.0);
	glVertex2i(x, y);
	glEnd();
	glFlush();
}

void drawLine(Point p1, Point p2) {
	//  开启反走样
	glEnable(GL_BLEND);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glLineWidth(width);
	glBegin(GL_LINES);
	glColor3f(0.5, 0.5, 1.0);
	glVertex2f(p1.x, p1.y);
	glVertex2f(p2.x, p2.y);
	glEnd();
	glFlush();
}

void drawTriangle(Point p, float topX, float topY, float offsetXL, float offsetXR, float offsetYL, float offsetYR) {
	glBegin(GL_TRIANGLES);
	glColor3f(0.5, 0.5, 1.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glVertex2f(p.x + topX, p.y + topY);
	glVertex2f(p.x + offsetXL, p.y + offsetYL);
	glVertex2f(p.x + offsetXR, p.y + offsetYR);
	glEnd();
	glFlush();
}

//Calculate the bezier point
// Point drawBezier(Point PT[], double t) {
// 	Point P;
// 	P.x = pow((1 - t), 3) * PT[0].x + 3 * t * pow((1 - t), 2) * PT[1].x + 3 * (1 - t) * pow(t, 2)* PT[2].x + pow(t, 3)* PT[3].x;
// 	P.y = pow((1 - t), 3) * PT[0].y + 3 * t * pow((1 - t), 2) * PT[1].y + 3 * (1 - t) * pow(t, 2)* PT[2].y + pow(t, 3)* PT[3].y;
// 
// 	return P;
// }


//Calculate the bezier point [generalized]
Point drawBezierGeneralized(Point PT[], double t) {
	Point P;
	P.x = 0; P.y = 0;

	double a1 = pow((1 - t), 3);
	double a2 = pow((1 - t), 2) * 3 * t;
	double a3 = 3 * t*t*(1 - t);
	double a4 = t*t*t;

	P.x = a1 * PT[0].x + a2 * PT[1].x + a3 * PT[2].x + a4 * PT[3].x;
	P.y = a1 * PT[0].y + a2 * PT[1].y + a3 * PT[2].y + a4 * PT[3].y;

// 	for (int i = 0; i < pointNumber; i++)
// 	{
// 		P.x = P.x + binomial_coff((float)(pointNumber - 1), (float)i) * pow(t, (double)i) * pow((1 - t), (pointNumber - 1 - i)) * PT[i].x;
// 		P.y = P.y + binomial_coff((float)(pointNumber - 1), (float)i) * pow(t, (double)i) * pow((1 - t), (pointNumber - 1 - i)) * PT[i].y;
// 	}
	//cout<<P.x<<endl<<P.y;
	//cout<<endl<<endl;
	return P;
}

void mouseMotion(int mX, int mY)
{
	float aX = mX;
	float aY = (SCREEN_HEIGHT - mY);

	int pointsMount = 0;

	// Test
	glBegin(GL_POINTS);
	glColor3f(0.0, 0.0, 1.0);
	glVertex2i(aX, aY);
	glEnd();
	glFlush();

	mousePoints[pointsMount].setxy((float)aX, (float)(SCREEN_HEIGHT - aY));
	pointsMount++;

	cout << abc[0].x << endl;
	cout << aX << endl;


	for (int i = 0; i < clicks; i++)
	{
		if (pow((aX - abc[i].x), 2) + pow((aY - abc[i].y), 2) < pow(vertexRadius * 2, 2))
		{
			abc[i].x = aX;
			abc[i].y = aY;

			mouseDown = true;
		}
		else
			mouseDown = false;
	}
}

void mouseButton(int button, int state, int x, int y) {
	// If left button was clicked
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN ) {

		// Store where mouse was clicked, Y is backwards.
		abc[points].setxy((float)x, (float)(SCREEN_HEIGHT - y));
		points++;

		// Draw the red dot.
		drawDot(x, SCREEN_HEIGHT - y);

		// If (click-amout) points are drawn do the curve.
 		if ((points) % 2 == 0 && points != 0)
 		{
			for(int i = 0; i <= 1; i++)
			{
				for (int i = 0; i < 2; i++) {
					drawVertices.push_back(Point(abc[i]));
				}
			}

			cout <<"points:"<< points << endl;
			float tanX = 0;
			float tanY = 0;
			float k;
			tanX = abs(abc[0].x - abc[1].x);
			tanY = abs(abc[0].y - abc[1].y);
			k = tanY / tanX;
			cout << "tanX" << tanX << endl;
			cout << "tanY" << tanY << endl;
			cout <<"tangent( 斜率 )："<< k << endl;

			abc[points - 2].isOrigin = true;


			abc[3] = abc[1];

			if (k<=1.0)	//横着画
			{
				if (abc[0].x <= abc[3].x)
				{
					abc[1].x = (abc[3].x + abc[0].x) / 2;
					abc[1].y = (abc[0].y);
					abc[2].x = (abc[0].x + abc[3].x) / 2;
					abc[2].y = (abc[3].y);
// 					drawDot(abc[1].x, abc[1].y);
// 					drawDot(abc[2].x, abc[2].y);
					drawTriangle(abc[3], TriangleSize, 0, 0, 0, -TriangleSize, TriangleSize);
				}
				else
				{
					abc[1].x = (abc[3].x + abc[0].x) / 2;
					abc[1].y = (abc[0].y);
					abc[2].x = (abc[0].x + abc[3].x) / 2;
					abc[2].y = (abc[3].y);
// 					drawDot(abc[1].x, abc[1].y);
// 					drawDot(abc[2].x, abc[2].y);
					drawTriangle(abc[3], -TriangleSize, 0, 0, 0, -TriangleSize, TriangleSize);
				}
			}

			else //竖着画
			{
				if (abc[0].y <= abc[3].y)
				{
					abc[1].x = abc[0].x;
					abc[1].y = (abc[0].y + abc[3].y) / 2;
					abc[2].x = abc[3].x;
					abc[2].y = (abc[3].y + abc[0].y) / 2;
// 					drawDot(abc[1].x, abc[1].y);
// 					drawDot(abc[2].x, abc[2].y);
					drawTriangle(abc[3], 0, TriangleSize, -TriangleSize, TriangleSize, 0, 0);
				}
				else
				{
					abc[1].x = (abc[0].x);
					abc[1].y = (abc[0].y + abc[3].y) / 2;
					abc[2].x = (abc[3].x);
					abc[2].y = (abc[3].y + abc[0].y) / 2;
// 					drawDot(abc[1].x, abc[1].y);
// 					drawDot(abc[2].x, abc[2].y);
					drawTriangle(abc[3], 0, -TriangleSize, TriangleSize, -TriangleSize, 0, 0);
				}
			}

//			UE4 Buleprint
// 			if (abc[0].x <= abc[1].x)
// 			{
// 				abc[3] = abc[1];
// 				abc[1].x = abc[0].x + (abc[3].x + abc[0].x) / 15;
// 				abc[1].y = (abc[0].y);
// 				abc[2].x = abc[3].x - (abc[0].x + abc[3].x) / 15;
// 				abc[2].y = (abc[3].y);
// 				drawDot(abc[1].x, abc[1].y);
// 				drawDot(abc[2].x, abc[2].y);
// 			}
// 			else
// 			{
// 				abc[3] = abc[1];
// 				abc[1].x = abc[0].x + (abc[3].x + abc[0].x) / 15;
// 				abc[1].y = (abc[0].y);
// 				abc[2].x = abc[3].x -(abc[0].x + abc[3].x) / 15;
// 				abc[2].y = (abc[3].y);
//  			drawDot(abc[1].x, abc[1].y);
// 				drawDot(abc[2].x, abc[2].y);
// 			}

//			glColor3f(0.0, 0.0, 1.0);
			// Drawing the control lines
// 			for (int k = 0; k < pointNumber - 1; k++)
// 				drawLine(abc[k], abc[k + 1]);

			Point p1 = abc[0];
			p1.isOrigin = true;
			/* Draw each segment of the curve.Make t increment in smaller amounts for a more detailed curve.*/
			for (double t = 0.0; t <= 1.01; t += 0.02)
			{
				Point p2 = drawBezierGeneralized(abc, t);
				cout <<"p1:"<< p1.x << "  ,  " << p1.y << endl;
				cout <<"p2:"<< p2.x << "  ,  " << p2.y << endl;
				cout << endl;
				drawLine(p1, p2);
				p1 = p2;
			}
			points = 0;
			//glColor3f(0.0, 0.0, 0.0);
		}
	}
}

void myDisplay() {

	glFlush();
}

void myInit() {

	glPointSize(4);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, SCREEN_WIDTH, 0.0, SCREEN_HEIGHT);

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	myDisplay();
}

int main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Bezier Curve");
	myInit();

	glutDisplayFunc(myDisplay);

	glutMouseFunc(mouseButton);
/*	glutMotionFunc(mouseMotion);*/
	glutMainLoop();
	return 0;
}