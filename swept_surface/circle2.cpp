#include<iostream>
#include<math.h>
#define PI 3.14159265

using namespace std;

int main() {
	int n;
	int a;
	int b;
	cin >> n;
	cin >> a;
	cin >> b;
	double d = 2 * PI / n;
	double t = 0.0;
	for(int i = 0; i < n; i++) {
		cout << a*cos(t) << " " << b*sin(t) << endl;
		t += d;
	}
}
