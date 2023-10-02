#include "XorShift.h"
#include <cmath>
uint32_t XorShift::xorsft_x = 123456789;
uint32_t XorShift::xorsft_y = 362436069;
uint32_t XorShift::xorsft_z = 521288629;
uint32_t XorShift::xorsft_w = 88675123;
static unsigned long seed[4] = { 123456789, 362436069, 521288629, 88675123 };


void XorShift::initrand(uint32_t seed) {
    do {
        seed = seed * 1812433253 + 1; seed ^= seed << 13; seed ^= seed >> 17;
        xorsft_x = 123464980 ^ seed;
        seed = seed * 1812433253 + 1; seed ^= seed << 13; seed ^= seed >> 17;
        xorsft_y = 3447902351 ^ seed;
        seed = seed * 1812433253 + 1; seed ^= seed << 13; seed ^= seed >> 17;
        xorsft_z = 2859490775 ^ seed;
        seed = seed * 1812433253 + 1; seed ^= seed << 13; seed ^= seed >> 17;
        xorsft_w = 47621719 ^ seed;
    } while (xorsft_x == 0 && xorsft_y == 0 && xorsft_z == 0 && xorsft_w == 0);
}
void XorShift::init_xor128(unsigned long s) {
    for (unsigned long i = 0; i < 4; ++i) seed[i] = s = 1812433253U * (s ^ (s >> 30)) + i;
}

double XorShift::urand() {
    uint32_t t;
    t = xorsft_x ^ (xorsft_x << 11);
    xorsft_x = xorsft_y;
    xorsft_y = xorsft_z;
    xorsft_z = xorsft_w;
    xorsft_w ^= t ^ (t >> 8) ^ (xorsft_w >> 19);
    return ((xorsft_x + 0.5) / 4294967296.0 + xorsft_w) / 4294967296.0;
}

unsigned long XorShift::xor128() {
    unsigned long* a = seed;
    unsigned long  t = (a[0] ^ (a[0] << 11));
    a[0] = a[1]; a[1] = a[2]; a[2] = a[3];
    return (a[3] = (a[3] ^ (a[3] >> 19)) ^ (t ^ (t >> 8)));
}