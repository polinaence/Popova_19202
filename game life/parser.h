#ifndef MAIN_CPP_PARSER_H
#define MAIN_CPP_PARSER_H
#include "console.h"
#include "game_board.h"

class parser {
private:
    game_board* game;
public:
    void get_comand(console& c);
    parser(game_board*);
};


#endif //MAIN_CPP_PARSER_H
