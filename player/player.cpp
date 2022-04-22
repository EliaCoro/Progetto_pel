//
// Created by Elia Corò on 20/04/22.
//
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include "player.hpp"
using namespace std;

#define playground_height 8
#define playground_width 8



struct Cell{
    int index;
    char playground[playground_height][playground_width];
    Cell* next;
};

struct History{
    Cell* head;
    Cell* tail;
    int playground_number;
};

struct Player::Impl{
    int player_nr;
    char pieces;
    History* history;
};

Player::Player(int player_nr){

    if(player_nr == 1 || player_nr == 2){
        this->pimpl = new Impl;
        this->pimpl->player_nr = player_nr;
        this->pimpl->history = new History{nullptr, nullptr, 0};
    }else{
        //TODO: controllare se lanciare l'eccezione in questo modo è corretto
        throw player_exception{player_exception::err_type(0), "Invalid Player_nr"};
    }
};

Player::~Player(){
    if(this->pimpl->history->head != nullptr){
        Cell* pc = this->pimpl->history->head;
        while(this->pimpl->history->head != nullptr){
            this->pimpl->history->head = this->pimpl->history->head->next;
            delete pc;
            pc = this->pimpl->history->head;
        }
        this->pimpl->history->tail = nullptr;
    }
    delete pimpl->history;
    delete pimpl;
}

Player::Player(const Player& player){
    this->pimpl = new Impl;
    this->pimpl->player_nr = player.pimpl->player_nr;
    this->pimpl->history = new History;
    this->pimpl->history->head = nullptr;
    this->pimpl->history->tail = nullptr;

}

Player& Player::operator=(const Player& player){

}

Player::piece Player::operator()(int r, int c, int history_offset)const{

}

void Player::printPlayground(){
    Cell* pc = this->pimpl->history->head;
    int counter = 0;
    while(pc){
        cout<<"Playground nr: " <<counter++<<endl;
        for (int i = 0; i < 8; ++i){
            for (int j = 0; j < 8; ++j)
                if(pc->playground[i][j] == 'e')
                    cout<<"  ";
                else
                    cout<<pc->playground[i][j]<<" ";
            cout<<endl;
        }
        cout<<endl<<endl<<endl;
        pc = pc->next;

    }
}


void Player::new_cell_history(char matrix[8][8]) {
    Cell *nc = new Cell;
    nc->next = nullptr;
    if (this->pimpl->history->head == nullptr) {
        this->pimpl->history->head = nc;
        this->pimpl->history->tail = nc;
    } else {
        this->pimpl->history->tail->next = nc;
        this->pimpl->history->tail = nc;
    }
    this->pimpl->history->playground_number = this->pimpl->history->playground_number + 1;

    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            if (matrix[i][j] != 0)
                nc->playground[i][j] = matrix[i][j];
            else
                nc->playground[i][j] = 'e';
}

void Player::load_board(const string& filename){
    ifstream file{filename};
    char playground[playground_height][playground_width];
    int i = 0;

    while(file.good()) {
        string s;
        getline(file, s);
        for (int j = 0; j < s.size(); j=j+2) {
            //TODO: controllare questione enum
            if(s.at(j) == ' ')
                playground[i][j/2] = 'e';
            else
                playground[i][j/2] = s.at(j);
        }
        i++;
    }
    this->new_cell_history(playground);
    this->printPlayground();
}

void Player::store_board(const string& filename, int history_offset) const{

}

void Player::init_board(const string& filename)const{
    //manage error

    string board = "o   o   o   o  \n"
                   "  o   o   o   o\n"
                   "o   o   o   o  \n"
                   "               \n"
                   "               \n"
                   "  x   x   x   x\n"
                   "x   x   x   x  \n"
                   "  x   x   x   x";

    ofstream file(filename);
    file << board;
    file.close();
}

void Player::move(){

}

bool Player::valid_move() const{

}

void Player::pop(){

}

bool Player::wins(int player_nr)const{

}

bool Player::wins() const{

}

bool Player::loses(int player_nr)const{

}

bool Player::loses()const{

}

int Player::recurrence()const{

}