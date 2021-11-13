#include <iostream>
#include <iomanip>
#include <algorithm>
#include <chrono>
#include <thread>
#include "gameDisplayer.h"

using namespace std;

const Color GameDisplayer::PLAYERS_COLOR[] = {CYAN, LIGHTRED, GREEN, YELLOW};
const char* GameDisplayer::WINNER_LABELS[] = {"1st", "2nd", "3rd", "4th"};

const Color GameDisplayer::TEXT_COLOR = LIGHTGRAY;
const Color GameDisplayer::ERROR_COLOR = RED;
const Color GameDisplayer::WARNING_COLOR = YELLOW;
const Color GameDisplayer::SWAP_LETTER_COLOR = YELLOW;
const Color GameDisplayer::LETTER_UNCOVERED_COLOR = BLACK;
const Color GameDisplayer::LETTER_COVERED_COLOR = RED;
const Color GameDisplayer::SCORE_COLOR = GREEN;
const Color GameDisplayer::BOARD_BACKGROUND = LIGHTGRAY;
const Color GameDisplayer::BOARD_HIGHLIGHTED_BACKGROUND = YELLOW;

const int GameDisplayer::WORD_COMPLETED_ANIMATION_DELAY = 200;
const int GameDisplayer::SCORE_INCREASE_DELAY = 500;
const int GameDisplayer::SWAP_LETTER_DELAY = 400;
const int GameDisplayer::NOTICE_DELAY = 1500;
const int GameDisplayer::SHORT_NOTICE_DELAY = 800;
const int GameDisplayer::AFTER_REFILL_DELAY = 750;

const unsigned int GameDisplayer::current_player_hand_x_offset = 12;

GameDisplayer::GameDisplayer(unsigned int board_width, unsigned int board_height):
    // Offsets are calculated based on the size of the board on screen.
    scoreboard_x_offset(board_width*2 + 2),
    turn_info_y_offset(max(8u, board_height + 2)) // Must not be smaller than 8 because of height of scoreboard.
{}

void GameDisplayer::printColoredId(int id, const char *prefix) {
    setcolor(PLAYERS_COLOR[id-1]);
    cout << prefix << id;
    setcolor(TEXT_COLOR);
}

std::ostream& GameDisplayer::getErrorStream() {
    return error_messages;
}

void GameDisplayer::clearErrors() {
    error_messages.str("");
}

void GameDisplayer::printBoard(const Board &board, CheckLegalMove check_legal_move) {
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
        
        for(unsigned int i = 0; i < board.getWidth(); i++) {
            Position position((int) i, (int) j);
            const Cell cell = board.getCell(position);
            Color letter_color, letter_background;

            if(cell.isCovered()) {
                letter_color = LETTER_COVERED_COLOR;
            } else {
                letter_color = LETTER_UNCOVERED_COLOR;
            }

            if(check_legal_move && check_legal_move(position, cell)) {
                letter_background = BOARD_HIGHLIGHTED_BACKGROUND;
            } else {
                letter_background = BOARD_BACKGROUND;
            }

            setcolor(letter_color, letter_background);
            cout << cell;
            
            if(i+1 != board.getWidth()) {
                setcolor(BLACK, BOARD_BACKGROUND);
                cout << ' ';
            }
        }
        setcolor(TEXT_COLOR);
        cout << '\n';
    }
}

void GameDisplayer::printScoreboard(const std::vector<Player> &players) const {
    gotoxy(scoreboard_x_offset + 4, 1);
    setcolor(TEXT_COLOR);
    cout << "SCORE       LETTERS";

    for(size_t i = 0; i < players.size(); i++) {
        gotoxy(scoreboard_x_offset, (int) i+2);
        unsigned int id = players[i].getId();
        unsigned int score = players[i].getScore();
        const Hand &hand = players[i].getHand();

        printColoredId(id, "P");
        cout << " " << setw(4) << score << "      " << hand;
    }
}

void GameDisplayer::printTurnInfo(const Player &current_player, unsigned int moves_left) const {
    clrscr(0, turn_info_y_offset);
    unsigned int id = current_player.getId();

    // Who is playing.
    printColoredId(id, "Player ");
    cout << " is playing this turn." << endl;

    // How many moves left.
    if(moves_left == 0) {
        cout << "No moves left this turn." << endl;
    } else if(moves_left == 1) {
        cout << "You have 1 move left this turn." << endl;
    } else {
        cout << "You have " << moves_left << " moves left this turn." << endl;
    }
    
    // Player's hand.
    cout << "Your hand:  " << current_player.getHand() << endl << endl;

    // Error messages.
    setcolor(ERROR_COLOR);
    cout << error_messages.str();
}

void GameDisplayer::printLeaderboard(const std::vector<Player> &players) const {
    gotoxy(scoreboard_x_offset + 9, 1);
    setcolor(TEXT_COLOR);
    cout << "SCORE";
    // A negative value is guaranteed to be different from any score.
    // Needed so first player will always show a 'WINNER_LABEL'.
    int previous_score = -1; 

    for(size_t i = 0; i < players.size(); i++) {
        gotoxy(scoreboard_x_offset, (int) i+2);
        unsigned int id = players[i].getId();
        unsigned int score = players[i].getScore();
        
        // Only makes sense to print a label if player is not tied with
        // previous one.
        if((int) score != previous_score) cout << WINNER_LABELS[i] << "  ";
        else cout << "     ";
        
        printColoredId(id, "P");
        cout << " " << setw(4) << score;
        
        previous_score = score;
    }
}

void GameDisplayer::declareWinners(const vector<int> &winners_id, int num_players) const {
    gotoxy(0, turn_info_y_offset);
    cout << "GAME OVER" << endl;

    if(winners_id.size() == 1) {
        int id = winners_id[0];
        setcolor(PLAYERS_COLOR[id-1]);
        cout << "Player " << id << " wins.";
    } else if((int) winners_id.size() == num_players) {
        if(winners_id.size() == 2) cout << "Both ";
        else cout << "All ";

        cout << "players tied for first place.";
    } else {
        cout << "Players ";
        if(winners_id.size() == 3) {
            printColoredId(winners_id[0]);
            cout << ", ";
            printColoredId(winners_id[1]);
            cout << " and "; 
            printColoredId(winners_id[2]);
        } else {
            printColoredId(winners_id[0]);
            cout << " and "; 
            printColoredId(winners_id[1]);
        }
        
        cout << " tied for first place.";
    }

    cout << endl;
}

void GameDisplayer::printWord(const Word &word, bool delay_each_letter) {
    // Find the position in screen given position in board.
    int x = word.getStart().getX()*2 + 1;
    int y = word.getStart().getY() + 1;

    for(char c: word) {
        gotoxy(x, y);
        cout << c;

        if(delay_each_letter) { 
            this_thread::sleep_for(chrono::milliseconds(WORD_COMPLETED_ANIMATION_DELAY));
        }

        // Advance position in screen
        if(word.getOrientation() == Horizontal) x += 2;
        else y += 1;
    }
}

Hand::SwapLetterAnimator GameDisplayer::getSwapLetterCallback() const {
    return [this](int index, char letter) {
        setcolor(SWAP_LETTER_COLOR);
        gotoxy(current_player_hand_x_offset + 2*index, turn_info_y_offset+2);
        cout << letter;
        setcolor(TEXT_COLOR);

        this_thread::sleep_for(chrono::milliseconds(SWAP_LETTER_DELAY));
    };
}

void GameDisplayer::animateWordComplete(const Player &player, const vector<Word> &words_completed) const {
    clrscr(0, turn_info_y_offset);
    setcolor(SCORE_COLOR);
    cout << "Score!";

    int score = player.getScore();
    int id = player.getId();

    for(const Word &word: words_completed) {
        // Highlight animation and then back to normal.
        setcolor(SCORE_COLOR, BOARD_BACKGROUND);
        printWord(word, true);
        setcolor(LETTER_COVERED_COLOR, BOARD_BACKGROUND);
        printWord(word, false);

        // Update score.
        score+=1; 
        gotoxy(scoreboard_x_offset+3, id+1);
        setcolor(SCORE_COLOR);
        cout << setw(4) << score;
        
        // Delay.
        this_thread::sleep_for(chrono::milliseconds(SCORE_INCREASE_DELAY));
    }

    setcolor(TEXT_COLOR);
}

void GameDisplayer::notice(const string &information, bool short_delay) {
    setcolor(WARNING_COLOR);
    cout << information;

    int delay = short_delay? SHORT_NOTICE_DELAY : NOTICE_DELAY;

    this_thread::sleep_for(chrono::milliseconds(delay));
}

void GameDisplayer::afterRefill(bool depleted_pool) const {
    if(depleted_pool) {
        gotoxy(0, turn_info_y_offset+6);
        notice("The pool has been depleted.");
    } else {
        this_thread::sleep_for(chrono::milliseconds(AFTER_REFILL_DELAY));
    }
}
