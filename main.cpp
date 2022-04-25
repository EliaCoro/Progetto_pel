#include "player/player.hpp"
string path ="../board/";

int main(){
    int i = 0;
    Player one(1);
    Player two(2);
    one.init_board(path+"board_"+ std::to_string(i) +".txt");

    one.print_playground();

    while(!one.loses() && !two.loses() && i <= 2000){
        one.load_board(path+"board_"+ std::to_string(i) +".txt");
        i++;
        one.move();
        one.store_board(path+"board_"+ std::to_string(i) +".txt");
        one.print_last_playground();

        two.load_board(path+"board_"+ std::to_string(i) +".txt");
        i++;
        two.move();
        two.store_board(path+"board_"+ std::to_string(i) +".txt");
        two.print_last_playground();
    }
    if(one.loses())
        cout<<"player 1 ha perso"<<endl;
    if(two.loses())
        cout<<"player 2 ha perso"<<endl;

    cout<<"finished"<<endl;
}