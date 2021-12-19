#include "../../src/geometry/quad.h"
#include "../../src/mathHelper.h"
#include <iostream>

using namespace std;
using namespace pathTracer;

int main() {
	Quad* quad = new Quad({ 0, 0, 1 }, { 0, 1, 0 }, { 0, 2, -1 }, { 1, 0, 0 }, nullptr, { 0, 0, 0 });
	cout << "area=" << quad->area() << endl;
	cout << "sample 100000 points" << endl;
	float avg_pdf = 0;
	Vector3f avg_point = { 0, 0, 0 };
	for (int i = 0; i < 100000; ++i) {
		float pdf;
		Vector3f point = quad->sample_point(pdf);
		avg_point += point;
		avg_pdf += pdf;
	}
	avg_point /= 100000;
	avg_pdf /= 100000;
	cout << vector3fToString(avg_point) << endl;
	cout << avg_pdf << endl;

}