#pragma once

#include "../../def.h"

dbl ExpSMin(dbl a, dbl b, dbl k)
{
    k *= 1.0;
    dbl r = exp2(-a / k) + exp2(-b / k);
    return -k * log2(r);
}

dbl RootSMin(dbl a, dbl b, dbl k)
{
    k *= 2.0;
    dbl x = b - a;
    return 0.5 * (a + b - sqrt(x * x + k * k));
}

dbl SigmoidSMin(dbl a, dbl b, dbl k)
{
    k *= log(2.0);
    dbl x = b - a;
    return a + x / (1.0 - exp2(x / k));
}