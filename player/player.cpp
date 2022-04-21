//
// Created by Elia Corò on 20/04/22.
//

#include "player.hpp"

int playground_height = 7, playground_width = 7;

struct Cell{
    char playground[7][7];
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
    //Todo: verificare
    /*if(this->pimpl->history->head || this->pimpl->history->tail){
        Cell* pc = this->pimpl->history->head;
        while(pc){
            this->pimpl->history->head->next;
            delete pc;
            pc = this->pimpl->history->head;
        }
    }*/
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

void Player::load_board(const string& filename){

}

void Player::store_board(const string& filename, int history_offset) const{

}

void Player::init_board(const string& filename)const{

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