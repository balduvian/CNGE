#pragma once

namespace Game
{
	class Cube : public CNGE7::VAO
	{
	public:
		Cube() : VAO(
			GL_TRIANGLES,
			CNGE7::Attribute::create_vertex_attribute(72, new float[72]{
				// front face
				0, 0, 0, // 0
				1, 0, 0, // 1
				1, 1, 0, // 2
				0, 1, 0, // 3

				// right face
				1, 0, 0, // 1
				1, 0, 1, // 5
				1, 1, 1, // 6
				1, 1, 0, // 2

				// back face
				1, 0, 1, // 5
				0, 0, 1, // 4
				0, 1, 1, // 7
				1, 1, 1, // 6

				// left face
				0, 0, 1, // 4
				0, 0, 0, // 0
				0, 1, 0, // 3
				0, 1, 1, // 7

				// top face
				0, 1, 0, // 3
				1, 1, 0, // 2
				1, 1, 1, // 6
				0, 1, 1, // 7

				// bottom face
				1, 0, 1, // 5
				1, 0, 0, // 1
				0, 0, 0, // 0
				0, 0, 1, // 4
				}),
			36,
			new int[36]{
				0, 1, 2,
				2, 3, 0,

				4, 5, 6,
				6, 7, 4,

				8, 9, 10,
				10, 11, 8,

				12, 13, 14,
				14, 15, 12,

				16, 17, 18,
				18, 19, 16,

				20, 21, 22,
				22, 23, 20
			},
			0,
			nullptr
		) {}
	};
}
