#include <iostream>
#include <algorithm>
#include <string>
using namespace std;

class Sphere {
public:
	string name;
	float radius;
	int satellite;
	float revolutionPeriod;
	float rotationPeriod;
	float revolutionSpeed;
	float tilt;
	float distanceToSun;
	int number;
	float distanceToForward;
	Sphere() { };
	Sphere(string name, float radius, int satellite, float revolutionPeriod, float rotationPeriod, float revolutionSpeed, float tilt, float distanceToSun, float distanceToForward) {
		this->name = name;
		this->radius = radius;
		this->satellite = satellite;
		this->revolutionPeriod = revolutionPeriod;
		this->rotationPeriod = rotationPeriod;
		this->revolutionSpeed = revolutionSpeed;
		this->tilt = tilt;
		this->distanceToSun = distanceToSun;
		this->distanceToForward = distanceToForward;
	}
};

Sphere solarSystem[8];

void Init() {
	solarSystem[0] = Sphere("수성", 0.383f, 0, 87.9691f, 58.646f, 47.36f, 0.0352f, 0.3871f, 0.3871f);
	solarSystem[1] = Sphere("금성", 0.95f, 0, 224.7f, 243.0158f, 35.020f, 177.3f, 0.7233f, 0.3362f);
	solarSystem[2] = Sphere("지구", 1, 1, 365.2564f, 0.9972f, 29.76f, 23.439f, 1, 0.2767f);
	solarSystem[3] = Sphere("화성", 0.532f, 2, 686.971f, 1.0259f, 24.077f, 25.19f, 1.6660f, 0.6660f);
	solarSystem[4] = Sphere("목성", 10.97f, 95, 88, 4332.59f, 13.05624f, 0.4135f, 5.2026f, 3.5366f);
	solarSystem[5] = Sphere("토성", 9.14f, 146, 243, 10759.22, 9.639f, 0.4443f, 9.5549f, 4.3523f);
	solarSystem[6] = Sphere("천왕성", 3.98f, 28, 88, 30688.5, 6.795f, 0.7183f, 19.2184f, 9.6635f);
	solarSystem[7] = Sphere("혜왕성", 3.87f, 16, 243, 60182, 5.43f, 0.6713f, 30.0689f, 10.8505f);
}

void ShowAll() {
	cout << "행성 이름\t반지름\t위성 개수\t공전 주기\t자전 주기\t공전 속도\t기울기\t태양과의 거리" << endl;
	cout << "-------------------------------------------------------------------------------------------------------------" << endl;
	for (int i = 0; i < 8; i++) {
		cout << solarSystem[i].name << "\t\t" << solarSystem[i].radius << "\t" << solarSystem[i].satellite << "\t\t" << solarSystem[i].revolutionPeriod << "\t\t" << solarSystem[i].rotationPeriod << "\t\t" << solarSystem[i].revolutionSpeed << "km/s\t" << solarSystem[i].tilt << "\t" << solarSystem[i].distanceToSun << endl;
	}
	cout << "-------------------------------------------------------------------------------------------------------------" << endl << endl;
}

void SortByRevolutionSpeed() {
	sort(solarSystem, solarSystem + 8, [](Sphere a, Sphere b) { return a.revolutionSpeed < b.revolutionSpeed; });
	ShowAll();
}

void SortByRadius() {
	sort(solarSystem, solarSystem + 8, [](Sphere a, Sphere b) { return a.radius > b.radius; });
	ShowAll();
}

void SortByDistanceForward() {
	sort(solarSystem, solarSystem + 8, [](Sphere a, Sphere b) { return a.distanceToForward > b.distanceToForward; });
	ShowAll();
}

void ChangePeriodToHour() {
	cout << "행성 이름\t반지름\t위성 개수\t공전 주기\t자전 주기\t공전 속도\t기울기\t태양과의 거리" << endl;
	cout << "-------------------------------------------------------------------------------------------------------------" << endl;
	for (int i = 0; i < 8; i++) {
		string parse = to_string(solarSystem[i].rotationPeriod * 24);
		cout << solarSystem[i].name << "\t\t" << solarSystem[i].radius << "\t" << solarSystem[i].satellite << "\t\t" << solarSystem[i].revolutionPeriod << "\t\t" << parse.substr(0, parse.size() - 4) << "시간\t" << solarSystem[i].revolutionSpeed << "km/s\t\t" << solarSystem[i].tilt << "\t" << solarSystem[i].distanceToSun << endl;
	}
	cout << "-------------------------------------------------------------------------------------------------------------" << endl << endl;
}

void SortByTilt() {
	sort(solarSystem, solarSystem + 8, [](Sphere a, Sphere b) { return a.tilt < b.tilt; });
	ShowAll();
}

int main() {
	Init();
	while (true) {
		char input;
		cout << "명령어: ";
		cin >> input;

		switch (input) {
			case 'p':
				ShowAll();
				break;
			case 'a':
				SortByRevolutionSpeed();
				break;
			case 'b':
				SortByRadius();
				break;
			case 'c':
				SortByDistanceForward();
				break;
			case 'd':
				ChangePeriodToHour();
				break;
			case 'e':
				SortByTilt();
				break;
			case 'q':
				return 0;
				break;
		}
	}
}