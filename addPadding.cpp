#include "problem.hpp"


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

    for(auto &s: P.level.getLevelVector())
        cout<<s<<endl;

}
