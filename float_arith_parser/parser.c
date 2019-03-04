#include <stdio.h>
#include <stdbool.h>

const char *stream = NULL;

bool accept(char c)
{
    if (*stream == c)
    {
        ++stream;
        return true;
    }
    return false;
}

bool not_zero(void)
{
    return accept('1') || accept('2') || accept('3') || accept('4') || accept('5') ||
        accept('6') || accept('7') || accept('8') || accept('9');
}

bool digit(void)
{
    return accept('0') || not_zero();
}

bool raw(void)
{
    if (digit())
    {
        raw();
        return true;
    }
    return false;
}

bool integer(void)
{
    if (not_zero())
    {
        raw();
        return true;
    }
    return accept('0');
}

bool dot(void)
{
    return accept('.');
}

bool float_num(void)
{
    if (integer())
    {
        if (dot())
        {
            raw();
            return true;
        }
        return false;
    }
    else if (dot())
    {
        return raw();
    }
    return false;
}


bool expr(void)
{
    if (float_num())
    {
        if (accept('+') || accept('-') || accept('*'))
        {
            return expr();
        }
        else
        {
            return accept(0);
        }
    }
    return false;
}

void check(const char* str)
{
    stream = str;
    printf("%s -> %d\n", str, expr());
}

int main(void)
{
    check("1.234");
    check("1.23+2.22");
    check("heelo");
    check("1.234hh");
    check("1.234+hhh");
    check("1.234+2.28890sss");
    check(".234");
    check("1.");
    check("1.*.234");
    return 0;
}
