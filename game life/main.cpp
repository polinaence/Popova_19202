#include <iostream>
#include "game_board.h"
#include "parser.h"
#include <gtest/gtest.h>
#include "console.h"


using namespace std;

TEST (test_set, test_1) /*NOLINT*/
{
   game_board g1, g2;
   g1.load_board("in");
   g2.add_live_cell(1,2);
   g2.add_live_cell(2,2);
   g2.add_live_cell(3,2);
   bool w = g1.compare_boards(g2);
   EXPECT_EQ(true, w);
}

TEST (test_step, test_2) /*NOLINT*/
{
    game_board g1,g2;
    g1.load_board("in");
    g1.next_state(1);
    g2.add_live_cell(2,1);
    g2.add_live_cell(2,2);
    g2.add_live_cell(2,3);
    bool w = g1.compare_boards(g2);
    EXPECT_EQ(true, w);
}

TEST (test_reset, test_3) /*NOLINT*/
{
    game_board g1,g2;
    g1.load_board("in");
    g1.reset();
    bool w = g1.compare_boards(g2);
    EXPECT_EQ(true, w);
}

TEST (test_clear, test_3) /*NOLINT*/
{
    game_board g1,g2;
    g1.load_board("in");
    g1.clear(2,2);
    g2.add_live_cell(1,2);
    //g2.add_live_cell(2,2);
    g2.add_live_cell(3,2);
    bool w = g1.compare_boards(g2);
    EXPECT_EQ(true, w);
}

TEST (test_back, test_4) /*NOLINT*/
{
    game_board g1,g2;
    g2.add_live_cell(1,2);
    g2.add_live_cell(2,2);
    g2.add_live_cell(3,2);
    g2.next_state(1);
    g2.back();
    g1.load_board("in");
    bool w = g1.compare_boards(g2);
    EXPECT_EQ(true, w);
}

TEST (test_1000steps, test_5)/*NOLINT*/
{
    game_board g1, g2;
    g2.add_live_cell(1,2);
    g2.add_live_cell(2,2);
    g2.add_live_cell(3,2);
    //g1.load_board();
    g1.add_live_cell(2,1);
    g1.add_live_cell(2,2);
    g1.add_live_cell(2,3);
    g2.next_state(1001);
    bool w = g1.compare_boards(g2);
    EXPECT_EQ(true, w);
}


int main(int argc, char* argv[]) {
    testing :: InitGoogleTest(&argc, argv);
    game_board game;
    cout << game;
    console c;
    while(!game.end_game()) {
        parser pars(&game);
        pars.get_comand(c);
    }
    cout << "GAME OVER...((" << endl;

    return RUN_ALL_TESTS();
}
