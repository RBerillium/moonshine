#pragma once

#include "../main.hpp"

class vec3
{
public:
	vec3()
	{
		x = y = z = 0.f;
	}

	vec3(float fx, float fy, float fz)
	{
		x = fx;
		y = fy;
		z = fz;
	}

	float x, y, z;

	vec3 operator+(const vec3& input) const
	{
		return vec3{ x + input.x, y + input.y, z + input.z };
	}

	vec3 operator-(const vec3& input) const
	{
		return vec3{ x - input.x, y - input.y, z - input.z };
	}

	vec3 operator/(float input) const
	{
		return vec3{ x / input, y / input, z / input };
	}

	vec3 operator*(float input) const
	{
		return vec3{ x * input, y * input, z * input };
	}

	vec3& operator+=(const vec3& input)
	{
		x += input.x;
		y += input.y;
		z += input.z;

		return *this;
	}

	vec3& operator-=(const vec3& input)
	{
		x -= input.x;
		y -= input.y;
		z -= input.z;

		return *this;
	}

	vec3& operator/=(float input)
	{
		x /= input;
		y /= input;
		z /= input;
		return *this;
	}

	vec3& operator*=(float input)
	{
		x *= input;
		y *= input;
		z *= input;
		return *this;
	}

	bool operator==(const vec3& input) const
	{
		return x == input.x && y == input.y && z == input.z;
	}

	bool operator!=(const vec3& input) const
	{
		return x != input.x && y != input.y && z != input.z;
	}

	float length_sqr() const
	{
		return (x * x) + (y * y) + (z * z);
	}

	float length() const
	{
		return (float)sqrt(length_sqr());
	}

	float distance(vec3 input) const
	{
		return (*this - input).length();
	}

	float dot(vec3 input) const
	{
		return (x * input.x) + (y * input.y) + (z * input.z);
	}

	vec3 normalize()
	{
		vec3 out = *this;
		auto len = length();
		if (!len)
			return *this;

		out.x /= len;
		out.y /= len;
		out.z /= len;
		return out;
	}

	vec3 cross(vec3 rhs)
	{
		return vec3(y * rhs.z - z * rhs.y, z * rhs.x - x * rhs.z, x * rhs.y - y * rhs.x);
	}

	float unity_magnitude()
	{
		return (float)sqrt((double)(x * x + y * y + z * z));
	}

	vec3 unity_normalize()
	{
		float num = unity_magnitude();
		if (num > 1E-05f)
		{
			x /= num;
			y /= num;
			z /= num;
		}
		else
		{
			x = 0;
			y = 0;
			z = 0;
		}

		return { x,y,z };
	}

	vec3 normalized() const
	{
		return { x == 0 ? 0 : x / length(), y == 0 ? 0 : y / length(), z == 0 ? 0 : z / length() };
	}
};