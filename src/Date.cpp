#include "Date.hpp"
#include <cstdio>
#include <stdexcept>

static bool _isLeap(int y){ return (y%4==0 && y%100!=0) || (y%400==0); }
static int _daysInMonth(int y,int m){
    static int dm[]={0,31,28,31,30,31,30,31,31,30,31,30,31};
    if(m==2) return dm[m]+(_isLeap(y)?1:0);
    return dm[m];
}

YMD parseYMD(const std::string &s){
    YMD a{0,0,0};
    if (s.size()<10) return a;
    a.y = std::stoi(s.substr(0,4));
    a.m = std::stoi(s.substr(5,2));
    a.d = std::stoi(s.substr(8,2));
    return a;
}
bool isLeap(int y){ return _isLeap(y); }
int daysInMonth(int y,int m){ return _daysInMonth(y,m); }

int weekday(int y,int m,int d){ // 0=Sun..6=Sat
    if (m<3){ m+=12; y-=1; }
    int K=y%100, J=y/100;
    int h=(d + (13*(m+1))/5 + K + K/4 + J/4 + 5*J) % 7;
    int dow=((h+6)%7);
    return dow;
}

int countWeekdays(const std::string &from, const std::string &to){
    YMD a=parseYMD(from), b=parseYMD(to);
    int cnt=0;
    int y=a.y, m=a.m, d=a.d;
    auto advance=[&](){
        d++;
        if (d>_daysInMonth(y,m)){ d=1; m++; if(m>12){ m=1; y++; } }
    };
    advance();
    while (true){
        char buf[11]; std::snprintf(buf,sizeof(buf), "%04d-%02d-%02d", y,m,d);
        std::string cur(buf);
        if (cur>to) break;
        int dow=weekday(y,m,d);
        if (dow>=1 && dow<=5) cnt++;
        advance();
    }
    return cnt;
}

std::vector<std::string> enumerateDates(const std::string &d0, const std::string &dN){
    std::vector<std::string> out;
    YMD a=parseYMD(d0), b=parseYMD(dN);
    int y=a.y, m=a.m, d=a.d;
    auto advance=[&](){
        d++;
        if (d>_daysInMonth(y,m)){ d=1; m++; if(m>12){ m=1; y++; } }
    };
    out.push_back(d0);
    while (true){
        char buf[11]; std::snprintf(buf,sizeof(buf), "%04d-%02d-%02d", y,m,d);
        std::string cur(buf);
        if (cur>dN) break;
        out.push_back(cur);
        advance();
    }
    return out;
}
