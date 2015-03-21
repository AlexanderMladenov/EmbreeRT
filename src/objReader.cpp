#include "objReader.h"

namespace embRT
{
    vector<string> lines;
    string strip(const string& s)
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

    vector<string> split(const string& s, char delim)
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

    FullTriangleData constructTriangle(std::string a, std::string b, std::string c)
    {
        FullTriangleData result;
        string items[3] = { a, b, c };

        for (int i = 0; i < 3; i++)
        {
            const string& item = items[i];

            vector<string> subItems = split(item, '/');
            result.v[i] = stoi(subItems[0]);
            if (subItems.size() > 2)
            {
                result.n[i] = stoi(subItems[2]);
            }
            else result.n[i] = 0;
        }
        return result;
    }

    size_t splitNewlines(const string& s)
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

    // fix up texture coordinates reading !!!!!
    PositionsNormalsUVsTris extractData(size_t startLine, size_t endLine)
    {
        std::cout << "Parsing data from OBJ.." << std::endl;
        auto t1 = std::chrono::high_resolution_clock::now();

        vector<Vertex> positions;
        positions.reserve(endLine);
        vector<vec3> normals;
        normals.reserve(endLine);
        vector<float> uvs;
        uvs.reserve(endLine);
        vector<FullTriangleData> tris;
        tris.reserve(endLine);

        tris.emplace_back(FullTriangleData());
        positions.emplace_back(Vertex());
        normals.emplace_back(vec3());
        uvs.emplace_back(-1.f);

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
                positions.emplace_back(v);
            }

            if (components[0] == "vn")
            {
                vec3 n;
                n.x = stof(components[1]);
                n.y = stof(components[2]);
                n.z = stof(components[3]);
                normals.emplace_back(n);
            }

            if (components[0] == "vt")
            {
                uvs.emplace_back(stof(components[1]));
                uvs.emplace_back(stof(components[2]));
            }

            if (components[0] == "f")
            {
                int numTriangles = components.size() - 3;

                for (int i = 0; i < numTriangles; i++)
                {
                    auto T = constructTriangle(components[1], components[2 + i], components[3 + i]);
                    tris.emplace_back(T);
                }
            }
        }
        auto t2 = std::chrono::high_resolution_clock::now();
        auto time = timePast(t1, t2);
        auto time2 = timePast<std::chrono::milliseconds>(t1, t2);

        std::cout << "Done! " << "Extracted " << positions.size() - 1 << " vertices " <<
            tris.size() - 1 << " triangles" << " for "
            << time << " sec " << time2 - (time * 1000) << " millisec" << std::endl;

        return PositionsNormalsUVsTris(positions, normals, uvs, tris);
    }

    // Reads a obj mesh file.
    PositionsNormalsUVsTris readOBJ(const string& fileName)
    {
        std::cout << "Reading file: " << fileName << std::endl;
        auto t1 = std::chrono::high_resolution_clock::now();

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

        auto t2 = std::chrono::high_resolution_clock::now();
        auto time = timePast(t1, t2);
        auto time2 = timePast<std::chrono::milliseconds>(t1, t2);
        std::cout << "Done! " << linesCount << " lines read" << " for "
            << time << " sec " << time2 - (time * 1000) << " millisec" << std::endl;

        return extractData(0, lines.size());
    }
} // namespace