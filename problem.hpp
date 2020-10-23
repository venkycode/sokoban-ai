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
    std::string actions;
    bool hashed;
    unsigned long long hashv;
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

    bool hasBoxAt(std::pair<int, int> pos)
    {
        return boxes.count(pos);
    }

    bool operator<(const ProblemState &P) const
    {
        return actions < P.actions;
    }

    bool operator==(const ProblemState &anotherState) const
    {
        return ((boxes == anotherState.boxes) && (robo == anotherState.robo));
    }

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

class Level
{
private:
    int h, w;                       // height and width
    std::vector<std::string> level; // padded level representation
    std::vector<std::vector<bool>> deadlockTable;

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
                if (level_representation[i][j] == _WALL_)
                    break;
                level_representation[i][j] = _WALL_;
            }
            for (int j = cw; j < w; j++)
            {
                level_representation[i].push_back(_WALL_);
            }
        }
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
            deadlockTable[x][y] = 0;
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

    bool isInsideBounds(int x, int y)
    {
        if (x < h && x >= 0 && y < w && y >= 0)
            return 1;
        return 0;
    }

    bool isWall(int x, int y)
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

    bool isVerticallyBlocked(std:: pair<int,int>  pos )
    {
        int x= pos.first;
        int y= pos.second;

        std::map<char, std::pair<int, int>> possibleMove;

        for (int i = 0; i < 4; i++)
            possibleMove[dir[i]] = {x + dx[i], y + dy[i]};


        return (isWall(possibleMove['U'])||isWall(possibleMove['D']));
        
    }

    bool isHorizontallyBlocked(std:: pair<int,int>  pos )
    {
        int x= pos.first;
        int y= pos.second;

        std::map<char, std::pair<int, int>> possibleMove;

        for (int i = 0; i < 4; i++)
            possibleMove[dir[i]] = {x + dx[i], y + dy[i]};


        return (isWall(possibleMove['R'])||isWall(possibleMove['L']));

    }

    bool checkDynamicDeadlock(std::pair<int, int> pos, ProblemState &state)
    {
        int x = pos.first;
        int y = pos.second;
        if (deadlockTable[x][y])
            return 1;

        std::map<char, std::pair<int, int>> possibleMove;

        for (int i = 0; i < 4; i++)
            possibleMove[dir[i]] = {x + dx[i], y + dy[i]};

        if(
            isVerticallyBlocked(pos) 
            &&
            (
                (state.hasBoxAt(possibleMove['R'])&& isVerticallyBlocked(possibleMove['R']))
                ||
                (state.hasBoxAt(possibleMove['L'])&& isVerticallyBlocked(possibleMove['L'])) 
            )            
        ) return 1;

        if(
            isHorizontallyBlocked(pos) 
            &&
            (
                (state.hasBoxAt(possibleMove['U'])&& isHorizontallyBlocked(possibleMove['U']))
                ||
                (state.hasBoxAt(possibleMove['D'])&& isHorizontallyBlocked(possibleMove['D'])) 
            )            
        ) return 1;

        return 0;
    }

    bool isDeadLock(std::pair<int, int> pos, ProblemState &state)
    {
        return (deadlockTable[pos.first][pos.second] || checkDynamicDeadlock(pos, state));
    }

    bool isDeadEnd(ProblemState &state)
    {
        for (auto pp : state.boxes)
        {
            if (ProblemState::holes.count(pp))
                continue;
            if (isDeadLock(pp, state))
                return true;
        }
        return false;
    }
    std ::vector<std::string> &getLevelVector()
    {
        return this->level;
    }
};

class Problem
{

public:
    Level level;
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