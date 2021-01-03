#include "parser.h"

using namespace std;

parser::parser(game_board* g) {
    game = g;
}

void parser::get_comand(console &c) {
    string comand_opt;
    comand_opt = c.get_com_option();
    size_t x,y;
    try {
        if (comand_opt == "reset") {
            game->reset();
            game->show_board();
            game->show_step_count();
        } else if (comand_opt == "clear") {
            string arg;
            arg = c.get_com_2arguments();
            x = arg[0] - '0' - 17;
            y = arg[1] - '0';
            game->clear(x, y);
            game->show_board();
            game->show_step_count();
        } else if (comand_opt == "set") {
            string arg;
            arg = c.get_com_2arguments();
            x = arg[0] - '0' - 17;
            y = arg[1] - '0';
            game->add_live_cell(x, y);
            game->show_board();
            game->show_step_count();
        } else if (comand_opt == "step") {
            string arg;
            arg = c.get_com_1arguments();
            y = arg[0] - '0';
            game->next_state(y);
            game->show_board();
            game->show_step_count();
        } else if (comand_opt == "back") {
            game->back();
            game->show_board();
            game->show_step_count();
        } else if (comand_opt == "save") {
            game->save_board();
            game->show_board();;
        } else if (comand_opt == "load") {
            string arg;
            arg = c.get_com_option();
            game->load_board(arg);
            game->show_board();
        }
    }
    catch (size_t x){
        cout << "Invalid input data..." << endl;
        //exit(1);
    }
}

