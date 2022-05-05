#include "player/player.hpp"
#include <chrono>
#include <thread>

using namespace std;
using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono; // nanoseconds, system_clock, seconds



string path ="../board/";

void game(){

    int board = 0, end = 50;
    float one_count = 0, two_count = 0, no_one = 0;

    //test dame

    while(board <= end) {
        cout << "game: " << board++ << endl;
        int i = 0;
        Player one(1);
        Player two(2);
        one.init_board(path + "board_" + std::to_string(i) + ".txt");

        //one.print_playground();

        while (!one.loses() && !two.loses() && i < 500) {
            one.load_board(path + "board_" + std::to_string(i) + ".txt");
            i++;
            //cout<<"one move"<<endl;
            one.move();
            one.store_board(path + "board_" + std::to_string(i) + ".txt");
            //one.print_last_playground();

            two.load_board(path + "board_" + std::to_string(i) + ".txt");
            i++;
            //cout<<"two move"<<endl;
            two.move();
            two.store_board(path + "board_" + std::to_string(i) + ".txt");
            //two.print_last_playground();
        }
        if (one.loses() && two.loses()){
            cout << "i player hanno perso in " << i << " mosse" << endl;
            no_one++;
            one.store_board("../end_game/board_"+to_string(board)+".txt");
        }
        else if (one.loses()){
            cout << "player 2 ha vinto " <<  i << " mosse" << endl;
            two_count++;
            one.store_board("../end_game/board_"+to_string(board)+".txt");
        }
        else if (two.loses()){
            cout << "player 1 ha vinto " <<  i << " mosse" << endl;
            one_count++;
            one.store_board("../end_game/board_"+to_string(board)+".txt");
        }else{
            no_one++;
        }
        sleep_for(nanoseconds(1));
        sleep_until(system_clock::now() + seconds(1));
    }
    cout << "total game: "<<board<<endl;
    cout << "One wins: "<<one_count/board*100<<" %"<<endl;
    cout << "two wins: "<<two_count/board*100<<" %"<<endl;
    cout << "no_one wins: "<<no_one/board*100<<" %"<<endl;
}

int main(){
    game();
    Player one(1);
    one.load_board(path+"/board_5.txt");
    cout<<one(0,0)<<endl;
}