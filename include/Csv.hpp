#pragma once
#include <string>
#include <vector>

struct CsvRow { std::vector<std::string> cols; };
struct CsvData { std::vector<std::string> header; std::vector<CsvRow> rows; };

CsvData readCsv(const std::string& path);
void writeCsv(const std::string& path, const std::vector<std::string>& header, const std::vector<std::vector<std::string>>& rows);
