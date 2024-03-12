#include <iostream>
#include <vector>
#include <random>
#include <Windows.h>
using namespace std;

int grid[10][10];
char shapes[5] = { 'o', 'x', '#', '@', '*' };

random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> dis(0, 4);

class Marker {
public:
	int x;
	int y;
	char shape;
	Marker() {};
	Marker(int x, int y, char shape){
		this->x = x;
		this->y = y;
		this->shape = shape;
	}
};

Marker playerA;
Marker playerB;

void UpdateGrid() {
	for (int i = 0; i < 10; i++) {
		cout << "---------------------" << endl;
		for (int j = 0; j < 10; j++) {
			cout << "|";
			if (grid[i][j] == 0) {
				cout << " ";
			}
			else if (grid[i][j] == 1 || grid[i][j] == 4) {
				cout << playerA.shape;
			}
			else if (grid[i][j] == 2 || grid[i][j] == 5) {
				cout << playerB.shape;
			}
		}
		cout << "|" << endl;
	}
	cout << "---------------------" << endl;
}

void MovePlayerA(char dir) {
	if (grid[playerA.y][playerA.x] >= 4) {
		grid[playerA.y][playerA.x] = 2;
		while (true) {
			playerA.shape = shapes[dis(gen)];
			if (playerA.shape != playerB.shape)
				break;
		}
	}
	else
		grid[playerA.y][playerA.x] = 0;

	switch (dir) {
		case 'w':
			playerA.y--;
			if (playerA.y < 0) {
				playerA.y = 9;
				while (true) {
					playerA.shape = shapes[dis(gen)];
					if (playerA.shape != playerB.shape)
						break;
				}
			}
			break;
		case 'a':
			playerA.x--;
			if (playerA.x < 0) {
				playerA.x = 9;
				while (true) {
					playerA.shape = shapes[dis(gen)];
					if (playerA.shape != playerB.shape)
						break;
				}
			}
			break;
		case 's':
			playerA.y++;
			if (playerA.y > 9) {
				playerA.y = 0;
				while (true) {
					playerA.shape = shapes[dis(gen)];
					if (playerA.shape != playerB.shape)
						break;
				}
			}
			break;
		case 'd':
			playerA.x++;
			if (playerA.x > 9) {
				playerA.x = 0;
				while (true) {
					playerA.shape = shapes[dis(gen)];
					if (playerA.shape != playerB.shape)
						break;
				}
			}
			break;
	}
	grid[playerA.y][playerA.x] += 1;
	if (grid[playerA.y][playerA.x] == 3) {
		grid[playerA.y][playerA.x] += 1;
		Beep(330, 500);
	}
}

void MovePlayerB(char dir) {
	if (grid[playerB.y][playerB.x] >= 4) {
		grid[playerB.y][playerB.x] = 1;
		while (true) {
			playerB.shape = shapes[dis(gen)];
			if (playerA.shape != playerB.shape)
				break;
		}
	}
	else
		grid[playerB.y][playerB.x] = 0;

	switch (dir) {
		case 'i':
			playerB.y--;
			if (playerB.y < 0) {
				playerB.y = 9;
				while (true) {
					playerB.shape = shapes[dis(gen)];
					if (playerA.shape != playerB.shape)
						break;
				}
			}
			break;
		case 'j':
			playerB.x--;
			if (playerB.x < 0) {
				playerB.x = 9;
				while (true) {
					playerB.shape = shapes[dis(gen)];
					if (playerA.shape != playerB.shape)
						break;
				}
			}
			break;
		case 'k':
			playerB.y++;
			if (playerB.y > 9) {
				playerB.y = 0;
				while (true) {
					playerB.shape = shapes[dis(gen)];
					if (playerA.shape != playerB.shape)
						break;
				}
			}
			break;
		case 'l':
			playerB.x++;
			if (playerB.x > 9) {
				playerB.x = 0;
				while (true) {
					playerB.shape = shapes[dis(gen)];
					if (playerA.shape != playerB.shape)
						break;
				}
			}
			break;
	}
	grid[playerB.y][playerB.x] += 2;
	if (grid[playerB.y][playerB.x] == 3) {
		grid[playerB.y][playerB.x] += 2;
		Beep(330, 500);
	}
}

int main() {
	bool loopFlag;

	while (true) {
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				grid[i][j] = 0;
			}
		}
		playerA = Marker(0, 4, shapes[dis(gen)]);
		while (true) {
			playerB = Marker(9, 5, shapes[dis(gen)]);
			if (playerA.shape != playerB.shape)
				break;
		}
		grid[4][0] = 1;
		grid[5][9] = 2;

		loopFlag = true;
		while (loopFlag) {
			UpdateGrid();
			char command;
			cout << "명령어: ";
			cin >> command;

			switch (command) {
				case 'w':
					MovePlayerA('w');
					break;
				case 'a':
					MovePlayerA('a');
					break;
				case 's':
					MovePlayerA('s');
					break;
				case 'd':
					MovePlayerA('d');
					break;
				case 'i':
					MovePlayerB('i');
					break;
				case 'j':
					MovePlayerB('j');
					break;
				case 'k':
					MovePlayerB('k');
					break;
				case 'l':
					MovePlayerB('l');
					break;
				case 'S':
					loopFlag = false;
					break;
				case 'q':
					return 0;
			}
		}
	}
}
