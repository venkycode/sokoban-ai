#include "problem.hpp"



long long manhat(std::pair<int, int> p1, std::pair<int, int> p2)
{
    return abs(p1.first - p2.first) + abs(p2.second - p1.second);
}



struct Cmp
{

    bool operator()(std::pair<std::pair<long long, long long>, ProblemState>& A, std::pair<std::pair<long long, long long>, ProblemState> &B)
    {
        if(A.first.first==B.first.first) 
            return A.first.second< B.first.second;
        
        return A.first.first < B.first.first;
    }
};

long long closestHoleHeur(ProblemState &state)
{
    long long ans = 0;

    for (auto pp : state.boxes)
    {
        if (state.holes.count(pp))
            continue;
        if (!ans)
            ans = manhat(pp, state.robo);
        remin(ans, manhat(pp, state.robo));
    }

    for (auto pp : state.boxes)
    {
        if (state.holes.count(pp))
            continue;
        long long cur = 0;
        for (auto ho : state.holes)
        {
            if (!cur)
                cur = manhat(ho, pp);
            remin(cur, manhat(pp, ho));
        }
        ans += cur;
    }
    return ans;
}

long long minBipartiteHeuristic(ProblemState& state)
{
    static std:: unordered_map<unsigned long long , long long> memoisedAnswer;
    long long ans = 0;

    for (auto pp : state.boxes)
    {
        if (state.holes.count(pp))
            continue;
        if (!ans)
            ans = manhat(pp, state.robo);
        remin(ans, manhat(pp, state.robo));
    }
    unsigned long long hashOfSet= hashSetOfPairs(state.boxes);
    if(memoisedAnswer.count(hashOfSet)) return ans+100*memoisedAnswer[hashOfSet];
    
    long long minBipartiteWeight=0;

    int sz= state.boxes.size();

    std:: set<std::pair<int,int>> remBoxes=state.boxes;
    std:: set<std::pair<int,int>> remHoles=ProblemState:: holes;


    for(int i=0; i< sz; i++)
    {
        std:: pair<int,int>a= *remBoxes.begin();
        std:: pair<int,int> b= *remHoles.begin();
        long long mn= manhat(a,b);

        for(auto aa: remBoxes)
        for(auto bb: remHoles)
        {
            if(manhat(aa,bb)<mn)
            {
                mn= manhat(aa,bb);
                a=aa;
                b=bb;
            }
        }
        minBipartiteWeight+=mn;
        remBoxes.erase(a);
        remHoles.erase(b);
    }

    memoisedAnswer[hashOfSet]= minBipartiteWeight;

    return ans+100*minBipartiteWeight;
     
}

std::string AStar(Problem problem, long long heuristicFunction (ProblemState &S))
{
    std::cout<<problem.nodesExpanded<<std::endl;
    std::set<std::pair<std::pair<long long, long long>, ProblemState>> fringe;

    ProblemState startState = problem.startState;

    fringe.insert({{heuristicFunction(startState), 0}, startState});
    std:: unordered_set<unsigned long long > visited;
    visited.insert(startState.hash());
    ProblemState s1 = problem.startState;

    while (fringe.size())
    {
        std::pair<std::pair<long long, long long>, ProblemState> cur_node = *fringe.begin();
        fringe.erase(fringe.begin());
        if (problem.isGoalState(cur_node.second))
        {
            std::cout<<problem.nodesExpanded<<std::endl;
            return cur_node.second.actions;
        }
        if(problem.nodesExpanded> 1e7 ) 
        {
            std::cout<<problem.nodesExpanded<<std::endl;
            std::cerr<<"Failure";
            return cur_node.second.actions;
        }
        long long cur_g = cur_node.first.second;
        ProblemState &cur_state = cur_node.second;
        cur_g++;
        for (auto &next_state : problem.getSuccessorStates(cur_state))
        {
            if(!visited.count(next_state.hash()))
            {
                fringe.insert({{cur_g + heuristicFunction(next_state), cur_g}, next_state});
                visited.insert(next_state.hash());
            }
        }
    }

    return "";
}