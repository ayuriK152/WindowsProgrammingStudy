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
	elements.clear();

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
	copy(&originMat[0][0], &originMat[0][0] + 20, &commandedMat[0][0]);

	cout << "행렬 생성:" << endl;
	for (int i = 0; i < 20; i++) {
		cout << originMat[i / 5][i % 5] << "\t";
		if ((i + 1) % 5 == 0)
			cout << endl;
	}

	sort(elements.begin(), elements.end());
}

void ResetChangedMat() {
	copy(&originMat[0][0], &originMat[0][0] + 20, &commandedMat[0][0]);

	cout << "Result:" << endl;
	for (int i = 0; i < 20; i++) {
		cout << commandedMat[i / 5][i % 5] << "\t";
		if ((i + 1) % 5 == 0)
			cout << endl;
	}
	isChanged = false;
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
		ResetChangedMat();
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
		ResetChangedMat();
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
		ResetChangedMat();
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
		ResetChangedMat();
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
		ResetChangedMat();
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
		ResetChangedMat();
	}
}

void SumColumnElements() {
	if (!isChanged) {
		int tempSum = 0;
		for (int i = 0; i < 20; i++) {
			tempSum += originMat[i / 5][i % 5];
			if ((i + 1) % 5 == 0) {
				commandedMat[i / 5][0] = tempSum;
				tempSum = 0;
			}
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
		ResetChangedMat();
	}
}

void MulRowElements() {
	if (!isChanged) {
		int tempMul = 1;
		for (int i = 0; i < 20; i++) {
			tempMul *= originMat[i % 4][i / 4];
			if ((i + 1) % 4 == 0) {
				commandedMat[3][i / 4] = tempMul;
				tempMul = 1;
			}
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
		ResetChangedMat();
	}
}

void ShuffleMat() {
	vector<int> randomIndex;
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> dis(0, 19);

	for (int i = 0; i < 20; i++) {
		originMat[i / 5][i % 5] = 0;
	}

	for (int i = 0; i < 20; i++) {
		int index = dis(gen);
		while (true) {
			if (originMat[index / 5][index % 5] != 0) {
				if (index == 19)
					index = 0;
				else
					index++;
			}
			else {
				originMat[index / 5][index % 5] = elements[i];
				break;
			}
		}
	}

	cout << "Result:" << endl;
	for (int i = 0; i < 20; i++) {
		cout << originMat[i / 5][i % 5] << "\t";
		if ((i + 1) % 5 == 0)
			cout << endl;
	}
}

int main() {
	bool loopFlag = true;
	while (true) {
		SetMatRandom();

		loopFlag = true;
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
				case 'p':
					SumColumnElements();
					break;
				case 'u':
					MulRowElements();
					break;
				case 'r':
					ShuffleMat();
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