//
// Created by elia on 09/06/22.
//

#ifndef PROGETTO_PEL_TEST_H
#define PROGETTO_PEL_TEST_H

#include <iostream>
#include "../player/player.hpp"

/*
 * N.B: Il file player.hpp NON deve essere importato nel file main altrimenti darà errore di double definitions
 * suggerimento: eseguite questo file con valgrind così potete anche vedere se ci sono problemi di memory leak o altra robaccia ;)
 *
 * */

/*
using namespace std;

bool constructor_destructor(){
    bool res = true;
    try{
        Player one;
        Player two(2);
        Player three(1);
    }catch(player_exception& e){
        cout << "Errore non previsto nel constructor_destructor" << endl;
        cout << e.t << " : " << e.msg << endl;
        res = false;
    }

    try{
        Player p(0);
    }catch(player_exception& e){
        if(e.t != player_exception::err_type(0)){
            cout << "Errore non previsto nel constructor_destructor" << endl;
            cout << e.t << " : " << e.msg << endl;
            res = false;
        }
    }
    cout << "finito testing..."<<endl;
    return res;
}

bool copy(){
    bool res = true;
    try{
        Player p1(1);
        p1.init_board("../test/test_files/board_0.txt");

        auto p2 = p1;


    }catch(player_exception& e){
        if(e.t != player_exception::err_type(0)){
            cout << "Errore non previsto nel constructor_destructor" << endl;
            cout << e.t << " : " << e.msg << endl;
            res = false;
        }
    }
}

bool operator_equal(){

}

bool operator_pos(){

}

bool init_board(){

}

bool load_board(){

}

bool store_board(){

}

bool move(){

}

bool valid_move(){

}

bool pop(){

}

bool win_loose(){

}

bool recurrence(){

}

void test(){
    cout << "Init testing..." << endl;
    cout << "constructor_destructor testing..." << endl;
    assert(constructor_destructor());

    cout << "init_board testing..." << endl;
    assert(init_board());

    cout << "copy constructor testing..." << endl;
    assert(copy());

    cout << "operator_equals testing..." << endl;
    assert(operator_equal());

    cout << "operator_pos testing..." << endl;
    assert(operator_pos());

    cout << "load_board testing..." << endl;
    assert(load_board());

    cout << "store testing..." << endl;
    assert(store_board());

    cout << "move testing..." << endl;
    assert(move());

    cout << "valid_move testing..." << endl;
    assert(valid_move());

    cout << "pop_board testing..." << endl;
    assert(pop());

    cout << "win_loose testing..." << endl;
    assert(win_loose());

    cout << "recurence testing..." << endl;
    assert(recurrence());

    cout << "Test finised ;)" << endl;
}
*/
#endif //PROGETTO_PEL_TEST_H
