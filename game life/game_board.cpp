#include "game_board.h"

using namespace std;

bool compare_matrix(char** m1, char** m2) {
    for ( size_t  i = 0; i < HEIGHT; i++){
        for ( size_t j = 0 ;j < WIDTH; j++){
            if (m1[i][j] != m2[i][j]) return false;
        }
    }
    return true;
}

void reassign_boards(char** m1, char** m2) {
    for (size_t  i = 0; i < HEIGHT; i++){
        memcpy(m1[i], m2[i], sizeof(char)*HEIGHT);
    }
}


bool game_board::load_board(string File_name)  {
    ifstream in;
   // cout << File_name << endl;

    File_name= File_name +".txt";
    in.open(File_name);
    if (in.is_open()){
        for (size_t h = 0; h <HEIGHT; h++){
            for (size_t w = 0; w <WIDTH; w++){
                in >> curr_gen[h][w];
            }
        }
    }
    in.close();
    step_count = 0;
    return false;

}


bool game_board::save_board() {
    ofstream  out;
    out.open(R"(C:\Users\user\CLionProjects\game_of_life\out.txt)");
    if (out.is_open()){
        out << "  0  1  2  3  4  5  6  7  8  9" << endl;
        for (size_t  h=0; h < HEIGHT; h++){
            char c;
            c = 'A' + h;
            out << c ;
            for (size_t w =0; w < WIDTH; w++){
                out << " " << curr_gen[h][w] << " ";
            }
            out << endl;
        }
    }
    out.close();
    return false;
}

void game_board::show_board() {
    cout << "  0  1  2  3  4  5  6  7  8  9" << endl;
    for (size_t h = 0; h < HEIGHT; h ++){
        char c;
        c = 'A' + h;
        cout << c ;
        for (size_t w = 0; w < WIDTH; w++){
            cout << " " << curr_gen[h][w] << " ";
        }
        cout << endl;
    }
    cout << endl;
}



void game_board::next_state(size_t x) {
   // if ( x < 0) throw x;
for (size_t i =0; i < x; i ++) {
    reassign_boards(prev_gen, curr_gen);

    for (size_t h = 0; h < HEIGHT; h++) {
        for (size_t w = 0; w < WIDTH; w++) {
            size_t count_live;

            count_live = neighbour_count(h, w);

            //RULES
            if (count_live < 2 && prev_gen[h][w] == LIVE) { curr_gen[h][w] = DEAD; }
            if ((count_live == 2 || count_live == 3) && prev_gen[h][w] == LIVE) { curr_gen[h][w] = LIVE; }
            if (count_live > 3 && prev_gen[h][w] == LIVE) { curr_gen[h][w] = DEAD; }
            if (count_live == 3 && prev_gen[h][w] == DEAD) { curr_gen[h][w] = LIVE; }
        }
    }
}
    step_count++;
    ispossible_go_back = true;
}

size_t game_board::neighbour_count(size_t x, size_t y) {
    size_t live_count = 0;

for ( size_t i = x -1; i <= x+1; i++){
    for ( size_t j = y-1; j <= y+1; j++){
        if ( i != x || j != y){
            if (prev_gen[(i+HEIGHT)%HEIGHT][(j+WIDTH)%WIDTH] == LIVE) live_count++;
        }
    }
}

 return live_count;
}

game_board::game_board() {
    Create();
    init_board();
    step_count = 0;
    ispossible_go_back = false;
}

game_board::~game_board() {
    Delete();
    step_count = 0;
}


void game_board::reset() {
    for (size_t h = 0; h < HEIGHT; h++){
        for (size_t w = 0; w < WIDTH; w++){
            curr_gen[h][w] = DEAD;
        }
    }
    step_count = 0;
    ispossible_go_back = false;
}

void game_board::clear(size_t x, size_t y) {
    if ( x < 0 || x > 9 || y < 0 || y > 9) throw x;
    curr_gen[x][y] = DEAD;
    step_count++;
   ispossible_go_back = true;
}

void game_board::add_live_cell(size_t x, size_t y) {
    if ( x < 0 || x > 9 || y < 0 || y > 9) throw x;
    reassign_boards(prev_gen, curr_gen);
    if ( x > HEIGHT || x < 0 || y > WIDTH || y < 0) return;
    if (curr_gen[x][y] == LIVE) { cout << "Oooops...this place's already taken"<< endl;}
    else { curr_gen[x][y] = LIVE;}
    ispossible_go_back = true;
    step_count++;
}

void game_board::back() {
    if (step_count == 0) {
        cout << "It's impossible to step back...." << endl;
        return;
    }
    if (!ispossible_go_back) {
        cout << "It's impossible to step back...." << endl;
        return;
    }

    swap(curr_gen, prev_gen);
    ispossible_go_back = false;
    step_count++;
}


bool game_board::end_game() {

    if (compare_matrix(prev_gen,curr_gen) && step_count != 0) return true;
    else return false;
}

std::ostream &operator<<(ostream &out, game_board &game) {
    cout << "  0  1  2  3  4  5  6  7  8  9" << endl;
    for (size_t h = 0; h < HEIGHT; h ++){
        char c;
        c = 'A' + h;
        cout << c ;
        for (size_t w = 0; w < WIDTH; w++){
            cout << " " << game.curr_gen[h][w] << " ";
        }
        cout << endl;
    }
    cout << endl;;
    return out;
}

void game_board::show_step_count() const {
    cout <<"STEP: " << step_count << endl;
}

bool game_board::compare_boards(game_board g) {
    for ( size_t  i = 0; i < HEIGHT; i++){
        for ( size_t j = 0 ;j < WIDTH; j++){
            if (curr_gen[i][j] != g.curr_gen[i][j]) return false;
        }
    }
    return true;
}







