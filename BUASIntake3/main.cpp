#include <fstream>
#include <iostream>  // For std::cerr and std::cout
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Level.h"
#include <filesystem>

void verifyFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "File does not exist or cannot be opened: " << filepath << "\n";
        throw std::runtime_error("File verification failed");
    }
    std::cout << "File verified: " << filepath << "\n";
    file.close();
}

std::string readFile(const std::string& filepath)
{
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filepath);
    }
    std::string content((std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>());
    file.close();
    return content;
}


int main() {  
  sf::RenderWindow window(sf::VideoMode(800, 600), "Save the princess");  
  Player player;  
  Level level("Assets/BG/Levels.ldtk");
  sf::Clock clock;  

 /* try {
      std::ifstream file("Assets/BG/Levelsmin.ldtk");
      if (!file.is_open()) {
          throw std::runtime_error("Cannot open file: Assets/BG/Levelsmin.ldtk");
      }
      file.close();
      std::cout << "File verified: Assets/BG/Levelsmin.ldtk\n";

      Level level("Assets/BG/Levelsmin.ldtk");
      std::cout << "Level loaded successfully.\n";
  }
  catch (const std::exception& e) {
      std::cerr << "Exception: " << e.what() << "\n";
      return 1;
  }*/

  while (window.isOpen()) {  
      sf::Event event;  
      while (window.pollEvent(event)) {  
          if (event.type == sf::Event::Closed)  
              window.close();  
      }  

      float deltaTime = clock.restart().asSeconds();  

      window.clear(sf::Color::Black);  
      level.draw(window);  
      player.update(deltaTime);  
      player.draw(window);  
      window.display();  
  }  

  return 0;  
}
