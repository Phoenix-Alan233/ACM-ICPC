#include <bits/stdc++.h>
using namespace std;

struct BigInt {
    static constexpr long long BASE = 1000000000;  // 10^9
    static constexpr int BASE_DIGITS = 9;

    std::vector<int> a;  // 低位在前，每个元素存 0..BASE-1
    int sign;            // 1 或 -1（0 用 sign=1, a=[0] 表示）

    BigInt() : a{0}, sign(1) {}

    BigInt(long long v) : sign(1) {
        if (v < 0) sign = -1, v = -v;
        while (v > 0) {
            a.push_back(v % BASE);
            v /= BASE;
        }
        if (a.empty()) a.push_back(0);
    }

    explicit BigInt(const std::string& s) {
        sign = 1;
        int start = 0;
        if (s[0] == '-') sign = -1, start = 1;
        if (s[0] == '+') sign = 1, start = 1;
        a.assign(1, 0);
        for (int i = start; i < (int)s.size(); i++) {
            mul_small(10);
            add_small(s[i] - '0');
        }
        trim();
    }

    BigInt(const BigInt&) = default;
    BigInt& operator=(const BigInt&) = default;

    void trim() {
        while (a.size() > 1 && a.back() == 0) a.pop_back();
        if (a.size() == 1 && a[0] == 0 && sign == -1) sign = 1;
    }

    int size() const { return (int)a.size(); }
    int& operator[](int i) { return a[i]; }
    int operator[](int i) const { return a[i]; }

    void mul_small(int b) {
        long long carry = 0;
        for (int i = 0; i < size(); i++) {
            carry += (long long)a[i] * b;
            a[i] = carry % BASE;
            carry /= BASE;
        }
        while (carry) {
            a.push_back(carry % BASE);
            carry /= BASE;
        }
        trim();
    }

    long long div_small(int b) {
        long long carry = 0;
        for (int i = size() - 1; i >= 0; i--) {
            carry = carry * BASE + a[i];
            a[i] = carry / b;
            carry %= b;
        }
        trim();
        return carry;
    }

    void add_small(int b) {
        long long carry = b;
        for (int i = 0; carry && i < size(); i++) {
            carry += a[i];
            a[i] = carry % BASE;
            carry /= BASE;
        }
        while (carry) {
            a.push_back(carry % BASE);
            carry /= BASE;
        }
    }

    void shift_right(int n) {  // 除以 BASE^n
        if (n >= size()) {
            a.assign(1, 0);
            sign = 1;
            return;
        }
        a.erase(a.begin(), a.begin() + n);
    }

    int cmp_abs(const BigInt& b) const {
        if (size() != b.size()) return size() < b.size() ? -1 : 1;
        for (int i = size() - 1; i >= 0; i--) {
            if (a[i] != b[i]) return a[i] < b[i] ? -1 : 1;
        }
        return 0;
    }

    // 无符号减法：要求 *this >= b
    void sub_abs(const BigInt& b) {
        long long borrow = 0;
        for (int i = 0; i < size(); i++) {
            long long cur = a[i] - borrow - (i < b.size() ? b[i] : 0);
            if (cur < 0) {
                cur += BASE;
                borrow = 1;
            } else {
                borrow = 0;
            }
            a[i] = (int)cur;
        }
        trim();
    }

    void add_abs(const BigInt& b) {
        a.resize(std::max(size(), b.size()) + 1, 0);
        long long carry = 0;
        for (int i = 0; i < (int)a.size(); i++) {
            carry += a[i];
            if (i < b.size()) carry += b[i];
            a[i] = carry % BASE;
            carry /= BASE;
        }
        trim();
    }

    std::string to_string() const {
        std::string s;
        if (sign == -1) s += '-';
        s += std::to_string(a.back());
        for (int i = size() - 2; i >= 0; i--) {
            std::string part = std::to_string(a[i]);
            s += std::string(BASE_DIGITS - part.size(), '0') + part;
        }
        return s;
    }

    bool operator<(const BigInt& b) const {
        if (sign != b.sign) return sign < b.sign;
        if (sign == 1) return cmp_abs(b) < 0;
        return cmp_abs(b) > 0;
    }
    bool operator>(const BigInt& b) const { return b < *this; }
    bool operator<=(const BigInt& b) const { return !(*this > b); }
    bool operator>=(const BigInt& b) const { return !(*this < b); }
    bool operator==(const BigInt& b) const {
        return sign == b.sign && a == b.a;
    }
    bool operator!=(const BigInt& b) const { return !(*this == b); }
    bool is_zero() const { return size() == 1 && a[0] == 0; }

    BigInt operator-() const {
        BigInt r = *this;
        if (!r.is_zero()) r.sign = -r.sign;
        return r;
    }

    BigInt operator+(const BigInt& b) const {
        BigInt r;
        if (sign == b.sign) {
            r.sign = sign;
            r = *this;
            r.add_abs(b);
        } else if (cmp_abs(b) >= 0) {
            r.sign = sign;
            r = *this;
            r.sub_abs(b);
        } else {
            r.sign = b.sign;
            r = b;
            r.sub_abs(*this);
        }
        return r;
    }

    BigInt operator-(const BigInt& b) const { return *this + (-b); }

    BigInt operator*(const BigInt& b) const {
        BigInt r;
        r.sign = sign * b.sign;
        r.a.assign(size() + b.size(), 0);
        for (int i = 0; i < size(); i++) {
            long long carry = 0;
            for (int j = 0; j < b.size(); j++) {
                carry += (long long)a[i] * b[j] + r[i + j];
                r[i + j] = carry % BASE;
                carry /= BASE;
            }
            int pos = i + b.size();
            while (carry) {
                carry += r[pos];
                r[pos] = carry % BASE;
                carry /= BASE;
                pos++;
            }
        }
        r.trim();
        return r;
    }

    BigInt operator/(const BigInt& b) const {
        BigInt q, _;
        div_mod(b, q, _);
        return q;
    }

    BigInt operator%(const BigInt& b) const {
        BigInt _, r;
        div_mod(b, _, r);
        return r;
    }

    BigInt& operator+=(const BigInt& b) { return *this = *this + b; }
    BigInt& operator-=(const BigInt& b) { return *this = *this - b; }
    BigInt& operator*=(const BigInt& b) { return *this = *this * b; }
    BigInt& operator/=(const BigInt& b) { return *this = *this / b; }
    BigInt& operator%=(const BigInt& b) { return *this = *this % b; }

    void div_mod(const BigInt& B, BigInt& q, BigInt& r) const {
        assert(!B.is_zero());
        int cmp = cmp_abs(B);
        if (cmp < 0) {
            q = BigInt(0);
            r = *this;
            r.sign = sign;
            return;
        }
        if (cmp == 0) {
            q = BigInt(1);
            q.sign = sign * B.sign;
            r = BigInt(0);
            return;
        }

        long long d = BASE / ((long long)B.a.back() + 1);
        BigInt U = *this;
        U.sign = 1;
        U.mul_small((int)d);
        BigInt V = B;
        V.sign = 1;
        V.mul_small((int)d);

        int n = V.size();
        int m = U.size() - n;
        q.a.assign(m + 1, 0);
        q.sign = sign * B.sign;

        U.a.push_back(0);

        for (int j = m; j >= 0; j--) {
            long long num = (long long)U[j + n] * BASE + U[j + n - 1];
            long long q_hat = num / V[n - 1];
            long long r_hat = num % V[n - 1];

            while (q_hat == BASE ||
                   (n >= 2 && q_hat * V[n - 2] > r_hat * BASE + U[j + n - 2])) {
                q_hat--;
                r_hat += V[n - 1];
                if (r_hat >= BASE) break;
            }

            std::vector<long long> prod(n + 1, 0);
            for (int i = 0; i < n; i++) {
                prod[i] += q_hat * V[i];
                prod[i + 1] += prod[i] / BASE;
                prod[i] %= BASE;
            }

            long long borrow = 0;
            for (int i = 0; i <= n; i++) {
                long long diff = (long long)U[j + i] - prod[i] - borrow;
                if (diff < 0) {
                    diff += BASE;
                    borrow = 1;
                } else {
                    borrow = 0;
                }
                U[j + i] = (int)diff;
            }

            if (borrow) {
                q_hat--;
                long long carry = 0;
                for (int i = 0; i < n; i++) {
                    carry += (long long)U[j + i] + V[i];
                    U[j + i] = (int)(carry % BASE);
                    carry /= BASE;
                }
                U[j + n] += (int)carry;
            }

            q[j] = (int)q_hat;
        }

        U.a.pop_back();
        U.div_small((int)d);
        r = U;
        r.sign = sign;
        r.trim();
        q.trim();
    }

    BigInt pow(int exp) const {
        BigInt base = *this, res(1);
        while (exp) {
            if (exp & 1) res = res * base;
            base = base * base;
            exp >>= 1;
        }
        return res;
    }

    BigInt pow_mod(BigInt exp, const BigInt& mod) const {
        BigInt base = *this % mod, res(1);
        while (!exp.is_zero()) {
            if (exp.a[0] & 1) res = (res * base) % mod;
            base = (base * base) % mod;
            exp = exp / BigInt(2);
        }
        return res;
    }

    BigInt abs() const {
        BigInt r = *this;
        r.sign = 1;
        return r;
    }
};

BigInt gcd(BigInt a, BigInt b) {
    a = a.abs();
    b = b.abs();
    while (!b.is_zero()) {
        BigInt t = a % b;
        a = b;
        b = t;
    }
    return a;
}

BigInt lcm(const BigInt& a, const BigInt& b) {
    return (a / gcd(a, b)) * b;
}

std::ostream& operator<<(std::ostream& os, const BigInt& v) {
    return os << v.to_string();
}

std::istream& operator>>(std::istream& is, BigInt& v) {
    std::string s;
    is >> s;
    v = BigInt(s);
    return is;
}

void sc() {
    BigInt n, e, d;
    cin >> n >> e >> d;
    BigInt K = e * d - 1;
    int power = 0;
    while (K % 2 == 0) {
        power++;
        K /= 2;
    }
    vector<BigInt> candidate_g = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    for (auto g : candidate_g) {
        if (g >= n) continue;
        BigInt x = g.pow_mod(K, n);
        if (x == 1 || x == n - 1) continue;
        for (int i = 0; i < power; i++) {
            BigInt lastx = x;
            x = x.pow_mod(2, n);
            if (x == 1) {
                BigInt p = gcd(lastx - 1, n);
                if (p > 1 && p < n) {
                    BigInt q = n / p;
                    if (p > q) swap(p, q);
                    cout << p << " " << q << "\n";
                    return;
                }
                break;
            }
        }
    }
    assert(0);
}

int main() {
    int T;
    cin >> T;
    while (T--) sc();
    return 0;
}