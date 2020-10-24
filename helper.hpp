#include <bits/stdc++.h>

const char _BOX_ON_FLOOR_ = '$';
const char _WALL_ = '#';
const char _HOLE_WITHOUT_BOX_ = '.';
const char _HOLE_WITH_BOX_ = '*';
const char _BOX_IN_HOLE_ = '*';
const char _ROBOT_ON_HOLE_ = '+';
const char _HOLE_WITH_ROBOT_ = '+';
const char _ROBOT_ON_FLOOR_ = '@';

const int dx[] = {1, 0, -1, 0};
const int dy[] = {0, 1, 0, -1};
const std::array<char, 4> dir = {'D', 'R', 'U', 'L'};
const std::map<char, int> rDir = {
                                    {'D', 0},
                                    {'R', 1},
                                    {'U', 2},
                                    {'L', 3}
                                };


class EfficientMovesLookup
{
    std::pair<int,int> pos;

    public:
    
    EfficientMovesLookup(std::pair<int,int> & p)
    {
        this->pos=p;
    }

    std::pair<int,int>  operator[] ( char ch)
    {
        int id=0;
        if(ch=='D') id=0;
        else if(ch=='R') id=1;
        else if(ch=='U') id=2;
        else if(ch=='L') id=3;
        else std::cerr<<"Invalid lookup for moves"<<ch<<std::endl;

        return {pos.first+dx[id],pos.second+dy[id]};
    }
};

template <class T>
inline bool remax(T &a, T b) // assign max value to a
{
    return a < b ? a = b, 1 : 0;
}

template <class T>
inline bool remin(T &a, T b) //  assign min value to a
{
    return a > b ? a = b, 1 : 0;
}


long long hashSetOfPairs(std::set<std:: pair<int,int>> &inp, unsigned long long seed=0 )
{
        seed^= inp.size();
        for(auto & pp: inp)
        {
            seed^= 1ll*pp.first + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            seed^= 1ll*pp.second + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            seed^= (1ll*pp.first+1ll*pp.second) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
}


inline void progressLogger(long long & cnt,long long epochLen, std::string & actions)
{
    static long long last=0;

    if(( (last+1)<<epochLen   ) <cnt) {std::cerr<<((last+1)<<epochLen)<<" "<<actions<<std::endl; last++;}

}