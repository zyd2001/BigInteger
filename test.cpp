#include <iostream>
#include <array>

using namespace std;

array<int, 36> a()
{
    array<int, 36> digits;

    uint64_t t = 1, v = 0;
    digits[0] = 0;
    digits[1] = 0;
    for (int i = 2; i < 36; i++)
    {
        int j = 1;
        for (; ; j++)
        {
            v = t * i;
            if (v == 0)
                break;
            if (v < t)
            {
                j--;
                break;
            }
            t = v;
        }
        digits[i] = j;
        t = 1;
    }
    return digits;
}

int main()
{
    cout << strtol("abc", NULL, 18) << endl;
}