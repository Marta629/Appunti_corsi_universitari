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
#include <list>
#include <vector>
#include <SFML/System.hpp>

#include "world_grid.hpp"
#include "utils.hpp"
#include "wall.hpp"
#include "grid.hpp"
#include "ant_mode.hpp"
#include "world_renderer.hpp"
#include "light.hpp"


struct World
{
	sf::Vector2f size;
	WorldGrid markers;
	DoubleObject<sf::VertexArray> va_markers;
	DoubleObject<sf::VertexArray> va_walls;
	WorldRenderer renderer;

	World(uint32_t width, uint32_t height)
		: markers(width, height, 4)
		, size(to<float>(width), to<float>(height))
		, renderer(markers, va_markers)
	{
		for (int32_t x(0); x < markers.width; x++) {
			for (int32_t y(0); y < markers.height; y++) {
				if (x == 0 || x == markers.width - 1 || y == 0 || y == markers.height - 1) {
					markers.get(sf::Vector2i(x, y)).wall = 1;
				}
			}
		}
	}

	void update(float dt)
	{
		markers.update(dt);
	}

	void addMarker(sf::Vector2f pos, Mode type, float intensity, bool permanent = false)
	{
		markers.addMarker(pos, type, intensity, permanent);
	}

	void addWall(const sf::Vector2f& position)
	{
		if (markers.checkCoords(position)) {
			markers.get(position).wall = 1;
		}
	}

	void removeWall(const sf::Vector2f& position)
	{
		if (markers.checkCoords(position)) {
			markers.get(position).wall = 0;
		}
	}

	void renderMarkers(sf::RenderTarget& target, sf::RenderStates states)
	{
		states.texture = &(*Conf::MARKER_TEXTURE);
		renderer.mutex.lock();
		target.draw(va_markers.getCurrent(), states);
		renderer.mutex.unlock();
	}

	void addFoodAt(float x, float y, uint32_t quantity)
	{
		const sf::Vector2f pos(x, y);
		if (markers.checkCoords(pos)) {
			addMarker(pos, Mode::ToFood, 1.0f, true);
			markers.addFood(pos, quantity);
		}
	}

	// get the ammount of food left in the scene
	uint32_t getFoodCount()
	{
		return markers.food_count;
	}

	// add a light cell just like for the walls
	void addLight(const sf::Vector2f& position)
	{
		if (markers.checkCoords(position)) {
			markers.get(position).light = 1;
		}
	}
};
