#include <bits/stdc++.h>

using namespace std;

class Solution{
public:
    bool isValidSudoku(vector<vector<char>> &board){
        for (int n = 0; n < 9; n++){
            if (!this->isValidBox(board, n)) return false;
        }for (int r = 0; r < 9; r++){
            if (!this->isValidRow(board, r)) return false;
        }for (int c = 0; c < 9; c++){
            if (!this->isValidColumn(board, c)) return false;
        }return true;
    }

    bool isValidBox(vector<vector<char>> &board, int n){
        int i = 3*(n%3);
        int j = 3*(n/3);
        vector<int> map(9, 0);
        for (int x = i; x < i+3; x++){
            for (int y = j; y < j+3; y++){
                if (board[x][y] == ' ') continue;
                int in = board[x][y] - '1';
                if (map[in]) return false;
                map[in]++;
            }
        }return true;
    }

    bool isValidRow(vector<vector<char>> &board, int r){
        vector<int> map(9, 0);
        for (int x = 0; x < 9; x++){
            if (board[r][x] == ' ') continue;
            int in = board[r][x] - '1';
            if (map[in]) return false;
            map[in]++;
        }return true;
    }

    bool isValidColumn(vector<vector<char>> &board, int c){
        vector<int> map(9, 0);
        for (int y = 0; y < 9; y++){
            if (board[y][c] == ' ') continue;
            int in = board[y][c] - '1';
            if (map[in]) return false;
            map[in]++;
        }return true;
    }
};

void display(vector<vector<char>> board){
    for (vector<char> row: board){
        for (char r: row){
            cout << r << ' ';
        }cout << endl;
    }
}

int main(){

    vector<vector<char>> board(9, vector<char>(9, ' '));

    for (int y = 0; y < 9; y++){
        for (int x = 0; x < 9; x++){
            char t;
            cin >> t;
            if (t == '-') continue;
            board[y][x] = t;
        }   
    }

    //display(board);

    Solution sol;
    bool valid = sol.isValidSudoku(board);
    cout << valid << endl;

    return 0;
}
