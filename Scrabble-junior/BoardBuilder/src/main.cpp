#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>
#include <cctype>
#include <algorithm>
#include "board.h"
#include "boardBuilder.h"
#include "boardBuilderDisplayer.h"
#include "cmd.h"

using namespace std;

// Alias to BoardBuilderDisplayer colors used in this file.
const Color TEXT_COLOR = BoardBuilderDisplayer::TEXT_COLOR;
const Color ERROR_COLOR = BoardBuilderDisplayer::ERROR_COLOR;
const Color WARNING_COLOR = BoardBuilderDisplayer::ERROR_COLOR;

// Checks if dictionary can be opened, otherwise warns the player
// and doesn't allow to progress until it is opened.
//
// This is so that the player can know from the start of the program
// that the dictionary is not available, instead of only finding out
// after starting building the board.
void checkDictionaryExistence() {
    while(!ifstream(BoardBuilder::DICTIONARY).is_open()) {
        setcolor(ERROR_COLOR);
        cout << "Dictionary file '" << BoardBuilder::DICTIONARY 
                << "' was not found in the current folder or could not be opened." << endl
                << "Without this file, you can't fill the boards with words, as they can't be validated." << endl;
        setcolor(TEXT_COLOR);
        cout << "Make sure such file exists and can be opened, then press ENTER: ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// Helper function for `promptBoardName`.
// Returns whether given 'char' can belong to a valid board name.
bool isValidCharForBoardName(char c) {
    return isalnum(c) || c == '_';
}

// Asks the user to input the name of the board being created.
// Keeps asking until user enters a valid name (in this case,
// returns true) or the stdin fails (in this case, returns false).
//
// The name is stored in 'board_name'
bool promptBoardName(string &board_name) {
    while(true) {
        setcolor(TEXT_COLOR);
        cout << "Name of new board: ";
        getline(cin, board_name);
        if(cin.fail()) return 0;

        if(board_name.size() == 0) {
            setcolor(ERROR_COLOR);
            cout << "Must input a name." << endl;
            continue;
        }

        // Search for an invalid char
        auto begin = board_name.begin();
        auto end = board_name.end();
        auto invalid_char = find_if_not(begin, end, isValidCharForBoardName);
        if(invalid_char != end) {
            setcolor(ERROR_COLOR);
            
            cout << "Only ASCII alphanumeric letters and underscores are allowed in board name." << endl;
            if(isspace(*invalid_char)) cout << "Whitespace is not allowed." << endl;
            else cout << "'" << *invalid_char << "' is not allowed." << endl;

            continue;
        }

        cout << "Board will be saved in file '" << board_name << ".txt'." << endl;

        return true;
    }
}

// Asks the user to input a given size (i.e. width or heigh) of the board being created.
//
// The prompt (what is asked to the user) and dimension (width/height) are both passed as arguments.
//
// Keeps asking until user enters a valid size (in this case,
// returns true) or the stdin fails (in this case, returns false).
//
// The size is stored in 'size'.
bool promptSize(int &size, const char *prompt, const char *dimension) {
    while(true) {
        setcolor(TEXT_COLOR);
        cout << prompt;
        string input;
        getline(cin, input);
        if(cin.fail()) return false;

        stringstream input_stream(input);
        input_stream >> size;
        
        if(input_stream.fail() || size <= 0) {
            setcolor(ERROR_COLOR);
            cout << "Expected a positive integer." << endl;
            continue;
        }

        // Check if player didn't enter anything unexpected after the size.
        std::string unexpected;
        input_stream >> unexpected;
        if(unexpected.size() != 0) {
            setcolor(ERROR_COLOR);
            cout << "Unexpected '" << unexpected << "'." << endl;
            continue;
        }

        // Maximum size in a board is 20.
        if(size > 20) {
            setcolor(ERROR_COLOR);
            cout << dimension << " can only be at most 20." << endl;
            continue;
        } 

        return true;
    }
}

// Asks the user to load an existing file and edit its board.
// Returns true if the answer is 'Y', and false if the answer is 
// 'N' (or the stdin fails, in which case the program is about to close).
bool askLoadFile() {
    string input;
    while(true) {
        setcolor(TEXT_COLOR);
        cout << "Do you wish to edit existing board? (Y/N): ";
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
    checkDictionaryExistence();

    string board_name;
    int board_width = 0; 
    int board_height = 0;

    // Ask user the name of the board, or exit if stdin fails.
    if(!promptBoardName(board_name)) return 0;

    bool will_overwrite_file = false; // true when a file already exists
    bool is_file_loadable = false; // true when the player wants to edit an existing board

    ifstream board_file(board_name + ".txt");
    if(board_file.is_open()) {
        // There already exists a file with this name.
        // The player has the option to edit it, or to create a new one and overwrite it.
        will_overwrite_file = true;

        setcolor(WARNING_COLOR);
        cout << "File '" << board_name << ".txt' already exists." << endl;

        if(askLoadFile()) {
            // Player wants to edit an existing board. Get its width and height.
            char _x; // Ignored delimiter character between height and width

            board_file >> board_height >> _x >> board_width;            

            // At least width and height must be valid to be able to open the board.
            if(board_file.fail() || board_height == 0 || board_height > 20 || board_width == 0 || board_width > 20) {
                setcolor(ERROR_COLOR);
                cout << "Board could not be loaded. Contents of the file may have been corrupted." << endl;
            } else {
                is_file_loadable = true;
            }
        }
    }

    if(!is_file_loadable) {
        if(will_overwrite_file) {
            setcolor(WARNING_COLOR);
            cout << "File '" << board_name << ".txt' will be overwritten when saving the board." << endl;
        }
        // Ask user the board width and height, or exit if stdin fails.
        if(!promptSize(board_width, "Width (number of columns): ", "Width")) return 0;
        if(!promptSize(board_height, "Height (number of rows): ", "Height")) return 0;
    }

    Board board(board_width, board_height);
    // File is assumed to be created by this program and, thus, valid.
    // Words will be parsed assuming they are valid and placed in valid positions.
    if(is_file_loadable) board.loadWords(board_file); 
    BoardBuilder builder(board_name, board);

    builder.run();
    builder.saveToFile();

    return 0;
}
