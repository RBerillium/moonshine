#pragma once

#include "../main.hpp"

class vec4
{
public:
	vec4()
	{
		x = y = z = w = 0.f;
	}

	vec4(float fx, float fy, float fz, float fw)
	{
		x = fx;
		y = fy;
		z = fz;
		w = fw;
	}

	float x, y, z, w;

	vec4 operator+(const vec4& input) const
	{
		return vec4{ x + input.x, y + input.y, z + input.z, w + input.w };
	}

	vec4 operator-(const vec4& input) const
	{
		return vec4{ x - input.x, y - input.y, z - input.z, w - input.w };
	}

	vec4 operator/(float input) const
	{
		return vec4{ x / input, y / input, z / input, w / input };
	}

	vec4 operator*(float input) const
	{
		return vec4{ x * input, y * input, z * input, w * input };
	}

	vec4& operator+=(const vec4& input)
	{
		x += input.x;
		y += input.y;
		z += input.z;
		w += input.w;

		return *this;
	}

	vec4& operator-=(const vec4& input)
	{
		x -= input.x;
		y -= input.y;
		z -= input.z;
		w -= input.w;

		return *this;
	}

	vec4& operator/=(float input)
	{
		x /= input;
		y /= input;
		z /= input;
		w /= input;
		return *this;
	}

	vec4& operator*=(float input)
	{
		x *= input;
		y *= input;
		z *= input;
		w *= input;
		return *this;
	}

	bool operator==(const vec4& input) const
	{
		return x == input.x && y == input.y && z == input.z && w == input.w;
	}
};