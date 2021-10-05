#include <bits/stdc++.h>
#include <conio.h>
using namespace std;
int dimension = 0;             //Dimension of the matrix
int win_condition = 2048;      //Win conditions
vector<vector<int>> board;     //Board that dynamincally sets assigned its size
int dirLine[] = {1, 0, -1, 0}; //Position of row with respect to the input direction of user
int dirCol[] = {0, 1, 0, -1};  //Position change of column with respect to the input direction of the user
int score = 0;                 //Score tracker
int b_score = 0;               //Best Score Tracker
int min_score = INT_MAX;       //To find best score in a session
int flag = 1;                  //Flag variable
//Set size of matrix and initial it to all zero's
void initial(vector<vector<int>> &board, int n)
{
    vector<vector<int>> b(n, vector<int>(n, 0));
    board.assign(b.begin(), b.end());
}

//To generate the position of the 2 generated after every operations
pair<int, int> generatefreeposition()
{
    int occupied = 1, line = 0, column = 0;
    while (occupied)
    {
        line = rand() % dimension;    //Generate a random row
        column = rand() % dimension;  //Generate a random column
        if (board[line][column] == 0) //checking if generated position is empty
            occupied = 0;
    }
    return {line, column}; //returning random empty position in matrix
}

//Function to check state of the board ,ie, to decide if a player has won, lost or still playing
int result()
{
    for (int i = 0; i < dimension; i++)
    {
        for (int j = 0; j < dimension; j++)
        {
            if (board[i][j] == win_condition) //checking if any cell in the maxtrix has the winning number(2048 usually)
            {
                return 1; //if true return 1 to print won in main function
            }
        }
    }
    for (int i = 0; i < dimension; i++)
    {
        for (int j = 0; j < dimension; j++)
        {
            if (board[i][j] == 0) //checking if atleast one element is empty in the matrix
                return 2;         //if true return 2 to indicate to continue playing the game
        }
    }
    //To check if any movement causes the filled matrix to shrink(done by checking if any two adjacent elements are same)
    for (int i = 0; i < dimension - 1; i++)
    {
        for (int j = 0; j < dimension - 1; j++)
        {
            if (board[i][j] == board[i + 1][j] || board[i][j] == board[i][j + 1]) //checking if same elements exist adjacently(last row and column are not checked)
                return 2;
        }
    }
    for (int i = 0; i < dimension - 1; i++)
    {
        if (board[i][dimension - 1] == board[i + 1][dimension - 1]) //checking if adjacent same element exist in the last column
            return 2;
    }
    for (int i = 0; i < 3; i++)
    {
        if (board[dimension - 1][i] == board[dimension - 1][i + 1]) //checking if adjacent same element exist in the last row
            return 2;
    }
    return 3; //if all the above condition doesnt return anything it implies that board is full
              //and no movement will shrink the board, return 3 to indicate you lost the game
}

//Add a 2 in a random position in the board which is currently empty
void addpiece()
{
    pair<int, int> pos = generatefreeposition(); //creates integer pair with empty position of the matrix
    vector<int> arr = {2, 2, 2, 2, 4};
    board[pos.first][pos.second] = arr[rand() % 5]; //Put a random 2 or 4 in the matrix
}

//Starts a newgame with the board initialized to all zeros
void newgame()
{
    if (min_score > score && score != 0)
        min_score = score;
    score = 0; //Score set to 0
    if (min_score != 0 && min_score != INT_MAX)
        b_score = min_score;
    for (int i = 0; i < dimension; i++)
    {
        for (int j = 0; j < dimension; j++)
        {
            board[i][j] = 0; //Initializes board with zero's
        }
    }
    addpiece(); // Adds a 2 or 4 to an empty position in the matrix
}

//To Print current state of the board
void print_ui()
{
    system("cls");
    cout << "\n\t\t\t\t\t\t2048 GAME\t\t\t\t\n";
    cout << "\n\n\t\t\t\t\t\t\t\t\t\t\t\t\t\tSCORE:" << score;
    cout << "\n\n\t\t\t\t\t\t\t\t\t\t\t\t\t\tBEST SCORE:" << b_score;
    for (int i = 0; i < dimension; i++)
    {
        cout << "\n\n\n";
        for (int j = 0; j < dimension; j++)
        {
            if (board[i][j] == 0)
                cout << "\t\t[" << setw(4) << " " << setw(4) << "]";
            else
                cout << "\t\t[" << setw(4) << board[i][j] << setw(4) << "]";
        }
    }
    cout << "\n\n\t\t\t\t\t\tN: New Game \t\t Q: Quit Game";
    cout << "\n\n\tW: To move up";
    cout << "\n\n\tS: To move down";
    cout << "\n\n\tA: To move left";
    cout << "\n\n\tD: To move right";
}

//To check if the adjacent movement of number is valid
bool isvalid(int line, int col, int nextline, int nextcol)
{
    if (nextline < 0 || nextcol < 0 || nextline >= dimension || nextcol >= dimension || board[line][col] != board[nextline][nextcol] && board[nextline][nextcol] != 0)
        return false;
    return true;
}

//To compute and update the movements of the pieces in the board according to their current positions in the board
void applymove(int dir)
{
    int row = 0, col = 0, rowstep = 1, colstep = 1;
    if (dir == 0)
    {
        row = dimension - 1;
        rowstep = -1;
    }
    if (dir == 1)
    {
        col = dimension - 1;
        colstep = -1;
    }
    int movepossible = 0, canAddpiece = 0;
    do //Loop till no morepossible movement exists for all non zero elements in the matrix in the user inputed directions
    {
        movepossible = 0;
        for (int i = row; i >= 0 && i < dimension; i += rowstep)
        {
            for (int j = col; j >= 0 && j < dimension; j += colstep)
            {
                int nexti = i + dirLine[dir], nextj = j + dirCol[dir]; //Finding adjacent position according to user inputed direction
                if (board[i][j] && isvalid(i, j, nexti, nextj))        //Check if movement to next position is valid
                {
                    if (board[nexti][nextj] == board[i][j]) //if adjacent elements are the same update score with their sum
                    {
                        score += (board[nexti][nextj] + board[i][j]);
                        if (b_score <= score && flag == 1)
                        {
                            b_score = score;
                        }
                    }
                    board[nexti][nextj] += board[i][j]; //Move element to next row by suming with either 0 or same element
                    board[i][j] = 0;                    //set previous cell to zero
                    movepossible = canAddpiece = 1;     //Set movepossible to 1 to check again for all elements a possible movement exists after updateing current matrix
                }
            }
        }
    } while (movepossible);
    if (canAddpiece) //after movement add a random 4 or 8 to the matrix
    {
        addpiece();
    }
}

//Main function thats binds together all the functions to implement the system
int main()
{
    srand(time(0));
    unordered_map<char, int> command_dir; //to map user input into numbers
    command_dir['s'] = 0;
    command_dir['d'] = 1;
    command_dir['w'] = 2;
    command_dir['a'] = 3;
    dimension = 4;             //changing dimension causes the size of the board to change
    initial(board, dimension); //changes size of the board with dimension
    win_condition = 2048;
start:           //if player wishes to play once more
    newgame();   //initializes the game
    int res = 2; //sets res to 2(result conditions)
    while (res == 2)
    {
        print_ui();
        char choice;
        choice = getch(); //gets choice from user
        if (choice == 'n')
            newgame();
        else if (choice == 'q')
            return 0;
        else
        {
            int curdir = command_dir[choice];
            applymove(curdir); //movement applied
        }
        res = result(); //stores current state of the game
    }
    print_ui(); // to show winning or the losing board
    if (res == 1)
        cout << "\n\nWon";
    else
        cout << "\n\nLost";
    char ch;
    cout << "\n\nDo you wish to continue - 'y' : 'n'";
    flag = 0;
    cin >> ch;
    if (ch == 'y')
        goto start; //if user wishes to play again control goes to start

    return 0;
}