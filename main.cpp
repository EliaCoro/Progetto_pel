#include "player/player.hpp"
string path ="../board/";

int main(){
    Player one(1);
    Player two(2);
    one.load_board(path+"board_1.txt");
    cout<<"finished"<<endl;
}