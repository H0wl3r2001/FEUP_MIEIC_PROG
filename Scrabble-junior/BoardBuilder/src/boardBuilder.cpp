#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>
#include <algorithm>
#include "boardBuilder.h"
#include "cmd.h"

using namespace std;

const char* BoardBuilder::DICTIONARY = "WORDS.txt";

BoardBuilder::BoardBuilder(std::string &board_name, Board &board):
    board_name(move(board_name)),
    board(board),
    displayer(board.getWidth(), board.getHeight()),
    max_players(0)
{}

bool BoardBuilder::parsePosition(istream &input, Position &position) {
    ostream &error_messages = displayer.getErrorStream();
    string position_str;

    input >> position_str;
        
    if(input.fail()) {
        error_messages << "Expected position in the form 'Aa'.\n";
        return false;
    }

    if(position_str.size() != 2) {
        error_messages << "Couldn't parse '" << position_str
                << "' as a position. Use an uppercase letter followed by a lowercase one, like 'Aa'.\n";
        return false;
    }

    char x_char = position_str[1];
    char y_char = position_str[0];
    
    if(x_char < 'a' || x_char > 'z' || y_char < 'A' || y_char > 'Z') {
        error_messages << "Couldn't parse '" << position_str
                << "' as a position. Use an uppercase letter followed by a lowercase one, like 'Aa'.\n";
        return false;
    }

    position = Position(x_char, y_char);

    return true;
}

bool BoardBuilder::parseOrientation(istream &input, Orientation &orientation) {
    ostream &error_messages = displayer.getErrorStream();
    string orientation_str;

    input >> orientation_str;
        
    if(input.fail()) {
        error_messages << "Expected orientation ('H' or 'V').\n";
        return false;
    }

    if(orientation_str.size() != 1) {
        error_messages << "Couldn't parse '" << orientation_str
                << "' as an orientation. Use 'H' for horizontal or 'V' for vertical.\n";
        return false;
    }

    char orientation_char = (char) toupper(orientation_str[0]);
    if(orientation_char == 'H') orientation = Horizontal;
    else if(orientation_char == 'V') orientation = Vertical;
    else {
        error_messages << "Couldn't parse '" << orientation_str
                << "' as an orientation. Use 'H' for horizontal or 'V' for vertical.\n";
        return false;
    }

    return true;
}

bool BoardBuilder::parseWordStr(istream &input, string &word_str) {
    ostream &error_messages = displayer.getErrorStream();
    input >> word_str;

    if(input.fail()) {
        error_messages << "Expected a word.\n";
        return false;
    }

    // Check if string can really be a word (may only have ASCII alphabetic characters).
    auto is_alpha_lambda = [](char c) { return (char) isalpha(c); };
    auto invalid_char = find_if_not(word_str.begin(), word_str.end(), is_alpha_lambda);
    if(invalid_char != word_str.end()) {            
        error_messages << "Only allowed words with ASCII alphabetic letters.\n";
        if(isspace(*invalid_char)) error_messages << "Whitespace is not allowed.\n";
        else error_messages << "'" << *invalid_char << "' is not allowed.\n";

        return false;
    }

    if(word_str.size() < 2) {
        error_messages << "Only allowed words with at least two letters. Found '" 
                << word_str << "'.\n";
        return false;
    }

    // Words are internally saved as fully uppercase, independently of in which
    // case the user input them.
    auto to_upper_lambda = [](char c) { return (char) toupper(c); }; 
    transform(word_str.begin(), word_str.end(), word_str.begin(), to_upper_lambda);
    return true;
}

void BoardBuilder::run() {
    clrscr();
    // Board is only fully printed once, at the start, because afterwards
    // it can be reused, only updating 'Cell's that change.
    displayer.printBoard(board);

    while(true) {
        // Update state and screen.
        max_players = min(4u, board.countLetters()/7);

        displayer.printBoardInfo(board_name, board, max_players);
        displayer.printPrompt(max_players, board.countLetters());
        displayer.clearErrors();

        ostream &error_messages = displayer.getErrorStream();

        // Read a whole line as input.
        string input;
        getline(cin, input);
        if(cin.fail()) {
            cin.clear();
            cout << endl;
            break;
        }
        stringstream input_stream(input);

        // Parse input.
        Position position;
        Orientation orientation;
        string word_str;

        if(!parsePosition(input_stream, position)) continue;
        if(!parseOrientation(input_stream, orientation)) continue;
        if(!parseWordStr(input_stream, word_str)) continue;

        // Player shouldn't input anything else.
        std::string unexpected;
        input_stream >> unexpected;
        if(unexpected.size() != 0) {
            error_messages << "Unexpected: '" << unexpected << "'\n";
            continue;
        }

        // Check if word is in dictionary.
        ifstream dict(DICTIONARY);
        if(!dict.is_open()) {
            error_messages << "Dictionary file '" << DICTIONARY << "' was not found in the current folder or could not be oppened.\n"
                    << "Make sure such file exists and can be opened before inputing your word again.\n";
            continue;
        }
        
        if(!inDict(dict, word_str)) {
            error_messages << "Word '" << word_str << "' not found in dictionary\n";
            continue;
        }

        // Create Word and check if it can be placed in the board.
        Word word(position, orientation, word_str);
        if(!board.isWordValid(word, error_messages)) continue;

        // Everything is OK; add to board.
        displayer.printNewWord(word, board);
        board.addWord(word);
    }
}

void BoardBuilder::saveToFile() const {
    if(max_players < 2) {
        setcolor(BoardBuilderDisplayer::WARNING_COLOR);
        cout << "Board does not have enough letters in order to be playable." << endl
                << "Board will still be saved and may be edited later." << endl;
    }
    
    string filename = board_name + ".txt";
    ofstream outfile(filename);

    // Be sure that the file may be written to before continuing.
    while(!outfile.is_open()) {
        if(cin.fail()) return;
        
        setcolor(BoardBuilderDisplayer::ERROR_COLOR);
        cout << "Could not open '" << filename << "' for writing." << endl;
        setcolor(BoardBuilderDisplayer::TEXT_COLOR);
        cout << "Make sure such file can be opened for writting, then press ENTER: ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        outfile.open(filename);
    }

    board.writeData(outfile);
    setcolor(BoardBuilderDisplayer::SUCCESS_COLOR);
    cout << "Board has been successfully saved to '" << filename << "'." << endl;
    setcolor(BoardBuilderDisplayer::TEXT_COLOR);
    cout << "Press ENTER to exit: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

bool BoardBuilder::inDict(istream &dict, const string &word) {
    string dict_word;

    while(dict >> dict_word) {
        auto to_upper_lambda = [](char c) { return (char) toupper(c); }; 
        transform(dict_word.begin(), dict_word.end(), dict_word.begin(), to_upper_lambda);
        
        if(word == dict_word) return true;
    } 
    
    return false;
}
