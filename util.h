#pragma once

namespace CNGE7
{
	void clear(float _r, float _g, float _b, float _a);

	void winding_order(bool _counter_clockwise);

	/// automatically enables cull face
	/// also sets which faces to cull
	void set_cull(bool _front, bool _back);

	void default_enables();

	float noise_map(float _x, float _y);

	float mod(float i, float n);

	extern float PI;

}
