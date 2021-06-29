#include <SFML/Graphics.hpp>
#include <vector>
#include <list>
#include <iostream>
#include "colony.hpp"
#include "config.hpp"
#include "display_manager.hpp"
#include <chrono>
#include <fstream>
#include <string>

const std::string fileName = "CircularLight4_log.txt";
const float dt = 0.016f;

void loadUserConf()
{
	std::ifstream conf_file("conf.txt");
	if (conf_file) {
		conf_file >> Conf::WIN_WIDTH;
		conf_file >> Conf::WIN_HEIGHT;
		conf_file >> Conf::ANTS_COUNT;
	}
	else {
		std::cout << "Couldn't find 'conf.txt', loading default" << std::endl;
	}
}

int execute() // int main()
{
	Conf::loadTextures();
	loadUserConf();

	sf::ContextSettings settings;
	settings.antialiasingLevel = 4;
	sf::RenderWindow window(sf::VideoMode(Conf::WIN_WIDTH, Conf::WIN_HEIGHT), "AntSim", sf::Style::Fullscreen, settings);
	window.setFramerateLimit(60);
	
	World world(Conf::WORLD_WIDTH, Conf::WORLD_HEIGHT);
	Colony colony(Conf::COLONY_POSITION.x, Conf::COLONY_POSITION.y, Conf::ANTS_COUNT);
	for (uint32_t i(0); i < 64; ++i) {
		float angle = float(i) / 64.0f * (2.0f * PI);
		world.addMarker(colony.position + 16.0f * sf::Vector2f(cos(angle), sin(angle)), Mode::ToHome, 10.0f, true);
	}
	
	DisplayManager display_manager(window, window, world, colony);
	sf::Vector2f last_clic;

	sf::Image preset_map;
	if (preset_map.loadFromFile("presets/CircularExpLight4.png")) { // presets/RectangularSimpleExpLight.png
		for (uint32_t x(0); x < preset_map.getSize().x; ++x) {
			for (uint32_t y(0); y < preset_map.getSize().y; ++y) {
				// const sf::Vector2f position = float(world.markers.cell_size) * sf::Vector2f(to<float>(x), to<float>(y));
				const sf::Vector2f position = sf::Vector2f(to<float>(x), to<float>(y));
				if (preset_map.getPixel(x, y).b > 100) {
					world.addWall(position);
				}
				else if (preset_map.getPixel(x, y).r > 100){
					world.addLight(position);
				}
				else if (preset_map.getPixel(x, y).g > 100 && preset_map.getPixel(x, y).b > 40) {
					world.addFoodAt(position.x, position.y, 5);
				}
			}
		}
	}

	// first ant to reach the food
	bool foundFood = false;
	// initial food quantity
	uint32_t initFood = world.getFoodCount();
	// food left
	uint32_t foodLeft = 0;
	// I/O file strem
	std::ofstream logFile;
	float time = 0.0f;
	// start time counter
	auto start = std::chrono::high_resolution_clock::now();
	while (window.isOpen())
	{
		display_manager.processEvents();
		// Add food on clic
		if (display_manager.clic) {
			const sf::Vector2i mouse_position = sf::Mouse::getPosition(window);
			const sf::Vector2f world_position = display_manager.displayCoordToWorldCoord(sf::Vector2f(to<float>(mouse_position.x), to<float>(mouse_position.y)));
			const float clic_min_dist = 1.5f;
			if (getLength(world_position - last_clic) > clic_min_dist) {
				if (display_manager.wall_mode) {
					world.addWall(world_position);
				}
				else if (display_manager.remove_wall) {
					world.removeWall(world_position);
				}
				else {
					world.addFoodAt(world_position.x, world_position.y, 5); // original quantity = 5
					foodLeft += 5;
				}
				last_clic = world_position;
			}
		}


		// CHECK FOR FOOD CONSUMPTION, WRITE ON FILE, END SIMULATION
		foodLeft = world.getFoodCount();
		if (!foundFood){
			if (foodLeft < initFood){
				foundFood = true;
				auto stop = std::chrono::high_resolution_clock::now(); 
				auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start); 
				time = duration.count() / 60.0f; // seconds to minutes;
				logFile.open(fileName, std::ios::app);
				logFile << "Uscita raggiunta in : " + std::to_string(time) + "min\t";
			}
		}

		if(foodLeft < static_cast<uint32_t>(initFood * 0.33f)){
			auto stop = std::chrono::high_resolution_clock::now(); 
			auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start); 
			time = duration.count() / 60.0f; // seconds to minutes;
			logFile << "Cibo esaurito in : " + std::to_string(time) + "min\n";
			logFile.close();

			return 0;
		}

		if (!display_manager.pause) {
			colony.update(dt, world);
			world.update(dt);
		}

		window.clear(sf::Color(94, 87, 87));
		
		display_manager.draw();

		window.display();
	}

	Conf::freeTextures();
	logFile.close();
	return 0;
}

void main(){
	// for (uint32_t i = 0; i < 10; i++){
	// 	execute();
	// }
	execute();
}

#if defined(_WIN32)
#include <windows.h>
int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow) {
	return main();
}
#endif