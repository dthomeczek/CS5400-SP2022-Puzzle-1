// Name: Devin Thomeczek
// Username: djtpfr
// Date Due: 2/28/2022
// Professor: Dr. Morales

#include <cstdlib>
#include <fstream>
#include <unistd.h>
#include <iostream>
#include <string>
using namespace std;

// Used to contain values for a Pengu object (I found this simpler
// than using classes for all of this personally, and it works the same)
struct Pengu
{
    bool alive = true; // Bool to check if Pengu is alive
    bool on_snow = false; // Bool to check if Pengu is on a snow space
    int fish = 0; // Int to hold number of fish obtained
    string moves = ""; // String to hold the chain of moves made by number
    int row_val = -1; // Initial row value of Pengu, to be determined later
    int col_val = -1; // Initial column value of Pengu, to be determined later
    int previous_row; // Value to hold Pengu's previous row value
    int previous_col; // Value to hold Pengu's previous column value
};

// This function takes in the pointer array, the rows and columns, and
// scans through each character of the array to determine how many fish
// the array starts with, and returns that value
int total_fish(char **arr, int rows, int cols)
{
    
    // Set a starting fish value of 0 and increment each time one is found
    // in the array
    int fish_count = 0;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (arr[i][j] == '*')
            {
                fish_count += 1;
            }
        }
    }

    // Returns the starting fish value
    return fish_count;
}

// This function takes in the pointer array, the rows and columns,
// and a Pengu struct value P, and determines where P is in the array, 
// and then returns the Pengu with its updated values
Pengu start_val(char **arr, int rows, int cols, Pengu P)
{
    // Checks each value of the array for Pengu
    // and sets Pengu's starting coordinates
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (arr[i][j] == 'P')
            {
                P.row_val = i;
                P.col_val = j;
            }
        }
    }

    // Return the updated Pengu
    return P;
}

// This function takes in the pointer array, the number of rows and columns,
// the number of fish on the starting array, and a Pengu struct value P and
// then moves the Pengu around the array based on the direction algorithm,
// while updating the values and the array and when movement is over, 
// returning said Pengu with its updated values
Pengu moves(char **arr, int rows, int cols, int max_fish, Pengu P)
{
    const int POS_MOVE = 1; // Used to increment row or col for row by 1
    const int NEG_MOVE = -1; // Used to decrement row or col for row by 1
    const int NO_MOVE = 0; // Used if row or col value is unchanged by a move

    // Below 9 values are used in correlation to numpad directions for 
    // Pengu's movement value
    const int SOUTH_WEST = 1;
    const int SOUTH = 2;
    const int SOUTH_EAST = 3;
    const int WEST = 4;
    const int PENGU_SPOT = 5;
    const int EAST = 6;
    const int NORTH_WEST = 7;
    const int NORTH = 8;
    const int NORTH_EAST = 9;

    bool valid_move = true; // Checks if a move is valid
    bool max_points = false; // Used to check if Pengu has 
    int direction = 0; // Used for the switch case to determine direction
    int row = 0; // Used to add or subtract one to the row per move
    int col = 0; // Used to add or subtract one to the column per move
    int new_row = 0; // Next row value to check and or move to
    int new_col = 0; // Next column value to check and or move to

    // Checks to make sure that Pengu is alive, Pengu hasn't obtained all
    // the fish yet, and that the total moves made is less than 6 (assignment 
    // 1 constraint)
    while (P.alive == true && max_points == false && P.moves.length() < 6)
    {
        // Reset valid_move to true above the while loop to make it step into
        // the loop while the above loop conditions remain capable
        valid_move = true;

        while (valid_move == true)
        {
            // Picks a random direction for Pengu to move
            direction = (rand() % (9) + 1);
            switch (direction)
            {
                case SOUTH_WEST:
                    // Assigns next move to southwest (1)
                    row = POS_MOVE;
                    col = NEG_MOVE;
                    break;
                case SOUTH:
                    // Assigns next move to south (2)
                    row = POS_MOVE;
                    col = NO_MOVE;
                    break;
                case SOUTH_EAST:
                    // Assigns next move to southeast (3)
                    row = POS_MOVE;
                    col = POS_MOVE;
                    break;
                case WEST:
                    // Assigns next move to west (4)
                    row = NO_MOVE;
                    col = NEG_MOVE;
                    break;
                case EAST:
                    // Assigns next move to east (6)
                    row = NO_MOVE;
                    col = POS_MOVE;
                    break;
                case NORTH_WEST:
                    // Assigns next move to northwest (7)
                    row = NEG_MOVE;
                    col = NEG_MOVE;
                    break;
                case NORTH:
                    // Assigns next move to north (8)
                    row = NEG_MOVE;
                    col = NO_MOVE;
                    break;
                case NORTH_EAST:
                    // Assigns next move to northeast (9)
                    row = NEG_MOVE;
                    col = POS_MOVE;
                    break;
                case PENGU_SPOT:
                    // Pengu can't move to its current position.
                    valid_move = false;
                    break;
                default:
                    break;
            }

            // Assign new values for row and column to check
            new_row = P.row_val + row;
            new_col = P.col_val + col;

            // Preliminary check to make sure Pengu isn't moving into
            // a wall immediately as that is an invalid move
            if (arr[new_row][new_col] == '#')
            {
                valid_move = false;
            }

            // Enters this if above clause is true
            if (valid_move == true)
            {
                while (valid_move == true)
                {                    
                    while (P.on_snow == true && valid_move == true)
                    {
                        // If Pengu moves from snow to a fish, previous
                        // space is set to '0', the fish value increments by
                        // 1, and then set on_snow to false and Pengu
                        // continues to move
                        if (arr[new_row][new_col] == '*')
                        {
                            P.previous_row = P.row_val;
                            P.previous_col = P.col_val;
                            P.fish += 1;
                            arr[P.previous_row][P.previous_col] = '0';
                            new_row += row;
                            new_col += col;
                            P.on_snow = false;                            
                        }

                        // If Pengu moves from snow to a bear or a shark,
                        // replaces previous space to '0', assigns the bear
                        // or shark space to 'X', assigns Pengu's on_snow 
                        // value to false, and Pengu's alive value is then set
                        // to treat Pengu as dead, and ends the move
                        if (arr[new_row][new_col] == 'U' || 
                        arr[new_row][new_col] == 'S')
                        {
                            P.previous_row = P.row_val;
                            P.previous_col = P.col_val;
                            P.row_val = new_row;
                            P.col_val = new_col;
                            arr[P.previous_row][P.previous_col] = '0';
                            arr[P.row_val][P.col_val] = 'X';
                            P.on_snow = false;
                            P.moves += to_string(direction);
                            valid_move = false;
                            P.alive = false;
                        }

                        // If Pengu moves from snow to ice, replaces previous
                        // space to '0', assigns on_snow to false, and
                        // continues to move
                        if (arr[new_row][new_col] == ' ')
                        {
                            P.previous_row = P.row_val;
                            P.previous_col = P.col_val;
                            P.row_val = new_row;
                            P.col_val = new_col;
                            arr[P.previous_row][P.previous_col] = '0';
                            P.on_snow = false;
                            new_row += row;
                            new_col += col;
                        }

                        // If Pengu is already on snow and tries to move to
                        // snow, assigns previous snow space to '0' and the
                        // new snow space to 'P' and ends that move while
                        // keeping on_snow as true
                        if (arr[new_row][new_col] == '0' && P.on_snow == true)
                        {
                            P.previous_row = P.row_val;
                            P.previous_col = P.col_val;
                            P.row_val = new_row;
                            P.col_val = new_col;
                            arr[P.previous_row][P.previous_col] = '0';
                            arr[P.row_val][P.col_val] = 'P';
                            P.moves += to_string(direction);
                            P.on_snow = true;
                            valid_move = false;
                        }

                        // I believe this shouldn't be used but I kept it in
                        // in case of Pengu deciding to be a madman and bypass
                        // my above check if the next move is a wall to try
                        // moving into a wall from snow which shouldn't be
                        // possible but it's like the wall treatment below
                        // in my no_snow while loop but for if Pengu is on snow
                        if (arr[new_row][new_col] == '#')
                        {
                            P.previous_row = P.row_val;
                            P.previous_col = P.col_val;
                            P.row_val = new_row - row;
                            P.col_val = new_col - col;
                            arr[P.previous_row][P.previous_col] = '0';
                            arr[P.row_val][P.col_val] = 'P';
                            P.moves += to_string(direction);
                            valid_move = false;
                        }
                    }
                    while (P.on_snow == false && valid_move == true)
                    {
                        // If Pengu is on ice and the next tile is fish,
                        // Pengu assigns previous tile to ' ', moves into
                        // the fish, increments Pengu's score, and then keeps
                        // moving
                        if (arr[new_row][new_col] == '*')
                        {
                            P.previous_row = P.row_val;
                            P.previous_col = P.col_val;
                            P.row_val = new_row;
                            P.col_val = new_col;
                            P.fish += 1;
                            arr[P.previous_row][P.previous_col] = ' ';
                            new_row += row;
                            new_col += col;
                        }

                        // If Pengu is on ice and the next tile is a bear or
                        // a shark, Pengu replaces previous tile to ' ', bear
                        // or shark tile to 'X', and ends move while assigning
                        // Pengu's alive value to treat him as dead
                        if (arr[new_row][new_col] == 'U' || 
                        arr[new_row][new_col] == 'S')
                        {
                            P.previous_row = P.row_val;
                            P.previous_col = P.col_val;
                            P.row_val = new_row;
                            P.col_val = new_col;
                            arr[P.previous_row][P.previous_col] = ' ';
                            arr[P.row_val][P.col_val] = 'X';
                            P.moves += to_string(direction);
                            valid_move = false;
                            P.alive = false;
                        }

                        // If Pengu is on ice and the next tile is ice, Pengu
                        // will continue to move while reassigning the tile
                        // it was last on to ' '
                        if (arr[new_row][new_col] == ' ')
                        {
                            P.previous_row = P.row_val;
                            P.previous_col = P.col_val;
                            P.row_val = new_row;
                            P.col_val = new_col;
                            arr[P.previous_row][P.previous_col] = ' ';
                            new_row += row;
                            new_col += col;
                        }

                        // If Pengu is on ice (sliding) and encounters a wall,
                        // Pengu will stop on the space before the wall,
                        // replace starting position with ' ' and end
                        // position to 'P', and ends move
                        if (arr[new_row][new_col] == '#')
                        {
                            P.previous_row = P.row_val;
                            P.previous_col = P.col_val;
                            P.row_val = new_row - row;
                            P.col_val = new_col - col;
                            arr[P.previous_row][P.previous_col] = ' ';
                            arr[P.row_val][P.col_val] = 'P';
                            P.moves += to_string(direction);
                            valid_move = false;
                        }

                        // If Pengu is on ice and moves to snow, sets
                        // previously known position back to ice and
                        // assigns the snow tile with 'P' and ends move
                        if (arr[new_row][new_col] == '0')
                        {
                            P.previous_row = P.row_val;
                            P.previous_col = P.col_val;
                            P.row_val = new_row;
                            P.col_val = new_col;
                            arr[P.previous_row][P.previous_col] = ' ';
                            arr[P.row_val][P.col_val] = 'P';
                            P.moves += to_string(direction);
                            valid_move = false;
                            P.on_snow = true;
                        }
                    }

                    // Checks at end of move if Pengu obtained all the
                    // fish (after checking if Pengu died)
                    if (max_fish == P.fish)
                    {
                        max_points = true;
                        valid_move = false;
                    }
                }
            }

            // Reassigning these to 0 after a successful move has been 
            // completed
            new_row = 0;
            new_col = 0;
            row = 0;
            col = 0;
        }
    }

    // Return updated Pengu values
    return P;    
}

int main(int argc, char** argv)
{
    srand(time(NULL));
    int max_fish = 0; // To be assigned a value based on max score
    int rows = 0; // Holds the input value for the number of rows
    int cols = 0; // Holds the input value for the number of columns
    Pengu P; // The Pengu itself
    string file_in = argv[1]; // Input file name
    string file_out = argv[2]; // Output file name
    ifstream inFile; // Value to hold input file
    ofstream outFile; // Value to hold input file
    inFile.open(file_in); // Open input file
    outFile.open(file_out); // Open output file

    // Take in the first two values, which should be rows and cols 
    // respectively
    inFile >> rows;
    inFile >> cols;

    // I make 2 arrays here, one of which I make usable via pointers for 
    // easier passing of the array in the functions.
    char **array;

    // Read in the lines of the file and assign them to a row
    array = new char* [rows];

    for (int i = 0; i <= rows; i++)
    {
        string line;

        getline(inFile, line);

        array[i] = new char[cols];
        for (int j = 0; j < cols; j++)
        {
            array[i][j] = line[j];
        }
    }

    // Close the input file
    inFile.close();

    // Assign Pengu's starting value
    P = start_val(array, rows, cols, P);

    // Determine what the max possible score is
    max_fish = total_fish(array, rows, cols);

    // Make Pengu move via the movement algorithm
    P = moves(array, rows, cols, max_fish, P);    

    // Output the moves made as a string and Pengu's score
    outFile << P.moves << endl;
    outFile << P.fish << endl;

    // Output the final array
    for (int i = 1; i <= rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            outFile << array[i][j];
        }
        outFile << endl;
    }

    // Close the output file
    outFile.close();
    
    for (int i = 0; i < rows; i++)
    {
        delete [] array[i];
    }
    delete [] array;
    array = NULL;

    return 0;
}
