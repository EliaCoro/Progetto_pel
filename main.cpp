#include <chrono>
#include <thread>
#include <vector>
#include "player/player.hpp"

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

       do {

            two.load_board(path + "board_" + std::to_string(i) + ".txt");
            i++;
            //cout<<"two move"<<endl;
            two.move();
            two.store_board(path + "board_" + std::to_string(i) + ".txt");
            //two.print_last_playground();

           one.load_board(path + "board_" + std::to_string(i) + ".txt");
           i++;
           //cout<<"one move"<<endl;
           one.move();
           one.store_board(path + "board_" + std::to_string(i) + ".txt");
           //one.print_last_playground();

       } while (!one.loses() && !two.loses() && i < 500);
        if (one.loses() && two.loses()){
            cout << "i player hanno perso in " << i << " mosse" << endl;
            no_one++;
        }
        else if (one.loses()){
            cout << "player 2 ha vinto " <<  i << " mosse" << endl;
            two_count++;
        }
        else if (two.loses()){
            cout << "player 1 ha vinto " <<  i << " mosse" << endl;
            one_count++;
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

void test_valid_move(){
    Player one(1);
    one.load_board(path + "board_" + std::to_string(1) + ".txt");
    int not_correct = 0;
    for (int i = 2; i <= 54; ++i) {
        one.load_board(path + "board_" + std::to_string(i) + ".txt");
        if(one.valid_move()){
        }else{
            not_correct++;
            cout <<"Non passato: "<< std::to_string(i) << endl;
        }
    }
    if(not_correct == 0)
        cout << "terminati tutti i test correttamente "<< endl;
}

int main(){
    //game();

        Player a(2);
        a.init_board(path+"init.txt");
        a.load_board(path+"init.txt");
        for (int i = 0; i < 20; ++i) {
            a.move();
            a.store_board(path+"board_" + std::to_string(i) + ".txt");
        }

        for (int i = 0; i < 20; ++i) {
            a.load_board(path+ + "board_" + std::to_string(i) + ".txt");
        }

        Player b(1);
        for (int i = 0; i < 1011; ++i) {
            for (int j = 0; j < 20; ++j) {
                b.load_board(path+"board_"+std::to_string(j) + ".txt");
            }
        }

        cout << b.recurrence() << endl;
        cout << a.recurrence() << endl;


    /*Player one(1);
    one.init_board(path+"/board_0.txt");
    one.load_board(path+"/board_0.txt");
    one.move();
    for (int i = 7; i >= 0; --i) {
        for (int j = 0; j < 8; ++j) {
            cout << from_enum_to_char(one(i, j, 1))<< " ";
        }
        cout << endl;
    }
    cout << one.valid_move();*/

}

/*

mkdir 892507
mv player.cpp 892507
tar -zcvf 892507.tar.gz 892507

*/