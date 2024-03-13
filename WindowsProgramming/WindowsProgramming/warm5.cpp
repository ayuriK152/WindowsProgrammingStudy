#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
using namespace std;

class Delegate {
public:
	int num;
	int classNum;
	int voteCnt;
	bool isVoted;
	Delegate(int num, int classNum, int voteCnt) {
		this->num = num;
		this->classNum = classNum;
		this->voteCnt = voteCnt;
		this->isVoted = false;
	}
};

int limit[13] = { 9, 12, 9, 13, 10, 6, 10, 6, 9, 5, 6, 7, 7 };
int delegateCnt[13];
vector<int> candidateCnt[13];
vector<Delegate> candidates;
random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> rndClass(0, 12);

void Reset() {
	for (int i = 0; i < 13; i++) {
		candidateCnt[i].clear();
		delegateCnt[i] = 0;
	}
	candidates.clear();
	for (int i = 0; i < 13; i++) {
		int tempIndex = rndClass(gen);
		while (delegateCnt[tempIndex] != 0) {
			tempIndex++;
			if (tempIndex >= 13)
				tempIndex = 0;
		}
		delegateCnt[tempIndex] = limit[i];
	}
}

void ShowAllAndVote() {
	uniform_int_distribution<int> rndVote(0, candidates.size() - 1);
	for (int i = 0; i < 1000; i++) {
		for (int j = 0; j < 109; j++) {
			int tempNum = rndVote(gen);
			while (candidates[tempNum].isVoted) {
				tempNum++;
				if (tempNum >= 109)
					tempNum = 0;
			}
			candidates[tempNum].voteCnt++;
			candidates[tempNum].isVoted = true;
		}
		for (int i = 0; i < candidates.size(); i++) {
			candidates[i].isVoted = false;
		}
	}

	for (int i = 0; i < 13; i++) {
		cout << i + 1 << "반 - " << candidateCnt[i].size() << "명\t";
		if ((i + 1) % 6 == 0 && i != 12)
			cout << endl;
	}
	cout << endl;
}

void ShowAllAsc() {
	cout << "---------------------------------------------------------------------" << endl;
	for (int i = 0; i < 13; i++) {
		sort(candidateCnt[i].begin(), candidateCnt[i].end(), [](int a, int b) { return candidates[a].voteCnt < candidates[b].voteCnt; });
		cout << i + 1 << "반:" << endl;
		for (int j = 0; j < candidateCnt[i].size(); j++) {
			cout << candidates[candidateCnt[i][j]].num + 1 << "번 - " << candidates[candidateCnt[i][j]].voteCnt << "표\t";
			if ((j + 1) % 6 == 0 && j != candidateCnt[i].size() - 1)
				cout << endl;
		}
		cout << endl << "---------------------------------------------------------------------" << endl;
	}
	cout << endl;
}

void ShowAllDsc() {
	cout << "---------------------------------------------------------------------" << endl;
	for (int i = 0; i < 13; i++) {
		sort(candidateCnt[i].begin(), candidateCnt[i].end(), [](int a, int b) { return candidates[a].voteCnt > candidates[b].voteCnt; });
		cout << i + 1 << "반:" << endl;
		for (int j = 0; j < candidateCnt[i].size(); j++) {
			cout << candidates[candidateCnt[i][j]].num + 1 << "번 - " << candidates[candidateCnt[i][j]].voteCnt << "표\t";
			if ((j + 1) % 6 == 0 && j != candidateCnt[i].size() - 1)
				cout << endl;
		}
		cout << endl << "---------------------------------------------------------------------" << endl;
	}
	cout << endl;
}

void JudgeMethodA() {
	for (int i = 0; i < candidates.size(); i++) {
		candidates[i].isVoted = false;
	}
	sort(candidates.begin(), candidates.end(), [](Delegate a, Delegate b) { return a.voteCnt > b.voteCnt; });

	int votedCnt = 0;
	for (int i = 0; i < 13; i++) {
		sort(candidateCnt[i].begin(), candidateCnt[i].end(), [](int a, int b) { return candidates[a].voteCnt > candidates[b].voteCnt; });
		for (int j = 0; j < candidateCnt[i].size(); j++) {
			candidates[candidateCnt[i][j]].isVoted = true;
			votedCnt++;
			if (j >= delegateCnt[i])
				break;
		}
	}

	for (int i = 0; i < candidates.size(); i++) {
		if (votedCnt >= 109)
			break;
		if (!candidates[i].isVoted) {
			votedCnt++;
			candidates[i].isVoted = true;
		}
	}

	cout << "---------------------------------------------------------------------" << endl;
	for (int i = 0; i < 13; i++) {
		sort(candidateCnt[i].begin(), candidateCnt[i].end(), [](int a, int b) { return candidates[a].voteCnt > candidates[b].voteCnt; });
		cout << i + 1 << "반:" << endl;

		int tempCnt = 0;
		for (int j = 0; j < candidateCnt[i].size(); j++) {
			if (candidates[candidateCnt[i][j]].isVoted) {
				cout << candidates[candidateCnt[i][j]].num + 1 << "번 - " << candidates[candidateCnt[i][j]].voteCnt << "표\t";
				tempCnt++;
				if (tempCnt % 6 == 0 && j != candidateCnt[i].size() - 1 && tempCnt > 0)
					cout << endl;
			}
		}
		cout << endl << "---------------------------------------------------------------------" << endl;
	}
	cout << endl;
}

void JudgeMethodB() {
	for (int i = 0; i < candidates.size(); i++) {
		candidates[i].isVoted = false;
	}
	sort(candidates.begin(), candidates.end(), [](Delegate a, Delegate b) { return a.voteCnt > b.voteCnt; });
	for (int i = 0; i < 109; i++) {
		cout << candidates[i].num + 1 << "번 - " << candidates[i].voteCnt << "표\t";
		if ((i + 1) % 6 == 0 && i != 108)
			cout << endl;
	}
}

int main() {
	int input;
	bool loopFlag;
	while (true) {
		Reset();

		cout << "대의원 후보자 숫자 입력: ";
		cin >> input;
		if (input < 150 || input > 250) {
			cout << "ERROR: 150~250 사이의 숫자를 입력해주세요." << endl << endl;
			continue;
		}
		cout << endl;

		for (int i = 0; i < input; i++) {
			candidates.push_back(Delegate(i, rndClass(gen), 0));
			candidateCnt[candidates[i].classNum].push_back(candidates[i].num);
			cout << i + 1 << "번 - " << candidates[i].classNum + 1 << "반\t";
			if ((i + 1) % 6 == 0 && i != input - 1)
				cout << endl;
		}

		loopFlag = true;
		while (loopFlag) {
			cout << endl;
			char command;
			cout << "명령어: ";
			cin >> command;

			switch (command) {
				case 'p':
					ShowAllAndVote();
					break;
				case 'a':
					ShowAllAsc();
					break;
				case 'd':
					ShowAllDsc();
					break;
				case 'm':
					JudgeMethodA();
					break;
				case 'n':
					JudgeMethodB();
					break;
				case 'r':
					loopFlag = false;
					break;
				case 'q':
					return 0;
					break;
			}
		}
	}
}