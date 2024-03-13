#include <iostream>
#include <random>
using namespace std;

class Player {
public:
	int x;
	int y;
	bool isInit;
	Player() { }
	Player(int x, int y) {
		this->x = x;
		this->y = y;
		isInit = true;
	}
};

int playBoard[11][11];
Player players[2];
random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> randomDice(1, 5);

void SetBoard() {
	for (int i = 0; i < 121; i++) {
		playBoard[i / 11][i % 11] = -1;
	}
	playBoard[0][0] = 0;
	playBoard[0][2] = 0;
	playBoard[0][4] = 0;
	playBoard[0][6] = 0;
	playBoard[0][8] = 0;
	playBoard[0][10] = 0;
	playBoard[1][1] = 0;
	playBoard[1][9] = 0;
	playBoard[2][0] = 0;
	playBoard[2][10] = 0;
	playBoard[3][3] = 0;
	playBoard[3][7] = 0;
	playBoard[4][0] = 0;
	playBoard[4][10] = 0;
	playBoard[5][5] = 0;
	playBoard[6][0] = 0;
	playBoard[6][10] = 0;
	playBoard[7][3] = 0;
	playBoard[7][7] = 0;
	playBoard[8][0] = 0;
	playBoard[8][10] = 0;
	playBoard[9][1] = 0;
	playBoard[9][9] = 0;
	playBoard[10][0] = 0;
	playBoard[10][2] = 0;
	playBoard[10][4] = 0;
	playBoard[10][6] = 0;
	playBoard[10][8] = 0;
	playBoard[10][10] = 0;
}

void DrawCurrentBoard() {
	for (int i = 120; i >= 0; i--) {
		if (playBoard[i % 11][i / 11] == -1) {
			cout << "  ";
		}
		else if (playBoard[i % 11][i / 11] == 0) {
			cout << "■ ";
		}
		else if (playBoard[i % 11][i / 11] == 1) {
			cout << "◎ ";
		}
		else if (playBoard[i % 11][i / 11] == 2) {
			cout << "※ ";
		}
		if (i % 11 == 0) {
			cout << endl;
		}
	}
}

void Move(bool isFirstPlayer) {
	int playerNum = isFirstPlayer ? 0 : 1;
	int moveMount = randomDice(gen);
	switch (moveMount) {
		case 1:
			cout << endl << "도!" << endl << endl;
			break;
		case 2:
			cout << endl << "개!!" << endl << endl;
			break;
		case 3:
			cout << endl << "걸!!!" << endl << endl;
			break;
		case 4:
			cout << endl << "윷!!!!" << endl << endl;
			break;
		case 5:
			cout << endl << "모!!!!!" << endl << endl;
			break;
	}
	moveMount *= 2;

	bool loopFlag = true;
	bool keepGo = false;
	while (loopFlag) {
		char moveCmd;
		cout << "움직일 방향 입력: ";
		cin >> moveCmd;
		switch (moveCmd) {
			case 'w':
				if ((players[playerNum].isInit) || (players[playerNum].x == 0 && players[playerNum].y > 0 && players[playerNum].y < 10)) {
					int movable = 10 - players[playerNum].y;
					playBoard[players[playerNum].x][players[playerNum].y] = 0;
					if (movable >= moveMount) {
						players[playerNum].y += moveMount;
						moveMount = 0;
					}
					else {
						players[playerNum].y += movable;
						moveMount -= movable;
					}
					playBoard[players[playerNum].x][players[playerNum].y] = playerNum + 1;

					players[playerNum].isInit = false;
					if (moveMount == 0)
						loopFlag = false;
					else {
						cout << "여유 이동 횟수가 있습니다!" << endl;
						keepGo = true;
					}
				}
				else {
					cout << "ERROR: 위로 이동할 수 없습니다!" << endl;
				}
				break;
			case 's':
				if (players[playerNum].x == 10 && players[playerNum].y > 0) {
					playBoard[players[playerNum].x][players[playerNum].y] = 0;
					players[playerNum].y -= 2;
					playBoard[players[playerNum].x][players[playerNum].y] = playerNum + 1;

					loopFlag = false;
				}
				else {
					cout << "ERROR: 아래로 이동할 수 없습니다!" << endl;
				}
				break;
			case 'a':
				if (players[playerNum].x < 10 && players[playerNum].y == 10) {
					playBoard[players[playerNum].x][players[playerNum].y] = 0;
					players[playerNum].x += 2;
					playBoard[players[playerNum].x][players[playerNum].y] = playerNum + 1;

					loopFlag = false;
				}
				else {
					cout << "ERROR: 왼쪽으로 이동할 수 없습니다!" << endl;
				}
				break;
			case 'd':
				if (players[playerNum].x > 0 && players[playerNum].y == 0) {
					playBoard[players[playerNum].x][players[playerNum].y] = 0;
					players[playerNum].x -= 2;
					playBoard[players[playerNum].x][players[playerNum].y] = playerNum + 1;

					loopFlag = false;
				}
				else {
					cout << "ERROR: 오른쪽으로 이동할 수 없습니다!" << endl;
				}
				break;
			case 'i':
				break;
			case 'k':
				break;
			case 'j':
				break;
			case 'l':
				break;
		}
	}
}

void Game() {
	system("cls");

	players[0] = Player(0, 0);
	players[1] = Player(0, 0);
	bool isFirstPlayer = true;
	SetBoard();
	string gameStatus = "게임 시작!";
	while (true) {
		cout << gameStatus << endl << endl;
		gameStatus.clear();
		if (isFirstPlayer) {
			cout << "1번 플레이어가 윷을 던질 차례입니다." << endl << endl;
		}
		else {
			cout << "2번 플레이어가 윷을 던질 차례입니다." << endl << endl;
		}
		DrawCurrentBoard();
		cout << endl;
		char gameCmd;
		cout << "명령어: ";
		cin >> gameCmd;
		switch (gameCmd) {
			case '1':
				if (!isFirstPlayer) {
					gameStatus = "ERROR: 2번 플레이어의 차례에 1번 플레이어가 윷을 던지려고 했습니다.";
				}
				Move(true);
				isFirstPlayer = false;
				break;
			case '2':
				if (isFirstPlayer) {
					gameStatus = "ERROR: 1번 플레이어의 차례에 2번 플레이어가 윷을 던지려고 했습니다.";
				}
				Move(false);
				isFirstPlayer = true;
				break;
		}
		system("cls");
	}
}

int main() {
	while (true) {
		char cmd;
		cout << "명령어: ";
		cin >> cmd;

		switch (cmd) {
			case 'p':
				Game();
				break;
			case 'q':
				return 0;
				break;
			default:
				cout << "ERROR: 올바르지 않은 명령어입니다." << endl;
				break;
		}
	}
}