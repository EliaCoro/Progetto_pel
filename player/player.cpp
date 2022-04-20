//
// Created by Elia Corò on 20/04/22.
//

#include "player.hpp"

struct Player::Impl{

};

Player::Player(int player_nr){

};

Player::~Player(){

}

Player::Player(const Player& player){

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

bool Player::loses()const{

}

int Player::recurrence()const{

}