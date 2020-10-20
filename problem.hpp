//#include <bits/stdc++.h>
#include "helper.hpp"


/*
abstract representation of state
*/
class ProblemState
{

public:
    std::set<std::pair<int, int>> boxes;        // location of boxes
    inline static std::set<std::pair<int, int>> holes; // location of holes common for all ProblemState obj hence static
    std::pair<int, int> robo;              // location of the agent/player
    std::string actions;
    ProblemState(std::set<std::pair<int, int>> boxes, std::pair<int, int> robo, std::string actions="")
    {
        this->boxes = boxes;
        this->robo = robo;
        this->actions= actions;
    }
    ProblemState()
    {
    }

    bool hasBoxAt(std::pair<int,int> pos)
    {
        return boxes.count(pos);
    }

    bool operator< (const ProblemState  & P) const
    {
        return actions < P.actions;
    }

};

class Level
{
private:
    int h, w; // height and width 
    std::vector<std::string> level; // padded level representation
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
        
        ProblemState:: holes= holes;

        return ProblemState(boxes,ini_robo_pos);
    }

    bool isInsideBounds(int x,int y)
    {
        if(x<h && x>=0&& y<w && y>=0) return 1;
        return 0;
    }

    bool isWall(int x, int y)
    {
        if(!isInsideBounds(x,y))
        {
            std::cerr<<"Queries for wrong position"<<std::endl;
            return 1;
        }
        if(level[x][y]==_WALL_) return 1;
        return 0;
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
        nodesExpanded=0;
    }

    bool isGoalState(ProblemState & state)
    {
        for(auto box_position: state.boxes)
        {
            if(ProblemState:: holes.count(box_position))continue;
            else return 0;
        }
        return 1;
    }

    std::vector<ProblemState>  getSuccessorStates( ProblemState & state)
    {
        std::pair<int,int> cur_pos= state.robo;
        std::set<std::pair<int,int>> & holes= ProblemState:: holes;
        std::set<std::pair<int,int>> cur_boxes= state.boxes;
        std::string cur_actions= state.actions;
        int cur_x= cur_pos.first;
        int cur_y= cur_pos.second;
        std::vector<ProblemState> successorStates;

        for(int i=0; i<4; i++)
        {
            int new_x= cur_x+dx[i];
            int new_y= cur_y+dy[i];
            if(!level.isInsideBounds(new_x,new_y)  || level.isWall(new_x,new_y) ) continue;

            if(! state.hasBoxAt({new_x,new_y}) ) 
            {
                std::pair<int,int> pp={new_x,new_y};
                std::set<std::pair<int,int>> new_boxes=cur_boxes;
                ProblemState  successorState(new_boxes,pp,cur_actions+dir[i]);
                successorStates.push_back(successorState);
                continue;
            }

            if(state.hasBoxAt({new_x,new_y}) && !state.hasBoxAt({new_x+dx[i],new_y+dy[i]})  &&  !level.isWall(new_x+dx[i],new_y+dy[i]))
            {
                std::pair<int,int> new_pos= {new_x,new_y};
                std::set<std::pair<int,int>> new_boxes= cur_boxes;

                new_boxes.erase({new_x,new_y});
                new_boxes.insert({new_x+dx[i],new_y+dy[i]});
                ProblemState successorState(new_boxes,new_pos,cur_actions+dir[i]);
                successorStates.push_back(successorState);
                continue;
            }
        }
        nodesExpanded++;
        return successorStates;
    }



};