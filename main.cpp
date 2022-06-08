#include "player/player.hpp"
#include <chrono>
#include <thread>
#include <vector>

using namespace std;
using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono; // nanoseconds, system_clock, seconds



string path ="../board/";

void game(){

    int board = 0, end = 0;
    float one_count = 0, two_count = 0, no_one = 0;
    vector<int> moves(0,0);
    //test dame

    while(board <= end) {
        cout << "game: " << board++ << endl;
        int i = 1;
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
            one.print_last_playground();

            two.load_board(path + "board_" + std::to_string(i) + ".txt");
            i++;
            //cout<<"two move"<<endl;
            two.move();
            two.store_board(path + "board_" + std::to_string(i) + ".txt");
            two.print_last_playground();
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
        moves.push_back(i);
        //sleep_for(nanoseconds(1));
        //sleep_until(system_clock::now() + seconds(1));
    }
    cout << "total game: "<<board<<endl;
    cout << "One wins: "<<one_count/board*100<<" %"<<endl;
    cout << "two wins: "<<two_count/board*100<<" %"<<endl;
    cout << "no_one wins: "<<no_one/board*100<<" %"<<endl;
    int somma = 0;
    for(auto i:moves)
        somma += i;
    cout << "mosse medie: "<<somma/board<<""<<endl;
}

char from_enum_to_char(int i)  {
    switch(i){
        case 0: return 'x';
        case 2: return 'X';
        case 1: return'o';
        case 3: return 'O';
        case 4: return ' ';
        default: throw "Invalid enum";
    }
}

int main(){
    //game();
    Player one(1);
    one.load_board(path + "board_" + std::to_string(19) + ".txt");
    Player two(one);
    two.print_last_playground();
    one.pop();
}