#include <iostream>
#include <string>
#include <cstring> 
#include <GLFW/glfw3.h>
#include <GL/glut.h>                                       
#include <SOIL/SOIL.h>    
#include <GL/glu.h>
#include <gl/GL.h>
using namespace std;
                                                                    // prototyping the functions
void drawBoard();
void drawText(const char *text, float x, float y);
void drawX(int row, int col);
void drawO(int row, int col);
void checkWinner();
void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
void initBoard();
void waitFor(float seconds);
void mainLoop(GLFWwindow *window);
void drawFancyText(const char* text, float x, float y);
void drawRoundedRectangle(float x, float y, float width, float height, float radius, int num_segments);
void drawCenteredText(const char* text, float x, float y, float width, float height);
void displayGameOverMessage();
void drawMenu();
void handleMenuInput(GLFWwindow* window, int button, int action, int mods);


// creating global variables
const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 800;
int board[3][3];                                             // 0 = empty, 1 = X, 2 = O
int  currentPlayer = 1;                                     // starting with player 1
GLuint gridTexture;



bool gameOver = false;
bool full = false;
bool draw = false;
bool showMenu = true;

                                                            // Function to initialize the game board
void initBoard() {
    memset(board, 0, sizeof(board));
    gameOver = false;
    draw = false;
    currentPlayer = 1;
}

                                                               // creating a function to make a delay of few seconds
void waitFor(float seconds) {
    double endTime = glfwGetTime() + seconds;
    while (glfwGetTime() < endTime) {
        glfwPollEvents();                                       // keeping the window responsive
    }
}

void drawText(const char *text, float x, float y) {
    glColor3f(0.494f, 0.145f, 0.325f); // Color #7E2553


    glRasterPos2f(x, y);                                         // to set the position for the text
    for (const char *c = text; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);     // drawing each character using a bitmap font
    }
}

void checkWinner() {
                                                                  // checking for win conditions
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
    // Check whether its a draw
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
        gameOver = true;                                                                    // It's a draw, game over
    }
}




void drawX(int i, int j) {
    float x = j * (SCREEN_WIDTH / 3) + (SCREEN_WIDTH / 6);
    float y = i * (SCREEN_HEIGHT / 3) + (SCREEN_HEIGHT / 6);
    float size = 40;
    glColor3f(1.0, 0.0, 0.0); // Red color
    glLineWidth(9.0); // Set line width
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
    glColor3f(0.718f, 0.788f, 0.949f); // Color #B7C9F2

    glLineWidth(9.0);                                             // setting line width
    glBegin(GL_LINE_LOOP);
    for (int ii = 0; ii < num_segments; ii++) {
        float theta = 2.0f * 3.14159f * float(ii) / float(num_segments);
        float dx = radius * cosf(theta);
        float dy = radius * sinf(theta);
        glVertex2f(x + dx, y + dy);
    }
    glEnd();
}




void drawMenu() {
                                                                                  // clearing the background to a light blue color
    glClearColor(0.68f, 0.85f, 0.90f, 1.0f);                                       // light blue color
    glClear(GL_COLOR_BUFFER_BIT);

                                                                              // drawing a rectangle for the "Play" button with a highlight effect
    float playButtonX = SCREEN_WIDTH / 2 - 150;
    float playButtonY = SCREEN_HEIGHT / 2 - 50;
    float playButtonWidth = 300;
    float playButtonHeight = 100;
   
    drawRoundedRectangle(playButtonX, playButtonY, playButtonWidth, playButtonHeight, 20.0f, 10);

    // Button highlight
    glColor4f(1.0f, 1.0f, 1.0f, 0.1f);                                                        // soft white highlight
    drawRoundedRectangle(playButtonX, playButtonY - playButtonHeight * 0.1f, playButtonWidth, playButtonHeight * 0.7f, 20.0f, 10);

                                                                               // drawing the "Play" text so that it'll be centered in the button
    glColor3f(1.0f, 1.0f, 1.0f);                                                              // setting white color for the text
    drawCenteredText("Play", playButtonX, playButtonY, playButtonWidth, playButtonHeight);
}
void handleMenuInput(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

                                                                                        // checking whether "Play" button is clicked or not
        if (xpos >= SCREEN_WIDTH / 2 - 100 && xpos <= SCREEN_WIDTH / 2 + 100 &&
            ypos >= SCREEN_HEIGHT / 2 - 25 && ypos <= SCREEN_HEIGHT / 2 + 25) {
            showMenu = false;                                                            // hiding menu and to start the game
            initBoard();                                                                 // initializing the game board for showing a new game
        }
    }
}


void drawRoundedRectangle(float x, float y, float width, float height, float radius, int num_segments) {
    glColor3f(0.537f, 0.506f, 0.129f); // Color #898121       
    glBegin(GL_POLYGON);

                                                                                // drawing the rectangle with rounded corners
    for (int i = 0; i < num_segments; ++i) {
        float theta = 2.0f * 3.1415926f * float(i) / float(num_segments);
        float dx = radius * cosf(theta);
        float dy = radius * sinf(theta);

                                                                            // each line represents each corner of the rectangle
        glVertex2f(x + radius + dx, y + radius + dy);                                      // Top-left
        glVertex2f(x + width - radius + dx, y + radius + dy);                              // Top-right
        glVertex2f(x + width - radius + dx, y + height - radius + dy);                    // Bottom-right
        glVertex2f(x + radius + dx, y + height - radius + dy);                             // Bottom-left
    }

    glEnd();
}

void drawCenteredText(const char* text, float x, float y, float width, float height) {
    glColor3f(0.514f, 0.753f, 0.757f); // Color #83C0C1

    glRasterPos2f(x + (width - glutBitmapLength(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)text)) / 2,
        y + height / 2 + 7);                                                                 // setting the text position of rectangle at center

                                                                                             // rendering the text
    for (const char* c = text; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);                                    // Using a thicker font
    }
}

void displayGameOverMessage() {
    float boxWidth = 300.0f;
    float boxHeight = 100.0f;
    float boxX = (SCREEN_WIDTH - boxWidth) / 2;
    float boxY = (SCREEN_HEIGHT - boxHeight) / 2;
    float cornerRadius = 20.0f;
    int cornerSegments = 50;                                                                   //adding more segments for a smoother corner

                                                                                                // drawing rounded rectangle
    drawRoundedRectangle(boxX, boxY, boxWidth, boxHeight, cornerRadius, cornerSegments);

                                                                                                // drawing text at the center of the rectangle
    drawCenteredText("Game Over", boxX, boxY, boxWidth, boxHeight);
}


                                                                                               // creating main loop for the game
void mainLoop(GLFWwindow* window) {
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);                                                          // clearing the color buffer bit to set the background color

        if (showMenu) {
            drawMenu();
                                                                                               // setting the callback for the menu input
            glfwSetMouseButtonCallback(window, handleMenuInput);
        }
        else {
            drawBoard();                                                                       // drawing the game board with X and O symbols

            if (gameOver) {
                                                                                              // displaying the game over message
                const char* winnerMessage;
                if (draw) {
                    winnerMessage = "It's a Draw!";
                }
                else {
                    winnerMessage = currentPlayer == 1 ? "Player X Wins!" : "Player O Wins!";
                }

                                                                                           // The winner message will be displayed for 5 seconds
                displayGameOverMessage();
                drawText(winnerMessage, SCREEN_WIDTH / 2 - 80, SCREEN_HEIGHT / 2 - 100);

                glfwSwapBuffers(window);                                             // swapping the front and back buffers to display the message
                waitFor(5.0);                                                        // waiting for 5 seconds

                // after 5 seconds, the menu will be shown again to start a new game
                showMenu = true;                                                       // showing the menu again 
                initBoard();                                                           // resetting the game board for a new game
                gameOver = false;                                                     // resetting the game over flag to false
                draw = false;                                                           // resetting the draw flag to false
                currentPlayer = 1;                                                     // setting the current player to 1
            }

                                                                       // setting the callback for the mouse button input for the game board
            glfwSetMouseButtonCallback(window, mouseButtonCallback);
        }

        glfwSwapBuffers(window);                                       // swapping the front and back buffers to display the game board
        glfwPollEvents();                                            // Poll for and process events like input, etc. to keep the window responsive
    }
}



void drawBoard() {
                                                                       // setting the background color to a light blue color
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_QUADS);
    glColor3f(0.5, 0.7, 1.0);                                           // light blue color for the background
    glVertex2f(0, 0);
    glVertex2f(SCREEN_WIDTH, 0);
    glColor3f(0.0, 0.3, 0.6);                                           // dark blue color for the background
    glVertex2f(SCREEN_WIDTH, SCREEN_HEIGHT);
    glVertex2f(0, SCREEN_HEIGHT);
    glEnd();

                                                                         // drawing the grid lines for the game board
    glEnable(GL_TEXTURE_2D);                                            // enable texturing for the grid lines
    glBindTexture(GL_TEXTURE_2D, gridTexture);                           // assuming gridTexture is the texture ID

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
    glDisable(GL_TEXTURE_2D);                                             // Disable texturing

                                                                          // drawing the X and O symbols on the game board
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == 1) {
				drawX(i, j);
			}
            else if (board[i][j] == 2) {
				drawO(i, j);
			}
		}
	}   



                                                                        // displaying the game over message if the game is over 
    if (gameOver) 
    {
        const char* message = "Game Over";
        const char* winner = currentPlayer == 1 ? "Player O Wins!" : "Player X Wins!";
        if (draw) 
        {
            winner = "It's a Draw!";
        }

                                                                         // setting the message box dimensions and position
        float messageBoxWidth = 300;
        float messageBoxHeight = 100;
        float messageBoxX = (SCREEN_WIDTH - messageBoxWidth) / 2;
        float messageBoxY = (SCREEN_HEIGHT - messageBoxHeight) / 2;

                                                                            // drawing the message box with color and transparency
        glColor4f(0.56f, 0.93f, 0.56f, 0.7f);                              // Light green with some transparency

        glBegin(GL_QUADS);
        glVertex2f(messageBoxX, messageBoxY);
        glVertex2f(messageBoxX + messageBoxWidth, messageBoxY);
        glVertex2f(messageBoxX + messageBoxWidth, messageBoxY + messageBoxHeight);
        glVertex2f(messageBoxX, messageBoxY + messageBoxHeight);
        glEnd();
    }
}




                                                                            // function to handle mouse button input 
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && !gameOver) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        int row = int(ypos / (SCREEN_HEIGHT / 3));
        int col = int(xpos / (SCREEN_WIDTH / 3));

        if (board[row][col] == 0) {                                         // checking whether the cell is empty or not
            board[row][col] = currentPlayer;                                // setting the current player's symbol in the cell

            checkWinner();                                                  // checking for the winner after each move 

                                                                            // if the game is not over, switch to the next player
            if (!gameOver) {
                currentPlayer = (currentPlayer == 1) ? 2 : 1;
            }
        }
    }
}





int main(int argc, char** argv) {
                                                                                    // initializing GLUT
    glutInit(&argc, argv);

                                                                                     // initializing GLFW
    if (!glfwInit()) return -1;

                                                                                     // creating a window with a specific size and title
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Tic Tac Toe", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

                                                                                    // setting the window position to the center of the screen
    glfwMakeContextCurrent(window);

                                                                                    // setting the viewport and projection matrix for the window
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


                                                                                    // Initialize the game board
    initBoard();

                                                                                     // Start the main loop
    mainLoop(window);

                                                                                     // Terminate GLFW
    glfwTerminate();
    return 0;
}