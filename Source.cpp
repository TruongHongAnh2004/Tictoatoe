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
				if (board[i][j] != ' ')
				{
					std::cout << board[i][j] << " | ";
				}
				else
				{
					std::cout << getIndex(i, j) << " | ";
				}
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
		board[row][col] = symbol;
	}

	char getCell(int row, int col) {
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
		if (board.getCell(row, col) != ' ') {
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
		std::cout << "Robot turn (" << symbol << ") " << std::endl;

		int minIndex = 1;
		int maxIndex = 9; // Inclusive upper bound

		// Initialize the random number generator with a random device seed
		std::random_device rd; // Obtain a random number from hardware
		std::mt19937 gen(rd()); // Seed the generator
		std::uniform_int_distribution<> dis(minIndex, maxIndex); // Define the range

		// Generate a random index
		int index = dis(gen);

		// Chuyển đổi số thành tọa độ hàng và cột
		int row = (index - 1) / 3;
		int col = (index - 1) % 3;

		// Kiểm tra nếu ô đã được đánh
		if (board.getCell(row, col) != ' ') {
			std::cout << "That spot is already taken, try again." << std::endl;

			// In ra bảng với ký tự $symbol trên ô đã được chọn
			board.displayBoard();

			return getEasyMove(board); // Gọi lại getMove() để người chơi nhập lại
		}

		// Đánh dấu ô với ký tự của người chơi
		board.setCell(row, col, symbol);
		board.displayBoard(); // In ra bảng với ký tự 'X' trên ô vừa được chọn

		return std::make_pair(row, col);
	}

	std::pair<int, int> getNormalMove(Board& board) {
    // Check for a winning move
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            if (board.getCell(row, col) == ' ') {
                board.setCell(row, col, symbol);
                if (board.checkWin(symbol)) {
                    return {row, col}; // Winning move found
                }
                board.setCell(row, col, ' '); // Reset the cell
            }
        }
    }

    // Block opponent's winning move
    char opponentSymbol = (symbol == 'X') ? 'O' : 'X';
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            if (board.getCell(row, col) == ' ') {
                board.setCell(row, col, opponentSymbol);
                if (board.checkWin(opponentSymbol)) {
                    board.setCell(row, col, symbol); // Block opponent
                    return {row, col};
                }
                board.setCell(row, col, ' '); // Reset the cell
            }
        }
    }

    // If no winning or blocking move, make a random move
    return getNormalMove(board);
}

	std::pair<int, int> getHardMove(Board& board) {
		// Check for a winning move
		for (int row = 0; row < 3; ++row) {
			for (int col = 0; col < 3; ++col) {
				if (board.getCell(row, col) == ' ') {
					board.setCell(row, col, symbol);
					if (board.checkWin(symbol)) {
						return { row, col }; // Winning move found
					}
					board.setCell(row, col, ' '); // Reset the cell
				}
			}
		}

		// Block opponent's winning move
		char opponentSymbol = (symbol == 'X') ? 'O' : 'X';
		for (int row = 0; row < 3; ++row) {
			for (int col = 0; col < 3; ++col) {
				if (board.getCell(row, col) == ' ') {
					board.setCell(row, col, opponentSymbol);
					if (board.checkWin(opponentSymbol)) {
						board.setCell(row, col, symbol); // Block opponent
						return { row, col };
					}
					board.setCell(row, col, ' '); // Reset the cell
				}
			}
		}

		// Check for a fork move (create two threats)
		for (int row = 0; row < 3; ++row) {
			for (int col = 0; col < 3; ++col) {
				if (board.getCell(row, col) == ' ') {
					board.setCell(row, col, symbol);
					int threatsCreated = 0;
					for (int i = 0; i < 3; ++i) {
						for (int j = 0; j < 3; ++j) {
							if (board.getCell(i, j) == ' ') {
								board.setCell(i, j, symbol);
								if (board.checkWin(symbol)) {
									++threatsCreated;
								}
								board.setCell(i, j, ' ');
							}
						}
					}
					board.setCell(row, col, ' ');
					if (threatsCreated >= 2) {
						return { row, col }; // Fork move found
					}
				}
			}
		}

		// Block opponent's fork move
		for (int row = 0; row < 3; ++row) {
			for (int col = 0; col < 3; ++col) {
				if (board.getCell(row, col) == ' ') {
					board.setCell(row, col, opponentSymbol);
					int threatsCreated = 0;
					for (int i = 0; i < 3; ++i) {
						for (int j = 0; j < 3; ++j) {
							if (board.getCell(i, j) == ' ') {
								board.setCell(i, j, opponentSymbol);
								if (board.checkWin(opponentSymbol)) {
									++threatsCreated;
								}
								board.setCell(i, j, ' ');
							}
						}
					}
					board.setCell(row, col, ' ');
					if (threatsCreated >= 2) {
						board.setCell(row, col, symbol); // Block opponent's fork
						return { row, col };
					}
				}
			}
		}

		// If no winning, blocking, or fork move, make a random move
		return getHardMove(board);
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

		board.displayBoard();		

		// Tiếp tục game
		while (true) {
			std::pair<int, int> move = currentPlayer->getMove(board);
		

			// Adjust the move to correspond to the correct position on the board
			int row = move.first;
			int col = move.second;


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