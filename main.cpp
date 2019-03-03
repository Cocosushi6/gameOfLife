#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <ctime>
#include <cstdlib>

using namespace std;

typedef std::array<std::array<int, 60>, 80> grid_t;

void drawGrid(sf::RenderWindow *window, int width, int height, int cellSize);
void drawCells(sf::RenderWindow &window, sf::Color color=sf::Color::Yellow);
void populate(int width, int height, int cellSize);
int getNbNeighbours(int x, int y);
grid_t applyRules();

int cellSize = 10;
int width = 800;
int height = 600;
grid_t grid;

int main() {
    sf::RenderWindow window(sf::VideoMode(width, height), "Game of Life", sf::Style::Titlebar | sf::Style::Close);
    populate(width, height, 10);
    window.setFramerateLimit(10);
    
    while(window.isOpen()) {
        sf::Event event;
        while(window.pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
                window.close();
            }
        }
        
        window.clear(sf::Color::Black);
        
        grid = applyRules();
        drawCells(window, sf::Color::Yellow);
        drawGrid(&window, width, height, cellSize);
        
        window.display();
    }
    
    return 0;
}

void drawGrid(sf::RenderWindow *window, int width, int height, int cellSize)
{
    for(int x = 0; x < width; x+=cellSize) {
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(x, 0)),
            sf::Vertex(sf::Vector2f(x, height))
        };
        window->draw(line, 2, sf::Lines);
    }
    
    for(int y = 0; y < height; y+=cellSize) {
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(0, y)),
            sf::Vertex(sf::Vector2f(width, y))
        };
        window->draw(line, 2, sf::Lines);
    }
}

void populate(int width, int height, int cellSize)
{
    std::srand(std::time(0));
    for(int x = 0; x < (width / cellSize); x++) {
        for(int y = 0; y < (height / cellSize); y++) {
            grid[x][y] = std::rand() % 2;
        }
    }
}

void drawCells(sf::RenderWindow &window, sf::Color color)
{
    for(int x = 0; x < grid.size(); x++) {
        for(int y = 0; y < grid[x].size(); y++) {
            sf::RectangleShape cell(sf::Vector2f(cellSize, cellSize));
            cell.setPosition(sf::Vector2f(x * cellSize, y * cellSize));
            if(grid[x][y] == 0) {
                cell.setFillColor(sf::Color::Black);
            } else if(grid[x][y] == 1) {
                cell.setFillColor(sf::Color::Yellow);
            }
            window.draw(cell);
        }
    }
}

//x and y are in terms of cell pos in the world, not in the screen (so must be divided / cellSize if you take real position)
int getNbNeighbours(int x, int y) {
    int neighbours = 0;
    for(int mod_x = -1; mod_x < 2; mod_x++) {
        for(int mod_y = -1; mod_y < 2; mod_y++) {
            if(mod_x == 0 && mod_y == 0) {
                continue;
            }
            int neighbourCell[] = {x + mod_x, y + mod_y};
            if(neighbourCell[0] < width / cellSize && neighbourCell[0] >= 0 && neighbourCell[1] < height / cellSize && neighbourCell[1] >= 0 && grid[neighbourCell[0]][neighbourCell[1]] == 1) {
                    neighbours++;
            }
        }
    }
    return neighbours;
}

grid_t applyRules() {
    grid_t newGrid{};
    for(int x = 0; x < grid.size(); x++) {
        for(int y = 0; y < grid[0].size(); y++) {
            int numNeighbours = getNbNeighbours(x, y);
            if(grid[x][y] == 1) {
                if(numNeighbours < 2 || numNeighbours > 3) {
                    newGrid[x][y] = 0;
                } else {
                    newGrid[x][y] = 1;
                }   
            } else {
                if(numNeighbours == 3) {
                    newGrid[x][y] = 1;
                }
            }
        }
    }
    return newGrid;
}

