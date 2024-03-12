#include <iostream>
#include <vector>
#include <string>
using namespace std;

vector<float> nums;
vector<char> operators;

void CalcNormal() {
	string resultStr = "";
	float result;
	vector<float> numsC = vector<float>(nums);
	vector<char> operatorsC = vector<char>(operators);

	int isPackaged = 0;
	for (int i = 0; i < operatorsC.size(); i++) {
		if (operatorsC[i] == '*') {
			if (isPackaged > 0 || i == 0) {
				resultStr.insert(resultStr.begin() + isPackaged, '(');
			}
			else {
				resultStr.push_back('(');
			}
			resultStr.push_back('*');
			resultStr.push_back(')');

			numsC[i] = numsC[i] * numsC[i + 1];
			numsC.erase(numsC.begin() + i + 1);
			operatorsC.erase(operatorsC.begin() + i);

			i--;
			isPackaged++;
		}
		else if (operatorsC[i] == '/') {
			if (isPackaged > 0 || i == 0) {
				resultStr.insert(resultStr.begin() + isPackaged, '(');
			}
			else {
				resultStr.push_back('(');
			}
			resultStr.push_back('/');
			resultStr.push_back(')');

			if (numsC[i + 1] == 0) {
				cout << endl << "ERROR: 0으로 나눌 수 없습니다!" << endl;
				return;
			}

			numsC[i] = numsC[i] / numsC[i + 1];
			numsC.erase(numsC.begin() + i + 1);
			operatorsC.erase(operatorsC.begin() + i);

			i--;
			isPackaged++;
		}
		else {
			resultStr.push_back(operatorsC[i]);
			isPackaged = 0;
		}
	}

	result = numsC[0];
	for (int i = 0; i < operatorsC.size(); i++) {
		if (operatorsC[i] == '+') {
			result += numsC[i + 1];
		}
		else {
			result -= numsC[i + 1];
		}
	}

	cout << "Result: ";
	int numsIndex = 0;
	bool isFrontEmpty = true;
	for (int i = 0; i < resultStr.size(); i++) {
		if (resultStr[i] != '(' && resultStr[i] != ')') {
			if (numsIndex == 0 || isFrontEmpty) {
				cout << to_string((int)nums[numsIndex++]);
				cout << resultStr[i];
				while (resultStr[++i] == '(') {
					cout << resultStr[i];
				}
				i--;
				cout << to_string((int)nums[numsIndex++]);
				isFrontEmpty = false;
			}
			else if (i > 0 && i < resultStr.size() - 1) {
				if (resultStr[i + 1] == '(') {
					cout << resultStr[i];
					isFrontEmpty = true;
				}
				else {
					cout << resultStr[i];
					cout << to_string((int)nums[numsIndex++]);
				}
			}
			else {
				cout << resultStr[i];
				cout << to_string((int)nums[numsIndex++]);
			}
		}
		else
			cout << resultStr[i];
	}
	cout << " = " << result << endl;
}

void CalcFromFront() {
	float result = nums[0];
	for (int i = 0; i < 4; i++) {
		switch (operators[i]) {
			case '+':
				result += nums[i + 1];
				break;
			case '-':
				result -= nums[i + 1];
				break;
			case '*':
				result *= nums[i + 1];
				break;
			case '/':
				if (nums[i + 1] == 0) {
					cout << endl << "ERROR: 0으로 나눌 수 없습니다!" << endl;
					return;
				}
				result /= nums[i + 1];
				break;
		}
	}

	cout << "Result: (((";
	for (int i = 0; i < 9; i++) {
		if (i % 2 != 0) {
			cout << operators[i / 2];
		}
		else if (i % 2 == 0) {
			cout << nums[i / 2];
			if (!(i / 2 == 0 || i / 2 == 4))
				cout << ')';
		}
	}
	cout << " = " << result << endl;
}

void CalcFromBack() {
	float result = nums[4];
	for (int i = 3; i >= 0; i--) {
		switch (operators[i]) {
		case '+':
			result += nums[i];
			break;
		case '-':
			result = nums[i] - result;
			break;
		case '*':
			result *= nums[i];
			break;
		case '/':
			if (nums[i + 1] == 0) {
				cout << endl << "ERROR: 0으로 나눌 수 없습니다!" << endl;
				return;
			}
			result = nums[i] / result;
			break;
		}
	}

	cout << "Result: ";
	for (int i = 0; i < 9; i++) {
		if (i % 2 != 0) {
			cout << operators[i / 2];
		}
		else if (i % 2 == 0) {
			if (!(i / 2 == 0 || i / 2 == 4))
				cout << '(';
			cout << nums[i / 2];
		}
	}
	cout << "))) = " << result << endl;
}

int main() {
	bool loopFlag;
	while (true) {
		nums.clear();
		operators.clear();
		char input[100];
		cout << "식 입력: ";
		cin >> input;
		
		int temp = 0;
		for (int i = 0; ; i++) {
			if (input[i] == '\0') {
				nums.push_back(temp);
				break;
			}
			else if (input[i] >= 48 && input[i] <= 57) {
				temp *= 10;
				temp += input[i] -= 48;
			}
			else {
				nums.push_back(temp);
				operators.push_back(input[i]);
				temp = 0;
			}
		}

		loopFlag = true;
		while (loopFlag) {
			char command;
			cout << "명령어 입력: ";
			cin >> command;
			switch (command) {
				case '1':
					CalcNormal();
					break;
				case '2':
					CalcFromFront();
					break;
				case '3':
					CalcFromBack();
					break;
				case '4':
					loopFlag = false;
					break;
				case 'q':
					return 0;
					break;
			}
		}
	}
}
