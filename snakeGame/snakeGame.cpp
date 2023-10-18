#include <iostream>
#include <stdlib.h>
#include <conio.h>
#include <cstdlib>  
#include <ctime> 
#include<windows.h> 

// INITIATE VARIABLES
class gameSnake
{
	public:
		std::string table[20][20]; // game coordinates
		std::string debug;

		int snakeHX, snakeHY; // snake's head coordinates
		int foodX, foodY; // coordinates of our food
		int tailX[100], tailY[100];
		int tailLenght;
		int points;

		bool gameRunning; // game running state
		bool shouldgenFood; // should we generate food?

		enum movementStates { UP = 1, DOWN, LEFT, RIGHT }; // possible movement 
		movementStates currentState; // current heading way

		char snakeHead;
		char snakeBody;
		char food;

	gameSnake(bool run) //constructor to look cool
	{
		for (int i = 0; i < 20; i++)
			for (int j = 0; j < 20; j++)
				table[i][j] = " ";

		this->tailLenght = 1;
		this->snakeHX = 11;
		this->snakeHY = 9;
		this->gameRunning = run;
		this->currentState = UP;
		this->points = 0;
		this->shouldgenFood = true;
		this->snakeHead = 'O';
		this->snakeBody = 'o';
		this->food = '+';
	}
};

void clearBoard()
{
	system("CLS"); //CLEARS CONSOLE
}

void drawBoard(gameSnake* gameGiven)
{
	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			for (int k = 0; k < gameGiven->tailLenght; k++)
			{
				if (gameGiven->tailX[k] == j && gameGiven->tailY[k]  == i && k != 0) //if we have a valid body continue
				{
					gameGiven->table[j][i] = gameGiven->snakeBody; // print body; we already added head and food to table so no need for those
				}
			}
			std::cout << gameGiven->table[j][i]; // print our walls ( | and _ )
		}

		std::cout << '\n'; // we completed the line, \n for the next one
	}	
}

void clearTable(gameSnake* gameGiven)
{
	for (int i = 0; i < 20; i++)
		for (int j = 0; j < 20; j++)
			gameGiven->table[i][j] = " "; //completely clear the board for next time
}

void calcBoard(gameSnake* gameGiven)
{
	clearTable(gameGiven);

	for (int i = 0; i < 20; i++) //build "y" walls
	{
		gameGiven->table[0][i] = "|";
		gameGiven->table[19][i] = "|";
		if (gameGiven->snakeHX == i && (gameGiven->snakeHX == 0 || gameGiven->snakeHX == 19)) // head is in wall?
			gameGiven->gameRunning = false;
	}

	for (int j = 0; j < 20; j++) //build "x" walls
	{
		gameGiven->table[j][0] = "_";
		gameGiven->table[j][19] = "_";

		if (gameGiven->snakeHX == j && (gameGiven->snakeHY == 0 || gameGiven->snakeHY == 19)) // head is in wall?
			gameGiven->gameRunning = false;
	}

	if (gameGiven->foodX == gameGiven->snakeHX && gameGiven->foodY == gameGiven->snakeHY) //check if head is in food
	{
		gameGiven->points += 10;	
		gameGiven->tailLenght++;
		gameGiven->shouldgenFood = true;
	}

	// tail follow
	int previousX = gameGiven->tailX[0];
	int previousY = gameGiven->tailY[0];
	int previousX2, previousY2;
	gameGiven->tailX[0] = gameGiven->snakeHX;
	gameGiven->tailY[0] = gameGiven->snakeHY;

	for (int a = 1; a < gameGiven->tailLenght; a++) //create temp tail coordinates for replacement
	{
		previousX2 = gameGiven->tailX[a];
		previousY2 = gameGiven->tailY[a];

		gameGiven->tailX[a] = previousX;
		gameGiven->tailY[a] = previousY;

		previousX = previousX2;
		previousY = previousY2;
	}

	// replace O with X if game over
	if (gameGiven->gameRunning == false)
		gameGiven->snakeHead = 'X';

	// put data in table
	gameGiven->table[gameGiven->snakeHX][gameGiven->snakeHY] = gameGiven->snakeHead;
	gameGiven->table[gameGiven->foodX][gameGiven->foodY] = gameGiven->food;

	drawBoard(gameGiven);

}

void getsetMovementKey(gameSnake* gameGiven)
{
	if ( _kbhit() ) // keyboard hit function 
	{
		switch ( _getch() ) // gets the key which was hit
		{
			case 'a':
				gameGiven->currentState = gameSnake::movementStates::LEFT; 
				gameGiven->debug = "a";
				break;
			case 'd':
				gameGiven->currentState = gameSnake::movementStates::RIGHT;
				gameGiven->debug = "d";
				break;
			case 's':
				gameGiven->currentState = gameSnake::movementStates::DOWN;
				gameGiven->debug = "s";
				break;
			case 'w':
				gameGiven->currentState = gameSnake::movementStates::UP;
				gameGiven->debug = "w";
				break;

			default: //w a s d keys above
				break;
		}
	}
}

void generateFood(gameSnake* gameGiven)
{
	if (!gameGiven->shouldgenFood) // already a food is out there
		return;
	int x, y; // initiate food coordinates

	do
	{
		srand((unsigned)time(NULL));
		x = rand() % 18; // random number between 0-17
	    y = rand() % 18; // ^^

		x++, y++; // increase it by 1 (1-18) - this is so that we dont generate food on walls

		gameGiven->foodX = x; // assign food coordinates
		gameGiven->foodY = y; // ^^
		gameGiven->shouldgenFood = false; // we generated the food, set this to false
	} while (gameGiven->table[x][y] == "O" || gameGiven->table[x][y] == "o"); //generate random again if the food was generated in snake

}


void moveSnake(gameSnake* gameGiven)
{
	switch (gameGiven->currentState)
	{
	
		case (gameSnake::movementStates::LEFT):
			gameGiven->snakeHX--; //to go left, lower x
			break;
		case (gameSnake::movementStates::RIGHT):
			gameGiven->snakeHX++; //to go right, increase x
			break;
		case (gameSnake::movementStates::UP):
			gameGiven->snakeHY--; //to go up, lower y
			break;
		case (gameSnake::movementStates::DOWN):
			gameGiven->snakeHY++; //to go down, increase y
			break;
		default:
			break;
	}
}

void debugGame(gameSnake* gameGiven)
{
	std::cout << "points: " << gameGiven->points << '\n';

	#ifdef _DEBUG //debug other info
		std::cout << "snake x,y: " << gameGiven->snakeHX << " , " << gameGiven->snakeHY << '\n';
		std::cout << "food x,y: " << gameGiven->foodX << " , " <<  gameGiven->foodY << '\n';
		std::cout << "moving to: " << gameGiven->currentState << '\n';
		std::cout << "key: " << gameGiven->debug << '\n';
		std::cout << "lenght: " << gameGiven->tailLenght << '\n';
		std::cout << "game: " << gameGiven->gameRunning << '\n';
	#endif
}

int main()
{
	gameSnake game(true);

	while (game.gameRunning == true)
	{
		if (game.table[game.snakeHX][game.snakeHY] == "o")
			game.gameRunning = false; // last check to make sure we are not over our body

		if (game.gameRunning == false)
			game.snakeHead = 'X'; // make head X is game is over

		clearBoard();
		getsetMovementKey(&game);
		moveSnake(&game);
		generateFood(&game);
		calcBoard(&game); 
		debugGame(&game);

		Sleep(190); // lower / increase this depending on your cpu speed

	}

	std::cout << "Thanks for playing!" << '\n';
	std::cout << "Game by Batuhan Arda" << '\n';
	return 0;
}

