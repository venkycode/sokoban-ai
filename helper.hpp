#include<bits/stdc++.h>

 
const char _BOX_ON_FLOOR_ = '$';
const char _WALL_ = '#';
const char _HOLE_WITHOUT_BOX_ = '.';
const char _HOLE_WITH_BOX_ = '*';
const char _BOX_IN_HOLE_ = '*';
const char _ROBOT_ON_HOLE_ = '+';
const char _HOLE_WITH_ROBOT_ = '+';
const char _ROBOT_ON_FLOOR_ = '@';

const int dx[]={1,0,-1,0};
const int dy[]={0,1,0,-1};
const std::array<char,4> dir={'D','R','U','L'};


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



 