#include <LastBossState.h>

LastBossState* LastBossState::GetInstance() {
	static LastBossState instance;
	return &instance;
}