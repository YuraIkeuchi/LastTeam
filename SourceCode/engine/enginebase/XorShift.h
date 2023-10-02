#pragma once
#include <stdint.h>
#include <stdio.h>
#include <time.h>
//XorShift‚ÌƒNƒ‰ƒX
class XorShift final{
public:
	static XorShift* GetInstance(){
		static XorShift instance;
		return &instance;
	};
	void initrand(uint32_t seed);
	double urand();
	void init_xor128(unsigned long s);
	unsigned long xor128();
private:
	static uint32_t xorsft_x;
	static uint32_t xorsft_y;
	static uint32_t xorsft_z;
	static uint32_t xorsft_w;
	unsigned long seed[4];
};

