#pragma once

namespace Game
{

	class World
	{
	public:

		World(
			int _width,
			int _height,
			CNGE7::Camera3D* _camera3d,
			CNGE7::Texture* grass_texture,
			CNGE7::Texture* bush_texture,
			Color_Shader* _color_shader,
			Fog_Shader* _fog_shader,
			Rect* _rect,
			Player_Rect* _player_rect
		);

		~World();

		int** get();

		void render(
			int _x,
			int _y,
			glm::vec3 &player_pos,
			Camera_Control*,
			int radius,
			float base_height);

	private:

		/// makes sure this position doesn't go over the edge
		int limit(int in, int bound);

		CNGE7::Transform3D transform3d;
		CNGE7::Camera3D* camera3d;
		CNGE7::Texture* grass_texture;
		CNGE7::Texture* bush_texture;
		Color_Shader* color_shader;
		Fog_Shader* fog_shader;
		Rect* rect;
		Player_Rect* player_rect;

		int width;
		int height;

		int** world;

	};
}
