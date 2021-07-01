/*
MIT License

Copyright (c) 2021 Jean Tampon

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
*/

#pragma once
#include "async_va_renderer.hpp"
#include "grid.hpp"
#include "config.hpp"
#include "world_grid.hpp"


struct WorldRenderer : public AsyncRenderer
{
	const Grid<WorldCell>& grid;
	bool draw_markers;

	WorldRenderer(Grid<WorldCell>& grid_, DoubleObject<sf::VertexArray>& target)
		: AsyncRenderer(target)
		, grid(grid_)
		, draw_markers(true)
	{
		AsyncRenderer::start();
	}

	void initializeVertexArray(sf::VertexArray& va) override
	{
		va = sf::VertexArray(sf::Quads, grid.width * grid.height * 4);
		uint64_t i = 0;
		const float cell_size = to<float>(grid.cell_size);
		for (int32_t x(0); x < grid.width; x++) {
			for (int32_t y(0); y < grid.height; y++) {
				const sf::Vector2f position(x * cell_size, y * cell_size);
				va[4 * i + 0].position = position;
				va[4 * i + 1].position = position + sf::Vector2f(cell_size, 0.0f);
				va[4 * i + 2].position = position + sf::Vector2f(cell_size, cell_size);
				va[4 * i + 3].position = position + sf::Vector2f(0.0f, cell_size);
				++i;
			}
		}
	}

	void updateVertexArray() override
	{
		sf::VertexArray& va = vertex_array.getLast();
		const sf::Vector3f to_home_color(Conf::TO_HOME_COLOR.r / 255.0f, Conf::TO_HOME_COLOR.g / 255.0f, Conf::TO_HOME_COLOR.b / 255.0f);
		const sf::Vector3f to_food_color(Conf::TO_FOOD_COLOR.r / 255.0f, Conf::TO_FOOD_COLOR.g / 255.0f, Conf::TO_FOOD_COLOR.b / 255.0f);
		const sf::Vector3f light_color(Conf::LIGHT_COLOR.r / 255.0f, Conf::LIGHT_COLOR.g / 255.0f, Conf::LIGHT_COLOR.b / 255.0f);

		uint64_t i = 0;
		const float cell_size = to<float>(grid.cell_size);
		for (int32_t x(0); x < grid.width; x++) {
			for (int32_t y(0); y < grid.height; y++) {
				const auto& cell = grid.getCst(sf::Vector2i(x, y));
				sf::Color color = sf::Color::Black;
				if (!cell.food && !cell.wall && draw_markers) {
					sf::Vector3f mixed_color(0.0f, 0.0f, 0.0f);
					const float intensity_factor = 0.27f;
					const sf::Vector3f intensity_1_color = intensity_factor * to_home_color * cell.intensity[0];
					const sf::Vector3f intensity_2_color = intensity_factor * to_food_color * cell.intensity[1];
					if (cell.light){
						const sf::Vector3f intensity_3_color = intensity_factor * light_color * ((cell.intensity[0] + cell.intensity[1]) / 2.0f);
						mixed_color.x = std::min(255.0f, intensity_1_color.x + intensity_2_color.x + intensity_3_color.x);
						mixed_color.y = std::min(255.0f, intensity_1_color.y + intensity_2_color.y + intensity_3_color.y);
						mixed_color.z = std::min(255.0f, intensity_1_color.z + intensity_2_color.z + intensity_3_color.z);
					}
					else{
						mixed_color.x = std::min(255.0f, intensity_1_color.x + intensity_2_color.x);
						mixed_color.y = std::min(255.0f, intensity_1_color.y + intensity_2_color.y);
						mixed_color.z = std::min(255.0f, intensity_1_color.z + intensity_2_color.z);
					}
					color = sf::Color(sf::Color(to<uint8_t>(mixed_color.x), to<uint8_t>(mixed_color.y), to<uint8_t>(mixed_color.z)));
					const float offset = 32.0f;
					va[4 * i + 0].texCoords = sf::Vector2f(offset, offset);
					va[4 * i + 1].texCoords = sf::Vector2f(100.0f - offset, offset);
					va[4 * i + 2].texCoords = sf::Vector2f(100.0f - offset, 100.0f - offset);
					va[4 * i + 3].texCoords = sf::Vector2f(offset, 100.0f - offset);
				}
				else if (cell.food) {
					color = Conf::FOOD_COLOR;
					const float offset = 4.0f;
					va[4 * i + 0].texCoords = sf::Vector2f(100.0f + offset, offset);
					va[4 * i + 1].texCoords = sf::Vector2f(200.0f - offset, offset);
					va[4 * i + 2].texCoords = sf::Vector2f(200.0f - offset, 100.0f - offset);
					va[4 * i + 3].texCoords = sf::Vector2f(100.0f + offset, 100.0f - offset);
				}
				else if (cell.wall) {
					color = Conf::WALL_COLOR;
					const float offset = 4.0f;
					va[4 * i + 0].texCoords = sf::Vector2f(200.0f + offset, offset);
					va[4 * i + 1].texCoords = sf::Vector2f(300.0f - offset, offset);
					va[4 * i + 2].texCoords = sf::Vector2f(300.0f - offset, 100.0f - offset);
					va[4 * i + 3].texCoords = sf::Vector2f(200.0f + offset, 100.0f - offset);
				}
				else if (cell.light) {
					color = Conf::LIGHT_COLOR;
					const float offset = 4.0f;
					va[4 * i + 0].texCoords = sf::Vector2f(200.0f + offset, offset);
					va[4 * i + 1].texCoords = sf::Vector2f(300.0f - offset, offset);
					va[4 * i + 2].texCoords = sf::Vector2f(300.0f - offset, 100.0f - offset);
					va[4 * i + 3].texCoords = sf::Vector2f(200.0f + offset, 100.0f - offset);
				}
				va[4 * i + 0].color = color;
				va[4 * i + 1].color = color;
				va[4 * i + 2].color = color;
				va[4 * i + 3].color = color;
				++i;
			}
		}
	}
};
