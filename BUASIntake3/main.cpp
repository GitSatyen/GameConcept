#include <fstream>
#include <iostream> 
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Player.h"
#include "Level.h"
#include "Enemy.h"
#include "Princess.h"
#include "Objects.h"
#include "GameStates.h"
#include "LDtkLoader/Project.hpp"

bool paused = false;
bool showInstructions = false;
int main() {
    int SCREEN_WIDTH = 800;
    int SCREEN_HEIGHT = 600;
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Midnight Turns");
    // Set minimum size (will enforce through event handling)
    sf::Vector2u minWindowSize(400, 300);

    Player player;
    Princess princess;
    std::vector<Enemy> enemies;
    std::vector<Objects> objects;
    Level level("Assets/BG/Levels.ldtk", window);
    sf::Clock clock;
    bool isFullscreen = false;
    Status status = Status::Start;

    //auto startScreen;
    
    // Pass references to the Level (containing collission logic)
    level.setPlayer(&player);
    level.setPrincess(&princess);

    // Pass Level to the Player (containing movement logic)
    player.setLevel(level);
    //Set player position of the start entity
    player.setStartPosition(level.getPlayerStartPosition());

    //Do same with princess
    princess.setLevel(level);
    princess.setPosition(level.getPrincessPosition());

    //Deepseek fix
    // Reserve space to avoid reallocation
    enemies.reserve(level.getEnemyPositions().size());
    //Set positions of enemies
    for (const auto& pos : level.getEnemyPositions()) {
        enemies.emplace_back();
        enemies.back().setPosition(pos);
        enemies.back().setLevel(level);
        level.setEnemy(&enemies.back());
    }

    //Do same for objects
    objects.reserve(level.getObjectPositions().size());
    //Set positions of enemies
    for (const auto& pos : level.getObjectPositions()) {
        objects.emplace_back();
        objects.back().setLevel(level);
        objects.back().setPosition(pos);
        level.setObject(&objects.back());
    }

    //Debugging player start spawn point
    sf::Vector2f startPos = level.getPlayerStartPosition();
    std::cout << "Start position: " << startPos.x << ", " << startPos.y << std::endl;
    std::cout << "Player position: " << player.getPosition().x << ", " << player.getPosition().y << std::endl;

    //Deepseek solution
    // Initial game setup function
    auto setupGame = [&]() {
        paused = false;
        level.resetGameState();
        player.setStartPosition(level.getPlayerStartPosition());
        princess.setPosition(level.getPrincessPosition());
        objects.clear();
        enemies.clear(); //Clear existing enemies
        level.clearEnemies();
        level.clearObjects();

        //Recreate enemies from their positions
        enemies.reserve(level.getEnemyPositions().size());
        for (const auto& pos : level.getEnemyPositions()) {
            enemies.emplace_back();
            enemies.back().setPosition(pos);
            enemies.back().setLevel(level);
            level.setEnemy(&enemies.back());
        }
        //Do same for objects
        objects.reserve(level.getObjectPositions().size());
        for (const auto& pos : level.getObjectPositions()) {
            objects.emplace_back();
            objects.back().setPosition(pos);
            objects.back().setLevel(level);
            level.setObject(&objects.back());
        }
    };
    setupGame();

    //Sound properties
    //sf::Music bgMusic;
    //if (!bgMusic.openFromFile("Assets/Sounds/bg-music.mp3")) {
    //    std::cerr << "Error: Could not load background music." << std::endl;
    //}

    //bgMusic.setLoop(true);     // Optional: loop the music
    //bgMusic.setVolume(50);     // Optional: set volume (0-100)
    //bgMusic.play();            // Start playing the music

    //// Keep the program running to hear the musicgithub
    //sf::sleep(sf::seconds(10));

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed) {
                // Start the game
                if (event.key.code == sf::Keyboard::Space && status == Status::Start) {
                    status = Status::Playing;
                }
                //Press R to reset game
                if (event.key.code == sf::Keyboard::R) {
                    setupGame();
                }
                //Press escape to pause/unpause game
                if (event.key.code == sf::Keyboard::Escape) {
                    paused = !paused;
                }
                //Press F to open/close instructions
                if (event.key.code == sf::Keyboard::F && (status == Status::Start || status == Status::Playing || paused)) {
                    showInstructions = !showInstructions;
                }
                //Press Q to quit
                if (event.key.code == sf::Keyboard::Q) {
                    window.close();
                }
        
                //Handle fulscreen mode
                if (event.key.code == sf::Keyboard::F11) {
                    isFullscreen = !isFullscreen;
                    window.close();
                    window.create(isFullscreen ?
                        sf::VideoMode::getDesktopMode() : sf::VideoMode(800, 600),
                        "Save the princess",
                        (isFullscreen ? sf::Style::Fullscreen : sf::Style::Default));
                    window.setVerticalSyncEnabled(true);
                    level.Resize(window);
                    window.setView(level.getGameView());
                }
            }
        }

        float deltaTime = clock.restart().asSeconds();
        // Only update game logic if not in completed state
        if (!level.hasWon && !level.hasLost) {
            // Update level collision
            level.updateCollision(deltaTime);
        }

        switch (status)
        {
        case Status::Start:{ //Handle start screen
                //Black background
                window.clear(sf::Color::Black);
                //Text startscreen
                sf::Text title;
                sf::Text startText;
                sf::Text instructionsOption;
                sf::Font font;
                if (font.loadFromFile("Assets/Fonts/trajan-pro/TrajanPro-Bold.otf"))
                {
                    //Set font
                    title.setFont(font);
                    startText.setFont(font);
                    instructionsOption.setFont(font);
                    //Set text strings
                    title.setString("Midnight Turns");
                    startText.setString("Press SPACE to start!");
                    instructionsOption.setString("Instructions [F]");
                    //Set text size
                    title.setCharacterSize(50);
                    startText.setCharacterSize(24);
                    instructionsOption.setCharacterSize(24);
                    //Set text positions
                    title.setPosition(170, 200);
                    startText.setPosition(250, 500);
                    instructionsOption.setPosition(250, 530);
                    //Set text color
                    title.setFillColor(sf::Color::Red);
                    //Draw text
                    window.draw(title);
                    window.draw(startText);
                    window.draw(instructionsOption);
                }
                //Draw instructions if F is pressed
                if (showInstructions) {
                    level.gameInstructions(window); 
                }
                else {
                    std::cerr << "Font not found\n";
                    //status = Status::Playing;
                }
            }
            break;
        //Display level and game objects and handles other game logic
        case Status::Playing: 
            window.setView(level.getGameView());
            window.clear(sf::Color::Black);
            //Draw map and objects
            level.draw(window);
            player.update(deltaTime);
            player.draw(window);
            princess.update(deltaTime);
            princess.draw(window);
            for (auto& enemy : enemies) {
                enemy.update(deltaTime);
                enemy.draw(window);
            }
            for (auto& object : objects) {
                object.update(deltaTime);
                object.draw(window);
            }

            //Initilize game functions
            //Always draw UI elements
            level.playerTurnCountDown(window, player.turns);

            //Do game win/lose/pause conditions 
            if (level.hasWon) {
                level.gameWon(window);
            }
            else if (level.hasLost) {
                level.gameOver(window);
                player.draw(window);
            }
            else if (paused) {
                if (showInstructions) {
                    level.gameInstructions(window);
                }
                else {
                    level.gamePaused(window);
                }
            }
            break;
        case Status::End:
            break;
        default:
            break;
        }
        
        window.display();
    }
    return 0;
}

 

