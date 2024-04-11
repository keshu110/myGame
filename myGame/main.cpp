#ifdef APIENTRY
#undef APIENTRY
#endif

#include <windows.h> // Ensures APIENTRY is defined by Windows API first if needed
#include <GLFW/glfw3.h>
#include <GL/glut.h>
#include <cmath>
#include <iostream>

// Redefine APIENTRY if necessary after all includes
#ifndef APIENTRY
#define APIENTRY __stdcall
#endif

void drawBoard();
void drawText(const char* text, float x, float y);
void drawX(int i, int j);
void drawO(int i, int j);
void checkWinner();
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void mainLoop(GLFWwindow* window);
void initBoard();

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 960;
int board[3][3]; // 0 = empty, 1 = X, 2 = O
int currentPlayer = 1; // 1 = X, 2 = O
bool gameOver = false;
bool full = false; // To check if the board is full

void initBoard() {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            board[i][j] = 0;
        }
    }
    gameOver = false;
    currentPlayer = 1; // X starts
}

// Implementation of drawText
void drawText(const char* text, float x, float y) {
    glColor3f(0.0, 0.0, 0.0); // Text color
    glRasterPos2f(x, y); // Position where the text will start
    for (const char* c = text; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);  // Display each character
    }
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
}


void initBoard() {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            board[i][j] = 0;
        }
    }
    gameOver = false;
    currentPlayer = 1; // X starts
}

void mainLoop(GLFWwindow* window) {
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT); // Clear the screen buffer

        drawBoard(); // Draw the game board and any X's or O's

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

        glfwSwapBuffers(window); // Swap the front and back buffers
        glfwPollEvents(); // Poll for and process events

        // Continue displaying the game over message until window is closed
        if (gameOver) {
            glfwWaitEvents(); // Wait for user interaction
        }
    }
}


void drawText(const char* text, float x, float y) {
    glColor3f(0.0, 0.0, 0.0); // Text color
    glRasterPos2f(x, y); // Position where the text will start

    for (const char* c = text; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);  // Display each character
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
    for (int i = 0; i < 3; i++) {
        if (board[i][0] != 0 && board[i][0] == board[i][1] && board[i][1] == board[i][2]) {
            gameOver = true;
            return;
        }
        if (board[0][i] != 0 && board[0][i] == board[1][i] && board[1][i] == board[2][i]) {
            gameOver = true;
            return;
        }
    }
    if (board[0][0] != 0 && board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
        gameOver = true;
        return;
    }
    if (board[0][2] != 0 && board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
        gameOver = true;
        return;
    }
    full = true;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == 0) {
                full = false;
                break;
            }
        }
    }
    if (full) {
        gameOver = true;
    }
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && !gameOver) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        int i = int(ypos / (SCREEN_HEIGHT / 3));
        int j = int(xpos / (SCREEN_WIDTH / 3));

        if (board[i][j] == 0) {
            board[i][j] = currentPlayer;
            checkWinner();
            currentPlayer = currentPlayer == 1 ? 2 : 1;
        }
    }
}

int main() {
    GLFWwindow* window;
    if (!glfwInit()) return -1;

    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Tic Tac Toe", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    initBoard();

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Start the main loop
    mainLoop(window);

    glfwTerminate();
    return 0;
}