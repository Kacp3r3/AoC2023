#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>

std::vector<std::string> split(std::string to_split, std::string delimiter)
{
    std::vector<std::string> rv;

    char *cstr = new char[to_split.length() + 1];
    strcpy(cstr, to_split.c_str());

    char * p = strtok(cstr, delimiter.c_str());
    
    do
    {
        rv.emplace_back(p);
        p = strtok(NULL, delimiter.c_str());
    }while( p != NULL );

    delete [] cstr;
    return rv;
}

int processGamePowers(std::string& game)
{
    //only 12 red cubes, 13 green cubes, and 14 blue cubes
    int MAX_RED = 0;
    int MAX_GREEN = 0;
    int MAX_BLUE = 0;

    bool possible = true;
    //Game 1: 4 red, 5 blue, 4 green; 7 red, 8 blue, 2 green; 9 blue, 6 red; 1 green, 3 red, 7 blue; 3 green, 7 red
    std::string results  = split(game, ":")[1];
    //4 red, 5 blue, 4 green; 7 red, 8 blue, 2 green; 9 blue, 6 red; 1 green, 3 red, 7 blue; 3 green, 7 red

    auto rounds = split(results, ";");
    // 4 red, 5 blue, 4 green
    // 7 red, 8 blue, 2 green
    // 9 blue, 6 red; 1 green, 3 red, 7 blue
    // 3 green, 7 red
    for( auto& round : rounds)
    {
        auto& cubes = split(round, ",");
        // 4 red
        // 5 blue
        // 4 green

        for(auto& count : cubes)
        {
            auto& pair = split(count," ");
            std::string color = pair[1];
            int value = atoi(pair[0].c_str());

            if(MAX_RED < value && color == "red")
            {
                MAX_RED = value;
            }
            else if(MAX_BLUE < value && color == "blue")
            {
                MAX_BLUE = value;
            }
            else if(MAX_GREEN < value && color == "green")
            {
                MAX_GREEN = value;
            }
        }
    }
    return MAX_BLUE*MAX_GREEN*MAX_RED;
}

int processGame(std::string& game, int id)
{
    //only 12 red cubes, 13 green cubes, and 14 blue cubes
    const int MAX_RED = 12;
    const int MAX_GREEN = 13;
    const int MAX_BLUE = 14;

    bool possible = true;
    //Game 1: 4 red, 5 blue, 4 green; 7 red, 8 blue, 2 green; 9 blue, 6 red; 1 green, 3 red, 7 blue; 3 green, 7 red
    std::string results  = split(game, ":")[1];
    //4 red, 5 blue, 4 green; 7 red, 8 blue, 2 green; 9 blue, 6 red; 1 green, 3 red, 7 blue; 3 green, 7 red

    auto rounds = split(results, ";");
    // 4 red, 5 blue, 4 green
    // 7 red, 8 blue, 2 green
    // 9 blue, 6 red; 1 green, 3 red, 7 blue
    // 3 green, 7 red
    for( auto& round : rounds)
    {
        auto& cubes = split(round, ",");
        // 4 red
        // 5 blue
        // 4 green

        for(auto& count : cubes)
        {
            auto& pair = split(count," ");
            std::string color = pair[1];
            int value = atoi(pair[0].c_str());

            if((color == "green" && value > MAX_GREEN) ||
               (color == "red" && value > MAX_RED) ||
               (color == "blue" && value > MAX_BLUE))
            {
                possible = false;
                break;
            }
        }

        if(!possible) break;
    }
    return possible ? id : 0;
}

int main()
{
    std::ifstream in;
    in.open("input.txt");
    //in.open("sample.txt");
    if(in.good())
    {
        int sumID = 0;
        int powers = 0;
        std::string tmp;
        std::vector<std::string> lines;
        lines.reserve(100);

        while(!in.eof())
        {
            std::getline(in, tmp);
            lines.emplace_back(tmp);
        }

        for(int i=0;i<lines.size();++i)
        {
            sumID += processGame(lines[i], i+1);
        }

        for(int i=0;i<lines.size();++i)
        {
            powers += processGamePowers(lines[i]);
        }


        std::cout << "Sum ID: " << sumID << std::endl;
        std::cout << "Sum Power: " << powers << std::endl;
    }
    else
    {
        std::cout<<"Cannot open file\n";
    }
    return 0;
}