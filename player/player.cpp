#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <random>
#include "player.hpp"

using namespace std;

#define playground_size 8



struct Cell;
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
    loose = -20
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

struct Player::Impl{
    int player_nr;
    History* history;

    void move_recursive(int player_nr, piece matrix[8][8], int& points, int depth);
    bool old_can_move(piece matrix[8][8], int r, int c);
    bool old_can_move_to(piece matrix[8][8], int r, int c, Directions direction);
    Cell* old_move_pawn(piece matrix[8][8], int r, int c, Directions direction);
    void old_move();

    bool last_move_to(piece matrix[8][8], int r, int c, Directions direction, int& point);
    Cell* last_move_pawn(piece matrix[8][8], int r, int c, Directions direction, int& point);
    void last_move();

    int new_coordinates(char p, int r, int c, Directions direction);
    piece from_char_to_enum(char c);
    char from_enum_to_char(piece p) const;
    bool correct_playground(piece matrix[8][8]);
    void delete_history();
    void new_cell_history(piece matrix[8][8]);
    void print_last_playground();
    void print_playground();
};

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

void Player::Impl::print_last_playground(){
    Cell* pc = this->history->tail;
    cout<<"----------------"<<endl;
    for (int i = 0; i < playground_size; ++i){
        for (int j = 0; j < playground_size; ++j)
            cout<<from_enum_to_char(pc->playground[i][j])<<" ";
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

struct Coordinates{
    Player::piece piece;
    int r;
    int c;
    bool valid;
};

bool Player::wins(int player_nr)const{

}

bool Player::wins() const{

}

bool Player::loses(int player_nr)const{

}

bool Player::loses(){
    if(this->pimpl->history->head) {
        Points points[12 * 4];
        for (int i = 0; i < 12 * 4; ++i) {
            points[i] = {-1, -1, Directions(i%4), 0, false};
        }

        int number_coordinate = 0;
        for (int i = 0; i < playground_size; ++i) {
            for (int j = 0; j < playground_size; ++j){
                if(this->pimpl->player_nr == 1 && (this->pimpl->history->tail->playground[i][j] == x || this->pimpl->history->tail->playground[i][j] == X) ||
                   this->pimpl->player_nr == 2 && (this->pimpl->history->tail->playground[i][j] == o || this->pimpl->history->tail->playground[i][j] == O)){

                    for (int k = 0; k < 4; ++k) {
                        points[number_coordinate].r = i;
                        points[number_coordinate].c = j;
                        points[number_coordinate].valid = this->pimpl->last_move_to(this->pimpl->history->tail->playground, i, j,
                                                                       points[number_coordinate].direction,
                                                                       points[number_coordinate].point);
                        number_coordinate++;
                    }
                }
            }
        }
        for (int i = 0; i < number_coordinate; ++i) {
            if(points[i].valid)
                return false;
        }
        return true;
    }
    return false;
}

int Player::recurrence()const{

}

//-------------------------------------------------------------------------------------------------------------------------------------------//

Cell* Player::Impl::last_move_pawn(Player::piece matrix[playground_size][playground_size], int r, int c, Directions direction, int& point){
    Cell* res = nullptr;
    Player::piece temp[8][8];
    for (int i = 0; i < playground_size; ++i)
        for (int j = 0; j < playground_size; ++j)
            temp[i][j] = matrix[i][j];

    int new_r = new_coordinates('r', r, c, direction), new_c = new_coordinates('c', r, c, direction);
    if(new_r != -1 && new_c != -1){
        res = new Cell;
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
        }
    }
    return res;
}


bool Player::Impl::last_move_to(piece matrix[8][8], int r, int c, Directions direction, int& point){
    Cell* temp = nullptr;
    piece pezzo = matrix[r][c];

    if(matrix[r][c] == x && (direction == top_left || direction == top_right)) {
        temp = last_move_pawn(matrix, r, c, direction, point);
    }
    if(matrix[r][c] == o && (direction == bottom_left || direction == bottom_right)) {
        temp = last_move_pawn(matrix, r, c, direction, point);
    }
    if(matrix[r][c] == X || matrix[r][c] == O)
        temp = last_move_pawn(matrix, r, c, direction, point);

    if(temp != nullptr){
        delete temp;
        return true;
    }else
        return false;

}

void Player::Impl::last_move(){
    Points points[12 * 4];
    for (int i = 0; i < 12 * 4; ++i) {
        points[i] = {-1, -1, Directions(i%4), 0, false};
    }

    int number_coordinate = 0;
    for (int i = 0; i < playground_size; ++i) {
        for (int j = 0; j < playground_size; ++j){
            if(this->player_nr == 1 && (this->history->tail->playground[i][j] == x || this->history->tail->playground[i][j] == X) ||
               this->player_nr == 2 && (this->history->tail->playground[i][j] == o || this->history->tail->playground[i][j] == O)){

                for (int k = 0; k < 4; ++k) {
                    points[number_coordinate].r = i;
                    points[number_coordinate].c = j;
                    points[number_coordinate].valid = last_move_to(this->history->tail->playground, i, j,
                                                                   points[number_coordinate].direction,
                                                                   points[number_coordinate].point);
                    number_coordinate++;
                }
            }
        }
    }

    int find_max_pos = 0, points_max = -1;
    for (int i = 0; i < number_coordinate; ++i) {
        if(points[i].valid && points[i].point > points_max){
            find_max_pos = i;
        }
    }

    if(number_coordinate > 0 && points[find_max_pos].valid) {
        int temp_points;
        Cell* last_move = last_move_pawn(this->history->tail->playground, points[find_max_pos].r,
                                         points[find_max_pos].c, points[find_max_pos].direction, temp_points);
        if(last_move){
            this->new_cell_history(last_move->playground);
            delete last_move;
        }
    }
}

void Player::Impl::move_recursive(int player_nr, Player::piece matrix[8][8], int& points, int depth){
    if(depth == 0){
        return;
    }else{
        Points points[12 * 4];
        int number_coordinate = 0;
        //init points

        for (int i = 0; i < number_coordinate; ++i) {
            Cell* temp = last_move_pawn(matrix, points->r, points->c, points->direction, points->point);

            move_recursive((player_nr+1) % 2, temp->playground, points->point, depth-1);
            delete temp;
        }
        //sorting
        if(number_coordinate != 0)
            return;
        else
            return;
    }
}



void Player::move() {
    if(this->pimpl->player_nr == 1)
        this->pimpl->last_move();
    else
        this->pimpl->old_move();
}

bool Player::valid_move() const{

}

//-------------------------------------------------------------//

bool Player::Impl::old_can_move(piece matrix[8][8], int r, int c) {
    switch (matrix[r][c]) {
        case x:
            return old_can_move_to(matrix, r, c, top_right) || old_can_move_to(matrix, r, c, top_left);
        case o:
            return old_can_move_to(matrix, r, c, bottom_right) || old_can_move_to(matrix, r, c, bottom_left);
        case X:
            return old_can_move_to(matrix, r, c, top_right) || old_can_move_to(matrix, r, c, top_left)
                   || old_can_move_to(matrix, r, c, bottom_right) || old_can_move_to(matrix, r, c, bottom_left);
        case O:
            return old_can_move_to(matrix, r, c, top_right) || old_can_move_to(matrix, r, c, top_left)
                   || old_can_move_to(matrix, r, c, bottom_right) || old_can_move_to(matrix, r, c, bottom_left);
        case e:
            return false;
    }
}

bool Player::Impl::old_can_move_to(piece matrix[8][8], int r, int c, Directions direction) {
    Cell* last_move = old_move_pawn(matrix, r, c, direction);
    bool res = false;
    if(last_move != nullptr){
        res = true;
        delete last_move;
    }
    return res;
}

Cell* Player::Impl::old_move_pawn(Player::piece matrix[playground_size][playground_size], int r, int c, Directions direction){
    Cell* res = nullptr;
    Player::piece temp[8][8];
    for (int i = 0; i < playground_size; ++i)
        for (int j = 0; j < playground_size; ++j)
            temp[i][j] = matrix[i][j];

    int new_r = new_coordinates('r', r, c, direction), new_c = new_coordinates('c', r, c, direction);
    if(new_r != -1 && new_c != -1){
        res = new Cell;
        res->next = nullptr;
        res->prev = nullptr;
        res->index = 0;
        for (int i = 0; i < playground_size; ++i)
            for (int j = 0; j < playground_size; ++j)
                res->playground[i][j] = matrix[i][j];

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
        if(res != nullptr){
            for (int i = 0; i < playground_size; ++i) {
                if(res->playground[0][i] == x)
                    res->playground[0][i] = X;
                if(res->playground[playground_size-1][i] == o)
                    res->playground[playground_size-1][i] = O;
            }
        }
    }
    return res;
}

void Player::Impl::old_move(){
    Coordinates coordinate[12];
    for (int i = 0; i < 12; ++i) {
        coordinate[i] = {e, -1, -1, false};
    }
    int number_coordinate = 0;
    for (int i = 0; i < playground_size; ++i) {
        for (int j = 0; j < playground_size; ++j){
            if((this->player_nr == 1 && (this->history->tail->playground[i][j] == x || this->history->tail->playground[i][j] == X) ||
                this->player_nr == 2 && (this->history->tail->playground[i][j] == o || this->history->tail->playground[i][j] == O)) &&
                    old_can_move(this->history->tail->playground, i, j)){
                coordinate[number_coordinate] = {this->history->tail->playground[i][j], i, j, true};
                number_coordinate++;
            }
        }
    }

    if(number_coordinate > 0) {
        srand( time(NULL) );
        int v1 = rand() % number_coordinate;
        int v2 = rand()%4;
        Cell* last_move = nullptr;

        while (last_move == nullptr) {
            if (coordinate[v1].piece == x)
                last_move = old_move_pawn(this->history->tail->playground, coordinate[v1].r, coordinate[v1].c,
                                      Directions((v2++) % 2));
            if (coordinate[v1].piece == o)
                last_move = old_move_pawn(this->history->tail->playground, coordinate[v1].r, coordinate[v1].c,
                                      Directions((v2++) % 2 + 2));
            if (coordinate[v1].piece == X || coordinate[v1].piece == O)
                last_move = old_move_pawn(this->history->tail->playground, coordinate[v1].r, coordinate[v1].c,
                                      Directions((v2++) % 4));
        }

        this->new_cell_history(last_move->playground);
        delete last_move;
    }
}