#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <random>
#include <algorithm>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>

using namespace std;

typedef std::unordered_multimap<std::string,std::string> stringmap;

int main(int argc, char *argv[])
{
    cout<<"Started\n";
    int n, m;
    bool first = true;
    string ln, referenceName, pos1, pos2, pos3, pos4, pos5, pos6, pos7, gene_chromosome_name, readSeq, numberOfHits;
    unordered_multimap<string, string> mappedRefs;
    unordered_set<string> chromosomes;
    unordered_map<string, int> genomes;
    double countRefMatch = 0;
    if(argc == 1)
    {
        cout<<"No input file specified\n"<<endl;
        exit(1);
    }
    ifstream fl(argv[1]);
    while(getline(fl, ln))
    {
        istringstream iss(ln);
        if (!(iss >> referenceName >> pos1 >> gene_chromosome_name >> pos2 >> pos3 >> pos4 >> pos5 >> pos6 >> pos7 >> readSeq >> numberOfHits)) 
        {
            if(first)
            {
                continue;
            }
            else
            {
                break;
            }
        }
        first = false;
        mappedRefs.emplace(string(referenceName), (string(gene_chromosome_name) + string("%")+ string(readSeq)));
        if((gene_chromosome_name[0] != 'E' && gene_chromosome_name[1] != 'N') && (gene_chromosome_name[0] != '*'))
        {
            auto chromosomesItr = chromosomes.find(string(gene_chromosome_name));
            if(chromosomesItr == chromosomes.end())
            {
                chromosomes.emplace(string(gene_chromosome_name));
            }
        }
    }
    cout<<"number of mappedRefs: "<<mappedRefs.size()<<endl;
    cout<<"Number of Chromosomes: "<<chromosomes.size()<<endl;
    for(auto chromo : chromosomes)
    {
        ifstream fl2(argv[1]);
        unordered_map<string, string> chr;
        // unordered_map<string, int> refCount;
        first = true;
        while(getline(fl2, ln))
        {
            istringstream iss2(ln);
            if (!(iss2 >> referenceName >> pos1 >> gene_chromosome_name >> pos2 >> pos3 >> pos4 >> pos5 >> pos6 >> pos7 >> readSeq >> numberOfHits)) 
            {
                if(first)
                {
                    continue;
                }
                else
                {
                    break;
                }
            }
            first = false;
            if(gene_chromosome_name == chromo)
            {
                auto chrItr = chr.find(string(referenceName));
                if(chrItr == chr.end())
                {
                        chr.emplace(make_pair(string(referenceName), string(readSeq)));
                }
                // auto refCountItr = refCount.find(string(referenceName));
                // if(refCountItr == refCount.end())
                // {
                //     refCount.emplace(make_pair(string(referenceName), int(1)));
                // }
                // else
                // {
                //     refCountItr->second += 1;
                // }
            }
        }
        // auto tmp1itr = refCount.begin();
        // cout<<"chr: "<<chromo<<".   tmp1itr: " <<tmp1itr->second<<endl;
        for(const auto &line2 : chr)
        {
            auto itr = mappedRefs.equal_range(string(line2.first));
            for(auto x = itr.first; x != itr.second; ++x)
            {
                auto c_temp = string(x->second);
                string par1, par2;
                stringstream iss3(c_temp);
                if(getline(iss3, par1, '%'))
                    if(!getline(iss3, par2, '%'))
                        {break;}
                //cout<<"par1 : "<<par1<<endl;
                //cout<<"par2: "<<par2<<endl;
                //cout<<"line2.second: "<<line2.second<<endl;
                if((par1[0] == 'E' && par1[1] == 'N') && (par2 == line2.second))
                {
                    string parse;
                    stringstream iss3(par1);
                    if(getline(iss3, parse, '|'))
                        if(getline(iss3, parse, '|'))
                         if(getline(iss3, parse, '|'))
                           if(getline(iss3, parse, '|'))
                             if(getline(iss3, parse, '|'))
                                if(getline(iss3, parse, '|'))
                                {
                                    auto genomesItr = genomes.find(string(parse));
                                    if(genomesItr == genomes.end())
                                    {
                                        genomes.emplace(make_pair(parse, 1));
                                    }
                                    else
                                    {
                                        genomesItr->second += 1;
                                        countRefMatch += 1;
                                    }
                                }
                }
            }
        }
    }
    cout << "Total Number of Matches: " << countRefMatch << endl;
    cout << "Number of Genes: " << genomes.size() << endl;
    int safeCount = ceil(countRefMatch * 0.00015);
    int geneFusions = 0;
    cout << "Safe Count assumed for Matches: " << safeCount << endl;
    for(const auto &genItr : genomes)
    {
//        cout << genItr.first << "    count: " << genItr.second << endl;
        if(genItr.second > safeCount)
        {
            geneFusions += 1;
        }
    }
    cout << "Total number of Gene Fusions detected: " << geneFusions << endl;
}
