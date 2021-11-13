#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <random>
#include <chrono>
#include <cctype>
#include <limits>
#include <algorithm>
#include "board.h"
#include "game.h"
#include "gameDisplayer.h"
#include "cmd.h"

using namespace std;

// Alias to BoardBuilderDisplayer colors used in this file.
const Color TEXT_COLOR = GameDisplayer::TEXT_COLOR;
const Color ERROR_COLOR = GameDisplayer::ERROR_COLOR;

// Helper function for `promptBoardName`.
// Returns whether given 'char' can belong to a valid board name.
bool isValidCharForBoardName(char c) {
    return isalnum(c) || c == '_';
}

// Opens 'board_file' with 'name'. 'name' must be either
// a board name, a file name or a path.
//
// Returns whether opening was successful.
bool openBoardFile(ifstream &board_file, string &name) {
    if(name.size() == 0) {
        setcolor(ERROR_COLOR);
        cout << "Must input a path, the name of a file or a board name." << endl;
        return false;
    }

    // If user inputs the name of the board instead of the filename (or a path), 
    // extension must be appended.
    auto begin = name.begin();
    auto end = name.end();
    auto first_invalid_char = find_if_not(begin, end, isValidCharForBoardName);
    if(first_invalid_char == end) {
        name += ".txt";
    }

    board_file.open(name);
    if(!board_file.is_open() ) {
        setcolor(ERROR_COLOR);
        cout << "File '" << name << "' does not exist or is unavailable." << endl;
        return false;
    }
    
    return true;
}

// Try to read 'width' and 'height' from 'board_file', validating the values.
// Returns whether it was successful.
bool readBoardSize(unsigned int &width, unsigned int &height, ifstream &board_file) {
    board_file >> height;
    if(board_file.fail() || height == 0 || height > 20) {
        setcolor(ERROR_COLOR);
        cout << "Failed to parse height in given file." << endl;
        return false;
    }

    char _x; // Ignored
    board_file >> _x;

    board_file >> width;
    if(board_file.fail() || width == 0 || width > 20) {
        setcolor(ERROR_COLOR);
        cout << "Failed to parse width in given file." << endl;
        return false;
    }

    return true;
}

// Asks the user to input the number of players for the game.
// Keeps asking until user enters a valid name (in this case,
// returns true) or the stdin fails (in this case, returns false).
//
// 'board_letters' is used to calculate the maximum number of players.
//
// The number is stored in 'num_players'.
bool promptNumberPlayers(int &num_players, unsigned int board_letters) {
    // Get maximum number of players from the number of letters.
    int max_players = min(4, (int) board_letters/7);

    if(max_players == 2) {
        // Max players is two, so user doesn't have a choice.
        setcolor(TEXT_COLOR);
        cout << "This board only allows you to play a game with " << max_players << " players." << endl;
        cout << "Press ENTER to start a game with 2 players . . . " << endl;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        num_players = 2;
        return true;
    }

    while(true) {
        // Player has a choice, between 2 and 'max_players'
        string input;

        setcolor(TEXT_COLOR);
        cout << "This board allows you to play a game with up to " << max_players << " players." << endl;
        cout << "Input the number of players (2-" << max_players << "): ";
        getline(cin, input);
        if(cin.fail()) return false; 

        stringstream input_stream(input);
        input_stream >> num_players;

        if(input_stream.fail()) {
            setcolor(GameDisplayer::ERROR_COLOR);
            cout << endl << "Expected a positive integer." << endl;
            continue;
        }

        // Check if player didn't enter anything unexpected after the number of players.
        std::string unexpected;
        input_stream >> unexpected;
        if(unexpected.size() != 0) {
            setcolor(ERROR_COLOR);
            cout << "Unexpected '" << unexpected << "'." << endl;
            continue;
        }
        
        if(num_players < 2) {
            setcolor(GameDisplayer::ERROR_COLOR);
            cout << endl << "Must have at least 2 players." << endl;
            continue;
        }
        
        if(num_players > max_players) {
            setcolor(GameDisplayer::ERROR_COLOR);
            cout << endl << "Must have at most " << max_players << " players." << endl;
            continue;
        }

        return true;
    }
}

// Asks the user if they want to play again.
// Returns true if the answer is 'Y', and false if the answer is 
// 'N' (or the stdin fails).
bool askPlayAgain() {
    string input;
    while(true) {
        setcolor(TEXT_COLOR);
        cout << endl << "Play again? (Y/N): ";
        getline(cin, input);
        if(cin.fail()) return false;

        char answer;
        stringstream input_stream(input);
        input_stream >> answer;

        if(input_stream.fail()) {
            setcolor(ERROR_COLOR);
            cout << "Invalid input." << endl;
        } else if(answer == 'Y' || answer == 'y') { 
            return true;
        } else if(answer == 'N' || answer == 'n') {
            return false;
        } else {
            setcolor(ERROR_COLOR);
            cout << "Invalid input." << endl;
        }
    }
}

int main() {
    // Initialize rng
    unsigned seed = (unsigned int) chrono::system_clock::now().time_since_epoch().count();
    default_random_engine rng(seed);
    
    while(true) {
        // First, a valid board must be loaded.
        string file_name;

        setcolor(TEXT_COLOR);
        cout << "Input the name of your board: ";
        getline(cin, file_name);
        if(cin.fail()) return 0; // stdin failing ends the program
        
        ifstream board_file;
        if(!openBoardFile(board_file, file_name)) continue;

        unsigned int width, height;
        if(!readBoardSize(width, height, board_file)) {
            board_file.close();
            continue;
        }

        Board board(width, height);
        board.loadWords(board_file);
        board_file.close();

        // At this point the board is loaded, but it is only valid
        // to be played if it has at least 14 letters.
        if(board.countLetters() < 14) {
            setcolor(ERROR_COLOR);
            cout << "Board must have at least 14 letters in order to be playable." << endl;
            continue;
        }

        // Board was successfuly loaded at this point.
        // Now it's time to show current board and prompt the user
        // to choose the number of players.
        cout << endl;
        GameDisplayer::printBoard(board);
        cout << endl;

        int num_players; 
        promptNumberPlayers(num_players, board.countLetters());

        // Everything is ready to start the game.
        Game game(board, num_players);
        
        bool game_ended_successfuly = game.play(rng);
        // if game ended because stdin failed, end the program.
        if(!game_ended_successfuly) return 0; 

        if(!askPlayAgain()) break;
    }

    return 0;
}
