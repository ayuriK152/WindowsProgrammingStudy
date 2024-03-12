#include <iostream>
#include <vector>
#include <random>
using namespace std;

int originMat[4][5];
int commandedMat[4][5];
bool isChanged = false;
vector<int> elements;

void SetMatRandom() {
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> dis(1, 50);

	for (int i = 0; i < 20; i++) {
		int temp = dis(gen);
		bool containFlag = false;
		for (int j = 0; j < elements.size(); j++) {
			if (elements[j] == temp) {
				i--;
				containFlag = true;
				break;
			}
		}
		if (containFlag)
			continue;
		elements.push_back(temp);
		originMat[i / 5][i % 5] = temp;
	}

	cout << "행렬 생성:" << endl;
	for (int i = 0; i < 20; i++) {
		cout << originMat[i / 5][i % 5] << "\t";
		if ((i + 1) % 5 == 0)
			cout << endl;
	}

	sort(elements.begin(), elements.end());
}

void SortMatAscend() {
	if (!isChanged) {
		for (int i = 0; i < 20; i++) {
			commandedMat[i / 5][i % 5] = elements[i];
		}

		cout << "Result:" << endl;
		for (int i = 0; i < 20; i++) {
			cout << commandedMat[i / 5][i % 5] << "\t";
			if ((i + 1) % 5 == 0)
				cout << endl;
		}
		isChanged = true;
	}
	else {
		copy(&originMat[0][0], &originMat[0][0] + 20, &commandedMat[0][0]);

		cout << "Result:" << endl;
		for (int i = 0; i < 20; i++) {
			cout << commandedMat[i / 5][i % 5] << "\t";
			if ((i + 1) % 5 == 0)
				cout << endl;
		}
		isChanged = false;
	}
}

void SortMatDescend() {
	if (!isChanged) {
		for (int i = 19; i >= 0; i--) {
			commandedMat[-(i % 4 - 3)][i / 4] = elements[i];
		}

		cout << "Result:" << endl;
		for (int i = 0; i < 20; i++) {
			cout << commandedMat[i / 5][i % 5] << "\t";
			if ((i + 1) % 5 == 0)
				cout << endl;
		}
		isChanged = true;
	}
	else {
		copy(&originMat[0][0], &originMat[0][0] + 20, &commandedMat[0][0]);

		cout << "Result:" << endl;
		for (int i = 0; i < 20; i++) {
			cout << commandedMat[i / 5][i % 5] << "\t";
			if ((i + 1) % 5 == 0)
				cout << endl;
		}
		isChanged = false;
	}
}

void ShowEvenElements() {
	if (!isChanged) {
		for (int i = 0; i < 20; i++) {
			commandedMat[i / 5][i % 5] = originMat[i / 5][i % 5] % 2 == 0 ? originMat[i / 5][i % 5] : 0;
		}

		cout << "Result:" << endl;
		for (int i = 0; i < 20; i++) {
			cout << commandedMat[i / 5][i % 5] << "\t";
			if ((i + 1) % 5 == 0)
				cout << endl;
		}
		isChanged = true;
	}
	else {
		copy(&originMat[0][0], &originMat[0][0] + 20, &commandedMat[0][0]);

		cout << "Result:" << endl;
		for (int i = 0; i < 20; i++) {
			cout << commandedMat[i / 5][i % 5] << "\t";
			if ((i + 1) % 5 == 0)
				cout << endl;
		}
		isChanged = false;
	}
}

void ShowOddElements() {
	if (!isChanged) {
		for (int i = 0; i < 20; i++) {
			commandedMat[i / 5][i % 5] = originMat[i / 5][i % 5] % 2 != 0 ? originMat[i / 5][i % 5] : 0;
		}

		cout << "Result:" << endl;
		for (int i = 0; i < 20; i++) {
			cout << commandedMat[i / 5][i % 5] << "\t";
			if ((i + 1) % 5 == 0)
				cout << endl;
		}
		isChanged = true;
	}
	else {
		copy(&originMat[0][0], &originMat[0][0] + 20, &commandedMat[0][0]);

		cout << "Result:" << endl;
		for (int i = 0; i < 20; i++) {
			cout << commandedMat[i / 5][i % 5] << "\t";
			if ((i + 1) % 5 == 0)
				cout << endl;
		}
		isChanged = false;
	}
}

void SumMaxElement() {
	if (!isChanged) {
		for (int i = 0; i < 20; i++) {
			commandedMat[i / 5][i % 5] = originMat[i / 5][i % 5] + elements[elements.size() - 1];
		}

		cout << "Result:" << endl;
		for (int i = 0; i < 20; i++) {
			cout << commandedMat[i / 5][i % 5] << "\t";
			if ((i + 1) % 5 == 0)
				cout << endl;
		}
		isChanged = true;
	}
	else {
		copy(&originMat[0][0], &originMat[0][0] + 20, &commandedMat[0][0]);

		cout << "Result:" << endl;
		for (int i = 0; i < 20; i++) {
			cout << commandedMat[i / 5][i % 5] << "\t";
			if ((i + 1) % 5 == 0)
				cout << endl;
		}
		isChanged = false;
	}
}

void SubMinElement() {
	if (!isChanged) {
		for (int i = 0; i < 20; i++) {
			commandedMat[i / 5][i % 5] = originMat[i / 5][i % 5] - elements[0];
		}

		cout << "Result:" << endl;
		for (int i = 0; i < 20; i++) {
			cout << commandedMat[i / 5][i % 5] << "\t";
			if ((i + 1) % 5 == 0)
				cout << endl;
		}
		isChanged = true;
	}
	else {
		copy(&originMat[0][0], &originMat[0][0] + 20, &commandedMat[0][0]);

		cout << "Result:" << endl;
		for (int i = 0; i < 20; i++) {
			cout << commandedMat[i / 5][i % 5] << "\t";
			if ((i + 1) % 5 == 0)
				cout << endl;
		}
		isChanged = false;
	}
}

void SumColumnElements() {

}

int main() {
	while (true) {
		SetMatRandom();

		bool loopFlag = true;
		while (loopFlag) {
			char command;
			cout << endl << "명령어: ";
			cin >> command;

			switch (command) {
				case 'a':
					SortMatAscend();
					break;
				case 'd':
					SortMatDescend();
					break;
				case 'e':
					ShowEvenElements();
					break;
				case 'o':
					ShowOddElements();
					break;
				case 'm':
					SumMaxElement();
					break;
				case 'n':
					SubMinElement();
					break;
				case 's':
					loopFlag = false;
					break;
				case 'q':
					return 0;
					break;
			}
		}
	}

	return 0;
}