#include "search.hpp"

using namespace std;

int main()
{
    std::vector<std::string> inp;
    while (!cin.eof())
    {
        std::string s;
        getline(cin,s);
        inp.push_back(s);
    }

    Problem P(inp);

 //cout<<AStar(P,closestHoleHeur)<<endl;
    cout<<AStar(P,minBipartiteHeuristic)<<endl;
}