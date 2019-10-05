#pragma once

//#include "vao.h"

namespace CNGE7
{
	class Rect : public VAO
	{
	public:
		Rect() : VAO(
			GL_TRIANGLES,
			Attribute::create_vertex_attribute(12, new float[12]
				{
					0, 0, 0,
					0, 1, 0,
					1, 1, 0,
					1, 0, 0
				}
			),
			6, new int[6]
			{
				0, 1, 2,
				2, 3, 0
			},
			0, nullptr
		) {}
	};
}
