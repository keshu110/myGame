#include <GLFW/glfw3.h>
#include <GL/glut.h>
#include <iostream>
#include <cstring>    // Needed for strcpy
#include <SOIL/SOIL.h>    // Simple OpenGL Image Library
#include <GL/glu.h>
#include <gl/GL.h>
#include <GL/GL.h>


// Function prototypes
void drawBoard();
void drawText(const char* text, float x, float y);
void drawX(int row, int col);
void drawO(int row, int col);
void checkWinner();
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void initBoard();
void waitFor(float seconds);
void mainLoop(GLFWwindow* window);
void loadGridTexture();
void drawAnimatedX(int i, int j);
void drawAnimatedO(int i, int j);
void drawFancyText(const char* text, float x, float y);



// Global variables
const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 800;
int board[3][3]; // 0 = empty, 1 = X, 2 = O
int currentPlayer = 1; // Start with player 1
bool gameOver = false;
bool full = false;
bool draw = false;

// Function to initialize the game board
void initBoard() {
    memset(board, 0, sizeof(board));
    gameOver = false;
    draw = false;
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
    glColor3f(1.0, 1.0, 1.0); // White color for the text
    glRasterPos2f(x, y); // Set the position for the text
    for (const char* c = text; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c); // Draw each character
    }
}

void drawAnimatedO(int i, int j) {
    float x = j * (SCREEN_WIDTH / 3) + (SCREEN_WIDTH / 6);
    float y = i * (SCREEN_HEIGHT / 3) + (SCREEN_HEIGHT / 6);
    float radius = 40;

    // Calculate scaling factor based on time for pulsating effect
    float scaleFactor = 1.0f + 0.1f * sin(glfwGetTime() * 5);

    glColor3f(0.0, 0.0, 1.0); // Blue color
    glLineWidth(3.0); // Set line width

    // Apply scaling transformation
    glPushMatrix();
    glTranslatef(x, y, 0.0);
    glScalef(scaleFactor, scaleFactor, 1.0);

    int numSegments = 30;
    glBegin(GL_LINE_LOOP);
    for (int ii = 0; ii < numSegments; ii++) {
        float theta = 2.0f * 3.14159f * float(ii) / float(numSegments);
        float dx = radius * cosf(theta);
        float dy = radius * sinf(theta);
        glVertex2f(dx, dy);
    }
    glEnd();

    // Restore the original transformation
    glPopMatrix();
}

void drawFancyText(const char* text, float x, float y) {
    // Example of fancy text rendering using bitmap fonts
    glColor3f(1.0, 1.0, 1.0); // White color
    glRasterPos2f(x, y); // Set the position for the text

    for (const char* c = text; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c); // Draw each character using a bitmap font
    }
}



void drawAnimatedX(int i, int j) {
    float x = j * (SCREEN_WIDTH / 3) + (SCREEN_WIDTH / 6);
    float y = i * (SCREEN_HEIGHT / 3) + (SCREEN_HEIGHT / 6);
    float size = 40;

    // Calculate scaling factor based on time for pulsating effect
    float scaleFactor = 1.0f + 0.1f * sin(glfwGetTime() * 5);

    glColor3f(1.0, 0.0, 0.0); // Red color
    glLineWidth(3.0); // Set line width

    // Apply scaling transformation
    glPushMatrix();
    glTranslatef(x, y, 0.0);
    glScalef(scaleFactor, scaleFactor, 1.0);

    glBegin(GL_LINES);
    glVertex2f(-size, -size);
    glVertex2f(size, size);
    glVertex2f(size, -size);
    glVertex2f(-size, size);
    glEnd();

    // Restore the original transformation
    glPopMatrix();
}


void checkWinner() {
    // Check for win conditions
    for (int line = 0; line < 3; ++line) {
        // Horizontal, Vertical check
        if ((board[line][0] != 0 && board[line][0] == board[line][1] && board[line][1] == board[line][2]) ||
            (board[0][line] != 0 && board[0][line] == board[1][line] && board[1][line] == board[2][line])) {
            gameOver = true;
            return;
        }
    }
    // Diagonal check
    if ((board[0][0] != 0 && board[0][0] == board[1][1] && board[1][1] == board[2][2]) ||
        (board[0][2] != 0 && board[0][2] == board[1][1] && board[1][1] == board[2][0])) {
        gameOver = true;
        return;
    }
    // Check for a draw
    draw = true;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == 0) {
                draw = false;
                break;
            }
        }
        if (!draw) break;
    }
    if (draw) {
        gameOver = true; // It's a draw, game over
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
    glLineWidth(3.0); // Set line width
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
    glLineWidth(3.0); // Set line width
    glBegin(GL_LINE_LOOP);
    for (int ii = 0; ii < num_segments; ii++) {
        float theta = 2.0f * 3.14159f * float(ii) / float(num_segments);
        float dx = radius * cosf(theta);
        float dy = radius * sinf(theta);
        glVertex2f(x + dx, y + dy);
    }
    glEnd();
}


GLuint gridTexture; // Global variable to store the texture ID

void loadGridTexture() {
    // Generate texture ID
    glGenTextures(1, &gridTexture);

    // Bind texture
    glBindTexture(GL_TEXTURE_2D, gridTexture);

    // Define texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load texture image (assuming you have a texture file named "grid_texture.png")
    int width, height, channels;
    unsigned char* image = SOIL_load_image("grid_texture.png", &width, &height, &channels, SOIL_LOAD_RGBA);
    if (image) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

        // Generate mipmaps manually
        int levels = static_cast<int>(std::floor(std::log2(std::max(width, height)))) + 1;
        for (int level = 1; level < levels; ++level) {
            int levelWidth = std::max(1, width >> level);
            int levelHeight = std::max(1, height >> level);
            glTexImage2D(GL_TEXTURE_2D, level, GL_RGBA, levelWidth, levelHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        }

        SOIL_free_image_data(image);
    }
    else {
        // Handle error
        std::cerr << "Failed to load texture" << std::endl;
    }

    // Unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);
}





void drawBoard() {
    // Set up gradient background
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_QUADS);
    glColor3f(0.5, 0.7, 1.0); // Light blue
    glVertex2f(0, 0);
    glVertex2f(SCREEN_WIDTH, 0);
    glColor3f(0.0, 0.3, 0.6); // Dark blue
    glVertex2f(SCREEN_WIDTH, SCREEN_HEIGHT);
    glVertex2f(0, SCREEN_HEIGHT);
    glEnd();

    // Draw textured grid lines
    glEnable(GL_TEXTURE_2D); // Enable texturing
    glBindTexture(GL_TEXTURE_2D, gridTexture); // Assuming gridTexture is the texture for grid lines

    glBegin(GL_LINES);
    for (int i = 1; i <= 2; i++) {
        // Vertical lines
        glTexCoord2f(0.0, 0.0); glVertex2f(i * (SCREEN_WIDTH / 3), 0);
        glTexCoord2f(1.0, 0.0); glVertex2f(i * (SCREEN_WIDTH / 3), SCREEN_HEIGHT);

        // Horizontal lines
        glTexCoord2f(0.0, 0.0); glVertex2f(0, i * (SCREEN_HEIGHT / 3));
        glTexCoord2f(1.0, 0.0); glVertex2f(SCREEN_WIDTH, i * (SCREEN_HEIGHT / 3));
    }
    glEnd();
    glDisable(GL_TEXTURE_2D); // Disable texturing

    // Draw X's and O's with animations
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board[i][j] == 1) drawAnimatedX(i, j); // Add animation for drawing X
            else if (board[i][j] == 2) drawAnimatedO(i, j); // Add animation for drawing O
        }
    }

    // Display the end game message with fancy styling
    if (gameOver) {
        const char* message = "Game Over";
        const char* winner = currentPlayer == 1 ? "Player O Wins!" : "Player X Wins!";
        if (draw) {
            winner = "It's a Draw!";
        }

        // Set up message box dimensions and position
        float messageBoxWidth = 300;
        float messageBoxHeight = 100;
        float messageBoxX = (SCREEN_WIDTH - messageBoxWidth) / 2;
        float messageBoxY = (SCREEN_HEIGHT - messageBoxHeight) / 2;

        // Draw a fancy message box background
        glColor4f(0.2, 0.2, 0.2, 0.7); // Dark gray with transparency
        glBegin(GL_QUADS);
        glVertex2f(messageBoxX, messageBoxY);
        glVertex2f(messageBoxX + messageBoxWidth, messageBoxY);
        glVertex2f(messageBoxX + messageBoxWidth, messageBoxY + messageBoxHeight);
        glVertex2f(messageBoxX, messageBoxY + messageBoxHeight);
        glEnd();

        // Draw the message text with fancy font
        glColor3f(1.0, 1.0, 1.0); // White color
        drawFancyText(message, messageBoxX + 20, messageBoxY + 40); // Add function for drawing fancy text
        drawFancyText(winner, messageBoxX + 20, messageBoxY + 70); // Add function for drawing fancy text
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