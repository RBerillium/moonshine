#pragma once

#include "../main.hpp"

#include "vector3.hpp"

#include "vector4.hpp"

#include "../main/memory_system.hpp"

namespace memory = memory_system;

namespace unity_transform
{
	struct matrix_t
	{
		union
		{
			struct
			{
				f32 _11, _12, _13, _14;
				f32 _21, _22, _23, _24;
				f32 _31, _32, _33, _34;
				f32 _41, _42, _43, _44;
			};
			f32 m[4][4];
		};
	};

	struct transform_access_t
	{
		u64 transform_data{};
	};

	struct matrix34_t
	{
		vec4 vec0{}; vec4 vec1{}; vec4 vec2{};
	};

	struct transform_data_t
	{
		u64 transform_array{};
		u64 transform_indices{};
	};

	vec3 to_vec3(u64 transform)
	{
		u64 transform_internal = memory::read<u64>(transform + 0x10);

		__m128 result{};

		const __m128 mulVec0 = { -2.000, 2.000, -2.000, 0.000 };
		const __m128 mulVec1 = { 2.000, -2.000, -2.000, 0.000 };
		const __m128 mulVec2 = { -2.000, -2.000, 2.000, 0.000 };

		transform_access_t transform_access_read_only = memory::read<transform_access_t>(transform_internal + 0x38);
		transform_data_t transform_data = memory::read<transform_data_t>(transform_access_read_only.transform_data + 0x18);
		u32 index = memory::read<u32>(transform_internal + 0x40);

		if (transform_data.transform_array && transform_data.transform_indices)
		{
			result = memory::read<__m128>(transform_data.transform_array + 0x30 * index);
			i32 transform_index = memory::read<i32>(transform_data.transform_indices + 0x4 * index); i32 safe = 0;

			while (transform_index >= 0 && safe++ < 200)
			{
				matrix34_t matrix = memory::read<matrix34_t>(transform_data.transform_array + 0x30 * transform_index);

				__m128 xxxx = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix.vec1), 0x00));	
				__m128 yyyy = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix.vec1), 0x55));	
				__m128 zwxy = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix.vec1), 0x8E));	
				__m128 wzyw = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix.vec1), 0xDB));	
				__m128 zzzz = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix.vec1), 0xAA));	
				__m128 yxwy = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix.vec1), 0x71));	
				__m128 tmp7 = _mm_mul_ps(*(__m128*)(&matrix.vec2), result);

				result = _mm_add_ps(_mm_add_ps(
					_mm_add_ps(
						_mm_mul_ps(
							_mm_sub_ps(
								_mm_mul_ps(_mm_mul_ps(xxxx, mulVec1), zwxy),
								_mm_mul_ps(_mm_mul_ps(yyyy, mulVec2), wzyw)),
							_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0xAA))),
						_mm_mul_ps(
							_mm_sub_ps(
								_mm_mul_ps(_mm_mul_ps(zzzz, mulVec2), wzyw),
								_mm_mul_ps(_mm_mul_ps(xxxx, mulVec0), yxwy)),
							_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0x55)))),
					_mm_add_ps(
						_mm_mul_ps(
							_mm_sub_ps(
								_mm_mul_ps(_mm_mul_ps(yyyy, mulVec0), yxwy),
								_mm_mul_ps(_mm_mul_ps(zzzz, mulVec1), zwxy)),
							_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0x00))),
						tmp7)), *(__m128*)(&matrix.vec0));

				transform_index = memory::read<i32>(transform_data.transform_indices + 0x4 * transform_index);
			}
		}

		auto m128_to_vec3 = [&](__m128& v)
		{
			union union128
			{
				__m128 v;   
				float a[4];
			};

			union128 u; u.v = v;
			return vec3(u.a[0], u.a[1], u.a[2]);
		};

		return (m128_to_vec3(result));

		//return vec3(result.m128_f32[0], result.m128_f32[1], result.m128_f32[2]);
	}
}