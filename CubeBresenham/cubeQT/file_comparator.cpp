#include "bits/stdc++.h"

using namespace std;

typedef long long ll;
typedef long double ld;
typedef pair<ll, ll> pll;
typedef pair<ld, ld> pld;
typedef pair<int, int> pii;

#define FOR(i, n) for (int i = 0; i < n; i++)
#define PI acos(-1)
#define x first
#define y second

int main()
{
    cin.tie(nullptr);
    ios::sync_with_stdio(false);

    const char* file1 = "C:/Users/Timoniche/Desktop/Graphics/CubeBresenham/cubeQT/screen";
    const char* file2 = "C:\\Users\\Timoniche\\Desktop\\Graphics\\CubeModel\\screen";
    //freopen(file1, "r", stdin);
    ifstream f1(file1);
    ifstream f2(file2);
    vector<tuple<int, int, int, int, int>> first;
    vector<tuple<int, int, int, int, int>> second;
    int in, in2, in3, in4, in5;
    while (f1 >> in) {
        f1 >> in2 >> in3 >> in4 >> in5;
        first.emplace_back(in, in2, in3, in4, in5);
    }
    //fclose(stdin);
    freopen(file2, "r", stdin);
    while (f2 >> in) {
        f2 >> in2 >> in3 >> in4 >> in5;
        second.emplace_back(in, in2, in3, in4, in5);
    }
    std::sort(first.begin(), first.end());
    std::sort(second.begin(), second.end());
    vector<tuple<int, int, int, int, int>> inter;
    std::set_intersection(first.begin(),first.end(),
                          second.begin(),second.end(),
                          back_inserter(inter));
    cout << first.size() << " " << second.size() << endl;
    cout << inter.size();
    return 0;
}







