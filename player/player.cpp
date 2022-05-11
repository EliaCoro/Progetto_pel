#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include "player.hpp"

using namespace std;

#define playground_size 8

enum Directions{
    top_left,
    top_right,
    bottom_left,
    bottom_right
};

struct Points{
    int r;
    int c;
    Directions direction;
    int point;
    bool valid;
};

enum point{
    nothing = 0,
    eat_pawn = 10,
    eat_dama = 15,
    make_dama = 13,
    win = 50
};

struct Cell{
    int index;
    Player::piece playground[playground_size][playground_size];
    Cell* prev;
    Cell* next;
};

struct History{
    Cell* head;
    Cell* tail;
    int playground_number;
};

struct Coordinates{
    Player::piece piece;
    int r;
    int c;
    bool valid;
};

struct Player::Impl{
    int player_nr;
    History* history;

    void print_this_playground(piece matrix[8][8]);

    Cell* move_recursive(int player_nr, piece matrix[8][8], int& points, int depth);
    bool old_can_move(piece matrix[8][8], int r, int c);
    bool old_can_move_to(piece matrix[8][8], int r, int c, Directions direction);
    Cell* old_move_pawn(piece matrix[8][8], int r, int c, Directions direction);
    void old_move();

    bool has_loose(int player_nymber, piece matrix[8][8]);

    bool last_move_to(piece matrix[8][8], int r, int c, Directions direction, int& point, bool calculate_loose);
    Cell* last_move_pawn(piece matrix[8][8], int r, int c, Directions direction, int& point, bool calculate_loose);
    void last_move();

    int new_coordinates(char p, int r, int c, Directions direction);
    piece from_char_to_enum(char c);
    char from_enum_to_char(piece p) const;
    bool correct_playground(piece matrix[8][8]);
    void delete_history();
    void new_cell_history(piece matrix[8][8]);
    //void print_last_playground();
    void print_playground();
    void init_points(int player_number, piece matrix[8][8], Points *points, int &number_coordinate);
};

void Player::Impl::print_this_playground(piece matrix[8][8]){
    cout<<"----------------"<<endl;
    for (int i = 0; i < playground_size; ++i){
        for (int j = 0; j < playground_size; ++j)
            cout<<from_enum_to_char(matrix[i][j])<<" ";
        cout<<endl;
    }
    cout<<"----------------"<<endl;
    cout<<endl<<endl<<endl;
}

Player::piece Player::Impl::from_char_to_enum(char c){
    switch(c){
        case 'x': return x;
        case 'X': return X;
        case 'o': return o;
        case 'O': return O;
        case ' ': return e;
        default: throw "Invalid char";
    }
}

char Player::Impl::from_enum_to_char(Player::piece p) const {
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

void Player::Impl::delete_history(){
    this->history;
    if (this != nullptr && this->history->head != nullptr) {
        Cell *pc = this->history->head;
        while (this->history->head != nullptr) {
            this->history->head = this->history->head->next;
            delete pc;
            pc = this->history->head;
        }
        this->history->tail = nullptr;
    }
}

Player::~Player(){
    if(this->pimpl != nullptr) {
        this->pimpl->delete_history();
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
    //new Player(1);    //forse è da decommentare

    this->pimpl = new Impl;
    this->pimpl->player_nr = player.pimpl->player_nr;
    this->pimpl->history = new History;
    this->pimpl->history->head = nullptr;
    this->pimpl->history->tail = nullptr;
    Cell* pc = player.pimpl->history->head;
    while(pc){
        this->pimpl->new_cell_history(pc->playground);
        pc = pc->next;
    }
    return *this;
}

Player::piece Player::operator()(int r, int c, int history_offset)const{
    //todo, controllare: modificare le pos 8-r e 8-c

    if(!(8 - r >= 0 && 8 - r < playground_size && 8 - c >= 0 && 8 - c < playground_size))
        throw player_exception{player_exception::err_type(0), "Invalid coordinates"};

    Cell* pc = this->pimpl->history->tail;
    if(pc == nullptr)
        throw player_exception{player_exception::err_type(0), "Invalid history_offset"};

    for (int i = 0; i < history_offset; ++i) {
        pc = pc->prev;
        if(pc == nullptr)
            throw player_exception{player_exception::err_type(0), "Invalid history_offset"};
    }
    piece pezzo = pc->playground[8-r][8-c];
    return pezzo;
}

void Player::Impl::print_playground(){
    Cell* pc = this->history->head;
    int counter = 0;
    cout<<"player nr: "<<this->player_nr<<endl;
    cout<<"----------------"<<endl;
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
    cout<<"----------------"<<endl;
}

void Player::print_last_playground(){
    Cell* pc = this->pimpl->history->tail;
    cout<<"----------------"<<endl;
    for (int i = 0; i < playground_size; ++i){
        for (int j = 0; j < playground_size; ++j)
            cout<<this->pimpl->from_enum_to_char(pc->playground[i][j])<<" ";
        cout<<endl;
    }
    cout<<"----------------"<<endl;
    cout<<endl<<endl<<endl;
}

void Player::Impl::new_cell_history(Player::piece matrix[playground_size][playground_size]) {
    Cell *nc = new Cell;
    nc->next = nullptr;
    nc->prev = this->history->tail;
    if (this->history->head == nullptr) {
        this->history->head = nc;
        this->history->tail = nc;
    } else {
        this->history->tail->next = nc;
        this->history->tail = nc;
    }
    this->history->playground_number = this->history->playground_number + 1;

    for (int i = 0; i < playground_size; ++i)
        for (int j = 0; j < playground_size; ++j)
            nc->playground[i][j] = matrix[i][j];
}

bool Player::Impl::correct_playground(Player::piece matrix[playground_size][playground_size]){
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
            playground[i][j/2] = this->pimpl->from_char_to_enum(s.at(j));
        }
        i++;
    }
    if (!file.eof())
        throw player_exception{player_exception::err_type(2),"We should be at the end of the file, but we are not"};

    if(this->pimpl->correct_playground(playground))
        this->pimpl->new_cell_history(playground);
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
            file << this->pimpl->from_enum_to_char(pc->playground[i][j]);
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
    //todo: vedere se va salvata nella history -> Ale dice che non serve ;)

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

int Player::Impl::new_coordinates(char p, int r, int c, Directions direction) {
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

bool Player::Impl::has_loose(int player_number, Player::piece matrix[8][8]) {
    if(this->history->head) {
        Points points[12 * 4];
        int number_coordinate = 0;
        init_points(player_number, matrix, points, number_coordinate);
        for (int i = 0; i < number_coordinate; ++i) {
            if(points[i].valid)
                return false;
        }
        return true;
    }
    return false;


}

bool Player::wins(int player_nr)const{
    return !(this->pimpl->has_loose(player_nr, this->pimpl->history->tail->playground));
}

bool Player::wins() const{
    return !(this->pimpl->has_loose(this->pimpl->player_nr, this->pimpl->history->tail->playground));
}

bool Player::loses(int player_nr)const{
    return (this->pimpl->has_loose(player_nr, this->pimpl->history->tail->playground));
}

bool Player::loses() const{
    return (this->pimpl->has_loose(this->pimpl->player_nr,this->pimpl->history->tail->playground));
}

int Player::recurrence()const{

}

//-------------------------------------------------------------------------------------------------------------------------------------------//

int get_player_number(Player::piece piece){
    if(piece == Player::x || piece == Player::X)
        return 1;
    else if(piece == Player::o || piece == Player::O)
        return 2;
    else
        throw "Invalid piece";
}

Cell* Player::Impl::last_move_pawn(Player::piece matrix[playground_size][playground_size], int r, int c, Directions direction, int& point, bool calculate_loose){
    Cell* res = nullptr;
    Player::piece temp[8][8];
    Player:: piece pezzo = matrix[r][c];
    for (int i = 0; i < playground_size; ++i)
        for (int j = 0; j < playground_size; ++j)
            temp[i][j] = matrix[i][j];

    int new_r = new_coordinates('r', r, c, direction), new_c = new_coordinates('c', r, c, direction);
    if(new_r != -1 && new_c != -1){
        res = new Cell;
        res->next = nullptr;
        res->next = nullptr;
        res->prev = nullptr;
        res->index = 0;
        for (int i = 0; i < playground_size; ++i)
            for (int j = 0; j < playground_size; ++j)
                res->playground[i][j] = matrix[i][j];

        if(matrix[new_r][new_c] == e){
            point = nothing;
            res->playground[new_r][new_c] = res->playground[r][c];
            res->playground[r][c] = e;
        }else{
            int last_r = new_coordinates('r', new_r, new_c, direction), last_c = new_coordinates('c', new_r, new_c, direction);
            if((last_r != -1 && last_c != -1) && res->playground[last_r][last_c] == e &&
               ((res->playground[r][c] == x && res->playground[new_r][new_c] == o) ||
                (res->playground[r][c] == o && res->playground[new_r][new_c] == x) ||
                (res->playground[r][c] == X && (res->playground[new_r][new_c] == o || res->playground[new_r][new_c] == O)) ||
                (res->playground[r][c] == O && (res->playground[new_r][new_c] == x || res->playground[new_r][new_c] == X)))){

                if(res->playground[new_r][new_c] == x || res->playground[new_r][new_c] == o)
                    point = eat_pawn;

                if(res->playground[new_r][new_c] == X || res->playground[new_r][new_c] == O)
                    point = eat_dama;

                res->playground[new_r][new_c] = e;
                res->playground[last_r][last_c] = res->playground[r][c];
                res->playground[r][c] = e;
            } else{
                delete res;
                res = nullptr;
            }
        }
        if(res != nullptr){
            for (int i = 0; i < playground_size; ++i) {
                if(res->playground[0][i] == x) {
                    res->playground[0][i] = X;
                    point+=make_dama;
                }
                if(res->playground[playground_size-1][i] == o) {
                    res->playground[playground_size - 1][i] = O;
                    point+=make_dama;
                }
            }
            if(calculate_loose && this->has_loose(get_player_number(pezzo), this->history->tail->playground))
                point=win;
        }
    }
    return res;
}


bool Player::Impl::last_move_to(piece matrix[8][8], int r, int c, Directions direction, int& point, bool calculate_loose){
    Cell* temp = nullptr;
    piece pezzo = matrix[r][c];

    if(matrix[r][c] == x && (direction == top_left || direction == top_right)) {
        temp = last_move_pawn(matrix, r, c, direction, point, calculate_loose);
    }
    if(matrix[r][c] == o && (direction == bottom_left || direction == bottom_right)) {
        temp = last_move_pawn(matrix, r, c, direction, point, calculate_loose);
    }
    if(matrix[r][c] == X || matrix[r][c] == O)
        temp = last_move_pawn(matrix, r, c, direction, point, calculate_loose);

    if(temp != nullptr){
        delete temp;
        return true;
    }else
        return false;

}

void Player::Impl::init_points(int player_number, Player::piece matrix[8][8], Points *points, int &number_coordinate){
    for (int i = 0; i < 12 * 4; ++i) {
        points[i] = {-1, -1, Directions(i%4), 0, false};
    }
    for (int i = 0; i < playground_size; ++i) {
        for (int j = 0; j < playground_size; ++j){
            if(player_number == 1 && (matrix[i][j] == x || matrix[i][j] == X) ||
               player_number == 2 && (matrix[i][j] == o || matrix[i][j] == O)){

                for (int k = 0; k < 4; ++k) {
                    points[number_coordinate].r = i;
                    points[number_coordinate].c = j;
                    Player temp_player(player_number);
                    auto temp = temp_player.pimpl->last_move_to(matrix, i, j,
                                                 points[number_coordinate].direction,
                                                 points[number_coordinate].point, false);
                    points[number_coordinate].valid = temp;
                    number_coordinate++;
                }
            }
        }
    }
}

void sorting(Points* points, int number_coordinate){
    for (int i = number_coordinate-1; i >= 0; --i) {
        for (int j = number_coordinate-1; j >= 0; --j) {
            if(points[j].valid || points[i].valid){
                if((points[j].valid && !points[i].valid) || (points[j].valid && points[i].valid && points[j].point > points[i].point)) {
                    auto temp = points[i];
                    points[i] = points[j];
                    points[j] = temp;
                }
            }
        }
    }
}

Cell* Player::Impl::move_recursive(int player_number, Player::piece matrix[8][8], int& punteggio, int depth){
    if(depth == 0){
        return nullptr;
    }else{
        Points points[12 * 4];
        int number_coordinate = 0;
        init_points(player_number, matrix, points, number_coordinate);


        for (int i = 0; i < number_coordinate; ++i) {
            if(points[i].valid) {
                int temp_points;
                Cell *temp = last_move_pawn(matrix, points[i].r, points[i].c, points[i].direction, temp_points, true);
                int temp_points2 = 0;
                //cout<<"r: "<<points[i].r<<" c: "<<points[i].c<<endl;
                //this->print_this_playground(matrix);
                move_recursive((player_nr + 1) % 3, temp->playground, temp_points2, depth - 1);
                //this->print_this_playground(matrix);
                points[i].point -= temp_points2 + depth;
                if(temp != nullptr){
                    delete temp;
                }
            }
        }

        sorting(points, number_coordinate);

        if(number_coordinate > 0 && points[0].valid) {
            Cell* last_move = last_move_pawn(matrix, points[0].r,
                                             points[0].c, points[0].direction, punteggio, true);
            if(last_move){
                //this->print_this_playground(last_move->playground);
                //cout<<"punteggio: "<<punteggio<<endl;
                return last_move;
            }
        }else
            return nullptr;
    }
}



void Player::move() {
    //implementare win nel recursive
    if(this->pimpl->player_nr == 1 || this->pimpl->player_nr == 2){
        int points, depth = 4;
        Cell* res = this->pimpl->move_recursive(this->pimpl->player_nr, this->pimpl->history->tail->playground, points, depth);
        if(res != nullptr){
            this->pimpl->new_cell_history(res->playground);
            delete res;
        }
    }
    else{
        //this->pimpl->old_move();
    }
}

bool Player::valid_move() const{

}

//-------------------------------------------------------------//