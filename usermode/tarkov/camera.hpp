#pragma once

#include "offset.hpp"

#include "../main.hpp"

#include "../unity/vector2.hpp"

#include "../unity/vector3.hpp"

#include "../main/memory_system.hpp"

#include "game.hpp"

namespace memory = memory_system;



namespace camera
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

	matrix_t cached_matrix{};

	//matrix_t main_matrix;

	//void update_matrix(u64 camera)
	//{
	//	main_matrix = memory::read<matrix_t>(camera + offset::camera::matrix);
	//}
	void cache_matrix()
	{
		cached_matrix = memory::read<matrix_t>(game::camera + offset::camera::matrix);
	}
	vec2 w2s(vec3 world)
	{
		
		matrix_t matrix{};

		matrix = cached_matrix;
		vec3 trans_vec{ matrix._14, matrix._24, matrix._34 };
		vec3 right_vec{ matrix._11, matrix._21, matrix._31 };
		vec3 up_vec{ matrix._12, matrix._22, matrix._32 };

		f32 w = trans_vec.dot(world) + matrix._44;

		if (w < 0.098f) return vec2(0, 0);

		f32 x = right_vec.dot(world) + matrix._41;

		f32 y = up_vec.dot(world) + matrix._42;

		vec2 buffer = vec2((screen_x / 2) * (1 + x / w), (screen_y / 2) * (1 - y / w));

		if (!buffer.y || !buffer.x || buffer.x == screen_x || buffer.y == screen_y)
		{
			return vec2(0, 0);
		}
		//if (zoom > 1.0f)
		//{
		//	buffer.x = buffer.x * zoom;
		//	buffer.y = buffer.y * zoom;
		//}
		return buffer;
	}

	//static vec2 WorldToScreen(vec3 origin, matrix_t gameViewMatrix, matrix_t scopeViewMatrix)
	//{
	//	try
	//	{
	//		matrix_t transposed;
	//		if (World.IsAimingCamera &&
	//			(World.OpticCameraActive && World.LocalPlayerZoom > 1.0f)
	//		{
	//			transposed = Matrix.Transpose(scopeViewMatrix);
	//		}
	//		else
	//		{
	//			transposed = Matrix.Transpose(gameViewMatrix);
	//		}

	//		var w = Vector3.Dot(transposed.TranslationVector, origin) + transposed.M44;

	//			if (w < 0.5f) return Vector2.Zero;

	//		var y = Vector3.Dot(transposed.Up, origin) + transposed.M24;
	//		var x = Vector3.Dot(transposed.Right, origin) + transposed.M14;

	//		if (World.IsAimingCamera &&
	//			(World.OpticCameraActive && World.LocalPlayerZoom > 1.0f)
	//		{
	//			var fov = World.CameraFov;
	//				var angleRadHalf = (float)(Math.PI / 180f) * fov * 0.5f;
	//				var angleCtg = (float)(Math.Cos(angleRadHalf) / Math.Sin(angleRadHalf));
	//				x /= angleCtg * World.AspectRatio * 0.5f;
	//				y /= angleCtg * 0.5f;
	//		}

	//		var screenX = H.ViewPortX / 2f * (1f + x / w);
	//			var screenY = H.ViewPortY / 2f * (1f - y / w);
	//		return new Vector2(screenX, screenY);
	//	}
	//	catch (Exception e)
	//	{
	//		if (Debug) MainCon.AddConsoleOutput($@"[W2S] Exception Thrown: {e}");

	//		return new Vector2(0, 0);
	//	}
	//}

}