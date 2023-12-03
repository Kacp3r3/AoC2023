#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>

struct Coord
{
    int x;
    int y;

    bool operator==(const Coord&rhs) { return x == rhs.x && y == rhs.y;}
};

struct Block
{
    Block() = default;
    Block(int v, Coord tl, Coord br) : value(v), topLeft(tl), bottomRight(br), valid(false){};
    int value;
    Coord topLeft;
    Coord bottomRight;
    Coord gearPosition;
    bool valid = false;
};

std::vector<Block> findBlocks(std::vector<std::string> lines)
{
    std::vector<Block> rv;
    int width = lines[0].size();
    int height = lines.size();

    for(int y = 0; y < lines.size(); ++y)
    {
        bool newNumber = true;
        std::string number = "";
        for(int x=0;x<lines[y].size(); ++x)
        {
            if(newNumber && lines[y][x] >= '0' && lines[y][x] <= '9')
            {
                //We found new number
                number += lines[y][x];
                newNumber = false;
            }
            else if(!newNumber && lines[y][x] >= '0' && lines[y][x] <= '9')
            {
                //We found next digit of a number
                number += lines[y][x];
            }
            else if(!newNumber || (newNumber && x+1 == width  &&  number.size()>0))
            {
                int newX = x-number.length();
                //New block found, save it
                Coord topLeft;
                topLeft.x = newX-1 > 0 ? newX-1 : 0;
                topLeft.y = y-1 > 0 ? y-1 : 0;
                Coord bottomRigh;
                bottomRigh.x = x < width ? x : width-1;
                bottomRigh.y = y + 1 < height ? y+1 : height-1;
                rv.emplace_back(atoi(number.c_str()), topLeft, bottomRigh);
                
                newNumber=true;
                number="";
            }

            if(!newNumber && x+1 == width)
            {
                int newX = x-number.length();
                //New block found, save it
                Coord topLeft;
                topLeft.x = newX-1 > 0 ? newX-1 : 0;
                topLeft.y = y-1 > 0 ? y-1 : 0;
                Coord bottomRigh;
                bottomRigh.x = x < width ? x : width-1;
                bottomRigh.y = y + 1 < height ? y+1 : height-1;
                rv.emplace_back(atoi(number.c_str()), topLeft, bottomRigh);
                
                newNumber=true;
                number="";
            }
        }
    }

    return rv;
}

void filterValidBlocks(std::vector<Block>& blocks, const std::vector<std::string>& engine)
{
    for(auto& block : blocks)
    {
        for(int y=block.topLeft.y;y<=block.bottomRight.y;++y)
        {
            for(int x = block.topLeft.x; x<= block.bottomRight.x; ++x)
            {
                char letter = engine[y][x];
                if( letter != '.' && (letter < '0' || letter > '9'))
                {
                    block.valid = true;
                    break;
                }
            }
            if(block.valid) break;
        }
    }

    auto& it = std::remove_if(blocks.begin(), blocks.end(), [](Block& b){ return !b.valid; });
    blocks.erase(it, blocks.end());
}

void filterBlocksWithGears(std::vector<Block>& blocks, const std::vector<std::string>& engine)
{
    for(auto& block : blocks)
    {
        for(int y=block.topLeft.y;y<=block.bottomRight.y;++y)
        {
            for(int x = block.topLeft.x; x<= block.bottomRight.x; ++x)
            {
                char letter = engine[y][x];
                if( letter == '*')
                {
                    block.valid = true;
                    block.gearPosition.x = x;
                    block.gearPosition.y = y;
                    break;
                }
            }
            if(block.valid) break;
        }
    }

    auto& it = std::remove_if(blocks.begin(), blocks.end(), [](Block& b){ return !b.valid; });
    blocks.erase(it, blocks.end());
}

long long calculateGearRatios(std::vector<Block>& blocks)
{
    long long sum = 0;
    std::for_each(blocks.begin(), blocks.end(), [](Block& b){b.valid=false;});
    for(int i=0;i<blocks.size();++i)
    {
        if(blocks[i].valid==false)
        {
            for(int j=0;j<blocks.size();++j)
            {
                if(i!=j)
                {
                    if(blocks[j].gearPosition == blocks[i].gearPosition)
                    {
                        sum += blocks[i].value * blocks[j].value;
                        blocks[j].valid=true;
                        break;
                    }
                }
            }
            blocks[i].valid=true;
        }
    }
    return sum;
}

//Bad answers 497536
int main()
{
    std::ifstream in;
    in.open("input.txt");
    //in.open("sample.txt");
    if(in.good())
    {
        int sum = 0;
        long long sumGears;
        std::string tmp;
        std::vector<std::string> engine;
        engine.reserve(140);

        while(!in.eof())
        {
            std::getline(in, tmp);
            engine.emplace_back(tmp);
        }
        //....................*..........698*119.......65.511................*....+....-......34......=........*......$..574...971.85.*630............
        //Find position of numbers and its length
        auto& blocks = findBlocks(engine);
        std::vector<Block> blocks2 = blocks;
        //Filter blocks not connected to any symbols
        filterValidBlocks(blocks, engine);

        //Sum all blocks
        for(auto& block : blocks)
        {
            sum += block.value;
        }
        
        std::cout<<"Sum of valid part numbers: " << sum << std::endl;

        //Part 2
        //Find all gear ratios
        filterBlocksWithGears(blocks2, engine);
        
        sum = calculateGearRatios(blocks2);
        std::cout<<"Sum of gear ratios: " << sum << std::endl;
    }
    else
    {
        std::cout<<"Cannot open file\n";
    }
    return 0;
}