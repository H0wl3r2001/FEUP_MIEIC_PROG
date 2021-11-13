#include<iostream>
#include<iomanip>
#include<string>
#include<string.h>
#include<ctime>
#include<cstdlib>
#include<windows.h>


using namespace std;

// Colours (player 1 related stuff is blue, player 2's green):
#define BLACK 0
#define BLUE 1 
#define GREEN 2
#define CYAN 3
#define RED 4
#define MAGENTA 5
#define BROWN 6
#define LIGHTGRAY 7
#define DARKGRAY 8
#define LIGHTBLUE 9
#define LIGHTGREEN 10
#define LIGHTCYAN 11
#define LIGHTRED 12
#define LIGHTMAGENTA 13
#define YELLOW 14
#define WHITE 15

// Functions needed to alter the text colour
void clrscr(void)
{
	COORD coordScreen = { 0, 0 }; // upper left corner
	DWORD cCharsWritten;
	DWORD dwConSize;
	HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hCon, &csbi);
	dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
	// fill with spaces
	FillConsoleOutputCharacter(hCon, TEXT(' '), dwConSize, coordScreen, &cCharsWritten);
	GetConsoleScreenBufferInfo(hCon, &csbi);
	FillConsoleOutputAttribute(hCon, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten);
	// cursor to upper left corner
	SetConsoleCursorPosition(hCon, coordScreen);
}
// Clears the console

void setcolor(unsigned int color)
{
	HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hcon, color);
}
// Allows to set the text colour


//Global variables
bool Game_Over = false;
int current_player = 0;                      //0 ----> 1st player; 1 ----> 2nd player
int draws = 0;
int Turn = 1;
int board[12] = {4,4,4,4,4,4,4,4,4,4,4,4};
bool repeat = true;


struct player {
	string name;
	int score = 0; //number of seeds captured
	int victory = 0; //number of victories after each game
}jogador1, jogador2;

void reset()
{
	for (int r = 0; r < 12; r++)
	{
		board[r] = 4;
	}
	current_player = 0;
	jogador1.score = 0;
	jogador2.score = 0;
	Turn = 1;
	Game_Over = false;
}
// Here, we manipulate the global variables to "reset" the board and set the current player to 0, the turn to 1 and thus restart the game.

void verify_GO()
{
	if (jogador1.score >= 25 || jogador2.score >= 25 || (jogador1.score == 24 && jogador2.score == 24))
		Game_Over = true;
	else
	{
		Game_Over = false;
		if (current_player == 0)
		{// If the current player's side turns out empty, insert condition here
			if (board[0] == 0 && board[1] == 0 && board[2] == 0 && board[3] == 0 && board[4] == 0 && board[5] == 0)
			{
				if (((6 + board[6]) % 12 > 5) && ((7 + board[7]) % 12 > 5) && ((8 + board[8]) % 12 > 5) && ((9 + board[9]) % 12 > 5) && ((10 + board[10]) % 12 > 5) && ((11 + board[11]) % 12 > 5))
				{
					for (int e = 6; e < 12; e++)
					{
						jogador2.score += board[e];
						board[e] = 0;
					}
					verify_GO();
				}
				else
				{
					setcolor(2);
					cout << jogador2.name; setcolor(6); cout << "has to play the following houses for the game to continue:" << endl;
					for (int r = 6; r < 12; r++)
					{
						if (r + board[r] <= 5) cout << (r - 5) << endl;
					}
				}
			}
			else
			{
				current_player = 1;
				Turn++;
			}
		}
		else
		{// If the current player's side turns out empty, insert condition here
			if (board[6] == 0 && board[7] == 0 && board[8] == 0 && board[9] == 0 && board[10] == 0 && board[11] == 0)
			{
				if (((0 + board[0]) % 12 < 5) && ((1 + board[1]) % 12 < 5) && ((2 + board[2]) % 12 < 5) && ((3 + board[3]) % 12 < 5) && ((4 + board[4]) % 12 < 5) && ((5 + board[5]) % 12 < 5))
				{
					for (int e = 0; e < 6; e++)
					{
						jogador1.score += board[e];
						board[e] = 0;
					}
					verify_GO();
				}
				else
				{
					setcolor(1);
					cout << jogador1.name; setcolor(6); cout << "has to play the following houses for the game to continue:" << endl;
					for (int r = 0; r < 6; r++)
					{
						if (r + board[r] >= 5) cout << (r - 5) << endl;
					}
				}
			}
			else
			{
				current_player = 0;
				Turn++;
			}
		}
	}

	if (Turn == 200) // Game has gone for far too long, time to end it
	{
		setcolor(4);
		cout << "Turn limit reached!" << endl;
		Game_Over = true;
		if (jogador1.score > jogador2.score)
		{
			setcolor(1);
			cout << jogador1.name; setcolor(6); cout << " wins by having the most seeds after 200 turns!" << endl;
			jogador1.victory++;
		}
		else if (jogador2.score > jogador1.score)
		{
			setcolor(2);
			cout << jogador2.name; setcolor(6); cout << " wins by having the most seeds after 200 turns!" << endl;
			jogador2.victory++;
		}
		else
		{
			setcolor(7);
			cout << "There is no winner after 200 turns! It is thereby declared a draw!" << endl;
			draws++;
		}
	}
	else
	{
		if (jogador1.score >= 25) // 1st player wins
		{
			setcolor(1);
			cout << jogador1.name << " has won!" << endl;
			jogador1.victory++;
		}
		else if (jogador2.score >= 25) // 2nd player wins
		{
			setcolor(2);
			cout << jogador2.name << " has won!" << endl;
			jogador2.victory++;
		}
		else if (jogador1.score == 24 && jogador2.score == 24) // There is a draw
		{
			setcolor(7);
			cout << "It's a draw!" << endl;
			draws++;
		}
	}
}
//"Simple" function that verifies the game ending conditions (turn based, score based, exception based), and what player wins. Announces it aswell.

void seed_capture(int& cpl, int &lp)
{
	//cout << "seed capture - " << cpl << "; " << lp << endl;

	if (cpl == 0)
	{
		while ((lp >= 6 && lp <= 11) && (board[lp] == 2 || board[lp] == 3))
		{
			cout << "seed capture player 1- " << cpl << "; " << lp << endl;
			jogador1.score += board[lp];
			board[lp] = 0;
			lp--;
		}
	}
	else
	{
		while ((lp >= 0 && lp <= 5) && (board[lp] == 2 || board[lp] == 3))
		{
			cout << "seed capture player 2 - " << cpl << "; " << lp << endl;
			jogador2.score += board[lp];
			board[lp] = 0;
			lp--;
		}
	}
	
	cout << "Current player 1 score: ";
	setcolor(9);
	cout << jogador1.score << endl;
	setcolor(14);
	cout << "Current player 2 score: ";
	setcolor(10);
	cout << jogador2.score << endl;
	
	setcolor(14);
	verify_GO();

}
// The name says it all. Verifies after each play if any seeds are captured. It then calls for a game-over verification.

void play(int &cp, int &starting_place) 
{

	int casa_atual = starting_place; 
	int num_sementes = board[starting_place - 1];
	int last_place = (starting_place - 1 + num_sementes) % 12;
	board[casa_atual - 1] = 0;

	for (int c = num_sementes; c > 0; c--)
	{

		casa_atual++;
		if ((casa_atual) > 12)
		{
			casa_atual = 1;
		}
		if (casa_atual == starting_place)
		{
			casa_atual++;
			
		}
		//cout << "nº de sementes: " << c << " " << casa_atual - 1 << endl;
		board[casa_atual - 1]++;
	}

	/*for (int i = 0; i <= 11; i++)
		cout << i <<": " << board[i] << " " << endl; */
	
	seed_capture(current_player, last_place);
}
// Specifies what a valid selection changes in the board composition. 

void draw_Board()
{
	//Limite superior
	cout << char(218);
	for (int i = 1; i <= 10; i++) cout << char(196);
	for (int j = 1; j <= 5; j++)
	{
		cout << char(194);
		for (int i = 1; i <= 10; i++) cout << char(196);
	}
	cout << char(191) << endl;

	//Parte do 2º jogador (meto uma condição para não desformatar o tabuleiro)
	for (int h = 1; h <= 4; h++)
	{
		if (h == 2)
		{
			for (int k1 = 1; k1 <= 6 ; k1++)
			{
				cout << char(179);
				for (int i1 = 1; i1 < 5; i1++) cout << " ";
				setcolor(2);
				cout << board[12 - k1];
				setcolor(5);
				if (board[12-k1] >= 10) for (int i1 = 1; i1 < 5; i1++) cout << " ";
				else for (int i1 = 1; i1 < 6; i1++) cout << " ";
				//cout << char(179);
			}
			cout << char(179) << endl;
		}
		else
		{
			for (int k = 1; k <= 6; k++)
			{
				cout << char(179);
				for (int i = 1; i <= 10; i++) cout << " ";
			}
			cout << char(179) << endl;
		}
		
	}

	//linha que divide o tabuleiro
	cout << char(195);
	for (int i = 1; i <= 10; i++) cout << char(196);
	for (int j = 1; j <= 5; j++)
	{
		cout << char(197);
		for (int i = 1; i <= 10; i++) cout << char(196);
	}
	cout << char(180) << endl;

	// Parte do 1º jogador
	for (int h = 1; h <= 4; h++)
	{
		if (h == 2)
		{
			for (int k1 = 1; k1 <= 6; k1++)
			{
				cout << char(179);
				for (int i1 = 1; i1 < 5; i1++) cout << " ";
				setcolor(1);
				cout << board[k1 - 1];
				setcolor(5);
				if (board[k1 - 1] >= 10) for (int i1 = 1; i1 < 5; i1++) cout << " ";
				else for (int i1 = 1; i1 < 6; i1++) cout << " ";
				//cout << char(179);
			}
			cout << char(179) << endl;
		}
		else
		{
			for (int k = 1; k <= 6; k++)
			{
				cout << char(179);
				for (int i = 1; i <= 10; i++) cout << " ";
			}
			cout << char(179) << endl;
		}

	}

	//Última linha
	cout << char(192);
	for (int i = 1; i <= 10; i++) cout << char(196);
	for (int j = 1; j <= 5; j++)
	{
		cout << char(193);
		for (int i = 1; i <= 10; i++) cout << char(196);
	}
	cout << char(217) << endl;

	//Game_Over = true;
}
// Each turn, displays a full board formed with couts of ascii characters and fors.

void oware()
{
	setcolor(5);
	draw_Board();
	setcolor(14);
	cout << "Turn number: " << Turn << endl;
	int place;
	if (current_player == 0)
	{
		setcolor(1);
		cout << "It's " << jogador1.name << "'s turn. Choose the place: " << endl;
	}
		
	else
	{
		setcolor(2);
		cout << "It's " << jogador2.name << "'s turn. Choose the place: " << endl;
	}
	if (current_player == 0)
	{
		while (!(cin >> place) || place < 1 || place > 6 || board[place - 1] == 0)
		{
			cin.clear();
			cin.ignore();
			setcolor(4);
			cout << "It's an invalid place. Must be between 1-6 and must have seeds. Choose another:" << endl;
			setcolor(1);
		}
	}
	else
	{
		while (!(cin >> place) || place < 1 || place > 6 || board[place - 1 + 6] == 0)
		{
			cin.clear();
			cin.ignore();
			setcolor(4);
			cout << "It's an invalid place. Must be between 1-6 and must have seeds. Choose another:" << endl;
			setcolor(2);
		}
	}
	if (current_player == 1) place += 6;

	setcolor(14);
	play(current_player, place);
}
// Verifies if each user input is valid or not, asking again if necessary.

int main()
{
	setcolor(1);
	cout << "Insert 1st player's name:" << endl;
	getline(cin, jogador1.name);
	setcolor(2);
	cout << "Insert 2nd player's name:" << endl;
	getline(cin, jogador2.name);
	while (repeat == true)
	{
		do
		{
			oware();
		} 
		while (!Game_Over);
		setcolor(14);
		cout << "For now, "; setcolor(1); cout << jogador1.name; setcolor(14); cout << " has "; setcolor(9); cout << jogador1.victory; setcolor(14); cout << " victories, ";
		cout << "while "; setcolor(2); cout << jogador2.name; setcolor(14); cout << " has "; setcolor(10); cout << jogador2.victory; setcolor(14); cout << " victories." << endl;
		cout << "There has been "; setcolor(8); cout << draws; setcolor(14); cout << " draws until now." << endl;
		
		int repeat_l;
		setcolor(15);
		cout << "Press 0 if you want to repeat, other integer to quit: " << endl;
		while (!(cin >> repeat_l))
		{
			cin.clear();
			cin.ignore();
			setcolor(4);
			cout << "It's an invalid input. Must be or zero or other integer (to repeat or not, respectively):" << endl;
			setcolor(15);
		}
		if (repeat_l != 0)
			repeat = false;
		else
			reset();
	}

	setcolor(15);
	cout << "GameOver! It ended with ";
	setcolor(1);
	cout << jogador1.name;
	setcolor(15);
	cout << " having "; setcolor(9);
	cout << jogador1.victory;
	setcolor(15);
	cout << " victories, while ";
	setcolor(2);
	cout << jogador2.name;
	setcolor(15);
	cout << " having ";
	setcolor(10);
	cout << jogador2.victory;
	setcolor(15);
	cout << " victories!" << endl;

	cout << "In the end, it also ended with ";
	setcolor(8);
	cout << draws;
	setcolor(15);
	cout << "draws" << endl;
	cout << "Thank you for playing!";

	return 0;
}
// Executes the game until game over. Keeps track of the player's victories and asks if the same users want to play again 