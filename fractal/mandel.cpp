#include <ncurses.h>
#include <cmath>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include <iostream>
#include <omp.h> // OpenMP
#include <chrono> // std::chrono::microseconds
#include <thread> // std::this_thread::sleep_for


using namespace std;

#define NUM_POINTS 10000
#define SCALE_FACTOR 0.1
#define ITER 42
#define NUM_THREADS 8
#define RESULTS_FILE "results_1P_4t.txt"


// Because I was unable to acces the github classroom I was unable use the provided framework. 
// I found my own function to render the julia set.  Source: https://github.com/mdabir1203/CursedFractals/tree/master
// This is my edited variant

float x_pos = 0, y_pos = 0;
float zoom = 1;
bool use_julia = true;

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

void color_pixel(int i, int x, int y){
    for (int j = 0; j < y; j++) {
        // Calculate the coordinates of the point based on x_pos, y_pos, and zoom which are global variables
        double x_coord = (((double)i - (double)x / 2.) * (4. / (double)x)) * zoom + x_pos;
        double y_coord = y_pos + zoom * ((double)j - (double)y / 2.) * (4. / (double)y * ((2.5 * 9.) / 16.));

        int result;
        if (use_julia)
            result = julia(x_coord, y_coord);

        switch (result % 7) {
            case 0:
                attron(COLOR_PAIR(1));
                break;
            case 1:
                attron(COLOR_PAIR(2));
                break;
            case 2:
                attron(COLOR_PAIR(3));
                break;
            case 3:
                attron(COLOR_PAIR(4));
                break;
            case 4:
                attron(COLOR_PAIR(5));
                break;
            case 5:
                attron(COLOR_PAIR(6));
                break;
            default:
                attron(COLOR_PAIR(7));
                break;
        }
        
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

// Function to draw the Julia set on the terminal window
void draw() {
    int x, y;
    getmaxyx(stdscr, y, x);
    omp_set_num_threads(NUM_THREADS);
    for (int i = 0; i < x; i+=NUM_THREADS) {
        color_pixel(i, x, y);
    }

    refresh();
}


int main(int argc, char *argv[]) {
    int id = -1, numProcesses = -1;
    double startTime = 0.0, totalTime = 0.0;
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
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);
    
    draw(); // Initial drawing
    
    for (int i = 0; i < 300; i++){
        int randomNum = rand() % 5;
        switch (randomNum){
            case 0:
                // Zooms in 
                y_pos -= 0.15 * zoom;
                break;
            case 1:
                // Zooms out
                y_pos += 0.15 * zoom;
                break;
            case 2:
                // Moves left
                x_pos -= 0.15 * zoom;
                break;
            case 3:
                // Moves right
                x_pos += 0.15 * zoom;
                break;
            case 4:
                // Zooms in
                zoom *= 0.9;
                break;
        }
        
        startTime = MPI_Wtime();
        draw();
        totalTime = MPI_Wtime() - startTime;
        // write the procces id, time taken and if it is the main thread to results.txt
        FILE *file = fopen("results.txt", "a");
        fprintf(file, "%d, %d, %d, %d, %f,\n", numProcesses, NUM_THREADS, id, randomNum, totalTime);
        fclose(file);
        std::this_thread::sleep_for(std::chrono::microseconds{50000});

    }
    endwin();
    MPI_Finalize();
    return 0;
}
