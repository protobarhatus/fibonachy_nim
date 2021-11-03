#include <assert.h>
#include <stdio.h>
#include "rational.h"
#include "math.h"
#include "../vector/algorithms.h"

static int sign(long long int x)
{
    return x > 0 ? 1 : (x < 0 ? -1 : 0);
}

long long int iabs(long long int denom)
{
    return denom > 0 ? denom : -denom;
}

Rational defaultRational(long long int num, long long int denom)
{
    assert(denom != 0);
    if (num == 0)
    {
        Rational res;
        res.num = 0;
        res.denom = 1;
        return res;
    }
    Rational res;
    res.num = num;
    res.denom = iabs(denom);
    res.num *= sign(denom);
    long long int div = gcd(num, denom);
    res.num /= div;
    res.denom /= div;
    return res;
}
void printfRational(const Rational * rat)
{
    printf("%lld/%lld ", rat->num, rat->denom);
}
Rational addRational(const Rational* a, const Rational* b)
{
    long long int comdiv = gcd(a->denom, b->denom);
    return defaultRational((a->num * (b->denom / comdiv) + b->num * (a->denom / comdiv)), (a->denom / comdiv) * b->denom);
}

Rational nullRational()
{
    Rational res;
    res.num = 0;
    res.denom = 1;
    return res;
}

Rational minusOneRational()
{
    Rational res;
    res.num = -1;
    res.denom = 1;
    return res;
}

Rational unitRational()
{
    Rational res;
    res.num = 1;
    res.denom = 1;
    return res;
}
Rational multRational(const Rational* a, const Rational* b)
{
    if (a->num != 0)
    assert(9223372036854775807 / iabs(a->num) > iabs(b->num));
    assert(9223372036854775807 / a->denom > b->denom);
    return defaultRational(a->num * b->num, a->denom * b->denom);
}

Rational subRational(const Rational* a, const Rational* b)
{
    Rational res;
    Rational bc = *b;
    bc.num *= -1;
    return addRational(a, &bc);
}
Rational divRational(const Rational* a, const Rational * b)
{
    return defaultRational(a->num * b->denom, a->denom * b->num);
}

Rational sqrtRational(const Rational * a)
{
    assert(false);
}

void addToRational(Rational * a, const Rational * b)
{
    *a = addRational(a, b);
}
void subToRational(Rational * a, const Rational * b)
{
    *a = subRational(a, b);
}
void multToRational(Rational * a, const Rational * b)
{
    *a = multRational(a, b);
}
void divToRational(Rational * a, const Rational * b)
{
    *a = divRational(a, b);
}
Rational minusRational(const Rational * a)
{
    return defaultRational(-a->num, a->denom);
}
bool isNull(const Rational * r)
{
    return r->num == 0 && r->denom != 0;
}