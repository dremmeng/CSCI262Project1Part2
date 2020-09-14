/*
    pathfinder.cpp
        
    Method implementations for the pathfinder class.
    
    assignment: CSCI 262 Project - Pathfinder        

    author: Drew Remmenga
*/

#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <sstream>
using namespace std;
#include "pathfinder.h"

using namespace std;

// constructor - optional, in case you need to initialize anything
pathfinder::pathfinder() { ; }

// public run() method - invokes private methods to do everything;
// returns the total elevation change cost of the best optimal path.
int pathfinder::run(string data_file_name, string save_file_name, bool use_dynamic) {
    // suggested implementation - modify as desired!
    _use_dynamic = use_dynamic;
    if (!_read_data(data_file_name)) {
        cout << "Error reading map data from \"" << data_file_name << "\"!" << endl;
        return -1;
    }

    _draw_map();
    drawtables();
    int best_cost = _draw_paths();

    _save_image(save_file_name);

    return best_cost;
}

/*******************/
/* PRIVATE METHODS */
/*******************/

// _read_data - read and store width, height, and elevation data from the
// provided data file; return false if something goes wrong
bool pathfinder::_read_data(string data_file_name) {
    // open input file
    ifstream file(data_file_name);
    string str = " ";
    vector<vector<int>> foo;
    // read in and store width and height
    file >> str;
    file >> _width;
    file >> str;
    file >> _height;
    _elevations = vector<vector<int>>(_height, vector<int>(_width));
    _costs = vector<vector<int>>(_height, vector<int>(_width));
    _direction = vector<vector<int>>(_height, vector<int>(_width));
    // read in and store elevation data
    for (int i = 0; i < _height; i++)
    {
        for (int j = 0; j < _width; j++)
        {
            file >> _elevations[i][j];
        }
    }
    // close input file
    file.close();
    // return true if everything worked
    return true;
}

// _draw_map - draw the elevation data as grayscale values on our Picture
// object.
void pathfinder::_draw_map() {
    //determine min/max elevation values in map
    _min = _elevations[0][0];
    _max = _elevations[0][0];
    for (int i = 0; i < _height; i++)
    {
        for (int j = 0; j < _width; j++)
        {
            if (_elevations[i][j] < _min)
            {
                _min = _elevations[i][j];
            }
            if (_elevations[i][j] > _max)
            {
                _max = _elevations[i][j];
            }
        }
    }
    //from min, max, determine a scaling factor for data to shift
    //       elevations into 0 - 255 range
    float factor =  (_max - _min)/255;
    //for each elevation point, determine its grayscale value and set the Picture point
    for (int i = 0; i < _height; i++)
    {
        for (int j = 0; j < _width; j++)
        {
            int r, g, b;
            r = g = b = (_elevations[i][j] - _min) / factor;
            _image.set(i, j, r, g, b);
        }
    }
}

// _draw_paths() - find and draw optimal paths from west to east using either recursion or dynamic programming
// return the best optimal path cost
int pathfinder::_draw_paths() {
    // based on the _use_recursion field, compute minimum cost for every point on map using either
    //  recursion or dynamic programming. Keep track of next move necessary to attain min cost.
    //  This will require additional data structures!
    drawtables();
    int lowest_cost = 100000000;
    int lowest_cost_height = 0;
    for (int i = 0; i < _height; i++)
    {
        colorpaths(0, 0, 255, i, 0);
        if (_costs[i][0] < lowest_cost)
        {
            lowest_cost = _costs[i][0];
 
         
            lowest_cost_height = i;
        }
        cout << _costs[i][1] << endl;
    }
    cout << lowest_cost << endl;
    colorpaths(255, 255, 0, lowest_cost_height, 0);
    return lowest_cost;
}
void pathfinder::costtoeast(int row, int col)
{
    int cost1 =0, cost2=0, cost3=0;
    if (col == _width-1)
    {
        _costs[row][col] = 0;
        _direction[row][col] = 0;
    }
    else if (col < _width-1)
    {
        cost1 = abs(_elevations[row][col] - _elevations[row][col + 1])+ _costs[row][col+1];
        if (row < _height - 1)
        {
            cost2 = abs(_elevations[row][col] - _elevations[row + 1][col + 1])+ _costs[row+1][col+1];
        }
        else
        {
            cost2 = 1000000;
        }
        if (row > 0)
        {
            cost3 = abs(_elevations[row][col] - _elevations[row - 1][col + 1])+ _costs[row-1][col+1];
        }
        else
        {
            cost3 = 1000000;
        }
    }
    if ((cost1 < cost2) && (cost1 < cost3))
    {
        
        _costs[row][col]= cost1;
       _direction[row][col] = 0;
    }
    else if ((cost2 < cost1) && (cost2 < cost3))
    {
        _costs[row][col] = cost2;
        _direction[row][col] = 1;
    }
    else if ((cost3 < cost2)&&(cost3 < cost1))
    {
        _costs[row][col] = cost3;
        _direction[row][col] = -1;
    }
    else
    {
        _direction[row][col] = 0;
    }
}
void pathfinder::drawtables()
{
    
    for (int j = _width-1; j > 0; j--)
    {
        for (int i = 0; i < _height; i++)
        {
            costtoeast(i, j);
        }
    }
}
void pathfinder::colorpaths(int r, int g, int b, int row, int col)
{
    if (col < _width)
    {
        _image.set(row, col, r, g, b);
        colorpaths(r, g, b, row + _direction[row][col], col + 1);
    }
}


// _save_image - we'll do this one for you :)
void pathfinder::_save_image(string save_file_name) {
    _image.save(save_file_name);
}

