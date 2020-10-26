#include "search.hpp"


using namespace std;

int main(int argc , char** argv)
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
    for(int i=0 ; i<argc;i++)
    {
        if(strcmp(argv[i],"-nameOfFile")==0) cout<<argv[i+1];
    }
    cout<<setprecision(3);
    cout<< " " << AStar(P,minBipartiteWithTopologicallyClosestHoleHeuristic)<<endl;
    //cout<<AStar(P,minBipartiteHeuristic);
    

}