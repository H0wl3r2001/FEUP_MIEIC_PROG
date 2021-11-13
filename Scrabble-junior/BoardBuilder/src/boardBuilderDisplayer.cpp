#include <iostream>
#include <fstream>
#include <algorithm>
#include <chrono>
#include <thread>
#include "boardBuilderDisplayer.h"

using namespace std;

const Color BoardBuilderDisplayer::TEXT_COLOR = LIGHTGRAY;
const Color BoardBuilderDisplayer::TEXT_COLOR_DARK = DARKGRAY;
const Color BoardBuilderDisplayer::ERROR_COLOR = RED;
const Color BoardBuilderDisplayer::WARNING_COLOR = YELLOW;
const Color BoardBuilderDisplayer::SUCCESS_COLOR = GREEN;
const Color BoardBuilderDisplayer::LETTER_COLOR = BLACK;
const Color BoardBuilderDisplayer::BOARD_BACKGROUND = LIGHTGRAY;

const int BoardBuilderDisplayer::ADD_LETTER_TO_BOARD_DELAY = 200;

BoardBuilderDisplayer::BoardBuilderDisplayer(unsigned int board_width, unsigned int board_height):
    // Offsets are calculated based on the size of the board on screen.
    board_info_x_offset(board_width*2 + 2),
    prompt_y_offset(max(8u, board_height + 2)) // Must not be smaller than 8 because of height of board properties
{}

std::ostream& BoardBuilderDisplayer::getErrorStream() {
    return error_messages;
}

void BoardBuilderDisplayer::clearErrors() {
    error_messages.str("");
}

void BoardBuilderDisplayer::printBoard(const Board &board) const {
    setcolor(TEXT_COLOR);
    cout << ' ';
    for(unsigned int i = 0; i < board.getWidth(); i++) {
        char letter = (char) i + 'a';
        cout << letter << ' ';
    }
    cout << '\n';

    for(unsigned int j = 0; j < board.getHeight(); j++) {
        char letter = (char) j + 'A';
        cout << letter;
        
        setcolor(LETTER_COLOR, BOARD_BACKGROUND);
        for(unsigned int i = 0; i < board.getWidth(); i++) {
            Position position((int) i, (int) j);
            cout << board.getCell(position);
            
            if(i+1 != board.getWidth()) {
                cout << ' ';
            }
        }
        setcolor(TEXT_COLOR);
        cout << '\n';
    }
}

void BoardBuilderDisplayer::printBoardInfo(const string &board_name, const Board &board, unsigned int max_players) const {
    // Board name
    gotoxy(board_info_x_offset, 1);
    setcolor(TEXT_COLOR_DARK);
    cout << "Board name: "; 
    setcolor(TEXT_COLOR);
    cout << board_name;

    // Playable by
    gotoxy(board_info_x_offset, 2);
    switch(max_players) {
        case 0:
        case 1:
            // 0 and 1 are below minimum number (2)
            setcolor(ERROR_COLOR);
            cout << "Not playable yet";
            break;
        case 2:
            setcolor(TEXT_COLOR_DARK);
            cout << "Playable by "; 
            setcolor(TEXT_COLOR);
            cout << "2 players";
            break;
        default:
            setcolor(TEXT_COLOR_DARK);
            cout << "Playable by "; 
            setcolor(TEXT_COLOR);
            cout << "2-" << max_players << " players";
            break;
    }

    // Number of letters
    gotoxy(board_info_x_offset, 4);
    setcolor(TEXT_COLOR_DARK);
    cout << "Number of letters: ";
    setcolor(TEXT_COLOR);
    cout << board.countLetters(); 

    // Number of words
    gotoxy(board_info_x_offset, 5);
    setcolor(TEXT_COLOR_DARK);
    cout << "Number of words: ";
    setcolor(TEXT_COLOR);
    cout << board.countWords(); 
}

void BoardBuilderDisplayer::printPrompt(unsigned int max_players, unsigned int total_letters) const {
    clrscr(0, prompt_y_offset);

    // Print a message of how many letters left for the board
    // to be playable with more players.
    unsigned int letters_needed;
    switch(max_players) {
        case 0:
        case 1:
            letters_needed = 14-total_letters;

            setcolor(ERROR_COLOR);
            cout << "This board needs " << letters_needed
                    << " more letters in order to be playable by at least 2 players." << endl << endl;
            break;
        case 2:
        case 3:
            letters_needed = (max_players+1)*7-total_letters;

            setcolor(WARNING_COLOR);
            cout << "This board needs " << letters_needed 
                    << " more letters in order to be playable by at least " << max_players+1 
                    << " players." << endl << "Ctrl+Z to save this board to a file and quit." << endl << endl;
            break;
        case 4:
            setcolor(SUCCESS_COLOR);
            cout << "This board is playable by 2-4 players." << endl 
                    << "Ctrl+Z to save this board to a file and quit." << endl << endl;
            break;
    }

    // Instructions for a valid input.
    setcolor(TEXT_COLOR);
    cout << "Inputs must be in the form 'Aa H WORD': " << endl
            << "  - The starting position of the word;" << endl
            << "  - The orientation ('H' for horizontal, 'V' for vertical);" << endl
            << "  - A valid word to place on the board." << endl << endl;

    // Error messages.
    setcolor(ERROR_COLOR);
    cout << error_messages.str();
    setcolor(TEXT_COLOR);

    // Actual prompt.
    cout << "Enter valid position in the form described above: ";
}

void BoardBuilderDisplayer::printNewWord(const Word &word, const Board &board) const {
    Position position = word.getStart();
    Orientation orientation = word.getOrientation();
    
    int letters = board.countLetters();

    for(char c: word) {
        // Only print letters that weren't already in board
        if(board.getCell(position).isEmpty()) {
            // Find the position in screen given position in board.
            int x = position.getX()*2 + 1;
            int y = position.getY() + 1;

            gotoxy(x, y);
            setcolor(LETTER_COLOR, BOARD_BACKGROUND);
            cout << c;

            // increase letters counter.
            letters += 1;
            gotoxy(board_info_x_offset+19, 4);
            setcolor(TEXT_COLOR);
            cout << letters;

            // Small delay between letters.
            this_thread::sleep_for(chrono::milliseconds(ADD_LETTER_TO_BOARD_DELAY));
        }

        position.stepForward(orientation);
    }

    // Increase word counter.
    gotoxy(board_info_x_offset+17, 5);
    setcolor(TEXT_COLOR);
    cout << board.countWords() + 1;
}
