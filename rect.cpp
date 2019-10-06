
#pragma once

#include <GL/glew.h>
#include <GL/GL.h>

#include "rect.h"

namespace Game
{

	Rect::Rect() : VAO(
		GL_TRIANGLES,
		CNGE7::Attribute::create_vertex_attribute(12, new float[12]{
			0, 0, 0,
			0, 1, 0,
			1, 1, 0,
			1, 0, 0
			}),
		6,
		new int[6]{
			0, 3, 2,
			0, 2, 1
		},
		1,
		new CNGE7::Attribute[1]
		{
			CNGE7::Attribute(2, 8, new float[8] {
				0, 1,
				0, 0,
				1, 0,
				1, 1
			})
		}
	) {}


	Player_Rect::Player_Rect() : VAO(
		GL_TRIANGLES,
		CNGE7::Attribute::create_vertex_attribute(12, new float[12]{
			-.5, 0, 0,
			.5, 0, 0,
			.5, 1, 0,
			-.5, 1, 0
			}),
		6,
		new int[6]{
			0, 1, 2,
			0, 2, 3
		},
		1,
		new CNGE7::Attribute[1]
		{
			CNGE7::Attribute(2, 8, new float[8] {
				0, 1,
				1, 1,
				1, 0,
				0, 0
			})
		}
	) {}

}
