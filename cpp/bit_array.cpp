#include <bitset>
#include <cmath>
#include <cstdio>
#include <iostream>
using namespace std;

#define MOD (2147483648)  // 2^31

// a bitarray where each position indicates if that number
// appeared in the sequence "a".
// big array so make it a static variable instead of allocating on the stack
// which also has the added bonus of initializing each bit to 0
static bitset<MOD> numbersSeen;

int main() {
    long long n, s, p, q;
    cin >> n;
    cin >> s;
    cin >> p;
    cin >> q;

    // populate the first value
    long long prevVal = s;
    numbersSeen[s] = true;

    // now calculate the remaining values
    for (int i = 1; i < n; i++)
    {
        auto rawVal = (prevVal * p) + q;
        auto val = rawVal % MOD;
        numbersSeen[val] = true;
        prevVal = val;
    }
    
    cout << numbersSeen.count();

    return 0;
}
