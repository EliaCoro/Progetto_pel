#include "player/player.hpp"


int main(){
    int board = 0, end = 0;

    //test dame
    /*string path ="../board/gameplay_"+std::to_string(board)+"/";
    std::__fs::filesystem::create_directory(path);
    Player one(1);
    Player two(2);
    int i = 0;
    one.load_board(path + "board_" + std::to_string(i) + ".txt");
    one.print_last_playground();
    one.move();
    one.print_last_playground();
    i++;
    one.store_board(path + "board_" + std::to_string(i) + ".txt");
    two.load_board(path + "board_" + std::to_string(i) + ".txt");
    two.move();
    two.print_last_playground();
    i++;
    two.store_board(path + "board_" + std::to_string(i) + ".txt");
*/
    /*while(board <= end) {
        string path ="../board/gameplay_"+std::to_string(board)+"/";
        std::__fs::filesystem::create_directory(path);
        cout << "board: " << board++ << endl;
        int i = 0;
        Player one(1);
        Player two(2);
        one.init_board(path + "board_" + std::to_string(i) + ".txt");

        one.print_playground();

        while (!one.loses() && !two.loses() && i <= 2000) {
            one.load_board(path + "board_" + std::to_string(i) + ".txt");
            i++;
            one.move();
            one.store_board(path + "board_" + std::to_string(i) + ".txt");
            one.print_last_playground();

            two.load_board(path + "board_" + std::to_string(i) + ".txt");
            i++;
            two.move();
            two.store_board(path + "board_" + std::to_string(i) + ".txt");
            two.print_last_playground();
        }
        if (one.loses() && two.loses())
            cout << "i player hanno perso" << endl;
        else if (one.loses())
            cout << "player 1 ha perso" << endl;
        else if (two.loses())
            cout << "player 2 ha perso" << endl;

        cout << "finished" << endl;
    }*/
}