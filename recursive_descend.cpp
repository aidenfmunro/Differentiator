#include <stdio.h>

#define SYNTAX_ASSERT(expr) if (!(expr)) printf("SYNTAX ERROR IN LINE %d\n", __LINE__)

const char* s = "5+5+7";

int         p = 0;

int GetG(const char* str);
int GetN();
int GetE();
int GetT();
int GetP();

int main(void)
{  
    printf("%d\n", GetG(s));
}

int GetG(const char* str)
{
    s = str;
    p = 0;
    int val = GetE();

    return val;
}

int GetN()
{
    int val = 0;

    int old_p = p;

    while ('0' <= s[p] && s[p] <= '9')
    {
        val = val * 10 + s[p] - '0';
        p++;
    }

    return val;
}

int GetE()
{
    int val = GetT();

    while (s[p] == '+' || s[p] == '-')
    {
        int op = s[p];
        p++;

        int val2 = GetT();
        switch (op)
        {
            case '-': val -= val2; break;
            case '+': val += val2; break;
            default: break;
        }
    }

    return val;
}

int GetT()
{
    int val = GetP();

    while (s[p] == '*' || s[p] == '/')
    {
        int op = s[p];
        p++;

        int val2 = GetP();
        switch (op)
        {
            case '*': val *= val2; break;
            case '/': val /= val2; break;
            default: break;
        }
    }

    return val;
}

int GetP()
{
    int val = 0;
    if (s[p] == '(')
    {
        p++;
        val = GetE();
        // SYNTAX_ASSERT(s[p] == ')');
        p++;
        return val;
    }
    else
    {
        return GetN();
    }
}





