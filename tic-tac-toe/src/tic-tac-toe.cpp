//============================================================================
// Name        : tic-tac-toe.cpp
// Author      : Aditya
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <vector>
#include <queue>
using namespace std;

class Player{
private:
	string playerName;
	int playerId;
	int playerRanking;
	char playerSymbol;
public:
	 string getPlayerName() {
		return this->playerName;
	}

	 void setPlayerName(string playerName) {
		this->playerName = playerName;
	}

	 int getPlayerId() {
		return this->playerId;
	}

	 void setPlayerId(int playerId) {
		this->playerId = playerId;
	}

	 int getPlayerRanking() {
		return this->playerRanking;
	}

	 void setPlayerRanking(int playerRanking) {
		this->playerRanking = playerRanking;
	}

	 char getPlayerSymbol() {
		return this->playerSymbol;
	}

	 void setPlayerSymbol(char playerSymbol) {
		this->playerSymbol = playerSymbol;
	}

};

class GameBoard{
private:
	vector<vector<char>>Board;
	int boardSize;
	queue<Player> nextTurn;

public:
    int getBoardSize() {
		return this->boardSize;
	}

	void setBoardSize(int boardSize) {
		this->boardSize = boardSize;
	}

	GameBoard(int boardSize, Player players[]){
		vector<vector<char>>temp(boardSize, vector<char>(boardSize, '_'));
		this->Board =  temp;
        this->boardSize = boardSize;
        initializeBoard(this->Board);
        queue<Player>turns;
        turns.push(players[0]);
        turns.push(players[1]);
        this->nextTurn = turns;
	}

    bool validateInput(int row, int col){
        int sz = this->boardSize;
        return (row>=0 && row<sz) && (col>=0 && col<sz) && Board[row][col]=='_';
    }

    bool checkEndGame(Player p, int row, int col){
        char symbol = p.getPlayerSymbol();
        string winString = "";
        for(int i=0; i<boardSize; i++){
            winString += symbol;
        }
        string rowString = "";
        string colString = "";
        string diagonalString = "";
        string reverseDiagonalString = "";
        for(int i=0; i<boardSize; i++){
            rowString += Board[row][i];
            colString += Board[i][col];
            if(row==col){
                diagonalString += Board[i][i];
            }
            if((row+col)==(boardSize-1)){
                reverseDiagonalString += Board[boardSize-1-i][i];
            }
        }
        if(rowString==winString || colString==winString || diagonalString==winString || reverseDiagonalString==winString){
            cout<<p.getPlayerName()<<" wins !";
            return true;
        }
        return false;
    }

    void initializeBoard(vector<vector<char>>Board){
        int rows = this->boardSize;
        int cols = this->boardSize;
        for(int i=0; i<rows; i++){
            for(int j=0; j<cols; j++){
                Board[i][j] = '_';
            }
        }
    }

    void printBoard(){
        cout<<"\n******* BOARD STATUS *********\n";
        int rows = this->boardSize;
        int cols = this->boardSize;
        for(int i=0; i<rows; i++){
            for(int j=0; j<cols; j++){
                cout<<"\t"<<this->Board[i][j]<<" ";
            }
            cout<<"\n";
        }
        cout<<"\n*******************************\n";
    }

    pair<int, int> getUserInput(Player p){
        int row, col;
        cout<<"Enter Row Number(1 based Indexing) - ";
        cin>>row;
        cout<<"\nEnter Column Number(1 based Indexing) - ";
        cin>>col;
        row--;
        col--;
        while(!validateInput(row, col)){
            cout<<"\n!!! Please enter a valid Input !!!\n";
            cout<<"Enter Row Number(1 based Indexing) - ";
            cin>>row;
            cout<<"\nEnter Column Number(1 based Indexing) - ";
            cin>>col;
            row--;
            col--;
        }
        return make_pair(row, col);
    }

    void startGame(){
        int count = 0;
        int boardSize = this->boardSize;
        while(true){
            count++;
            if(count == ((boardSize*boardSize)+1)){
                cout<<"Match Drawn\n";
                break;
            }
            Player p = nextTurn.front();
            int rowNum, colNum;
            pair<int, int>pos = getUserInput(p);
            rowNum = pos.first;
            colNum = pos.second;
            char symbol = p.getPlayerSymbol();
            cout<<"symbol - "<<symbol<<"\n";
            this->Board[rowNum][colNum] = symbol;
            printBoard();
            if(count>=boardSize && checkEndGame(p, rowNum, colNum)) break;
            nextTurn.pop();
            nextTurn.push(p);
        }
    }



};

int main() {
	//cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
    Player p1;
    p1.setPlayerId(1);
    p1.setPlayerName("Adi");
    p1.setPlayerSymbol('o');
    Player p2;
    p2.setPlayerId(2);
    p2.setPlayerName("Abhi");
    p2.setPlayerSymbol('x');
    //cout<<p2.getPlayerName();
    Player players[] = {p1, p2};
    GameBoard gb(3, players);
    gb.startGame();
	return 0;
}
