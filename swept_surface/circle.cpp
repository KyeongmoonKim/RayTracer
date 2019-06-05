#include<iostream>
#include<math.h>
#define PI 3.14159265

using namespace std;

int main() {
	int r;
	int n;
	cin >> r;
	cin >> n;
	double d = 2 * PI / n;
	double t = 0.0;
	for(int i = 0; i < n; i++) {
		cout << r*cos(t) << " " << r*sin(t) << endl;
		t += d;
	}
}
