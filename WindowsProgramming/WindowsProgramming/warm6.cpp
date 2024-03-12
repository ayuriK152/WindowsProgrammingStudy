#include <iostream>
#include <vector>
using namespace std;

vector<float> nums;
vector<char> operators;

void CalcNormal() {
	vector<float> numsC;
	vector<char> operatorsC;

	float result = 0;
	bool isChecked = false;

	for (int i = 0; i < 4; i++) {
		if (operators[i] == '*' || operators[i] == '/') {
			int j = 0;
			for (; ; j++) {
				if (!(operatorsC[j] == '*' || operatorsC[j] == '/'))
					break;
			}
			operatorsC.insert(operatorsC.begin() + j, operators[i]);
			if (!isChecked) {
				numsC.insert(numsC.begin(), nums[i]);
				numsC.insert(numsC.begin() + 1, nums[i + 1]);
				isChecked = true;
			}
			else {
				numsC.insert(numsC.begin() + j + 1, nums[i + 1]);
			}
		}
		else {
			operatorsC.push_back(operators[i]);
			if (!isChecked) {
				numsC.push_back(nums[i]);
			}
			else {
				numsC.push_back(nums[i + 1]);
			}
		}
	}
	if (!isChecked) {
		numsC.push_back(nums[4]);
	}

	result = numsC[0];
	for (int i = 0; i < 4; i++) {
		switch (operatorsC[i]) {
		case '+':
			result += numsC[i + 1];
			break;
		case '-':
			result -= numsC[i + 1];
			break;
		case '*':
			result *= numsC[i + 1];
			break;
		case '/':
			result /= numsC[i + 1];
			break;
		}
	}
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
