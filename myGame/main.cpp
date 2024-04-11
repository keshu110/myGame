#include <GLFW/glfw3.h>
#include <GL/glut.h>
#include <iostream>

// Function prototypes
void drawBoard();
void drawText(const char* text, float x, float y);
void drawX(int row, int col);
void drawO(int row, int col);
void checkWinner();
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

// Global variables
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 960;
int board[3][3]; // 0 = empty, 1 = X, 2 = O
int currentPlayer = 1; // Start with player 1
bool gameOver = false;
bool full = false;

// Function to initialize the game board
void initBoard() {
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            board[row][col] = 0;
        }
    }
    gameOver = false;
    currentPlayer = 1; // Player X starts
}

// Function to wait for a given number of seconds
void waitFor(float seconds) {
    double endTime = glfwGetTime() + seconds;
    while (glfwGetTime() < endTime) {
        glfwPollEvents(); // Keep processing window events
    }
}

void drawText(const char* text, float x, float y) {
    // Set the color for the text
    glColor3f(1.0, 1.0, 1.0);

    // Set the position for the text
    glRasterPos2f(x, y);

    // Loop through each character in the string
    for (const char* c = text; *c != '\0'; c++) {
        // Draw each character
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
    }
}


// Main loop of the game
void mainLoop(GLFWwindow* window) {
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT); // Clear the screen
        drawBoard(); // Draw the game board

        if (gameOver) {
            // Set the winner message
            const char* message = "Game Over: ";
            const char* winner = currentPlayer == 1 ? "Player O Wins!" : "Player X Wins!";
            bool isDraw = true;
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) {
                    if (board[i][j] == 0) {
                        isDraw = false; // If any cell is empty, it's not a draw
                    }
                }
            }
            if (isDraw) {
                winner = "Draw!";
            }

            // Display the winner message
            char finalMessage[50];
            snprintf(finalMessage, sizeof(finalMessage), "%s%s", message, winner);
            drawText(finalMessage, SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2);
            glfwSwapBuffers(window); // Swap buffers to show the final message
            waitFor(5.0); // Wait for 5 seconds
            break; // Break the loop to end the game
        }

        glfwSwapBuffers(window); // Swap the front and back buffers
        glfwPollEvents(); // Poll for and process events
    }
}








void drawX(int i, int j) {
    float x = j * (SCREEN_WIDTH / 3) + (SCREEN_WIDTH / 6);
    float y = i * (SCREEN_HEIGHT / 3) + (SCREEN_HEIGHT / 6);
    float size = 40;

    glColor3f(1.0, 0.0, 0.0); // Red color
    glLineWidth(3.0); // Thicker line

    glBegin(GL_LINES);
    glVertex2f(x - size, y - size);
    glVertex2f(x + size, y + size);
    glVertex2f(x + size, y - size);
    glVertex2f(x - size, y + size);
    glEnd();
}

void drawO(int i, int j) {
    float x = j * (SCREEN_WIDTH / 3) + (SCREEN_WIDTH / 6);
    float y = i * (SCREEN_HEIGHT / 3) + (SCREEN_HEIGHT / 6);
    float radius = 40;
    int num_segments = 30;

    glColor3f(0.0, 0.0, 1.0); // Blue color
    glLineWidth(3.0); // Thicker line

    glBegin(GL_LINE_LOOP);
    for (int ii = 0; ii < num_segments; ii++) {
        float theta = 2.0f * 3.14159f * float(ii) / float(num_segments);
        float dx = radius * cosf(theta);
        float dy = radius * sinf(theta);
        glVertex2f(x + dx, y + dy);
    }
    glEnd();
}



void drawBoard() {
    glClearColor(0.8, 0.8, 0.8, 1.0); // Light gray background
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.3, 0.3, 0.3); // Darker gray for the grid lines
    glLineWidth(2.0); // Set line width for drawing

    glBegin(GL_LINES);
    for (int i = 1; i <= 2; i++) {
        glVertex2f(i * (SCREEN_WIDTH / 3), 0);
        glVertex2f(i * (SCREEN_WIDTH / 3), SCREEN_HEIGHT);
        glVertex2f(0, i * (SCREEN_HEIGHT / 3));
        glVertex2f(SCREEN_WIDTH, i * (SCREEN_HEIGHT / 3));
    }
    glEnd();

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board[i][j] == 1) drawX(i, j);
            else if (board[i][j] == 2) drawO(i, j);
        }
    }

    if (gameOver) {
        const char* message = "Game Over: ";
        const char* winner = currentPlayer == 1 ? "O Wins!" : "X Wins!";
        if (full) {
            winner = "Draw!";
        }
        
        char finalMessage[50];
        sprintf_s(finalMessage, sizeof(finalMessage), "%s%s", message, winner);


        drawText(finalMessage, SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2);
    }
}

void checkWinner() {
    // Your winning condition checks

    // Check if the game is full and there's no winner
    full = true;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == 0) {
                full = false; // The game is not full
                break;
            }
        }
    }

    // If the board is full and no winner, it's a draw
    if (full && !gameOver) {
        gameOver = true; // End the game
        // You may want to print a message or handle a draw differently
    }
}




// Callback function for mouse button events
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && !gameOver) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        int row = int(ypos / (SCREEN_HEIGHT / 3));
        int col = int(xpos / (SCREEN_WIDTH / 3));

        if (board[row][col] == 0) { // Check if the cell is empty
            board[row][col] = currentPlayer; // Set the cell to the current player

            checkWinner(); // Check for a winner or a draw

            // If the game is not over, switch to the other player
            if (!gameOver) {
                currentPlayer = (currentPlayer == 1) ? 2 : 1;
            }
        }
    }
}




int main(int argc, char** argv) {

    // Initialize GLUT
    glutInit(&argc, argv);
    

    // Initialize GLFW
    if (!glfwInit()) return -1;

    // Create a GLFW window
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Tic Tac Toe", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    // Set the current context
    glfwMakeContextCurrent(window);
    // Set the mouse button callback
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    // Initialize the game board
    initBoard();

    // Set up the viewport
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Start the main loop
    mainLoop(window);

    // Terminate GLFW
    glfwTerminate();
    return 0;
}