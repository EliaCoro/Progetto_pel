#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <random>
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
    //this->delete_history();   //not working todo: c'è un problema di memoria alla riga successiva
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
    return pezzo;
}

void Player::print_playground(){
    Cell* pc = this->pimpl->history->head;
    int counter = 0;
    cout<<"player nr: "<<this->pimpl->player_nr<<endl;
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

void Player::print_last_playground(){
    Cell* pc = this->pimpl->history->tail;
    int counter = 0;
    for (int i = 0; i < playground_size; ++i){
        for (int j = 0; j < playground_size; ++j)
            cout<<from_enum_to_char(pc->playground[i][j])<<" ";
        cout<<endl;
    }
    cout<<endl<<endl<<endl;
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
    cout<<"input file:"<<endl;
    while(file.good()) {
        string s;
        getline(file, s);
        if(s.size() != 15)
            throw player_exception{player_exception::err_type(2), "Invalid board"};
        for (int j = 0; j < s.size(); j=j+2) {
            playground[i][j/2] = from_char_to_enum(s.at(j));
        }
        cout<<s<<endl;
        i++;
    }
    cout<<endl<<endl<<endl<<endl;
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
            if(j != playground_size-1)
                file << " ";
        }
        if(i != playground_size-1)
            file << endl;
    }

    if(!file.good())
        throw player_exception{player_exception::err_type(1), "Impossible to write in: "+filename};
    file.close();
    if(file.fail())
        throw player_exception{player_exception::err_type(1), "Impossible to write in: "+filename};
}


void Player::init_board(const string& filename)const{
    //todo: vedere se va salvata nella history

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

int Player::new_coordinates(char p, int r, int c, Directions direction) {
    int res;
    switch (direction) {
        case top_left:
            if(p == 'r')
                res = r - 1;
            else if (p == 'c')
                res = c - 1;
            break;
        case top_right:
            if(p == 'r')
                res = r - 1;
            else if (p == 'c')
                res = c + 1;
            break;
        case bottom_left:
            if(p == 'r')
                res = r + 1;
            else if (p == 'c')
                res = c - 1;
            break;
        case bottom_right:
            if(p == 'r')
                res = r + 1;
            else if (p == 'c')
                res = c + 1;
            break;
    }
    if(res >= 0 && res < playground_size)
        return res;
    else
        return -1;
}


//la funzione ritorna nullptr se il movimento non è consentito.
Player::Cell* Player::move_pawn(Player::piece matrix[playground_size][playground_size], int r, int c, Player::Directions direction){
    Player::Cell* res = nullptr;
    Player::piece temp[8][8];
    for (int i = 0; i < playground_size; ++i)
        for (int j = 0; j < playground_size; ++j)
            temp[i][j] = matrix[i][j];

    int new_r = new_coordinates('r', r, c, direction), new_c = new_coordinates('c', r, c, direction);
    //this->print_last_playground();
    if(new_r != -1 && new_c != -1){
        res = new Player::Cell;
        res->next = nullptr;
        res->prev = nullptr;
        res->index = 0;
        for (int i = 0; i < playground_size; ++i)
            for (int j = 0; j < playground_size; ++j)
                res->playground[i][j] = matrix[i][j];

        char temp2 = from_enum_to_char(matrix[new_r][new_c]);
        piece temp3 = (matrix[new_r][new_c]);

        if(matrix[new_r][new_c] == e){
            res->playground[new_r][new_c] = res->playground[r][c];
            res->playground[r][c] = e;
        }else{
            int last_r = new_coordinates('r', new_r, new_c, direction), last_c = new_coordinates('c', new_r, new_c, direction);
            if((last_r != -1 && last_c != -1) && res->playground[last_r][last_c] == e &&
                    ((res->playground[r][c] == x && res->playground[new_r][new_c] == o) ||
                    (res->playground[r][c] == o && res->playground[new_r][new_c] == x) ||
                    (res->playground[r][c] == X && (res->playground[new_r][new_c] == o || res->playground[new_r][new_c] == O)) ||
                    (res->playground[r][c] == O && (res->playground[new_r][new_c] == x || res->playground[new_r][new_c] == X)))){

                res->playground[new_r][new_c] = e;
                res->playground[last_r][last_c] = res->playground[r][c];
                res->playground[r][c] = e;
            } else{
                delete res;
                res = nullptr;
            }
        }
        //trasformare in dama!!!
    }
    return res;
}

struct Coordinates{
    Player::piece piece;
    int r;
    int c;
    bool valid;
};

//ritorna se si può muovere verso una direzione
bool Player::can_move(piece matrix[8][8], int r, int c) {
    switch (matrix[r][c]) {
        case x:
            return can_move_to(matrix, r, c, top_right) || can_move_to(matrix, r, c, top_left);
        case o:
            return can_move_to(matrix, r, c, bottom_right) || can_move_to(matrix, r, c, bottom_left);
        case X:
            return can_move_to(matrix, r, c, top_right) || can_move_to(matrix, r, c, top_left)
            || can_move_to(matrix, r, c, bottom_right) || can_move_to(matrix, r, c, top_left);
        case O:
            return can_move_to(matrix, r, c, top_right) || can_move_to(matrix, r, c, top_left)
                   || can_move_to(matrix, r, c, bottom_right) || can_move_to(matrix, r, c, top_left);
        case e:
            return false;
    }
}

bool Player::can_move_to(piece matrix[8][8], int r, int c, Player::Directions direction) {
    Cell* last_move = move_pawn(matrix, r,c, direction);
    bool res = false;
    if(last_move != nullptr){
        res = true;
        delete last_move;
    }
    return res;
}

void Player::move(){
    //il giocatore 1 deve muovere solo le x e il giocatore 2 solo le o
    //assicurarsi che le coordinate siano corrette altrimenti va in segmentation fault

    Coordinates coordinate[12];
    for (int i = 0; i < 12; ++i) {
        coordinate[i] = {e, -1, -1, false};
    }
    int number_coordinate = 0;
    for (int i = 0; i < playground_size; ++i) {
        for (int j = 0; j < playground_size; ++j){
            if((this->pimpl->player_nr == 1 && (this->pimpl->history->tail->playground[i][j] == x || this->pimpl->history->tail->playground[i][j] == X) ||
                this->pimpl->player_nr == 2 && (this->pimpl->history->tail->playground[i][j] == o || this->pimpl->history->tail->playground[i][j] == O)) &&
                        can_move(this->pimpl->history->tail->playground, i, j)){
                coordinate[number_coordinate] = {this->pimpl->history->tail->playground[i][j], i, j, true};
                number_coordinate++;
            }
        }
    }

    srand( time(NULL) );
    int v1 = rand() % number_coordinate;
    int v2 = rand()%4;
    Cell* last_move = nullptr;

    while(last_move == nullptr){
        if(coordinate[v1].piece == x)
            last_move = move_pawn(this->pimpl->history->tail->playground, coordinate[v1].r, coordinate[v1].c, Directions((v2++)%2));
        if(coordinate[v1].piece == o)
            last_move = move_pawn(this->pimpl->history->tail->playground, coordinate[v1].r, coordinate[v1].c, Directions((v2++)%2+2));
    }

    this->new_cell_history(last_move->playground);
    delete last_move;
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

bool Player::loses(){
    if(this->pimpl->history->head) {
        Coordinates coordinate[12];
        for (int i = 0; i < 12; ++i) {
            coordinate[i] = {e, -1, -1, false};
        }
        int number_coordinate = 0;
        for (int i = 0; i < playground_size; ++i) {
            for (int j = 0; j < playground_size; ++j) {
                if ((this->pimpl->player_nr == 1 && (this->pimpl->history->tail->playground[i][j] == x ||
                                                     this->pimpl->history->tail->playground[i][j] == X) ||
                     this->pimpl->player_nr == 2 && (this->pimpl->history->tail->playground[i][j] == o ||
                                                     this->pimpl->history->tail->playground[i][j] == O)) &&
                    can_move(this->pimpl->history->tail->playground, i, j)) {
                    coordinate[number_coordinate] = {this->pimpl->history->tail->playground[i][j], i, j, true};
                    number_coordinate++;
                }
            }
        }
        if (number_coordinate == 0)
            return true;
        else
            return false;
    }else
        return false;
}

int Player::recurrence()const{

}