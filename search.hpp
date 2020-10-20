#include "problem.hpp"

using namespace std;


long long manhat(pair<int,int> p1, pair<int,int> p2)
{
    return abs(p1.first-p2.first)+abs(p2.second-p1.second);
}

bool comp(pair<pair<long long,long long>, ProblemState> A, pair<pair<long long,long long>, ProblemState> B)
{
    return A.first>B.first;
}

long long closestHoleHeur(ProblemState& state)
{
    long long ans=0;
    
    for(auto pp: state.boxes)
    {
        if(ProblemState:: holes.count(pp)) continue;
        if(!ans) ans= manhat(pp,state.robo);
        remin(ans, manhat(pp,state.robo));
    }

    for(auto pp : state.boxes)
    {
        if(ProblemState:: holes.count(pp)) continue;
        long long cur=0;
        for(auto ho: ProblemState:: holes)
        {
            if(!cur) cur= manhat(ho,pp);
            remin(cur, manhat(pp,ho));
        }
        ans+=cur;
    }
    return ans;
}

string AStar(Problem problem)
{
    set<pair<pair<long long,long long>, ProblemState>> fringe;
    
    ProblemState startState= problem.startState;

    fringe.insert({{closestHoleHeur(startState),0}, startState}); 

    ProblemState s1= problem.startState;

    while (fringe.size())
    {
        pair<pair<long long,long long>, ProblemState> cur_node= *fringe.begin();
        fringe.erase(fringe.begin());

        if(problem.isGoalState(cur_node.second)) 
        {
            return cur_node.second.actions;
        }

        long long cur_g= cur_node.first.second;
        ProblemState & cur_state= cur_node.second;
        cur_g++;
        for(auto &next_state : problem.getSuccessorStates(cur_state))
        {
            fringe.insert({{cur_g+closestHoleHeur(next_state),cur_g},next_state});
        }
    }

    return "";
    
}