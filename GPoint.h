#pragma once

#include <math.h>
#include <atltypes.h>


class GPoint
{
public:
	double x;
	double y;

public:
	GPoint(const CPoint& _pt) :x(_pt.x), y(_pt.y) {};
	GPoint(double _x, double _y) :x(_x), y(_y) {};
	GPoint(const GPoint& _pt) :x(_pt.x), y(_pt.y) {};
	GPoint() :x(0.), y(0.) {};

	// simple operators
	GPoint& operator= (const GPoint& _pt);
	GPoint& operator-=(const GPoint& _pt);
	GPoint& operator+=(const GPoint& _pt);
	GPoint operator-(const GPoint& _pt) const;
	GPoint operator+(const GPoint& _pt) const;
	GPoint operator*(float _ff) const;
	friend GPoint operator*(float _ff, const GPoint& _pt);


	bool operator==(const GPoint& _pt);
	bool operator!=(const GPoint& _pt);

	// distance from a point to another
	double dist2Point(const GPoint& _pt);

	virtual ~GPoint();
};

