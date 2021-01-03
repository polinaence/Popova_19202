#ifndef MAIN_CPP_GAME_BOARD_H
#define MAIN_CPP_GAME_BOARD_H

#include <array>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

const size_t WIDTH = 10;
const size_t HEIGHT = 10;
const char LIVE = 'X';
const char DEAD = '.';

class game_board {
private:
    char** curr_gen = nullptr;
    char** prev_gen = nullptr;
    size_t step_count;
    bool ispossible_go_back;
    void Create(){
        curr_gen = new char* [WIDTH];
        prev_gen = new char* [WIDTH];
        for ( size_t i = 0; i < HEIGHT; i++){
            curr_gen[i] = new char [HEIGHT];
            prev_gen[i] = new char [HEIGHT];
        }
    }

    void init_board(){
        for (size_t h = 0; h < HEIGHT; h++){
            for ( size_t w = 0; w < WIDTH; w++){
                curr_gen[h][w] = DEAD;
                prev_gen[h][w] = DEAD;
            }
        }
    }
    void Delete(){
        for ( size_t i = 0 ;i < HEIGHT; i++){
            delete [] curr_gen[i];
            delete [] prev_gen[i];
        }
        delete [] curr_gen;
        delete [] prev_gen;
    }
public:
    bool compare_boards(game_board);
    game_board();
    ~game_board();
    bool end_game();
    bool load_board(string);
    bool save_board();
    void show_board();
    void next_state(size_t);
    size_t neighbour_count(size_t, size_t);
    void show_step_count() const;
    void reset();
    void clear(size_t, size_t);
    void add_live_cell(size_t, size_t);
    void back();
    friend std:: ostream& operator<< (std:: ostream &, game_board&);
};


#endif //MAIN_CPP_GAME_BOARD_H
