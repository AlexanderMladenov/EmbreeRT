/* The MIT License(MIT)
*
* Copyright(c) 2014 Alexander Mladenov
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files(the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions :
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE. */


#ifndef __OBJREADER_H
#define __OBJREADER_H

using std::tuple;
using std::vector;

namespace embRT
{
    struct Vertex { float x, y, z, dummy; };
    struct Triangle { int v0, v1, v2; };

    typedef tuple<vector<float>, vector<float>, vector<float>> PositionsNormalsUVs;
    std::string strip(const std::string& s)
    {
        size_t spacesFront = 0;
        size_t spacesBack = 0;

        for (char c : s)
        {
            if (c != ' ')
            {
                break;
            }

            spacesFront++;
        }

        for (auto it = s.rbegin(); it != s.rend(); it++)
        {
            if (*it != ' ')
            {
                break;
            }

            spacesBack++;
        }

        std::stringstream ss;
        for (auto it = s.begin() + spacesFront; it != s.end() - spacesBack; it++)
        {
            if (it < s.end() - spacesBack - 1)
            {
                if (*it == ' ' && *(it + 1) == ' ')
                {
                    continue;
                }
            }

            ss << *it;
        }

        auto s = ss.str();
        return s;
    }

}

#endif