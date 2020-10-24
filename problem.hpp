//#include <bits/stdc++.h>
#include "helper.hpp"

/*
abstract representation of state
*/

class ProblemState
{

public:
    std::set<std::pair<int, int>> boxes;               // location of boxes
    inline static std::set<std::pair<int, int>> holes; // location of holes common for all ProblemState obj hence static
    std::pair<int, int> robo;                          // location of the agent/player
    std::string actions;                               // actions to reach current state
    bool hashed;                                       // bool to check whether the object was previously hashed
    unsigned long long hashv;                          // hash of object

    ProblemState(std::set<std::pair<int, int>> boxes, std::pair<int, int> robo, std::string actions = "")
    {
        this->boxes = boxes;
        this->robo = robo;
        this->actions = actions;
        hashed = 0;
        this->hashv = hash();
        hashed = 1;
    }

    ProblemState()
    {
    }

    inline bool hasBoxAt(std::pair<int, int> pos)
    {
        return boxes.count(pos);
    }

    /*
        comparators to make the class compatible with
        std:: set 
    */
    inline bool operator<(const ProblemState &P) const
    {
        return this->hashv < P.hashv;
    }

    bool operator==(const ProblemState &anotherState) const
    {
        return hashv==hashv;
    }

    //standard hash function for object
    unsigned long long hash() const
    {
        unsigned long long seed = boxes.size();
        if (hashed)
            return hashv;
        for (auto &pp : boxes)
        {
            seed ^= 1ll * pp.first + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            seed ^= 1ll * pp.second + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            seed ^= (pp.first + pp.second) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        seed ^= 1ll + robo.first + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= 1ll + robo.second + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= 1ll + (robo.first + robo.second) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        return seed;
    }
};

/*
    Wrapper around static elements (walls and holes) 
    of the level
    Contains static deadlockTable to detect deadLock positions
*/
class Level
{
private:
    int h, w;                                     // height and width
    std::vector<std::string> level;               // padded level representation
    std::vector<std::vector<bool>> deadlockTable; // tells if box at [x,y] can be ever moved to a hole
public:
    Level(std::vector<std::string> &level_representation)
    {

        this->h = level_representation.size();
        this->w = 0;
        for (auto s : level_representation)
            remax(w, int(s.size()));

        for (int i = 0; i < h; i++)
        {
            int cw = level_representation[i].size();
            //adding wall to left
            for (int j = 0; j < cw; j++)
            {
                if( (level_representation[i][j] == _WALL_ ))
                    break;
                level_representation[i][j] = _WALL_;
            }
            // adding wall to right
            for (int j = cw; j < w; j++)
            {
                level_representation[i].push_back(_WALL_);
            }
        }
        //initial assignement as all positions deadLocked
        deadlockTable.assign(h, std::vector<bool>(w, 1));

        this->level = level_representation;
    }

    Level()
    {
    }

    ProblemState getStartState()
    {
        std::pair<int, int> ini_robo_pos;
        std::set<std::pair<int, int>> boxes, holes;

        for (int i = 0; i < h; i++)
            for (int j = 0; j < w; j++)
            {
                if (level[i][j] == _ROBOT_ON_FLOOR_ || level[i][j] == _ROBOT_ON_HOLE_)
                {
                    ini_robo_pos = {i, j};
                }
                if (level[i][j] == _BOX_ON_FLOOR_ || level[i][j] == _BOX_IN_HOLE_)
                    boxes.insert({i, j});

                if (level[i][j] == _HOLE_WITHOUT_BOX_ || level[i][j] == _HOLE_WITH_BOX_ || level[i][j] == _HOLE_WITH_ROBOT_)
                    holes.insert({i, j});
            }

        ProblemState::holes = holes;

        createDeadlockTable(holes);

        return ProblemState(boxes, ini_robo_pos);
    }

    // creates static database of deadLock positions using Multi Source BFS
    void createDeadlockTable(std::set<std::pair<int, int>> &holes)
    {

        std::set<std::pair<int, std::pair<int, int>>> bfsq;

        for (auto pp : holes)
            bfsq.insert({0, pp});

        std::vector<std::vector<bool>> vis(h, std::vector<bool>(w, 0));

        while (!bfsq.empty())
        {
            auto pp = *bfsq.begin();
            bfsq.erase(bfsq.begin());

            int x = pp.second.first;
            int y = pp.second.second;

            deadlockTable[x][y] = 0; // reachable
            vis[x][y] = 1;

            int c = pp.first;
            c++;
            for (int i = 0; i < 4; i++)
            {
                if (isWall(x + dx[i], y + dy[i]) || isWall(x + 2 * dx[i], y + 2 * dy[i]))
                    continue;

                if (vis[x + dx[i]][y + dy[i]])
                    continue;

                bfsq.insert({c, {x + dx[i], y + dy[i]}});
            }
        }
    }

    //checks if the co-ordinates are inside level bounds
    bool isInsideBounds(int x, int y) const
    {
        if (x < h && x >= 0 && y < w && y >= 0)
            return 1;
        return 0;
    }

    //checks for wall
    bool isWall(int x, int y) const
    {
        if (!isInsideBounds(x, y))
        {
            std::cerr << "Queries for wrong position" << std::endl;
            return 1;
        }
        if (level[x][y] == _WALL_)
            return 1;
        return 0;
    }

    bool isWall(std::pair<int, int> pos)
    {
        int x = pos.first;
        int y = pos.second;
        if (!isInsideBounds(x, y))
        {
            std::cerr << "Queries for wrong position" << std::endl;
            return 1;
        }
        if (level[x][y] == _WALL_)
            return 1;
        return 0;
    }

    //checks if the block is blocked vertically by walls
    bool isVerticallyBlocked(std::pair<int, int> pos)
    {
        EfficientMovesLookup possibleMove(pos); // outputs cordinates in perticular direction
        return (isWall(possibleMove['U']) || isWall(possibleMove['D']));
    }

    //checks if the block is blocked horizontally by walls
    bool isHorizontallyBlocked(std::pair<int, int> pos)
    {
        EfficientMovesLookup possibleMove(pos); // outputs cordinates in perticular direction
        return (isWall(possibleMove['R']) || isWall(possibleMove['L']));
    }

    //checks for deadlocks caused by box positions
    bool checkDynamicDeadlock(std::pair<int, int> pos, ProblemState &state)
    {
        int x = pos.first;
        int y = pos.second;
        if (deadlockTable[x][y])
            return 1;

        std::map<char, std::pair<int, int>> possibleMove;

        for (int i = 0; i < 4; i++)
            possibleMove[dir[i]] = {x + dx[i], y + dy[i]};

        if (
            isVerticallyBlocked(pos) &&
            ((state.hasBoxAt(possibleMove['R']) && isVerticallyBlocked(possibleMove['R'])) ||
             (state.hasBoxAt(possibleMove['L']) && isVerticallyBlocked(possibleMove['L']))))
            return 1;

        if (
            isHorizontallyBlocked(pos) &&
            ((state.hasBoxAt(possibleMove['U']) && isHorizontallyBlocked(possibleMove['U'])) ||
             (state.hasBoxAt(possibleMove['D']) && isHorizontallyBlocked(possibleMove['D']))))
            return 1;

        return 0;
    }


    //checks for dynamic and static deadlock
    bool isDeadLock(std::pair<int, int> pos, ProblemState &state)
    {
        return (deadlockTable[pos.first][pos.second] || checkDynamicDeadlock(pos, state));
    }

    /*
    Deadend: A state which will never result in a solution
    check if the state is deadend 
    */
    bool isDeadEnd(ProblemState &state)
    {

        static std:: unordered_map<long long , bool> memoiseResults;
        std::set<std::pair<int,int>> reachablePos;
        for(int i=0 ; i<h; i++)
        for(int j=0; j<w;j++) 
        {
            if(!isWall({i,j}) && !state.hasBoxAt({i,j})) reachablePos.insert({i,j});
        }

        unsigned long long seed= hashSetOfPairs(reachablePos);
        seed= hashSetOfPairs(state.boxes,seed);
        if(memoiseResults.count(seed)) return memoiseResults[seed];
        memoiseResults[seed]=0;
        std:: set<std:: pair<int,int>> solvedBoxInDeadlock;
        for (auto pp : state.boxes)
        {
            if (ProblemState::holes.count(pp))
            {
                if (isDeadLock(pp, state)) solvedBoxInDeadlock.insert(pp);
                continue;
            }
            if (isDeadLock(pp, state))
            {
                memoiseResults[seed]=true;
                return true;
            }
        }
        
        for(auto pp: state.boxes)
        {
            if (ProblemState::holes.count(pp))
                continue;
            

            std:: set<std:: pair<int,std::pair<int,int>>> bfsq;

            bfsq.insert({0,pp});
            std:: unordered_set<long long> vis;
            int f=0;
            while (bfsq.size())
            {
                auto cur= *bfsq.begin();
                bfsq.erase(cur);
                int c= cur.first;
                long long x= cur.second.first;
                long long y = cur.second.second; 
                if(cur.second==state.robo) 
                {
                    f=1;
                    break;
                }          
                vis.insert(x+ (y<<20));
                c++;
                EfficientMovesLookup movesLookup(cur.second);

                if(!(
                    (vis.count(1ll*movesLookup['U'].first+(1ll*movesLookup['U'].second<<20)))
                    ||
                    (isWall(movesLookup['U']))
                    ||
                    (solvedBoxInDeadlock.count(movesLookup['U']))
                    ||
                    (state.hasBoxAt(movesLookup['U']) && isHorizontallyBlocked(movesLookup['U'])
                    &&state.hasBoxAt(cur.second)) 
                    )
                ) bfsq.insert({c,movesLookup['U']}) ;
                
                if(!(
                    (vis.count(1ll*movesLookup['D'].first+(1ll*movesLookup['D'].second<<20)))
                    ||
                    (isWall(movesLookup['D']))
                    ||
                    (solvedBoxInDeadlock.count(movesLookup['D']))
                    ||
                    (state.hasBoxAt(movesLookup['D']) && isHorizontallyBlocked(movesLookup['D'])
                    &&state.hasBoxAt(cur.second)) 
                    )
                ) bfsq.insert({c,movesLookup['D']}) ;
                if(!(
                    (vis.count(1ll*movesLookup['L'].first+(1ll*movesLookup['L'].second<<20)))
                    ||
                    (isWall(movesLookup['L']))
                    ||
                    (solvedBoxInDeadlock.count(movesLookup['L']))
                    ||
                    (state.hasBoxAt(movesLookup['L']) && isVerticallyBlocked(movesLookup['L'])
                    &&state.hasBoxAt(cur.second)) 
                    )
                ) bfsq.insert({c,movesLookup['L']}) ;

                if(!(
                    (vis.count(1ll*movesLookup['R'].first+(1ll*movesLookup['R'].second<<20)))
                    ||
                    (isWall(movesLookup['R']))
                    ||
                    (solvedBoxInDeadlock.count(movesLookup['R']))
                    ||
                    (state.hasBoxAt(movesLookup['R']) && isVerticallyBlocked(movesLookup['R'])
                    &&state.hasBoxAt(cur.second)) 
                    )
                ) bfsq.insert({c,movesLookup['R']}) ;

            }
            if(!f)
            { 
                memoiseResults[seed]=true;
                return 1;
            }
        }
        return false;
    }
    
    std ::vector<std::string> &getLevelVector()
    {
        return this->level;
    }
    long long averageDimension()
    {
        return h+w;
    }
};



class Problem
{

public:
    Level level; // level abstraction
    ProblemState startState;
    long long nodesExpanded;

    Problem(std::vector<std::string> &level_representation)
    {
        this->level = Level(level_representation);
        this->startState = level.getStartState();
        nodesExpanded = 0;
    }

    bool isGoalState(ProblemState &state)
    {
        for (auto box_position : state.boxes)
        {
            if (ProblemState::holes.count(box_position))
                continue;
            else
                return 0;
        }
        return 1;
    }

    std::vector<ProblemState> getSuccessorStates(ProblemState &state)
    {
        std::pair<int, int> cur_pos = state.robo;
        std::set<std::pair<int, int>> &holes = ProblemState::holes;
        std::set<std::pair<int, int>> cur_boxes = state.boxes;
        std::string cur_actions = state.actions;
        int cur_x = cur_pos.first;
        int cur_y = cur_pos.second;
        std::vector<ProblemState> successorStates;

        for (int i = 0; i < 4; i++)
        {
            int new_x = cur_x + dx[i];
            int new_y = cur_y + dy[i];
            if (!level.isInsideBounds(new_x, new_y) || level.isWall(new_x, new_y))
                continue;

            if (!state.hasBoxAt({new_x, new_y}))
            {
                std::pair<int, int> pp = {new_x, new_y};
                std::set<std::pair<int, int>> new_boxes = cur_boxes;
                ProblemState successorState(new_boxes, pp, cur_actions + dir[i]);
                if (!level.isDeadEnd(successorState))
                    successorStates.push_back(successorState);
                continue;
            }

            if (state.hasBoxAt({new_x, new_y}) && !state.hasBoxAt({new_x + dx[i], new_y + dy[i]}) && !level.isWall(new_x + dx[i], new_y + dy[i]))
            {
                std::pair<int, int> new_pos = {new_x, new_y};
                std::set<std::pair<int, int>> new_boxes = cur_boxes;

                new_boxes.erase({new_x, new_y});
                new_boxes.insert({new_x + dx[i], new_y + dy[i]});
                ProblemState successorState(new_boxes, new_pos, cur_actions + dir[i]);
                if (!level.isDeadEnd(successorState))
                    successorStates.push_back(successorState);
                continue;
            }
        }
        nodesExpanded++;
        return successorStates;
    }
};