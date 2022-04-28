#include "player/player.hpp"


int main(){
    float board = 0, end = 100;
    float one_count = 0, two_count = 0, no_one = 0;

    //test dame
   string path ="../board/";

    while(board <= end) {
        cout << "board: " << board++ << endl;
        int i = 0;
        Player one(1);
        Player two(2);
        one.init_board(path + "board_" + std::to_string(i) + ".txt");

        one.print_playground();

        while (!one.loses() && !two.loses() && i < 35000) {
            one.load_board(path + "board_" + std::to_string(i) + ".txt");
            i++;
            one.move();
            one.store_board(path + "board_" + std::to_string(i) + ".txt");
            one.print_last_playground();

            two.load_board(path + "board_" + std::to_string(i) + ".txt");
            i++;
            two.move();
            two.store_board(path + "board_" + std::to_string(i) + ".txt");
            two.print_last_playground();
        }
        if (one.loses() && two.loses()){
            cout << "i player hanno perso" << endl;
            no_one++;
        }
        else if (one.loses()){
            cout << "player 1 ha perso" << endl;
            two_count++;
        }
        else if (two.loses()){
            cout << "player 2 ha perso" << endl;
            one_count++;
        }else{
            no_one++;
        }

        cout << "finished" << endl;
    }
    cout << "total game: "<<board<<endl;
    cout << "One wins: "<<one_count/board<<endl;
    cout << "two wins: "<<two_count/board<<endl;
    cout << "no_one wins: "<<no_one/board<<endl;
}