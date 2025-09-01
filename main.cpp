#include <bits/stdc++.h>
#include <windows.h>
#include <conio.h>
#include <cstdlib>
#define sleep Sleep
#define clear ClearScreen()

using namespace std;

void ClearScreen()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD count, cellCount;
    COORD homeCoords = {0, 0};

    if (hConsole == INVALID_HANDLE_VALUE)
        return;

    // Get console buffer info
    if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
        return;
    cellCount = csbi.dwSize.X * csbi.dwSize.Y;

    // Fill console with spaces
    if (!FillConsoleOutputCharacter(hConsole, (TCHAR)' ', cellCount, homeCoords, &count))
        return;

    // Fill attributes
    if (!FillConsoleOutputAttribute(hConsole, csbi.wAttributes, cellCount, homeCoords, &count))
        return;

    // Move cursor to top-left
    SetConsoleCursorPosition(hConsole, homeCoords);
}

class TicTacToe
{
    vector<vector<int>> board;

    int EMPTY;
    vector<int> MARK;
    int turn;

    vector<vector<int>> rowMap;
    vector<vector<int>> colMap;
    vector<map<int, int>> diagMap;

public:
    TicTacToe()
    {
        // enter your mark
        char playerMark;

        while (true)
        {
            cout << "Please enter the your mark (X or O): ";
            cin >> playerMark;
            if (playerMark == 'o' || playerMark == 'O')
            {
                playerMark = 0;
                break;
            }
            if (playerMark == 'x' || playerMark == 'X')
            {
                playerMark = 1;
                break;
            }
            else
            {
                cout << "incorrect input\n";
            }
        }

        rowMap.resize(2, vector<int>(3, 0));
        colMap.resize(2, vector<int>(3, 0));
        diagMap.resize(2);

        EMPTY = -1; // signifies empty cells
        MARK.resize(2);
        // 0 ->  circle, 1 -> cross mark
        MARK[0] = playerMark;     // assigining mark of player 1
        MARK[1] = 1 - playerMark; // assigning mark of player 2

        board.resize(3, vector<int>(3, EMPTY));
        turn = 0; // -> player always starts first

        game();
    }
    void game()
    {
        clear;
        while (true)
        {
            display();
            bool isEnd = checkResponse();
            if (isEnd)
                break; // game ends
            if (turn)
            { // true -> computer's turn
                computerPlay();
            }
            else
            { // false -> player's turn
                playerPlay();
            }
            turn = 1 - turn;
            clear;
        }
    }
    bool computerAttack()
    {
        for (int row = 0; row < 3; row++)
        {
            if (rowMap[1][row] == 2)
            {
                for (int col = 0; col < 3; col++)
                {
                    if (board[row][col] == -1)
                    {
                        board[row][col] = MARK[1];
                        return true;
                    }
                }
            }
        }
        for (int col = 0; col < 3; col++)
        {
            if (colMap[1][col] == 2)
            {
                for (int row = 0; row < 3; row++)
                {
                    if (board[row][col] == -1)
                    {
                        board[row][col] = MARK[1];
                        return true;
                    }
                }
            }
        }

        if (diagMap[1][0] == 2)
        {
            for (int row = 0; row < 3; row++)
            {
                int col = row;
                if (board[row][col] == -1)
                {
                    board[row][col] = MARK[1];
                    return true;
                }
            }
        }

        if (diagMap[1][2] == 2)
        {
            for (int row = 0; row < 3; row++)
            {
                int col = 2 - row;
                if (board[row][col] == -1)
                {
                    board[row][col] = MARK[1];
                    return true;
                }
            }
        }
        return false;
    }
    void computerDefend()
    {
        vector<int> handlePoints(3, 0);
        vector<int> markPos(2, -1);

        for (int row = 0; row < 3; row++)
        {
            for (int col = 0; col < 3; col++)
            {
                if (board[row][col] != -1)
                    continue;
                vector<int> tmpHandlePoints(3, 0);
                tmpHandlePoints[rowMap[0][row]]++;
                tmpHandlePoints[colMap[0][col]]++;

                if (row - col == 0)
                    tmpHandlePoints[diagMap[0][row - col]]++;
                if (row + col == 2)
                    tmpHandlePoints[diagMap[0][row + col]]++;

                int i = 2;
                while (i >= 0 && handlePoints[i] == tmpHandlePoints[i])
                    i--;
                if (i != -1 && tmpHandlePoints[i] > handlePoints[i])
                {
                    handlePoints = tmpHandlePoints;
                    markPos[0] = row;
                    markPos[1] = col;
                }
            }
        }

        int x = markPos[0], y = markPos[1];
        board[x][y] = MARK[1];
        rowMap[1][x]++;
        colMap[1][y]++;
        if (x - y == 0)
            diagMap[1][x - y]++;
        if (x + y == 2)
            diagMap[1][x + y]++;
    }
    void computerPlayHandler()
    {
        bool isAttack = computerAttack();
        if (!isAttack)
        {
            computerDefend();
        }
    }
    void computerPlay()
    {
        cout << "COMPUTER PLAYING ITS MOVE" << "\n";
        computerPlayHandler();
        sleep(1200);
    }
    bool checkResponse()
    {
        int ref = check();
        if (ref == -2)
            return false;
        if (ref == MARK[0])
        {
            cout << "HURRAH! you won\n";
        }
        else if(ref == MARK[1])
        {
            cout << "OOPS! the computer is smarter\n";
        }else if(ref == -1){
            cout << "No more moves board filled\n";
        }
        sleep(600);
        return true;
    }
    int check()
    {
        // check columns
        for (int row = 0; row < 3; row++)
        {
            bool isColumnEqual = true;
            int ref = board[row][0];
            for (int col = 0; col < 3; col++)
            {
                if (ref != board[row][col])
                    isColumnEqual = false;
            }
            if (isColumnEqual && ref != -1)
                return ref;
        }

        for (int col = 0; col < 3; col++)
        {
            bool isRowEqual = true;
            int ref = board[0][col];
            for (int row = 0; row < 3; row++)
            {
                if (ref != board[row][col])
                    isRowEqual = false;
            }
            if (isRowEqual && ref != -1)
                return ref;
        }

        if (board[0][0] != -1 && board[0][0] == board[1][1] && board[1][1] == board[2][2])
            return board[0][0];
        if (board[0][2] != -1 && board[0][2] == board[1][1] && board[1][1] == board[2][0])
            return board[0][2];

        int cnt = 0;
        for(int i = 0; i < 3; i++){
            for(int j = 0; j < 3; j++){
                cnt += (board[i][j] != -1);
            }
        }
        if(cnt == 9) return -1;
        return -2;
    }
    void display()
    {
        // Add some vertical spacing
        cout << "\n\n";

        // Horizontal centering (adjust spaces as per your console width)
        string indent = "        ";

        // Print column headers
        cout << indent << "    1   2   3\n";
        cout << indent << "  +---+---+---+\n";

        for (int row = 0; row < 3; row++)
        {
            cout << indent << row + 1 << " |";
            for (int col = 0; col < 3; col++)
            {
                char mark = ' ';
                if (board[row][col] == 0)
                    mark = 'O';
                else if (board[row][col] == 1)
                    mark = 'X';

                cout << " " << mark << " |";
            }
            cout << "\n"
                 << indent << "  +---+---+---+\n";
        }
    }

    void playerPlay()
    {
        int x, y;
        while (true)
        {
            cout << "Enter coordinates to mark\n";
            cout << "Enter Row (1-3) : ";
            cin >> x;
            cout << "Enter Col (1-3) : ";
            cin >> y;
            x--, y--;
            if (x < 0 || y < 0 || x >= 3 || y >= 3)
            {
                cout << "Wrong coordinate range\n";
                continue;
            }
            if (board[x][y] != -1)
            {
                cout << "Cell alredy occupied, re-enter\n";
                continue;
            }
            break;
        }

        rowMap[0][x]++;
        colMap[0][y]++;

        if (x + y == 2)
            diagMap[0][x + y]++;
        if (x - y == 0)
            diagMap[0][x - y]++;

        board[x][y] = MARK[0];
    }
};
int main()
{
    TicTacToe *game = new TicTacToe();
    return 0;
}