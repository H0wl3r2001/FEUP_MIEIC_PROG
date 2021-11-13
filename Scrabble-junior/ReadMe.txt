Scrabble Junior Game
PROG 2020
T7G02

Group members:
- Pedro Gonçalo de Castro Correia [up201905348] (55% do trabalho)
- Afonso Duarte de Carvalho Monteiro [up201907284] (45% do trabalho)

======================================================================

This project is an implementation of two programs, one to play the game "Scrabble Junior", and the other to build game boards for said game. Both work in console mode with a colored characters interface.

This project was made for and tested on the Windows 10 command prompt.

The project includes adapted parts of the code provided by the teacher on moodle which facilitates printing colored characters and clearing the console. All that code is contained in the files `cmd_utils.cpp` and `cmd_utils.h` in both programs. 

All intended features have been incorporated into the project, namely:
    [For BoardBuilder]
    - the base functionality (create a board with size up to 20x20; making sure that all words being added are valid and at valid positions; save the contents to a file, following the format specified on Moodle)
    - robust input processing (program correctly handles invalid inputs and gives appropriate error messages)
    - colorful interface with helpful insights about the board being built (such as number of letters and how many more are needed to be playable with more players)
    - ability to edit an existing board, adding more words
    
    
    [For ScrabbleJunior]
    - the base functionality (choose a board file in the format specified on Moodle; choose the number of players (up to 4); play in turns until the end of the game and then announce the winners)
    - robust input processing (program correctly handles invalid inputs and gives appropriate error messages)
    - colorful interface with information about the current state of the game (such as number of moves left this turn and the letters each player)

======================================================================
BoardBuilder

Upon start, the program detects whether the dictionary file (WORDS.txt) is avaliable, not progressing further until it is, so the user can be aware of that right on start, instead of only realizing while already building the board.

The user may then choose a name for the board being created, which may only use ASCII alphanumerical letters and underscores. If such name already exists, the user has the choice to either edit the existing board (adding more letters) or to overwrite it with a completely new board.

When loading the board, the file is assumed to have been created by BoardBuilder (similar to what happens when loading boards in ScrabbleJunior) and thus the words are assumed to be valid and placed at valid positions. Similar to ScrabbleJunior, if parsing the width and height of the board fails, the load is cancelled. Failing to parse any other lines, on the other hand, only ignores all lines from that point on (which may be, for example, the 2D representation of the board).

When creating a board from scratch, width and height (both between 1 and 20, inclusive) must be input.

Once in the editor, the board is shown along with some information (number of letters, number of players that may play such board, etc.). Just as pointed out in the interface, in order to add a word, inputs must be in the form 'Aa H WORD':
    - The starting position of the word;
    - The orientation ('H' for horizontal, 'V' for vertical);
    - A valid word to place on the board.
    
Words must be present in the dictionary file (WORDS.txt). All letters must be inside the board; the word may only intersect with others if both words have the same letter at the intersection position. Words cannot be adjacent (without crossing) to other others. All those requirements are checked when trying to add a word, and the program shows an appropriate error message so the user knows what is wrong.
    
At any point the user may save the board and quit the program with 'Ctrl+Z'. This, however, is only pointed out when the board has at least 14 letters, so as to discourage creating unplayable boards.

If the board being saved has less that 14 letters, the program warns that it won't be playable. However, the board is still saved and may in the future be edited to add more letters. The board is saved in the format specified on Moodle, with a 2D representation of the board at the end. The file name is the board name followed by the '.txt' extension.

======================================================================
ScrabbleJunior

Upon start, the user should input the name of a board to play. Instead of the name of the board, the user may input the file name (with the extension '.txt') or a path to the file. The file is assumed to have been created by BoardBuilder and thus the words are assumed to be valid and placed at valid positions. If parsing the width and height of the board fails, the load is aborted. Failing to parse any other lines, on the other hand, only ignores all lines from that point on (which may be, for example, the 2D representation of the board).

Then the user is prompted to input the number of players (2-4) that are going to play. There must be enough letters in pool to give 7 to each player at the start, which may limit the maximum number of players if the board doesn't have enough letters. 

Once the game starts, players play in turns. In each turn, the current player has to make two valid moves. Each move is specified by a position (for example 'Aa'). The board highlights possible moves. After playing, the player refills their hand with letters from the Pool until they have 7 letters again (unless the Pool has been depleted). Each word completed gives one point.

The player always has to make two moves unless they can only move once. Normally, the player makes one move (independent from the second) and the turn ends if they can't do a second one (otherwise they have to do the second move). However, there is an edge case to this approach. If all possible moves are with the same letter and the player only has one of that letter in hand, the player would, in principle, be only able to make one move; however, some of those moves may actually unlock another letter (as a continuation of the word) that allows the player to move. This means that the player can only move once if their first move doesn't unlock such letter, but twice if it does. Because the player is forced to move twice whenever they can, moves that wouldn't unlock another valid move would be invalid in that case. The game has a special check to handle this edge case.

If the player can't move at all (not even once), they must exchange two letters with the pool (or 1 letter if the Pool only has one letter left). They do so by inputing the letters of their hand that they want to exchange. If the Pool is empty, the turn is just skipped.

The game ends after all letters in board has been covered. The player with the highest score wins. The program shows a leaderboard (an ordered list of players by their score, with the labels '1st', '2nd', '3rd' and '4th'). Players that are tied (have the same score) are considered to be in the same position. This is shown by omitting the label of a player if they are in the same position as the player that appears above them in the leaderboard.