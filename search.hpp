#include "problem.hpp"
#include<chrono>


long long manhat(std::pair<int, int> p1, std::pair<int, int> p2)
{
    return abs(p1.first - p2.first) + abs(p2.second - p1.second);
}


long long find_satisfied(ProblemState& s)
{
    long long cnt=0;
    for(auto &pp : s.boxes)
    {
        if(s.holes.count(pp)) cnt++;
    }
    return ((cnt*400)/(s.holes.size()));
}

std::pair<int,std::pair<int,int>> closestPathToHole(std:: pair<int,int> pos,Problem & P, ProblemState & state,const std::set<std:: pair<int,int>> & remHoles,long long depthLimit)
{
    std:: set<std::pair<int,std::pair<int,int>>> bfsq;
    std:: unordered_set<long long> vis;
    bfsq.insert({0,pos});

    while (bfsq.size())
    {
        auto cur= *bfsq.begin();
        bfsq.erase(bfsq.begin());
        long long x= cur.second.first;
        long long y= cur.second.second;
        long long c= cur.first;
        if(remHoles.count({x,y})) return {c,{x,y}};
        if(c>depthLimit) return {INT_MAX,{-1,-1}};

        vis.insert(x+(y<<20));

        for(int i=0; i<4; i++)
        {
            int nx= x+dx[i];
            int ny= y+dy[i];
            
            if(vis.count(nx+(ny<<20))) continue;
            if(P.level.isWall({nx,ny})) continue;
            if(state.hasBoxAt({nx,ny})) bfsq.insert({c+6,{nx,ny}});
            else bfsq.insert({c+1,{nx,ny}});
        }
    }
    std::cerr<<"Control reached invalid end"<<std::endl;
}


/*
Each box consider closest hole as goal
*/
long long closestHoleHeur(ProblemState &state,Problem & P)
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


/*

Create a Min weight bipartite considering two partites as holesPositions and boxesPositions
and weights of any edge as manhatten distance between them

O(S^3) where S=number of holes

*/
long long minBipartiteHeuristic(ProblemState& state,Problem & P)
{
    /*
    unordered_map which keep track if the answer was calculated previously
    */
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
    if(memoisedAnswer.count(hashOfSet)) return ans+(P.level.averageDimension())*memoisedAnswer[hashOfSet];
    
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

    return ans+ (P.level.averageDimension())*minBipartiteWeight;
     
}


long long minBipartiteWithTopologicallyClosestHoleHeuristic(ProblemState& state,Problem & p)
{
    /*
    unordered_map which keep track if the answer was calculated previously
    */
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
    if(memoisedAnswer.count(hashOfSet)) return ans+(memoisedAnswer[hashOfSet]);
    
    long long minBipartiteWeight=0;

    

    std:: set<std::pair<int,int>> remBoxes=state.boxes;
    std:: set<std::pair<int,int>> remHoles=ProblemState:: holes;

    std::vector<std::pair<int,int>> common;

    for(auto pp: remBoxes) if(remHoles.count(pp)) common.push_back(pp);
    for(auto pp: common) {remHoles.erase(pp); remBoxes.erase(pp);}
    
    int sz= remBoxes.size();
    for(int i=0; i< sz; i++)
    {
        std:: pair<int,int>a= *remBoxes.begin();
        int mn;
        std::pair<int,int> b;
        std::pair<int,std::pair<int,int>> tup= closestPathToHole(a,p,state,remHoles,INT_MAX);
        mn= tup.first;
        b= tup.second;
        if(mn==INT_MAX) std::cerr<<"Unexpected value in tup"<<std::endl;
        for(auto aa: remBoxes)
        {
            if(!mn) break;
            tup=closestPathToHole(aa,p,state,remHoles,mn);
            if(tup.first< mn)
            {
                b= tup.second;
                a=aa;
                mn= tup.first;
            }
        }
        //std::cerr<<mn<<std::endl;
        minBipartiteWeight+=mn+p.level.averageDimension();
        remBoxes.erase(a);
        remHoles.erase(b);
    }

    memoisedAnswer[hashOfSet]= minBipartiteWeight;

    return ans+(minBipartiteWeight);
     
}

void print_state(ProblemState & s)
{   
    std::cerr<<"State"<<std::endl;
    std:: cerr<<s.robo.first<<" "<<s.robo.second<<std::endl;
    for(auto robo: s.boxes)
    {
        std:: cerr<<robo.first<<" "<<robo.second<<std::endl;
    }
}

std::string AStar(Problem problem, long long heuristicFunction (ProblemState &S,Problem & p))
{
    auto start = std::chrono::high_resolution_clock::now();

    std::set<std::pair<std::pair<long long, long long>, ProblemState>> fringe;

    ProblemState startState = problem.startState;

    fringe.insert({{heuristicFunction(startState,problem), 0}, startState});
    std:: unordered_set<unsigned long long > visited;
    visited.insert(startState.hash());
    ProblemState s1 = problem.startState;
    auto finish= std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    while (fringe.size())
    {
        std::pair<std::pair<long long, long long>, ProblemState> cur_node = *fringe.begin();
        fringe.erase(fringe.begin());
        
        finish= std::chrono::high_resolution_clock::now();
        elapsed = finish - start;
        if (problem.isGoalState(cur_node.second))
        {
            std::cout<<"Solved! "<<problem.nodesExpanded<<" "<<elapsed.count()<<" ";
            std::cerr<<cur_node.second.actions<<std::endl;
            return cur_node.second.actions;
        }

        //progressLogger(problem.nodesExpanded,18,cur_node.second.actions);

        if(elapsed.count()>600)
        {
            std::cout<<"Failure"<<" "<<problem.nodesExpanded<<" "<<elapsed.count()<<" ";
            return cur_node.second.actions;
        }    

        if(problem.nodesExpanded> 1e8 ) 
        {
            std::cout<<"Failure"<<" "<<problem.nodesExpanded<<" "<<elapsed.count()<<" ";
            return cur_node.second.actions;
        }
        long long cur_g = cur_node.first.second;
        ProblemState &cur_state = cur_node.second;
        cur_g++;
        for (auto &next_state : problem.getSuccessorStates(cur_state))
        {
            if(!visited.count(next_state.hash()))
            {
                long long cur_gg= cur_g-find_satisfied(next_state);   
                fringe.insert({{cur_gg + heuristicFunction(next_state,problem), cur_g}, next_state});
                visited.insert(next_state.hash());
            }
        }
    }
    std::cerr<<"unexpected failure in A*"<< std::endl;
    return "";
}