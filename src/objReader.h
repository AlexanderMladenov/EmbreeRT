#ifndef __OBJREADER_H
#define __OBJREADER_H

#include <vector>
#include <tuple>
#include <string>
#include <iostream>
#include <sstream>
#include <chrono>
#include <fstream>

#include "util.h"
#include "types.h"

using std::tuple;
using std::vector;
using std::string;

namespace embRT
{

    extern vector<string> lines;

    string strip(const string& s);

    vector<string> split(const string& s, char delim);

    FullTriangleData constructTriangle(std::string a, std::string b, std::string c);
    
    size_t splitNewlines(const string& s);

    // fix up texture coordinates reading !!!!!
    PositionsNormalsUVsTris extractData(size_t startLine, size_t endLine);

    // Reads a obj mesh file.
    PositionsNormalsUVsTris readOBJ(const string& fileName);

} // namespace

#endif