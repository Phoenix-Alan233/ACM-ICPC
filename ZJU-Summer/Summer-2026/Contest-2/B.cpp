#include <bits/stdc++.h>
using namespace std;

const int N = 1000005;

char s[N];
int n, B;

int main() {
    scanf("%d%s", &n, s + 1);
    if (n == 1) {
        puts("a");
        return 0;
    }
    B = sqrt(n) + 1;
    for (int i = 1; i <= B; i++) s[i] = 'a';
    for (int i = 2 * B; i <= n; i += B) s[i] = 'b';
    s[n] = 'b';
    puts(s + 1);
    return 0;
}