#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

using namespace sf;

const int BOARD_WIDTH = 10;
const int BOARD_HEIGHT = 20;

std::vector<std::vector<char>> board(BOARD_HEIGHT, std::vector<char>(BOARD_WIDTH));

// x, y pair
// player is current pos, shadow is your potential next pos
std::pair<int, int> player[4], shadow[4];

/* Returns whether or not a move is legal
TODO:
Have the player passed in as param (i dont like global vars :) )
*/
bool isLegal() {
    for (int i = 0; i < 4; i++) {
        if (
            player[i].first < 0 || 
            player[i].first >= BOARD_WIDTH || 
            player[i].second >= BOARD_HEIGHT) return false;
        else if (board[player[i].second][player[i].first]) return false;
    }
    return true;
}

/* TODO
Divide up into functions or classes
*/
int main() {
    RenderWindow window(VideoMode(480, 960), "Ctris");

    Texture t1, t2;

    // Blocks are 32 x 32 px each, (sprite sheet is 224 x 32)
    t1.loadFromFile("images/tiles.png");

    // Background is 480 x 960 px
    t2.loadFromFile("images/background.png");

    Sprite block(t1), background(t2);

    int direction = 0;
    bool rotate = 0;
    int colorNum = 1;
    float timer = 0;
    float delay = 0.3;

    Clock clock;
    while (window.isOpen()) {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;

        Event e;
        while (window.pollEvent(e)) {
            if (e.type == Event::Closed)
                window.close();
            
            if (e.type == Event::KeyPressed) {
                switch (e.key.code) {
                    case Keyboard::Up:
                        rotate=true;
                        break;
                    case Keyboard::Left:
                        direction=-1;
                        break;
                    case Keyboard::Right:
                        direction=1;
                        break;
                }
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Down)) delay=0.05;

        /* Movement */

        for (int i = 0; i < 4; i++) {
            shadow[i] = player[i];
            player[i].first += direction;
        }

        // If not legal, return to prev position
        if (!isLegal())
            for (int i = 0; i < 4; i++)
                player[i] = shadow[i];

        /* Drawing 

        For reference: each block is 32x32 px
        "s.move(80, 160)" is to offset blocks into the board
        */

        window.clear(Color::White);
        window.draw(background);
        
        for (int i = 0; i < BOARD_HEIGHT; i++) {
            for (int j = 0; j < BOARD_WIDTH; j++) {
                if (board[i][j] == 0) continue;

                block.setTextureRect(IntRect(board[i][j]*32, 0, 32, 32));
                block.setPosition(j*32, i*32);
                block.move(80, 160);
                window.draw(block);
            }
        }

        for (int i = 0; i < 4; i++) {
            block.setTextureRect(IntRect(colorNum*32, 0, 32, 32));
            block.setPosition(player[i].first * 32, player[i].second * 32);
            block.move(80, 160);
            window.draw(block);
        }

        window.display();
    }
}