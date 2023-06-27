#include <iostream>
#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>
#include <conio.h>
#include <windows.h>
#include <unistd.h>
#include <stdlib.h>

#define pb push_back

using namespace sf;
using namespace std;

const int maxn = 40, INF = 1e9;
int Ip, Jp, esc, Ich, Jch, n, m, chance, dirG[4], dis[maxn * maxn], par[maxn * maxn];
bool adj[maxn * maxn][maxn * maxn], mark[maxn * maxn], HasGhost[maxn][maxn], GameOver = 0;;
char Map[maxn][maxn];
vector<int>path;
enum Direction { Left, Right, Up, Down, Stop };
Direction dir = Stop;
pair<int, int>ghost[4];

int to_int(string s) {
    int ans = 0;
    for (int i = 0; i < s.size(); i++) {
        ans *= 10;
        ans += (s[i] - '0');
    }
    return ans;
}

void save() {
    ofstream myfile("Game.txt");
    if (myfile.is_open()) {
        myfile << n << ' ' << m << ' ';
        myfile << Ip << ' ' << Jp << ' ';
        myfile << Ich << ' ' << Jch << ' ';
        for (int i = 0; i < 4; i++) {
            myfile << ghost[i].first << ' ' << ghost[i].second << ' ';
        }
        for (int i = 0; i < maxn * maxn; i++) {
            myfile << Map[i / m][i % m] << ' ';
        }
        myfile.close();
    }
}

void load() {
    ifstream file("Game.txt");
    if (file.is_open()) {
        string line;
        getline(file, line);
        string temp = "";
        vector<string>data;
        for (int i = 0; i < line.size(); i++) {
            if (line[i] == ' ') {
                if (temp == "") {
                    temp += ' ';
                }
                data.push_back(temp);
                temp = "";
            }
            else {
                temp += line[i];
            }
        }
        n = to_int(data[0]);
        m = to_int(data[1]);
        Ip = to_int(data[2]);
        Jp = to_int(data[3]);
        Ich = to_int(data[4]);
        Jch = to_int(data[5]);
        for (int i = 0; i < 4; i++) {
            ghost[i].first = to_int(data[6 + i * 2]);
            ghost[i].second = to_int(data[7 + i * 2]);
            HasGhost[ghost[i].first][ghost[i].second] = true;
        }
        for (int i = 0; i < maxn * maxn; i++) {
            if (data[i + 14] == "#") {
                Map[i / m][i % m] = '#';
            }
            else if (Map[i / m][i % m] == '.') {
                Map[i / m][i % m] = '.';
            }
            else {
                Map[i / m][i % m] = ' ';
            }
        }
        file.close();
    }
}

void RandCherry() {
    int sz = (int)path.size();
    int loc;
    loc = path[rand() % sz];
    Ich = loc / m;
    Jch = loc % m;
}

void dfs(int cur) {
    mark[cur] = 1;
    for (int i = 0; i < n * m; i++) {
        if (adj[cur][i] && !mark[i] && Map[i / m][i % m] != '#') {
            path.pb(i);
            dfs(i);
        }
    }
}

void check() {
    int cur = (Ip * m + Jp);
    dfs(cur);
}

void make_map() {
    for (int i = 0; i < n; i++) {
        Map[i][0] = Map[i][m - 1] = '#';
    }
    for (int j = 0; j < m; j++) {
        Map[0][j] = Map[n - 1][j] = '#';
    }
    for (int i = 1; i < n - 1; i++) {
        for (int j = 1; j < m - 1; j++) {
            if ((rand() % 6) == 0) {
                Map[i][j] = '#';
            }
        }
    }
    int ver;
    for (int i = 1; i < n - 1; i++) {
        for (int j = 1; j < m - 1; j++) {
            ver = i * m + j;
            adj[ver][ver + 1] = adj[ver][ver - 1] = adj[ver][ver + m] = adj[ver][ver - m] = 1;
        }
    }
    Ip = 3 * n / 4;
    Jp = m / 2;
    Map[Ip][Jp] = ' ';
    check();
    int sz = (int)path.size();
    for (int i = 0; i < 4; i++) {
        int ghosti, ghostj;
        int loc = path[(rand()) % sz];
        ghosti = loc / m;
        ghostj = loc % m;
        ghost[i] = make_pair(ghosti, ghostj);
        HasGhost[ghosti][ghostj] = 1;
    }
    for (int i = 1; i < n - 1; i++) {
        for (int j = 1; j < m - 1; j++) {
            if (i == Ip && j == Jp) {
                Map[i][j] = ' ';
                continue;
            }
            else if (Map[i][j] != '#' && mark[i * m + j]) {
                Map[i][j] = '.';
            }
            else if (Map[i][j] != '#' && !mark[i * m + j]) {
                Map[i][j] = '#';
            }
        }
    }
    RandCherry();
}

void Print() {
    sleep(1);
    system("cls");
    for (int i = 0; i < 2 * n; i++) {
        for (int j = 0; j < 2 * m; j++) {
            if (i == Ip * 2 && j == Jp * 2) {
                cout << "\033[33m" << 'C';
            }
            else if (i == Ich * 2 && j == Jch * 2) {
                cout << "\033[31m" << '@';
            }
            else if (HasGhost[i / 2][j / 2] && i % 2 == 0 && j % 2 == 0) {
                if (esc) {
                    cout << "\033[34m" << 'G';
                }
                else {
                    cout << "\033[36m" << 'G';
                }
            }
            else if (Map[i / 2][j / 2] == '#') {
                cout << "\033[37m" << "\u2588";
            }
            else if (Map[i / 2][j / 2] == '.' && i % 2 == 0 && j % 2 == 0) {
                cout << "\033[32m" << Map[i / 2][j / 2];
            }
            else {
                cout << ' ';
            }
        }
        cout << '\n';
    }
}

void isGhost() {
    if (HasGhost[Ip][Jp] && !esc) {
        chance--;
        Ip = 3 * n / 4;
        Jp = m / 2;
    }
    if (chance == 0) {
        GameOver = 1;
        return;
    }
}

void MoveGhost() {
    int Ig, Jg;
    for (int i = 0; i < 4; i++) {
        if (esc) {
            dirG[i] = 3 - dirG[i];
        }
        Ig = ghost[i].first;
        Jg = ghost[i].second;
        if (dirG[i] == 1) {//Left
            if (Map[Ig][Jg - 1] == ' ' || Map[Ig][Jg - 1] == '.') {
                HasGhost[Ig][Jg] = 0;
                Jg--;
                HasGhost[Ig][Jg] = 1;
            }
        }
        if (dirG[i] == 2) {//right
            if (Map[Ig][Jg + 1] == ' ' || Map[Ig][Jg + 1] == '.') {
                HasGhost[Ig][Jg] = 0;
                Jg++;
                HasGhost[Ig][Jg] = 1;
            }
        }
        if (dirG[i] == 0) {//up
            if (Map[Ig - 1][Jg] == ' ' || Map[Ig - 1][Jg] == '.') {
                HasGhost[Ig][Jg] = 0;
                Ig--;
                HasGhost[Ig][Jg] = 1;
            }
        }
        if (dirG[i] == 3) {//down
            if (Map[Ig + 1][Jg] == ' ' || Map[Ig + 1][Jg] == '.') {
                HasGhost[Ig][Jg] = 0;
                Ig++;
                HasGhost[Ig][Jg] = 1;
            }
        }
        ghost[i] = { Ig,Jg };
    }
}

void bfs(int v) {
    int u;
    for (int i = 0; i < maxn * maxn; i++) {
        dis[i] = INF;
    }
    dis[v] = 0;
    queue<int>q;
    q.push(v);
    par[v] = v;
    while (q.size()) {
        u = q.front();
        q.pop();
        for (int i = 0; i < maxn * maxn; i++) {
            if (adj[u][i] && dis[i] > dis[u] + 1 && Map[i / m][i % m] != '#') {
                dis[i] = dis[u] + 1;
                par[i] = u;
                q.push(i);
            }
        }
    }
}

void MoveGPro() {
    int Ig, Jg, v;
    bfs(Ip * m + Jp);
    for (int i = 0; i < 4; i++) {
        Ig = ghost[i].first;
        Jg = ghost[i].second;
        v = Ig * m + Jg;
        if (par[v] == v + m) {//down
            dirG[i] = 3;
        }
        if (par[v] == v - m) {//up
            dirG[i] = 0;
        }
        if (par[v] == v - 1) {//left
            dirG[i] = 1;
        }
        if (par[v] == v + 1) {//right
            dirG[i] = 2;
        }
    }
    if (esc) {
        esc--;
    }
    MoveGhost();
}

void Dir() {//keys
    //keyboard Key;
//    if (_kbhit()) {
    if (sf::Keyboard::isKeyPressed((sf::Keyboard::Left))) {
        dir=Left;
    }
    else if (sf::Keyboard::isKeyPressed((sf::Keyboard::Right))) {w
        dir= Right;
    }
    else if (sf::Keyboard::isKeyPressed((sf::Keyboard::Up))) {
        dir=Up;
    }
    else if (sf::Keyboard::isKeyPressed((sf::Keyboard::Down))) {
        dir=Down;
    }
}

void MovePacman() {
    if (dir == Left) {
        if (Map[Ip][Jp - 1] == ' ' || Map[Ip][Jp - 1] == '.') {//*else if
            Map[Ip][Jp] = ' ';
            Jp--;
        }
    }
    if (dir == Right) {
        if (Map[Ip][Jp + 1] == ' ' || Map[Ip][Jp + 1] == '.') {
            Map[Ip][Jp] = ' ';
            Jp++;
        }
    }
    if (dir == Up) {
        if (Map[Ip - 1][Jp] == ' ' || Map[Ip - 1][Jp] == '.') {
            Map[Ip][Jp] = ' ';
            Ip--;
        }
    }
    if (dir == Down) {
        if (Map[Ip + 1][Jp] == ' ' || Map[Ip + 1][Jp] == '.') {
            Map[Ip][Jp] = ' ';
            Ip++;
        }
    }
    if (esc && HasGhost[Ip][Jp]) {
        int ghosti, ghostj;
        int sz = path.size();
        int loc = path[(rand()) % sz];
        ghosti = loc / m;
        ghostj = loc % m;
        HasGhost[ghosti][ghostj] = 1;
        HasGhost[Ip][Jp] = 0;
    }
    if (Ip == Ich && Jp == Jch) {
        esc = 25;
        RandCherry();
    }


    isGhost();
}

void MoveTime(double st,double cur){
    double diff=st-cur;
    if(diff-(int)diff==0.25 || diff-(int)diff==0.5 || diff-(int)diff==0.75 || diff-(int)diff==0){
        Dir();
        MovePacman();
    }
    else if(diff-(int)diff==0.33 || diff-(int)diff==0.67 || diff-(int)diff==0){
        MoveGPro();
    }
}

void Move(){
    Dir();
    MovePacman();
    sleep(0.5);
    MoveGPro();
}

int main() {
    RenderWindow window(VideoMode(960, 720), "Main");
    n=20;m=20;
    double start_time=time(0);
    make_map();
    while (window.isOpen()) {

        Event event;
        while (window.pollEvent(event)) {

            if (event.type == Event::Closed) {
                window.close();
            }
        }

        window.clear();
        for (int i = 0; i < maxn; i++) {
            for (int j = 0; j < maxn; j++) {
                if (Map[i][j] == '#') {
                    RectangleShape rectangle(Vector2f(20, 20));
                    rectangle.setFillColor(Color(0, 0, 100));
                    rectangle.setPosition((j) * 20, (i) * 20);
                    window.draw(rectangle);
                } else if (Map[i][j] == '.') {
                    CircleShape shape(3);
                    shape.setFillColor(Color(100, 250, 50));
                    shape.setPosition((j) * 20 +10, (i) * 20 +10);
                    window.draw(shape);
                }
            }
        }
                CircleShape square(9, 9);
                square.setFillColor(Color(400, 0, 0));
                square.setPosition(Jp * 20, Ip * 20);
                window.draw(square);

        for (int i = 0; i < 4; i++) {

            sf::CircleShape octagon(9, 9);
            octagon.setFillColor(Color(250, 50, 250));
            octagon.setPosition((ghost[i].second) * 20, (ghost[i].first) * 20);
            window.draw(octagon);
        }


        CircleShape octagon(10, 10);
        octagon.setFillColor(Color(0, 0, 50));
        octagon.setPosition(Jch * 20, Ich * 20);
        window.draw(octagon);
        sleep(0.25);
//      MoveTime(start_time,time(0));
        Move();
        window.display();

    }
    return 0;
}
