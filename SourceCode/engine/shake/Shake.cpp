#include "Shake.h"
#include <random>
using namespace std;
void Shake::ShakePos(float& pos, const int ShakeMax, const int ShakeMin, const int TargetTimer, const int Division) {

	if (m_ShakeStart) {
		if (m_ShakeTimer < TargetTimer) {
			//—”Žw’è
			mt19937 mt{ std::random_device{}() };
			uniform_int_distribution<int> l_dist(ShakeMin, ShakeMax);
			pos = float(l_dist(mt)) / Division;
			m_ShakeTimer++;
		} else {

			m_ShakeTimer = 0;
			pos = 0.0f;
			m_ShakeStart = false;
		}
	}
}

void Shake::ShakePos(float& pos, const int ShakeMax, const int ShakeMin, const int TargetTimer, const float Division) {

	if (m_ShakeStart) {
		if (m_ShakeTimer < TargetTimer) {
			//—”Žw’è
			mt19937 mt{ std::random_device{}() };
			uniform_int_distribution<int> l_dist(ShakeMin, ShakeMax);
			pos = float(l_dist(mt)) / Division;
			m_ShakeTimer++;
		} else {
			pos = 0.0f;
			m_ShakeStart = false;
		}
	}
}