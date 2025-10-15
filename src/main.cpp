#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdlib>
#include "Csv.hpp"
#include "Date.hpp"
#include "Stats.hpp"
#include "BlackScholes.hpp"
#include "ImpliedVol.hpp"
#include "MarketData.hpp"
#include "Hedger.hpp"
#include "Simulator.hpp"
#include "ResultWriter.hpp"

using namespace std;

static bool hasFlag(const vector<string>& args, const string& f){
    return find(args.begin(), args.end(), f)!=args.end();
}
static string getArg(const vector<string>& args, const string& key, const string& def=""){
    for (size_t i=0;i+1<args.size();++i){
        if (args[i]==key) return args[i+1];
    }
    return def;
}

int main(int argc, char** argv){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    vector<string> args;
    for (int i=1;i<argc;++i) args.push_back(argv[i]);
    if (args.empty()){
        cerr<<"Usage:\n";
        cerr<<"  run simulate --S0 100 --mu 0.05 --sigma 0.24 --r 0.025 --T 0.4 --N 100 --paths 1000 --out out/\n";
        cerr<<"  run hedge-real --t0 2011-07-05 --tN 2011-07-29 --K 500 --exdate 2011-09-17 --sec data/sec_GOOG.csv --opt data/op_GOOG.csv --rf data/interest.csv --out out/result.csv\n";
        return 1;
    }
    string mode = args[0];
    if (mode=="simulate"){
        double S0 = stod(getArg(args,"--S0","100"));
        double mu = stod(getArg(args,"--mu","0.05"));
        double sigma = stod(getArg(args,"--sigma","0.24"));
        double r = stod(getArg(args,"--r","0.025"));
        double T = stod(getArg(args,"--T","0.4"));
        int N = stoi(getArg(args,"--N","100"));
        int paths = stoi(getArg(args,"--paths","1000"));
        string outdir = getArg(args,"--out","out/");
        auto R = simulateAndHedge(paths,N,S0,mu,sigma,r,T);
        vector<string> header;
        header.push_back("t_index");
        for (size_t p=0;p<R.sample_paths.size();++p){
            header.push_back(string("path_")+to_string(p));
        }
        vector<vector<string>> rows;
        for (int i=0;i<=N;i++){
            vector<string> row;
            row.push_back(to_string(i));
            for (size_t p=0;p<R.sample_paths.size();++p){
                row.push_back(to_string(R.sample_paths[p][i]));
            }
            rows.push_back(move(row));
        }
        writeCsv(outdir+"sim_paths.csv", header, rows);
        vector<vector<string>> rows2;
        for (size_t i=0;i<R.terminal_HE.size();++i){
            rows2.push_back({to_string(i), to_string(R.terminal_HE[i])});
        }
        writeCsv(outdir+"sim_terminal_HE.csv", {"path_index","HE_T"}, rows2);
        cout<<"Simulation outputs written to "<<outdir<<"\n";
        return 0;
    } else if (mode=="hedge-real"){
        string t0 = getArg(args,"--t0","2011-07-05");
        string tN = getArg(args,"--tN","2011-07-29");
        string exdate = getArg(args,"--exdate","2011-09-17");
        double K = stod(getArg(args,"--K","500"));
        string sec = getArg(args,"--sec","data/sec_GOOG.csv");
        string opt = getArg(args,"--opt","data/op_GOOG.csv");
        string rf  = getArg(args,"--rf","data/interest.csv");
        string outp= getArg(args,"--out","out/result.csv");
        auto S = loadAdjClose(sec);
        auto R = loadRiskFree(rf);
        auto O = loadOptions(opt);
        auto hp = runHedgeSeries(t0,tN,exdate,K,S,R,O);
        writeResultCsv(outp, hp);
        cout<<"Wrote "<<hp.size()<<" rows to "<<outp<<"\n";
        return 0;
    } else {
        cerr<<"Unknown mode: "<<mode<<"\n";
        return 2;
    }
}
