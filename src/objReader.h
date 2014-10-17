/* The MIT License(MIT)
*
* Copyright(c) 2014 Alexander Dzhoganov, Alexander Mladenov
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
using std::string;

namespace embRT
{
    __declspec(align(16)) struct Vertex
    {
        float x, y, z;
    };

    struct Triangle
    {
        int v[3];
    };

    typedef tuple<vector<Vertex>, vector<Vertex>, vector<float>, vector<Triangle>> PositionsNormalsUVsTris;

    class OBJMeshProvider
    {
    public:
        static vector<string> lines;

        static string strip(const string& s)
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

            return ss.str();
        }

        static vector<string> split(const string& s, char delim)
        {
            vector<string> elems;
            std::stringstream ss(s);
            string item;
            while (getline(ss, item, delim))
            {
                elems.push_back(item);
            }
            return elems;
        }

        static Triangle constructTriangle(std::string a, std::string b, std::string c)
        {
            Triangle result;
            string items[3] = { a, b, c };

            for (int i = 0; i < 3; i++)
            {
                const string& item = items[i];

                vector<string> subItems = split(item, '/');
                result.v[i] = stoi(subItems[0]);
            }
            return result;
        }

        static size_t splitNewlines(const string& s)
        {
            char* ptr = (char*)s.c_str();
            char* start = ptr;

            size_t newLinesCount = 0;

            for (auto i = 0u; i < s.size(); i++)
            {
                if (ptr[i] == '\n')
                {
                    ptr[i] = '\0';
                    auto length = (ptr + i) - start;
                    lines.emplace_back(start, length);
                    start = ptr + i + 1;
                }
            }
            return lines.size();
        }

        static vector<string>& readFile(const string& fileName)
        {
            std::cout << "Reading file: " << fileName << std::endl;

            std::ifstream f(fileName);
            if (!f.is_open())
            {
                throw std::runtime_error("error opening file");
            }

            string s;

            f.seekg(0, std::ios::end);
            auto length = f.tellg();
            s.reserve(length);
            f.seekg(0, std::ios::beg);

            s.assign(std::istreambuf_iterator<char>(f), std::istreambuf_iterator<char>());

            auto linesCount = splitNewlines(s);

            std::cout << "Done! " << linesCount << " lines read" << std::endl;
            return lines;
        }

        static PositionsNormalsUVsTris extractData(size_t startLine, size_t endLine)
        {
            std::cout << "Extracting data.." << std::endl;
            vector<Vertex> positions;
            vector<Vertex> normals;
            vector<float> uvs;
            vector<Triangle> tris;
            tris.push_back(Triangle());
            positions.push_back(Vertex());

            for (auto l = startLine; l < endLine; l++)
            {
                auto line = lines[l];
                auto stripped = strip(line);

                if (line.size() < 6)
                {
                    continue;
                }

                auto components = split(stripped, ' ');

                if (stripped[0] == '#')
                {
                    continue;
                }

                if (components[0] == "v")
                {
                    Vertex v;
                    v.x = stof(components[1]);
                    v.y = stof(components[2]);
                    v.z = stof(components[3]);
                    positions.push_back(v);
                }

                if (components[0] == "vn")
                {
                    Vertex n;
                    n.x = stof(components[1]);
                    n.y = stof(components[2]);
                    n.z = stof(components[3]);
                    normals.push_back(n);
                }

                if (components[0] == "vt")
                {
                    uvs.push_back(stof(components[1]));
                    uvs.push_back(stof(components[2]));
                }
                if (components[0] == "f")
                {
                    int numTriangles = components.size() - 3;

                    for (int i = 0; i < numTriangles; i++)
                    {
                        auto T = constructTriangle(components[1], components[2 + i], components[3 + i]);
                        tris.push_back(T);
                    }
                }
            }
            std::cout << "Done!" << std::endl;
            return PositionsNormalsUVsTris(positions, normals, uvs, tris);
        }
    };
}

#endif