
#include <iostream>
#include "transform.h"

namespace CNGE7
{
	float Transform::values[16] {};

	float Transform::default_model[16]
	{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	float Transform::default_projview[16]
	{
		2, 0, 0, 0,
		0, 2, 0, 0,
		0, 0, 1, 0,
		-1, -1, 0, 1
	};

}
