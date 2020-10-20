#include <bits/stdc++.h>
#include "helper.hpp"
using namespace std;


/*
abstract representation of state
*/
class ProblemState
{

public:
    set<pair<int, int>> boxes;        // location of boxes
    static set<pair<int, int>> holes; // location of holes common for all ProblemState obj hence static
    pair<int, int> robo;              // location of the agent/player
    string actions;
    ProblemState(set<pair<int, int>> boxes, pair<int, int> robo, string actions="")
    {
        this->boxes = boxes;
        this->robo = robo;
        this->actions= actions;
    }
    ProblemState()
    {
    }

    bool hasBoxAt(pair<int,int> pos)
    {
        return boxes.count(pos);
    }


};

class Level
{
private:
    int h, w; // height and width 
    vector<string> level; // padded level representation
public:
    Level(vector<string> &level_representation)
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
        pair<int, int> ini_robo_pos;
        set<pair<int, int>> boxes, holes;

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
            cerr<<"Queries for wrong position"<<endl;
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
    
    Problem(vector<string> &level_representation)
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

    vector<ProblemState>  getSuccessorStates( ProblemState & state)
    {
        pair<int,int> cur_pos= state.robo;
        set<pair<int,int>> & holes= ProblemState:: holes;
        set<pair<int,int>> cur_boxes= state.boxes;
        string cur_actions= state.actions;
        int cur_x= cur_pos.first;
        int cur_y= cur_pos.second;
        vector<ProblemState> successorStates;

        for(int i=0; i<4; i++)
        {
            int new_x= cur_x+dx[i];
            int new_y= cur_y+dy[i];
            if(!level.isInsideBounds(new_x,new_y)  || level.isWall(new_x,new_y) ) continue;

            if(! state.hasBoxAt({new_x,new_y}) ) 
            {
                pair<int,int> pp={new_x,new_y};
                set<pair<int,int>> new_boxes=cur_boxes;
                ProblemState  successorState(new_boxes,pp,cur_actions+dir[i]);
                successorStates.push_back(successorState);
                continue;
            }

            if(state.hasBoxAt({new_x,new_y}) && !state.hasBoxAt({new_x+dx[i],new_y+dy[i]})  &&  !level.isWall(new_x+dx[i],new_y+dy[i]))
            {
                pair<int,int> new_pos= {new_x,new_y};
                set<pair<int,int>> new_boxes= cur_boxes;

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