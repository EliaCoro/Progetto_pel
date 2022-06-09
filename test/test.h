//
// Created by elia on 09/06/22.
//

#ifndef PROGETTO_PEL_TEST_H
#define PROGETTO_PEL_TEST_H

//modificare il path di player.hpp se necessario
#include "../player/player.hpp"
#include <iostream>

using namespace std;

bool constructor_destructor(){

}

bool copy(){

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

    cout << "copy constructor testing..." << endl;
    assert(copy());

    cout << "operator_equals testing..." << endl;
    assert(operator_equal());

    cout << "operator_pos testing..." << endl;
    assert(operator_pos());

    cout << "init_board testing..." << endl;
    assert(init_board());

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

#endif //PROGETTO_PEL_TEST_H
