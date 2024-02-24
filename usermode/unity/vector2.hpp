#pragma once

#include "../main.hpp"

class vec2
{
public:
	vec2()
	{
		x = y = 0;
	};

	vec2(float fx, float fy)
	{
		x = fx;
		y = fy;
	};

	float x, y;

	vec2 operator+(const vec2& input)
	{
		return vec2{ x + input.x, y + input.y };
	}

	vec2& operator*=(float input)
	{
		x *= input;
		y *= input;
		return *this;
	}

	float distance(vec2 input)
	{
		return sqrt(pow(input.x - x, 2) + pow(input.y - y, 2));
	}

	vec2 normalize() {
		float l = x * x + y * y;
		if (l != 0) {
			l = sqrt(l);
			x /= l;
			y /= l;
		}
		return {x,y };
	}
};