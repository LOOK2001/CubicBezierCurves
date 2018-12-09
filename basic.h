#pragma once

#include <iostream>
#include <vector>
#include <math.h>

using namespace std;

#define vertexRadius 4.f
#define TriangleSize 12.0f

/********************************************************************
Struct:		 point
Description: each point contains x, y coordinate
********************************************************************/
struct Point {
public:
	Point() : x(0.0f),y(0.0f){}
	~Point(){}

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
	}
};

/********************************************************************
Struct:		 triangle
Description: each triangle contains three points
********************************************************************/
struct Triangle
{
public:
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

/********************************************************************
Struct:		 line
Description: each line contains a set of point, and the origin point and the end point of the line,
and a triangle which display at end point of the line
********************************************************************/
struct  Line
{
public:
	vector<Point> allPoint;

	Point startPoint;
	Point endPoint;
	Triangle endTriangle;

	void setSegPoint(Point _p1, Point _p2)
	{
		startPoint = _p1;
		endPoint = _p2;
	}

	// compute the control point of the cubic-bezier line
	void computeBeizerCtrlPoint(vector<Point>& _points)
	{
		// Four points would decide the shape of cubic-bezier line
		Point bPoint[4];
		bPoint[0] = _points[0];
		bPoint[1] = _points[1];

		// compute the slope K of the line
		float tanX = 0;
		float tanY = 0;
		float k;
		tanX = abs(bPoint[0].x - bPoint[1].x);
		tanY = abs(bPoint[0].y - bPoint[1].y);
		k = tanY / tanX;

		bPoint[3] = bPoint[1];

		// if the slope K <= 1.0, then drawing the end triangle horizontally
		if (k <= 1.0)
		{
			// The triangle toward left or right
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
		// if the slope K > 1.0, then drawing the end triangle vertically
		else
		{
			// The triangle toward up or down
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

	// generalize all cubic-bezier point through control point
	void bezierGeneralized(vector<Point> ctrlPoints)
	{
		Point p1 = ctrlPoints[0];
		Point p2;
		Line tempLine;
		allPoint.clear();
		tempLine.setSegPoint(ctrlPoints[0], ctrlPoints[3]);
		allPoint.push_back(p1);

		// compute each segment of the curve.Make t increment in smaller amounts for a more detailed curve.
		for (double t = 0.0; t <= 1.02; t += 0.02)
		{
			p2 = _computeBezierPoints(ctrlPoints, t);
			p1 = p2;
			allPoint.push_back(p2);
		}
		int i = 1;
	}

private:
	Point _computeBezierPoints(vector<Point> PT, double t)
	{
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