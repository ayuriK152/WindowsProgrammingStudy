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
			cout << "   ";
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

bool CornerJudge(int x, int y) {
	if ((x == 0 && (y == 0 || y == 10)) || x == 10 && (y == 0 || y == 10))
		return true;
	return false;
}

string ChangeMoveResultToString(int moveResult) {
	switch (moveResult) {
	case 1:
		return "도!";
	case 2:
		return "개!!";
	case 3:
		return "걸!!!";
	case 4:
		return "윷!!!!";
	case 5:
		return "모!!!!!";
	}
}

int Move(bool isFirstPlayer) {
	int playerNum = isFirstPlayer ? 0 : 1;
	int moveMount = randomDice(gen);
	//이동 후 게임 결과. 0=이동만 함, 1=1번 플레이어 골, 2=2번 플레이어 골, 3=잡음, 1x~5x=도/개/걸/윷/모
	int moveResult = 0 + moveMount * 10;
	cout << ChangeMoveResultToString(moveResult / 10) << endl;
	moveMount *= 2;

	bool loopFlag = true;
	bool keepGo = false;
	bool diagonalMove = false;
	while (loopFlag) {
		if (!keepGo && (players[playerNum].y == 10 && (players[playerNum].x == 0 || players[playerNum].x == 10))) {
			bool iscornerSelected = false;
			char cornerSelect = ' ';
			while (!iscornerSelected) {
				cout << "대각선으로 이동 가능합니다! 대각선 이동하시겠습니까? (y / n): ";
				cin >> cornerSelect;
				switch (cornerSelect) {
				case 'y':
					diagonalMove = true;
					iscornerSelected = true;
					break;
				case 'n':
					iscornerSelected = true;
					break;
				default:
					cout << "ERROR: 잘못된 입력" << endl;
					break;
				}
			}
		}

		keepGo = false;
		//상하좌우 이동 로직
		if ((players[playerNum].isInit) || (players[playerNum].x == 0 && players[playerNum].y > 0 && players[playerNum].y < 10 && players[playerNum].y % 2 == 0)) {
			int movable = 10 - players[playerNum].y;

			if (playBoard[players[playerNum].x][players[playerNum].y] == 3) {
				playBoard[players[playerNum].x][players[playerNum].y] = 3 - playerNum - 1;
			}
			else {
				playBoard[players[playerNum].x][players[playerNum].y] = 0;
			}

			if (movable >= moveMount) {
				players[playerNum].y += moveMount;
				moveMount = 0;
			}
			else {
				players[playerNum].y += movable;
				moveMount -= movable;
			}
		}
		else if (players[playerNum].x == 10 && players[playerNum].y > 0 && players[playerNum].y % 2 == 0 && !diagonalMove) {
			int movable = players[playerNum].y;

			if (playBoard[players[playerNum].x][players[playerNum].y] == 3) {
				playBoard[players[playerNum].x][players[playerNum].y] = 3 - playerNum - 1;
			}
			else {
				playBoard[players[playerNum].x][players[playerNum].y] = 0;
			}

			if (movable >= moveMount) {
				players[playerNum].y -= moveMount;
				moveMount = 0;
			}
			else {
				players[playerNum].y -= movable;
				moveMount -= movable;
			}
		}
		else if (players[playerNum].x < 10 && players[playerNum].y == 10 && players[playerNum].x % 2 == 0 && !diagonalMove) {
			int movable = 10 - players[playerNum].x;

			if (playBoard[players[playerNum].x][players[playerNum].y] == 3) {
				playBoard[players[playerNum].x][players[playerNum].y] = 3 - playerNum - 1;
			}
			else {
				playBoard[players[playerNum].x][players[playerNum].y] = 0;
			}

			if (movable >= moveMount) {
				players[playerNum].x += moveMount;
				moveMount = 0;
			}
			else {
				players[playerNum].x += movable;
				moveMount -= movable;
			}
		}
		else if (players[playerNum].y == 0 && players[playerNum].x % 2 == 0 && !diagonalMove) {
			int movable = players[playerNum].x;

			if (playBoard[players[playerNum].x][players[playerNum].y] == 3) {
				playBoard[players[playerNum].x][players[playerNum].y] = 3 - playerNum - 1;
			}
			else {
				playBoard[players[playerNum].x][players[playerNum].y] = 0;
			}

			if (movable >= moveMount) {
				players[playerNum].x -= moveMount;
				moveMount = 0;
			}
			else {
				playBoard[0][0] = playerNum + 1;
				moveResult += playerNum + 1;
				break;
			}
		}

		// 대각선 이동 로직
		else if (players[playerNum].x == players[playerNum].y) {
			int movable = players[playerNum].x;

			playBoard[players[playerNum].x][players[playerNum].y] = 0;

			if (CornerJudge(players[playerNum].x, players[playerNum].y) || players[playerNum].x == 1) {
				moveMount -= 2;
				movable -= 1;
				players[playerNum].x -= 1;
				players[playerNum].y -= 1;
				if (moveMount > 0)
					keepGo = true;
			}
			if (movable > 0) {
				if (movable > moveMount) {
					players[playerNum].x -= moveMount;
					players[playerNum].y -= moveMount;
					moveMount = 0;
				}
				else {
					playBoard[0][0] = playerNum + 1;
					moveResult += playerNum + 1;
					break;
				}
			}
		}

		else if (players[playerNum].x + players[playerNum].y == 10) {
			int movable = 10 - players[playerNum].x;

			playBoard[players[playerNum].x][players[playerNum].y] = 0;

			if (CornerJudge(players[playerNum].x, players[playerNum].y) || players[playerNum].x == 9) {
				moveMount -= 2;
				movable -= 1;
				players[playerNum].x += 1;
				players[playerNum].y -= 1;
				if (moveMount > 0)
					keepGo = true;
			}
			if (movable > 0) {
				if (movable > moveMount) {
					players[playerNum].x += moveMount;
					players[playerNum].y -= moveMount;
					moveMount = 0;
				}
				else {
					playBoard[0][0] = playerNum + 1;
					moveResult += playerNum + 1;
					break;
				}
			}
		}

		players[playerNum].isInit = false;
		if (moveMount == 0) {
			loopFlag = false;
		}
		else {
			keepGo = true;
		}

		if (playBoard[players[playerNum].x][players[playerNum].y] != 0) {
			if (keepGo) {
				playBoard[players[playerNum].x][players[playerNum].y] = 3;
			}
			else {
				playBoard[players[playerNum].x][players[playerNum].y] = playerNum + 1;
				players[(playerNum + 1) % 2].isInit = true;
				players[(playerNum + 1) % 2].x = 0;
				players[(playerNum + 1) % 2].y = 0;
				moveResult += 3;
			}
		}
		else {
			playBoard[players[playerNum].x][players[playerNum].y] = playerNum + 1;
		}
	}
	return moveResult;
}

void Game() {
	system("cls");

	players[0] = Player(0, 0);
	players[1] = Player(0, 0);
	bool isFirstPlayer = true;
	bool isGameEnd = false;
	int moveResult = 0;
	string moveResultStr = "";
	string gameStatus = "게임 시작!";

	SetBoard();
	while (true) {
		cout << moveResultStr << endl;
		cout << gameStatus << endl << endl;
		gameStatus.clear();
		if (isGameEnd) {
			cout << "아무 버튼이나 입력해서 메뉴로 돌아가세요." << endl << endl;
		}
		else {
			if (isFirstPlayer) {
				cout << "1번 플레이어가 윷을 던질 차례입니다." << endl << endl;
			}
			else {
				cout << "2번 플레이어가 윷을 던질 차례입니다." << endl << endl;
			}
		}
		DrawCurrentBoard();
		cout << endl;
		char gameCmd;
		cout << "명령어: ";
		cin >> gameCmd;
		switch (gameCmd) {
		case '1':
			if (isGameEnd)
				return;
			if (!isFirstPlayer) {
				gameStatus = "ERROR: 2번 플레이어의 차례에 1번 플레이어가 윷을 던지려고 했습니다.";
				break;
			}
			moveResult = Move(true);
			moveResultStr = ChangeMoveResultToString(moveResult / 10);
			if (moveResult % 10 == 1) {
				gameStatus = "게임 끝! 1번 플레이어가 한 바퀴를 돌았습니다!";
				isGameEnd = true;
				break;
			}
			else if (moveResult % 10 == 3) {
				gameStatus = "1번 플레이어가 2번 플레이어를 잡았습니다!";
				break;
			}
			isFirstPlayer = false;
			break;
		case '2':
			if (isGameEnd)
				return;
			if (isFirstPlayer) {
				gameStatus = "ERROR: 1번 플레이어의 차례에 2번 플레이어가 윷을 던지려고 했습니다.";
				break;
			}
			moveResult = Move(false);
			moveResultStr = ChangeMoveResultToString(moveResult / 10);
			if (moveResult % 10 == 2) {
				gameStatus = "게임 끝! 2번 플레이어가 한 바퀴를 돌았습니다!";
				isGameEnd = true;
				break;
			}
			else if (moveResult % 10 == 3) {
				gameStatus = "2번 플레이어가 1번 플레이어를 잡았습니다!";
				break;
			}
			isFirstPlayer = true;
			break;
		default:
			if (isGameEnd)
				return;
			break;
		}
		system("cls");
	}
}

int main() {
	while (true) {
		system("cls");
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