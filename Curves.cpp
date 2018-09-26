#include <iostream>
#include <gl/glut.h>
#include <math.h>
#include <vector>

using namespace std;

#define vertexRadius 4.f
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

float TriangleSize = 12.0f;
float lineWidth = 2.0f;
float pointSize = 6.0f;

struct Point {
public:
	Point(){}

	float x, y;
	void setxy(float x2, float y2)
	{
		x = x2; y = y2;
	}

	bool getMouseIn(float _x, float _y)
	{
		bool bx = _x > x - 10 && _x < x + 10;
		bool by = _y > y - 10 && _y < y + 10;

		float i = pow((_x - x), 2) + pow((_y - y), 2);
		float j = pow(vertexRadius * 2, 2);

		if (pow((_x - x), 2) + pow((_y - y), 2) < pow(vertexRadius * 2, 2))
		{
			return true;
		}
		else
		{
			return false;
		}

		//return ;
	}
	//operator overloading for '=' sign
// 	const Point & operator=(const Point &rPoint)
// 	{
// 		x = rPoint.x;
// 		y = rPoint.y;
// 		return *this;
// 	}

};

struct Triangle
{
public:
	Triangle() {}

	Point Point1;
	Point Point2;
	Point Point3;

	void setTriangle(Point _p1, Point _p2, Point _p3)
	{
		Point1 = _p1;
		Point2 = _p2;
		Point3 = _p3;
	}
	void setTriangle(Point p, float topX, float topY, float offsetXL, float offsetXR, float offsetYL, float offsetYR)
	{
		Point1.setxy(p.x + topX, p.y + topY);
		Point2.setxy(p.x + offsetXL, p.y + offsetYL);
		Point3.setxy(p.x + offsetXR, p.y + offsetYR);
	}
};

struct  Line 
{
public:
	Line() {}
	vector<Point> allPoint;

	Point startPoint;
	Point endPoint;
	Triangle endTriangle;

	void setSegPoint(Point _x, Point _y)
	{
		startPoint = _x;
		endPoint = _y;
	}

	void computeBeizerCtrlPoint(vector<Point>& _points)
	{
		Point bPoint[4];
		bPoint[0] = _points[0];
		bPoint[1] = _points[1];

		float tanX = 0;
		float tanY = 0;
		float k;
		tanX = abs(bPoint[0].x - bPoint[1].x);
		tanY = abs(bPoint[0].y - bPoint[1].y);
		k = tanY / tanX;

		bPoint[3] = bPoint[1];
		if (k <= 1.0)	//横着画
		{
			if (bPoint[0].x <= bPoint[3].x)
			{
				bPoint[1].x = (bPoint[3].x + bPoint[0].x) / 2;
				bPoint[1].y = (bPoint[0].y);
				bPoint[2].x = (bPoint[0].x + bPoint[3].x) / 2;
				bPoint[2].y = (bPoint[3].y);
				endTriangle.setTriangle(bPoint[3], TriangleSize, 0, 0, 0, -TriangleSize, TriangleSize);
			}
			else
			{
				bPoint[1].x = (bPoint[3].x + bPoint[0].x) / 2;
				bPoint[1].y = (bPoint[0].y);
				bPoint[2].x = (bPoint[0].x + bPoint[3].x) / 2;
				bPoint[2].y = (bPoint[3].y);
				endTriangle.setTriangle(bPoint[3], -TriangleSize, 0, 0, 0, -TriangleSize, TriangleSize);
			}
		}
		else //竖着画
		{
			if (bPoint[0].y <= bPoint[3].y)
			{
				bPoint[1].x = bPoint[0].x;
				bPoint[1].y = (bPoint[0].y + bPoint[3].y) / 2;
				bPoint[2].x = bPoint[3].x;
				bPoint[2].y = (bPoint[3].y + bPoint[0].y) / 2;
				endTriangle.setTriangle(bPoint[3], 0, TriangleSize, -TriangleSize, TriangleSize, 0, 0);
			}
			else
			{
				bPoint[1].x = (bPoint[0].x);
				bPoint[1].y = (bPoint[0].y + bPoint[3].y) / 2;
				bPoint[2].x = (bPoint[3].x);
				bPoint[2].y = (bPoint[3].y + bPoint[0].y) / 2;
				endTriangle.setTriangle(bPoint[3], 0, -TriangleSize, TriangleSize, -TriangleSize, 0, 0);
			}
		}
		_points.clear();
		for (int i = 0; i < 4; i++)
		{
			_points.push_back(bPoint[i]);
		}
	}

	void bezierGeneralized(vector<Point> ctrlPoints)
	{
		Point p1 = ctrlPoints[0];
		Point p2;
		Line tempLine;
		allPoint.clear();
		tempLine.setSegPoint(ctrlPoints[0], ctrlPoints[3]);
		/* Draw each segment of the curve.Make t increment in smaller amounts for a more detailed curve.*/
		allPoint.push_back(p1);
		for (double t = 0.0; t <= 1.02; t += 0.02)
		{
			p2 = _computeBezierPoints(ctrlPoints, t);
			p1 = p2;
			allPoint.push_back(p2);
		}
		int i = 1;
	}
private:
	Point _computeBezierPoints(vector<Point> PT, double t) {
		Point P;
		P.x = 0; P.y = 0;

		double a1 = pow((1 - t), 3);
		double a2 = pow((1 - t), 2) * 3 * t;
		double a3 = 3 * t*t*(1 - t);
		double a4 = t*t*t;

		P.x = a1 * PT[0].x + a2 * PT[1].x + a3 * PT[2].x + a4 * PT[3].x;
		P.y = a1 * PT[0].y + a2 * PT[1].y + a3 * PT[2].y + a4 * PT[3].y;

		return P;
	}
};

int clickNum = 0;
Point startPoint;
Point endPoint;
vector<Line> m_vAllLine;

void drawDot(int x, int y) {
	glPointSize(pointSize);
	glBegin(GL_POINTS);
	glColor3f(0.0, 0.0, 1.0);
	glVertex2i(x, y);
	glEnd();
	glFlush();
}
void drawDot(Point _point) {
	glPointSize(pointSize);
	glBegin(GL_POINTS);
	glColor3f(0.0, 0.0, 1.0);
	glVertex2i(_point.x, _point.y);
	glEnd();
	glFlush();
}

void drawLine(Point p1, Point p2) {
	//  开启反走样
	glEnable(GL_BLEND);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glLineWidth(lineWidth);
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

void drawTriangle(Point _p1, Point _p2, Point _p3) {
	glBegin(GL_TRIANGLES);
	glColor3f(0.5, 0.5, 1.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glVertex2f(_p1.x, _p1.y);
	glVertex2f(_p2.x, _p2.y);
	glVertex2f(_p3.x, _p3.y);
	glEnd();
	glFlush();
}

void mouseMotion(int mX, int mY)
{
	float aX = mX;
	float aY = (SCREEN_HEIGHT - mY);

	for (int i = 0; i < m_vAllLine.size(); i++)
	{
		bool isMouseIn;

		if (m_vAllLine[i].startPoint.getMouseIn(float(aX), float(aY)))
		{
			m_vAllLine[i].startPoint.x = aX;
			m_vAllLine[i].startPoint.y = aY;

			vector<Point> tempCtrlPoints;
			tempCtrlPoints.push_back(m_vAllLine[i].startPoint);
			tempCtrlPoints.push_back(m_vAllLine[i].endPoint);

			m_vAllLine[i].computeBeizerCtrlPoint(tempCtrlPoints);
			m_vAllLine[i].bezierGeneralized(tempCtrlPoints);
		}
		else if (m_vAllLine[i].endPoint.getMouseIn(float(aX), float(aY)))
		{
			isMouseIn = true;

			m_vAllLine[i].endPoint.x = aX;
			m_vAllLine[i].endPoint.y = aY;

			vector<Point> tempCtrlPoints;
			tempCtrlPoints.push_back(m_vAllLine[i].startPoint);
			tempCtrlPoints.push_back(m_vAllLine[i].endPoint);

			m_vAllLine[i].computeBeizerCtrlPoint(tempCtrlPoints);
			m_vAllLine[i].bezierGeneralized(tempCtrlPoints);
		}
	}
}

void mouseButton(int button, int state, int x, int y) {

	// If left button was clicked
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN ) 
	{
		bool isMouseIn = false;

		// whether mouse in the existed point
		for (int i = 0; i < (int)m_vAllLine.size(); i++)
		{
			bool firstIn = m_vAllLine[i].startPoint.getMouseIn(float(x), float(SCREEN_HEIGHT - y));
			bool secondIn = m_vAllLine[i].endPoint.getMouseIn(float(x), float(SCREEN_HEIGHT - y));

			isMouseIn = firstIn || secondIn;
			if (isMouseIn)
			{
				cout << "mouse in" << endl;
				clickNum = 0;
				break;
			}
		}

		if (clickNum == 0 && !isMouseIn)
		{
			startPoint.setxy((float)x, (float)(SCREEN_HEIGHT - y));
		}
		if (clickNum == 1 && !isMouseIn)
		{
			endPoint.setxy((float)x, (float)(SCREEN_HEIGHT - y));
		}

		if (!isMouseIn)
		{
			// Store where mouse was clicked, Y is backwards.
			clickNum++;

			// Draw the red dot.
			//drawDot(x, SCREEN_HEIGHT - y);

			// If (click-amout) points are drawn do the curve.
			if ((clickNum) % 2 == 0 && clickNum != 0)
			{
				vector<Point> ctrlPoints;
				Line tempLine;
				ctrlPoints.push_back(startPoint);
				ctrlPoints.push_back(endPoint);
				tempLine.computeBeizerCtrlPoint(ctrlPoints);

				Point p1 = ctrlPoints[0];
				Point p2;
				tempLine.setSegPoint(ctrlPoints[0], ctrlPoints[3]);
				/* Draw each segment of the curve.Make t increment in smaller amounts for a more detailed curve.*/
				tempLine.bezierGeneralized(ctrlPoints);

				m_vAllLine.push_back(tempLine);
				clickNum = 0;
			}
		}	
	}
}

void drawBeizerLine()
{
	for (int i=0; i<(int)m_vAllLine.size(); i++)
	{
		// draw the start point and end point
		drawDot(m_vAllLine[i].startPoint);
		drawDot(m_vAllLine[i].endPoint);

		// draw the end triangle
		drawTriangle(m_vAllLine[i].endTriangle.Point1, m_vAllLine[i].endTriangle.Point2, m_vAllLine[i].endTriangle.Point3);

		vector<Point> tempPoint = m_vAllLine[i].allPoint;
		for (int j = 0; j < (int)tempPoint.size()-1; j++)
		{
			drawLine(tempPoint[j], tempPoint[j+1]);
		}
	}
}

void processMenuEvents(int option) {
	switch (option)
	{
	case 0:
		pointSize++;
	break;
	case 1:
		lineWidth++;
		break;
	default:
		break;
	}
}

void createGLUTMenus() {
	int menu;
	menu = glutCreateMenu(processMenuEvents);

	glutAddMenuEntry("Point Size+", 0);
	glutAddMenuEntry("Line Width+", 1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void myDisplay() 
{
	glEnable(GL_POLYGON_SMOOTH_HINT);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawBeizerLine();

	glFlush();
	glutSwapBuffers();
}

void myIdle()
{
	myDisplay();
}

void myInit() {
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

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_MULTISAMPLE);

	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Bezier Curve");
	myInit();

	glutDisplayFunc(&myDisplay);
	glutIdleFunc(&myIdle);

	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMotion);

	createGLUTMenus();

	glutMainLoop();
	return 0;
}