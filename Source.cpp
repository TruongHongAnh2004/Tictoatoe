#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

class Board {
private:
    std::vector<std::vector<char>> board;

public:
    Board() {
        board.resize(3, std::vector<char>(3, ' '));
    }

    int getIndex(int row, int col) {
        return row * 3 + col + 1;
    }

    void displayBoard() {
        std::cout << "-------------" << std::endl;
        for (int i = 0; i < 3; i++) {
            std::cout << "| ";
            for (int j = 0; j < 3; j++) {
                std::cout << getIndex(i, j) << " | ";
            }
            std::cout << std::endl << "-------------" << std::endl;
        }
    }

    bool isFull() {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == ' ') {
                    return false;
                }
            }
        }
        return true;
    }

    bool checkWin(char player) {
        // Check rows
        for (int i = 0; i < 3; i++) {
            if (board[i][0] == player && board[i][1] == player && board[i][2] == player) {
                return true;
            }
        }

        // Check columns
        for (int i = 0; i < 3; i++) {
            if (board[0][i] == player && board[1][i] == player && board[2][i] == player) {
                return true;
            }
        }

        // Check diagonals
        if (board[0][0] == player && board[1][1] == player && board[2][2] == player) {
            return true;
        }
        if (board[0][2] == player && board[1][1] == player && board[2][0] == player) {
            return true;
        }

        return false;
    }

    bool setMove(int row, int col, char player) {
        if (board[row][col] == ' ') {
            board[row][col] = player;
            return true;
        }
        return false;
    }

    void setCell(int row, int col, char symbol) {
        // Kiểm tra xem ô đó có hợp lệ không
        if (row >= 0 && row < 3 && col >= 0 && col < 3 && board[row][col] == ' ') {
            // Đánh dấu ô đó bằng ký hiệu 'X'
            board[row][col] = 'X';
        }
        // Nếu không hợp lệ, có thể in ra thông báo lỗi
        else {
            std::cout << "Invalid move!" << std::endl;
        }
    }

    char& getCell(int row, int col) {
        return board[row][col];
    }

};

class Player {
protected:
    char symbol;

public:
    Player(char symbol) : symbol(symbol) {}

    virtual std::pair<int, int> getMove(Board& board) = 0;

    char getSymbol() {
        return symbol;
    }
};

class HumanPlayer : public Player {
public:
    HumanPlayer(char symbol) : Player(symbol) {}

    std::pair<int, int> getMove(Board& board) override {
        int index;
        std::cout << "Your turn (" << symbol << "), enter a number (1-9): ";
        std::cin >> index;

        // Chuyển đổi số thành tọa độ hàng và cột
        int row = (index - 1) / 3;
        int col = (index - 1) % 3;
        
         

        // Kiểm tra nếu ô đã được đánh
        if (board.getCell(row, col) != 'X') {
            std::cout << "That spot is already taken, try again." << std::endl;

            // In ra bảng với ký tự 'X' trên ô đã được chọn
            board.displayBoard();

            return getMove(board); // Gọi lại getMove() để người chơi nhập lại
        }

        // Đánh dấu ô với ký tự của người chơi
        board.setCell(row, col, symbol);
        board.displayBoard(); // In ra bảng với ký tự 'X' trên ô vừa được chọn

        return std::make_pair(row, col);
    }
};

class Robot : public Player {
private:
    std::pair<int, int> getEasyMove(Board& board) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 2);

        int row, col;
        do {
            row = dis(gen);
            col = dis(gen);
        } while (board.getCell(row, col) != ' ');

        return std::make_pair(row, col);
    }

    std::pair<int, int> getNormalMove(Board& board) {
        // Implement normal AI logic here
        return getEasyMove(board);
    }

    std::pair<int, int> getHardMove(Board& board) {
        // Implement hard AI logic here
        return getEasyMove(board);
    }

    int difficulty;
public:
    Robot(char symbol, int difficulty) : Player(symbol), difficulty(difficulty) {}

    std::pair<int, int> getMove(Board& board) override {
        switch (difficulty) {
        case 0:
            return getEasyMove(board);
        case 1:
            return getNormalMove(board);
        case 2:
            return getHardMove(board);
        default:
            return getEasyMove(board);
        }
    }
};

class Game {
private:
    Board board;
    std::unique_ptr<Player> player1;
    std::unique_ptr<Player> player2;

public:
    Game() : board(), player1(new HumanPlayer('X')), player2(new Robot('O', 2)) {}

    void run() {
        Player* currentPlayer = player1.get();
        int gameMode = 0;

        // Hiển thị menu lựa chọn chế độ chơi
        while (gameMode < 1 || gameMode > 2) {
            std::cout << "Select game mode:" << std::endl;
            std::cout << "1. Player vs Player" << std::endl;
            std::cout << "2. Player vs Computer" << std::endl;

            std::cout << "Enter number: ";
            std::cin >> gameMode;

            if (gameMode < 1 || gameMode > 2) {
                std::cout << "Invalid choice. Please try again." << std::endl;
            }
        }

        // Tạo đối tượng Player2 dựa trên chế độ chơi đã chọn
        if (gameMode == 1) {
            player2 = std::unique_ptr<Player>(new HumanPlayer('O'));
        }
        else {
            int difficulty = 0;
            while (difficulty < 1 || difficulty > 3) {
                std::cout << "Select difficulty level:" << std::endl;
                std::cout << "1. Easy" << std::endl;
                std::cout << "2. Medium" << std::endl;
                std::cout << "3. Hard" << std::endl;
                std::cout << "Enter number: ";
                std::cin >> difficulty;

                if (difficulty < 1 || difficulty > 3) {
                    std::cout << "Invalid choice. Please try again." << std::endl;
                }
            }

            if (difficulty == 1) {
                player2 = std::unique_ptr<Player>(new Robot('O', 1));
            }
            else if (difficulty == 2) {
                player2 = std::unique_ptr<Player>(new Robot('O', 2));
            }
            else {
                player2 = std::unique_ptr<Player>(new Robot('O', 3));
            }
        }

        // Tiếp tục game
        while (true) {
            board.displayBoard();
            std::pair<int, int> move = currentPlayer->getMove(board);
            if (!board.setMove(move.first, move.second, currentPlayer->getSymbol())) {
                std::cout << "That spot is already taken, try again." << std::endl;
                continue;
            }
            else {
                std::cout << "Player " << currentPlayer->getSymbol() << " moved to position (" << move.first << ", " << move.second << ")" << std::endl;
            }

            if (board.checkWin(currentPlayer->getSymbol())) {
                board.displayBoard();
                std::cout << "Player " << currentPlayer->getSymbol() << " wins!" << std::endl;
                return;
            }

            if (board.isFull()) {
                board.displayBoard();
                std::cout << "It's a tie!" << std::endl;
                return;
            }

            if (currentPlayer == player1.get()) {
                currentPlayer = player2.get();
            }
            else {
                currentPlayer = player1.get();
            }
        }
    }
};

int main() {
    Game game;
    game.run();
    return 0;
}