//
// Created by lorenzo on 09/06/22.
//

#ifndef PROGETTO_TEST_H
#define PROGETTO_TEST_H

#include "assert.h"
#include "./player/player.hpp"

using namespace std;

void test_not_valid() {
    Player player(1);

    // board uguale
    player.load_board("./valid_move/not_valid/test1_old.txt");
    player.load_board("./valid_move/not_valid/test1_new.txt");
    assert(player.valid_move() == 0);

    //mossa di x verso il basso
    player.load_board("./valid_move/not_valid/test2_old.txt");
    player.load_board("./valid_move/not_valid/test2_new.txt");
    assert(player.valid_move() == 0);

    //mossa di o verso l'alto
    player.load_board("./valid_move/not_valid/test3_old.txt");
    player.load_board("./valid_move/not_valid/test3_new.txt");
    assert(player.valid_move() == 0);

    //pedina non trasformata in dama (x)
    player.load_board("./valid_move/not_valid/test4_old.txt");
    player.load_board("./valid_move/not_valid/test4_new.txt");
    assert(player.valid_move() == 0);

    //pedina non trasformata in dama (o)
    player.load_board("./valid_move/not_valid/test5_old.txt");
    player.load_board("./valid_move/not_valid/test5_new.txt");
    assert(player.valid_move() == 0);

    //x si sposta sopra una pedina diversa da e
    player.load_board("./valid_move/not_valid/test6_old.txt");
    player.load_board("./valid_move/not_valid/test6_new.txt");
    assert(player.valid_move() == 0);

    //x mangia una pedina propria
    player.load_board("./valid_move/not_valid/test7_old.txt");
    player.load_board("./valid_move/not_valid/test7_new.txt");
    assert(player.valid_move() == 0);

    //o mangia una pedina propria
    player.load_board("./valid_move/not_valid/test8_old.txt");
    player.load_board("./valid_move/not_valid/test8_new.txt");
    assert(player.valid_move() == 0);

    //o mangia una X (dama)
    player.load_board("./valid_move/not_valid/test9_old.txt");
    player.load_board("./valid_move/not_valid/test9_new.txt");
    assert(player.valid_move() == 0);

    //x mangia una O (dama)
    player.load_board("./valid_move/not_valid/test10_old.txt");
    player.load_board("./valid_move/not_valid/test10_new.txt");
    assert(player.valid_move() == 0);

    //vengono mangiate più pedine
    player.load_board("./valid_move/not_valid/test11_old.txt");
    player.load_board("./valid_move/not_valid/test11_new.txt");
    assert(player.valid_move() == 0);

    //viene mangiata una pedina e la x va sopra una o ("mangiate due pedine")
    player.load_board("./valid_move/not_valid/test12_old.txt");
    player.load_board("./valid_move/not_valid/test12_new.txt");
    assert(player.valid_move() == 0);

    //viene mangiata una pedina e la o va sopra una x ("mangiate due pedine")
    player.load_board("./valid_move/not_valid/test13_old.txt");
    player.load_board("./valid_move/not_valid/test13_new.txt");
    assert(player.valid_move() == 0);

    //movimento di una pedina x oltre 1 blocco senza aver mangiato (viene spostata in modo casuale)
    player.load_board("./valid_move/not_valid/test14_old.txt");
    player.load_board("./valid_move/not_valid/test14_new.txt");
    assert(player.valid_move() == 0);

    //movimento di una pedina o oltre 1 blocco senza aver mangiato (viene spostata in modo casuale)
    player.load_board("./valid_move/not_valid/test15_old.txt");
    player.load_board("./valid_move/not_valid/test15_new.txt");
    assert(player.valid_move() == 0);

}

void test_valid() {
    Player player(1);

    //mossa di x di un blocco
    player.load_board("./valid_move/valid/test1_old.txt");
    player.load_board("./valid_move/valid/test1_new.txt");
    assert(player.valid_move() == 1);

    //mossa di o di un blocco
    player.load_board("./valid_move/valid/test2_old.txt");
    player.load_board("./valid_move/valid/test2_new.txt");
    assert(player.valid_move() == 1);

    //mossa di O di un blocco verso l'alto
    player.load_board("./valid_move/valid/test3_old.txt");
    player.load_board("./valid_move/valid/test3_new.txt");
    assert(player.valid_move() == 1);

    //mossa di X di un blocco verso il basso
    player.load_board("./valid_move/valid/test4_old.txt");
    player.load_board("./valid_move/valid/test4_new.txt");
    assert(player.valid_move() == 1);

    //x mangia o
    player.load_board("./valid_move/valid/test5_old.txt");
    player.load_board("./valid_move/valid/test5_new.txt");
    assert(player.valid_move() == 1);

    //o mangia x
    player.load_board("./valid_move/valid/test6_old.txt");
    player.load_board("./valid_move/valid/test6_new.txt");
    assert(player.valid_move() == 1);

    //X mangia o
    player.load_board("./valid_move/valid/test7_old.txt");
    player.load_board("./valid_move/valid/test7_new.txt");
    assert(player.valid_move() == 1);

    //O mangia x
    player.load_board("./valid_move/valid/test8_old.txt");
    player.load_board("./valid_move/valid/test8_new.txt");
    assert(player.valid_move() == 1);

    //Mossa border line di x
    player.load_board("./valid_move/valid/test9_old.txt");
    player.load_board("./valid_move/valid/test9_new.txt");
    assert(player.valid_move() == 1);

    //x si muove in i = 0 e diventa dama X
    player.load_board("./valid_move/valid/test10_old.txt");
    player.load_board("./valid_move/valid/test10_new.txt");
    assert(player.valid_move() == 1);

    //X mangia verso il basso
    player.load_board("./valid_move/valid/test11_old.txt");
    player.load_board("./valid_move/valid/test11_new.txt");
    assert(player.valid_move() == 1);

    //O mangia verso l'alto
    player.load_board("./valid_move/valid/test12_old.txt");
    player.load_board("./valid_move/valid/test12_new.txt");
    assert(player.valid_move() == 1);
}

void test_valid_vac() {
    test_not_valid();
    test_valid();
    cout << "Tutti i test case sono passati!" << endl;
}

#endif //PROGETTO_TEST_H