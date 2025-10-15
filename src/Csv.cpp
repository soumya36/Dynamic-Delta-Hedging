#include "Csv.hpp"
#include <fstream>
#include <stdexcept>
#include <algorithm>

static std::string trim(const std::string &s) {
    size_t b = s.find_first_not_of(" \t\r\n");
    if (b == std::string::npos) return "";
    size_t e = s.find_last_not_of(" \t\r\n");
    return s.substr(b, e - b + 1);
}

static std::vector<std::string> splitCSVLine(const std::string &line) {
    std::vector<std::string> out;
    std::string cur;
    bool inq=false;
    for (size_t i=0;i<line.size();++i){
        char c=line[i];
        if (inq){
            if (c=='"'){
                if (i+1<line.size() && line[i+1]=='"'){ cur.push_back('"'); ++i; }
                else inq=false;
            } else cur.push_back(c);
        } else {
            if (c==','){ out.push_back(cur); cur.clear(); }
            else if (c=='"'){ inq=true; }
            else cur.push_back(c);
        }
    }
    out.push_back(cur);
    for (auto &t: out) t=trim(t);
    return out;
}

CsvData readCsv(const std::string &path){
    std::ifstream fin(path);
    if(!fin) throw std::runtime_error("Cannot open: "+path);
    CsvData d;
    std::string line;
    if (!std::getline(fin,line)) throw std::runtime_error("Empty CSV: "+path);
    d.header = splitCSVLine(line);
    while (std::getline(fin,line)){
        auto cols=splitCSVLine(line);
        while (cols.size()<d.header.size()) cols.push_back("");
        d.rows.push_back({cols});
    }
    return d;
}

void writeCsv(const std::string &path, const std::vector<std::string>& header, const std::vector<std::vector<std::string>>& rows){
    std::ofstream fout(path);
    if(!fout) throw std::runtime_error("Cannot write: "+path);
    auto emit=[&](const std::vector<std::string>& cols){
        for(size_t i=0;i<cols.size();++i){
            std::string v=cols[i];
            bool needQuote = v.find(',')!=std::string::npos || v.find('"')!=std::string::npos || v.find('\n')!=std::string::npos;
            if (needQuote){
                std::string q="";
                q.push_back('"');
                for(char ch: v){ if (ch=='"') { q += "\"\""; } else q.push_back(ch); }
                q.push_back('"');
                fout<<q;
            } else fout<<v;
            if(i+1<cols.size()) fout<<",";
        }
        fout<<"\n";
    };
    emit(header);
    for (auto &r: rows) emit(r);
}
