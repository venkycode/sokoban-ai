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
    //for(auto s: inp) cout<<s<<std::endl;
    Problem P(inp);

    cout<<AStar(P);       
}