#include "player/player.hpp"
string path ="../board/";

int main(){
    Player one(1);
    //one.init_board(path+"board_1.txt");
    one.load_board(path+"board_1.txt");
    //one.load_board(path+"board_2.txt");
    Player two(one);

    two.move();
    //one.print_playground();
    cout<<"finished"<<endl;
}