#include "search.hpp"

using namespace std;

int main()
{
    vector<string> inp;
    while (!cin.eof())
    {
        string s;
        getline(cin,s);
        inp.push_back(s);
    }
    for(auto s: inp) cout<<s<<endl;
    Problem P(inp);

    cout<<AStar(P);       
}