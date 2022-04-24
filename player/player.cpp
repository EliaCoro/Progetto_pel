#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include "player.hpp"

using namespace std;

#define playground_size 8

struct Player::Cell{
    int index;
    Player::piece playground[playground_size][playground_size];
    Cell* prev;
    Cell* next;
};

struct History{
    Player::Cell* head;
    Player::Cell* tail;
    int playground_number;
};

struct Player::Impl{
    int player_nr;
    char pieces;
    History* history;
};

Player::piece Player::from_char_to_enum(char c){
    switch(c){
        case 'x': return x;
        case 'X': return X;
        case 'o': return o;
        case 'O': return O;
        case ' ': return e;
        default: throw "Invalid char";
    }
}

char Player::from_enum_to_char(Player::piece p) const {
    switch(p){
        case x: return 'x';
        case X: return 'X';
        case o: return'o';
        case O: return 'O';
        case e: return ' ';
        default: throw "Invalid enum";
    }
}

Player::Player(int player_nr){
    if(player_nr == 1 || player_nr == 2){
        this->pimpl = new Impl;
        this->pimpl->player_nr = player_nr;
        this->pimpl->history = new History{nullptr, nullptr, 0};
    }else{
        throw player_exception{player_exception::err_type(0), "Invalid Player_nr"};
    }
};

void Player::delete_history(){
    if (this->pimpl != nullptr && this->pimpl->history->head != nullptr) {
        Cell *pc = this->pimpl->history->head;
        while (this->pimpl->history->head != nullptr) {
            this->pimpl->history->head = this->pimpl->history->head->next;
            delete pc;
            pc = this->pimpl->history->head;
        }
        this->pimpl->history->tail = nullptr;
    }
}

Player::~Player(){
    if(this->pimpl != nullptr) {
        this->delete_history();
        delete pimpl->history;
        delete pimpl;
    }
}

Player::Player(const Player& player){
    *this = player;
}

Player& Player::operator=(const Player& player){
    //delete this;  //not working
    this->delete_history();
    new Player(1);
    this->pimpl = new Impl;
    this->pimpl->player_nr = player.pimpl->player_nr;
    this->pimpl->history = new History;
    this->pimpl->history->head = nullptr;
    this->pimpl->history->tail = nullptr;
    Cell* pc = player.pimpl->history->head;
    while(pc){
        this->new_cell_history(pc->playground);
        pc = pc->next;
    }
    return *this;
}

Player::piece Player::operator()(int r, int c, int history_offset)const{
    if(!(r >= 0 && r < playground_size && c >= 0 && c < playground_size))
        throw player_exception{player_exception::err_type(0), "Invalid coordinates"};

    Cell* pc = this->pimpl->history->tail;
    if(pc == nullptr)
        throw player_exception{player_exception::err_type(0), "Invalid history_offset"};

    for (int i = 0; i < history_offset; ++i) {
        pc = pc->prev;
        if(pc == nullptr)
            throw player_exception{player_exception::err_type(0), "Invalid history_offset"};
    }
    piece pezzo = pc->playground[r][c];
    cout << from_enum_to_char(pezzo)<<endl;
    return pezzo;
}

void Player::print_playground(){
    Cell* pc = this->pimpl->history->head;
    int counter = 0;
    while(pc){
        cout<<"Playground nr: " <<counter++<<endl;
        for (int i = 0; i < playground_size; ++i){
            for (int j = 0; j < playground_size; ++j)
                cout<<from_enum_to_char(pc->playground[i][j])<<" ";
            cout<<endl;
        }
        cout<<endl<<endl<<endl;
        pc = pc->next;
    }
}

void Player::new_cell_history(Player::piece matrix[playground_size][playground_size]) {
    Cell *nc = new Cell;
    nc->next = nullptr;
    nc->prev = this->pimpl->history->tail;
    if (this->pimpl->history->head == nullptr) {
        this->pimpl->history->head = nc;
        this->pimpl->history->tail = nc;
    } else {
        this->pimpl->history->tail->next = nc;
        this->pimpl->history->tail = nc;
    }
    this->pimpl->history->playground_number = this->pimpl->history->playground_number + 1;

    for (int i = 0; i < playground_size; ++i)
        for (int j = 0; j < playground_size; ++j)
            nc->playground[i][j] = matrix[i][j];
}

bool Player::correct_playground(Player::piece matrix[playground_size][playground_size]){
    int count_x = 0, count_o = 0, count_e = 0;
    bool correct = true;
    for (int i = 0; i < playground_size; ++i) {
        for (int j = 0; j < playground_size; ++j) {
            piece temp = matrix[i][j];
            if(matrix[i][j] == x || matrix[i][j] == X)
                count_x++;
            else if(matrix[i][j] == o || matrix[i][j] == O)
                count_o++;
            else if(matrix[i][j] == e)
                count_e++;
            else
                correct = false;
        }
    }
    return correct && count_x <= 12 && count_o <= 12;
}

void Player::load_board(const string& filename){
    ifstream file{filename};
    if (!file.good())
        throw player_exception{player_exception::err_type(1), "Missing file: "+filename};

    Player::piece playground[playground_size][playground_size];
    int i = 0;
    while(file.good()) {
        string s;
        getline(file, s);
        if(s.size() != 15)
            throw player_exception{player_exception::err_type(2), "Invalid board"};
        for (int j = 0; j < s.size(); j=j+2) {
            playground[i][j/2] = from_char_to_enum(s.at(j));
        }
        i++;
    }
    if (!file.eof())
        throw player_exception{player_exception::err_type(2),"We should be at the end of the file, but we are not"};

    if(correct_playground(playground))
        this->new_cell_history(playground);
    else
        throw player_exception{player_exception::err_type(2), "Invalid board"};
}

void Player::store_board(const string& filename, int history_offset) const{
    ofstream file(filename);
    Cell* pc = this->pimpl->history->tail;
    if(pc == nullptr)
        throw player_exception{player_exception::err_type(0), "Invalid history_offset"};

    for (int i = 0; i < history_offset; ++i) {
        pc = pc->prev;
        if(pc == nullptr)
            throw player_exception{player_exception::err_type(0), "Invalid board"};
    }
    for (int i = 0; i < playground_size; ++i) {
        for (int j = 0; j < playground_size; ++j) {
            file << from_enum_to_char(pc->playground[i][j]);
            file << " ";
        }
        file << endl;
    }

    if(!file.good())
        throw player_exception{player_exception::err_type(1), "Impossible to write in: "+filename};
    file.close();
    if(file.fail())
        throw player_exception{player_exception::err_type(1), "Impossible to write in: "+filename};
}


void Player::init_board(const string& filename)const{
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
    if(!file.good())
        throw player_exception{player_exception::err_type(1), "Impossible to write in: "+filename};
    file.close();
    if(file.fail())
        throw player_exception{player_exception::err_type(1), "Impossible to write in: "+filename};
}

Player::Cell* move_pawn(Player::piece matrix[playground_size][playground_size], int r, int c, Player::directions direction){
    Player::Cell* res = new Player::Cell;
    res->next = nullptr;
    res->prev = nullptr;
    res->index = 0;
    for (int i = 0; i < playground_size; ++i)
        for (int j = 0; j < playground_size; ++j)
            res->playground[i][j] = matrix[i][j];


    switch(direction){
        case Player::top_left:
            if(r + 1 < playground_size && c - 1 >= 0){

            }
            break;
        case Player::top_right:
            if(r + 1 < playground_size && c + 1 < playground_size){

            }
            break;
        case Player::bottom_left:
            if(r - 1 >= 0 && c - 1 >= 0){

            }
            break;
        case Player::bottom_right:
            if(r - 1 >= 0 && c + 1 < playground_size){

            }
            break;
        default:
            throw "Invalid direction: " + direction;
    }

}

void Player::move(){
    //il giocatore 1 deve muovere solo le x e il giocatore 2 solo le o

}

bool Player::valid_move() const{

}

void Player::pop(){
    if(this->pimpl->history->tail != nullptr){
        Cell *temp = this->pimpl->history->tail;
        if(this->pimpl->history->head == this->pimpl->history->tail){
            temp = this->pimpl->history->tail;
            this->pimpl->history->head = nullptr;
            this->pimpl->history->tail = nullptr;
        }else {
            this->pimpl->history->tail = this->pimpl->history->tail->prev;
            this->pimpl->history->tail->next = nullptr;
        }
        delete temp;
    } else
        throw player_exception{player_exception::err_type(0), "history empty"};
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