#include <ncurses.h>
#include <cmath>
#include <stdlib.h>
#include <math.h>

#define NUM_POINTS 10000000
#define SCALE_FACTOR 0.5
#define ITER 42

"""Because I was unable to acces the github classroom I was unable use the provided framework. 
I found my own function to render the julia set.  Source: https://github.com/mdabir1203/CursedFractals/tree/master"""



void stemTransformation(double *x, double *y)
{
    *y += 0.05;
}

void leftLeafTransformation(double *x, double *y)
{
    double new_x = 0.85 * (*x) - 1.6;
    *y += 0.04;
    *x = new_x;
}

void rightLeafTransformation(double *x, double *y)
{
    double new_x = 0.2 * (*x) + 0.23;
    *y -= 0.26;
    *x = new_x;
}

void centerLeafTransformation(double *x, double *y)
{
    double new_x = -0.15 * (*x) + 0.26;
    *y += 0.28;
    *x = new_x;
}

int fernFractal(double x, double y) {
    // Initialize variables
    double z_real = x, z_imag = y;
    int iteration = 0;

    // Loop until the point escapes the circle of radius 2 or we reach the maximum number of iterations
    while (z_real * z_real + z_imag * z_imag <= 4 && iteration < NUM_POINTS) {
        // Generate a random number
        int randomNum = rand() % 100;

        // Apply a transformation based on the random number
        if (randomNum < 1) {
            stemTransformation(&z_real, &z_imag);
        } else if (randomNum < 86) {
            leftLeafTransformation(&z_real, &z_imag);
        } else if (randomNum < 93) {
            rightLeafTransformation(&z_real, &z_imag);
        } else {
            centerLeafTransformation(&z_real, &z_imag);
        }

        // Scale and translate the point
        z_real += SCALE_FACTOR;
        z_imag += SCALE_FACTOR;
        z_real += 2.5;
        z_imag += 1.0;

        // Increment the iteration count
        iteration++;
    }

    // Return the number of iterations, or 0 if the point did not escape
    return iteration == NUM_POINTS ? 0 : iteration;
}
// Function to calculate whether a point belongs to the Julia set
int julia(double x, double y) {
    double z_real = x, z_imag = y;
    double c_real = -0.7, c_imag = 0.27015; // Constants for the Julia set
    int iteration = 0;
    
    while (z_real * z_real + z_imag * z_imag <= 4 && iteration < ITER) {
        double temp = z_real * z_real - z_imag * z_imag + c_real;
        z_imag = 2 * z_real * z_imag + c_imag;
        z_real = temp;
        iteration++;
    }
    
    return iteration == ITER ? 0 : iteration;
}

// Function to calculate whether a point belongs to the Buddhabrot set
int buddhabrot(double x, double y, int *trajectory, int max_trajectory_length)
{
    double z_real = 0, z_imag = 0;
    int iteration = 0;

    while (z_real * z_real + z_imag * z_imag <= 4 && iteration < ITER)
    {
        if (iteration < max_trajectory_length)
        {
            trajectory[iteration * 2] = z_real;
            trajectory[iteration * 2 + 1] = z_imag;
        }

        double temp = z_real * z_real - z_imag * z_imag + x;
        z_imag = 2 * z_real * z_imag + y;
        z_real = temp;
        iteration++;
    }

    return iteration == ITER ? 0 : iteration;
}


float x_pos = 0, y_pos = 0;
float zoom = 1;

bool use_julia = true;
bool use_fern  = false;

// Function to draw the Julia set on the terminal window
void draw() {
    int x, y;
    getmaxyx(stdscr, y, x);

    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            double x_coord = (((double)i - (double)x / 2.) * (4. / (double)x)) * zoom + x_pos;
            double y_coord = y_pos + zoom * ((double)j - (double)y / 2.) * (4. / (double)y * ((2.5 * 9.) / 16.));
        int result;
        if (use_julia)
            result = julia(x_coord, y_coord);
        else if (use_fern)
            result = fernFractal(x_coord, y_coord);
        else {
            int trajectory[ITER * 2];
            result = buddhabrot(x_coord, y_coord, trajectory, ITER);
            }
            // Color mappingp
            if (result == 0)
                attron(COLOR_PAIR(1));
            else if (result % 7 == 1)
                attron(COLOR_PAIR(2));
            else if (result % 7 == 2)
                attron(COLOR_PAIR(3));
            else if (result % 7 == 3)
                attron(COLOR_PAIR(4));
                                    else if (result % 7 == 4)
                attron(COLOR_PAIR(5));
            else if (result % 7 == 5)
                attron(COLOR_PAIR(6));
            else
                attron(COLOR_PAIR(7));
            
            // Display character
            if (result == 0)
                mvprintw(j, i, "#");
            else
                mvprintw(j, i, " ");

            // Turn off color pairs
            attroff(COLOR_PAIR(1));
            attroff(COLOR_PAIR(2));
            attroff(COLOR_PAIR(3));
            attroff(COLOR_PAIR(4));
            attroff(COLOR_PAIR(5));
            attroff(COLOR_PAIR(6));
            attroff(COLOR_PAIR(7));
        }
    }

    refresh();
}

int main(void) {
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);
    start_color();
    
    // Initializing color pairs
    init_pair(1, COLOR_WHITE, COLOR_BLACK);   // Inside Julia set
    init_pair(2, COLOR_BLACK, COLOR_BLUE);    // Deep blue
    init_pair(3, COLOR_BLACK, COLOR_CYAN);    // Cyan
    init_pair(4, COLOR_BLACK, COLOR_GREEN);   // Green
    init_pair(5, COLOR_BLACK, COLOR_YELLOW);  // Yellow
    init_pair(6, COLOR_BLACK, COLOR_RED);     // Red
    init_pair(7, COLOR_BLACK, COLOR_MAGENTA); // Magenta
    
    draw(); // Initial drawing
    
     while (1)
    {
        int ch = getch();
        switch (ch) {
            case 'j':
                use_julia = true;
                use_fern = false;
                break;
            case 'b':
                use_julia = false;
                use_fern =  false;
                break;
            case 'f':
                use_fern = true;
                use_julia = false;
                break;
            case 'w':
                y_pos -= 0.15 * zoom;
                break;
            case 's':
                y_pos += 0.15 * zoom;
                break;
            case 'a':
                x_pos -= 0.15 * zoom;
                break;
            case 'd':
                x_pos += 0.15 * zoom;
                break;
            case 'q':
                zoom *= 0.9;
                break;
            case 'e':
                zoom /= 0.9;
                break;
            case ' ':
                endwin();
                return 0;
                break;
        }
        draw();
    }
    return 0;
}