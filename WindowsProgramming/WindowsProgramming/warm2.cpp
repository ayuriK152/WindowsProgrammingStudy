#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
using namespace std;

vector<int> numberList;
vector<int> sortedList;

void ShowMultipleOfThree() {
	vector<int> tempList;
	int sum = 0;

	for (int i = 0; i < 50; i++) {
		if (numberList[i] % 3 == 0) {
			tempList.push_back(numberList[i]);
			sum += numberList[i];
		}
	}

	sort(tempList.begin(), tempList.end());

	cout << "Result: ";
	for (int i = 0; i < tempList.size(); i++) {
		cout << tempList[i] << " ";
	}
	cout << "-> " << sum << endl;
}

void ShowMultipleOfFive() {
	vector<int> tempList;
	int sum = 0;

	for (int i = 0; i < 50; i++) {
		if (numberList[i] % 5 == 0) {
			tempList.push_back(numberList[i]);
			sum += numberList[i];
		}
	}

	sort(tempList.begin(), tempList.end());

	cout << "Result: ";
	for (int i = 0; i < tempList.size(); i++) {
		cout << tempList[i] << " ";
	}
	cout << "-> " << sum << endl;
}

void ShowMultipleOfThreeOrFive() {
	vector<int> tempList;

	for (int i = 0; i < 50; i++) {
		if (numberList[i] % 3 == 0 || numberList[i] % 5 == 0) {
			tempList.push_back(numberList[i]);
		}
	}

	sort(tempList.begin(), tempList.end());

	cout << "Result: ";
	for (int i = 0; i < tempList.size(); i++) {
		cout << tempList[i] << " ";
	}
	cout << "-> ";
	for (int i = 0; i < tempList.size(); i++) {
		cout << tempList[i] + 1 << " ";
	}
	cout << endl;
}

void ShowMultipleOfFif() {
	vector<int> tempList;

	for (int i = 0; i < 50; i++) {
		if (numberList[i] % 15 == 0) {
			tempList.push_back(numberList[i]);
		}
	}

	sort(tempList.begin(), tempList.end());

	cout << "Result: ";
	for (int i = 0; i < tempList.size(); i++) {
		cout << tempList[i] << " ";
	}
	cout << "-> ";
	for (int i = 0; i < tempList.size(); i++) {
		cout << tempList[i] - 1 << " ";
	}
	cout << endl;
}

int main() {
	int input;

	while (true) {
		numberList.clear();
		sortedList.clear();

		cin >> input;
		if (input <= 100) {
			cout << "ERROR: 100보다 큰 수를 입력하세요." << endl;
			continue;
		}

		random_device rd;
		mt19937 gen(rd());
		uniform_int_distribution<int> dis(1, input);

		for (int i = 0; i < 50; i++) {
			int temp = dis(gen);
			bool containFlag = false;
			for (int j = 0; j < numberList.size(); j++) {
				if (numberList[j] == temp) {
					containFlag = true;
					break;
				}
			}
			if (containFlag) {
				i--;
				continue;
			}
			numberList.push_back(temp);
		}

		copy(numberList.begin(), numberList.end(), back_inserter(sortedList));

		cout << "Generated numbers: ";
		for (int i = 0; i < 50; i++) {
			cout << numberList[i] << " ";
		}
		cout << endl;

		bool loopFlag = true;
		while (loopFlag) {
			char command;
			cout << "Command: ";
			cin >> command;
			switch (command) {
				case 'a':
					sort(sortedList.begin(), sortedList.end());

					cout << "Result: ";
					for (int i = 0; i < 50; i++) {
						cout << sortedList[i] << " ";
					}
					cout << endl;
					break;
				case 'd':
					sort(sortedList.begin(), sortedList.end(), [](int a, int b) { return a > b; });

					cout << "Result: ";
					for (int i = 0; i < 50; i++) {
						cout << sortedList[i] << " ";
					}
					cout << endl;
					break;
				case 'e':
					ShowMultipleOfThree();
					break;
				case 'f':
					ShowMultipleOfFive();
					break;
				case'm':
					ShowMultipleOfThreeOrFive();
					break;
				case'n':
					ShowMultipleOfFif();
					break;
				case 'r':
					sortedList.clear();
					copy(numberList.begin(), numberList.end(), back_inserter(sortedList));

					cout << "Result: ";
					for (int i = 0; i < 50; i++) {
						cout << sortedList[i] << " ";
					}
					cout << endl;
					break;
				case 'p':
					loopFlag = false;
					break;
				case 'Q':
					return 0;
					break;
			}
		}
	}
}