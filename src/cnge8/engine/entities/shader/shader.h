
#pragma once

#include <GL/glew.h>
#include <gl/GL.h>

#include "cnge8/types.h"
#include "cnge8/load/resource.h"

namespace CNGE {
	class Shader : public Resource {
	public:
		constexpr static char const* MODEL_NAME = "model";
		constexpr static char const* PROJVIEW_NAME = "projView";

		Shader(const char* vertexPath, const char* fragmentPath);

		auto getUniform(const char* name) -> i32;

		/// start using this shader
		/// after calling this then give other things
		auto enable(f32 model[], f32 projview[]) -> void;

		/// enables the shader and covers the entire ndc coordinates
		auto enable() -> void;

		/*
		 * GIVERS
		 */

		auto giveFloat(int, float) -> void;
		auto giveVector2(int, float, float) -> void;
		auto giveVector3(int, float, float, float) -> void;
		auto giveVector4(int, float, float, float, float) -> void;
		auto giveVector4(int, const float[]) -> void;
		auto giveMatrix4(int, const float[]) -> void;

		~Shader();

	protected:
		virtual auto getUniforms() -> void = 0;

	private:
		/* loading */
		const char* vertexPath;
		const char* fragmentPath;

		char* vertexData;
		char* fragmentData;

		auto customGather() -> LoadError override;
		auto customProcess() -> LoadError override;
		auto customDiscard() -> LoadError override;
		auto customUnload() -> LoadError override;

		static auto printShaderError(GLuint) -> void;

		/* use */
		GLuint program;

		GLint modelLocation;
		GLint projviewLocation;
	};

}
