/*=========================================================================

Program:   Curves_01

This program is about an implementation of Cubic Bezier Curves 
in C++ using OpenGL, glut.

=========================================================================*/
#include <iostream>
#include <gl/glut.h>
#include <math.h>
#include <vector>
#include "basic.h"

using namespace std;

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

float lineWidth = 2.0f;
float pointSize = 6.0f;

/********************************************************************
Function:	 drawDot
Description: draw a point which located (x,y)
Parameter:	 X-Coordinate and Y-Coordinate
Returns:	 void
********************************************************************/
void drawDot(float x, float y);
void drawDot(Point _point);

/********************************************************************
Function:	 drawLine
Description: draw a line with two points
Parameter:	 starting point and end point
Returns:	 void
********************************************************************/
void drawLine(Point p1, Point p2);

/********************************************************************
Function:	 drawTriangle
Description: draw a triangle with three points
Parameter:	 starting point and end point
Returns:	 void
********************************************************************/
void drawTriangle(Point _p1, Point _p2, Point _p3);
void drawTriangle(Point p, float topX, float topY, float offsetXL, float offsetXR, float offsetYL, float offsetYR);

int clickNum = 0;			// the number of click
Point startPoint;			// the point of the first click
Point endPoint;				// the point of the second click
vector<Line> m_vAllLine;	

void drawDot(float x, float y) {
	glPointSize(pointSize);
	glBegin(GL_POINTS);
	glColor3f(0.0, 0.0, 1.0);
	glVertex2f(x, y);
	glEnd();
	glFlush();
}

void drawDot(Point _point) {
	glPointSize(pointSize);
	glBegin(GL_POINTS);
	glColor3f(0.0, 0.0, 1.0);
	glVertex2f(_point.x, _point.y);
	glEnd();
	glFlush();
}

void drawLine(Point p1, Point p2) {
	// enable anti-aliasing
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