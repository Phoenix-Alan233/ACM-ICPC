#include <bits/stdc++.h>
using namespace std;

#define ll long long

const int N = 100005;

struct Point {
    ll x, y;
};

ll cross(const Point &a, const Point &b, const Point &c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

double D, L;
int n;

vector<Point> convexHull(vector<Point> &points) {
    int n = points.size();
    if (n <= 3) return points;
    sort(points.begin(), points.end(), [&](const Point &a, const Point &b) {
        if (a.x != b.x) return a.x < b.x;
        else return a.y < b.y;
    });

    vector<Point> hull(2 * n);
    int k = 0;
    for (int i = 0; i < n; i++) {
        while (k >= 2 && cross(hull[k - 2], hull[k - 1], points[i]) <= 0) {
            k--;
        }
        hull[k++] = points[i];
    }
    for (int i = n - 2, t = k + 1; i >= 0; i--) {
        while (k >= t && cross(hull[k - 2], hull[k - 1], points[i]) <= 0) {
            k--;
        }
        hull[k++] = points[i];
    }
    hull.resize(k - 1);
    return hull;
}

int main() {
    ios::sync_with_stdio(false), cin.tie(0);
    cin >> n >> D;
    vector<Point> points;
    for (int i = 1; i <= n; i++) {
        int x, y;
        cin >> x >> y;
        points.push_back({x, y});
    }
    vector<Point> hull = convexHull(points);

    for (int i = 0; i < hull.size(); i++) {
        Point cur = hull[i], nxt = hull[(i + 1) % hull.size()];
        L += sqrt(pow(cur.x - nxt.x, 2) + pow(cur.y - nxt.y, 2));
    }

    cout << fixed << setprecision(10) << L / (acos(-1) * D) << endl;
    return 0;
}