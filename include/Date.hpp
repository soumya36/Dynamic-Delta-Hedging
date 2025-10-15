#pragma once
#include <string>
#include <vector>

struct YMD { int y,m,d; };
YMD parseYMD(const std::string& s);
bool isLeap(int y);
int daysInMonth(int y,int m);
int weekday(int y,int m,int d); // 0=Sun..6=Sat
int countWeekdays(const std::string& from, const std::string& to);
std::vector<std::string> enumerateDates(const std::string& d0, const std::string& dN);
