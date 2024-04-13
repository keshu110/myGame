# Tic Tac Toe Game

## Description
This project is a Tic Tac Toe game developed in C++ using OpenGL for rendering. The game supports two players on the same computer and includes features like a menu system, game-over detection.

## Installation and Setup
To run this project, you will need to install the following:

- GLFW
- GLUT
- OpenGL

### Instructions:
1. Install GLFW and GLUT. On Ubuntu, you can use `sudo apt-get install freeglut3-dev`.
2. Clone the repository: `git clone <URL of your repository>`
3. Navigate to the project directory and compile the project using the provided makefile with the command `make`.
4. Run the executable with `./TicTacToe`.

## Usage
After launching the game, use the mouse to interact with the game menu. Click on "Play" to start the game. Players alternate turns by clicking on the grid to place their X or O. The game checks for a winner or a draw and displays a game-over message accordingly.

## Features
- Two-player gameplay on the same computer.
- Interactive menu system to start new games.
- Game state detection including win and draw conditions.
- Simple graphics using OpenGL.

## Code Examples
Here is how the game board is initialized and drawn:

```c++
void initBoard() {
    // Initialize the game board array to zero
}

void drawBoard() {
    // Code to draw the game board grid
}

## Contributing
Contributions to this project are welcome. Please fork the repository, make your changes, and submit a pull request.

## Acknowledgments
- Thanks to GLFW and GLUT libraries for providing the graphics API.
- Inspired by the classic game Tic Tac Toe.



