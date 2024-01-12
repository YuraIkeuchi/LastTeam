#include<math.h>
#include"Easing.h"

static const float PI = 3.141592f;

using namespace std;

namespace EasingFunction {
    //イージング関数
    float InQuad(float t)     {
        return t * t;
    }

    float InCubic(float t)     {
        return t * t * t;
    }

    float InQuart(float t)     {
        return t * t * t * t;
    }

    float InQuint(float t)     {
        return t * t * t * t * t;
    }

    float InSine(float t)     {
        return 1 - cosf(t * PI / 2);
    }

    float InExp(float t)     {
        return powf(2, -(1 - t) * 10);
    }

    float InCirc(float t)     {
        return 1 - sqrtf((0 > 1 - t * t) ? 0 : 1 - t * t);
    }

    float InBack(float t)     {
        return t * t * (2.70158f * t - 1.70158f);
    }
    float InSoftBack(float t)     {
        return t * t * (2 * t - 1);
    }

    float InElastic(float t)     {
        return 56 * t * t * t * t * t - 105 * t * t * t * t + 60 * t * t * t - 10 * t * t;
    }

    float InBounce(float t)     {
        const float n1 = 7.5625f;
        const float d1 = 2.75f;

        if (t < 1.f / d1) {
            return n1 * t * t;
        } else if (t < 2.f / d1) {
            return n1 * (t -= 1.5f / d1) * t + 0.75f;
        } else if (t < 2.5f / d1) {
            return n1 * (t -= 2.25f/ d1) * t + 0.9375f;
        } else {
            return n1 * (t -= 2.625f / d1) * t + 0.984375f;
        }
    }

    float InLinear(float t)     {
        return t;
    }
}

using namespace EasingFunction;
static float (*FuncPtr[12])(float t)
= {
    InQuad,InCubic,InQuart,InQuint,InSine,InExp,InCirc,InBack,InSoftBack,InElastic,InBounce,InLinear
};


float Ease(type Type, category Ease, float t, float start, float end) {
    if (Type == type::In)     {
        return (end - start) * FuncPtr[Ease](t) + start;
    }     else if (Type == type::Out)     {
        return (end - start) * (1 - FuncPtr[Ease](1 - t)) + start;
    }     else if (Type == type::InOut)     {
        if (t < 0.5)
            return (end - start) * (FuncPtr[Ease](t * 2) / 2) + start;

        return (end - start) * (1 - FuncPtr[Ease](2 - 2 * t) / 2) + start;
    } else {
        return false;
    }
}