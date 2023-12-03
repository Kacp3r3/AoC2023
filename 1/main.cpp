#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>

int processLine(std::string& line)
{
    int left = 0;
    int right = 0;
    //std::cout << line << std::endl;
    for(int i=0;i<line.length();++i)
    {
        char letter = line[i];
        if(letter >= '0' && letter <= '9')
        {
            left = (letter - '0') * 10;
            break;
        }
    }

    for(int i=line.length()-1;i>=0;--i)
    {
        char letter = line[i];
        if(letter >= '0' && letter <= '9')
        {
            right = (letter - '0');
            break;
        }
    }

    //std::cout << left+right << std::endl;
    return left+right;
}

void parseLines(std::vector<std::string>& nums, std::vector<std::string>& lines)
{
    for(auto& line : lines)
    {
        std::string new_value="";
        std::vector<int> hitsLeft;
        for(int i=0;i<nums.size(); ++i)
        {
            hitsLeft.push_back(line.find(nums[i]));
        }

        size_t smallest= std::string::npos;
        int indexLeft = -1;
        for(int i=0;i<hitsLeft.size();++i)
        {
            if(hitsLeft[i] < smallest)
            {
                smallest = hitsLeft[i];
                indexLeft = i;
            }
        }
        if(indexLeft == -1 && smallest == std::string::npos)
        {
            for(int i=0;i<line.size();++i)
            {
                if(line[i] >= '0' && line[i] <= '9')
                {
                    new_value = line[i];
                    break;
                }
            }
        }
        else
        {
             //Sprawdzamy czy nie poprzedza go jakas liczba
            bool greenFlag = true;
            for(int i=0;i<hitsLeft[indexLeft];++i)
            {
                if(line[i] >= '0' && line[i] <= '9')
                {
                    new_value = line[i];
                    greenFlag=false;
                    break;
                }
            }

            if(greenFlag)
            {
                new_value = std::to_string(indexLeft+1);
                //line.replace(smallest, nums[indexLeft].length(), std::to_string(indexLeft));
            }
        }

       

        //Now from right
        std::vector<int> hitsRight;
        for(int i=0;i<nums.size(); ++i)
        {
            hitsRight.push_back(line.rfind(nums[i]));
        }

        long long biggest = -1;
        int indexRight = -1;
        for(int i=0;i<hitsRight.size();++i)
        {
            if(hitsRight[i] > biggest && hitsRight[i] != std::string::npos)
            {
                biggest = hitsRight[i];
                indexRight = i;
            }
        }

        if(indexRight == -1 && biggest == -1)
        {
            for(int i=line.size()-1;i>=0;--i)
            {
                if(line[i] >= '0' && line[i] <= '9')
                {
                    new_value += line[i];
                    break;
                }
            }
        }
        else
        {
             //Sprawdzamy czy nie poprzedza go jakas liczba
            bool greenFlag = true;
            for(int i=line.size()-1;i>=hitsRight[indexRight];--i)
            {
                if(line[i] >= '0' && line[i] <= '9')
                {
                    new_value += line[i];
                    greenFlag=false;
                    break;
                }
            }

            if(greenFlag)
            {
                new_value += std::to_string(indexRight+1);
                //line.replace(smallest, nums[indexLeft].length(), std::to_string(indexLeft));
            }
        }

        
        line = new_value;
        //std::cout << line << std::endl;
    }
}

void parseLines(std::map<std::string, int>& nums, std::vector<std::string>& lines)
{
    for(auto& line : lines)
    {
        std::string new_value = "";
        for(auto& pair : nums)
        {
            while(line.find(pair.first) != std::string::npos)
            {
                line.replace(line.find(pair.first), pair.first.length(), std::to_string(pair.second));
            }
        }

        //std::cout << line << std::endl;
    }
}
int main()
{
    std::ifstream in;
    in.open("input.txt");
    //in.open("sample.txt");
    if(in.good())
    {
        int sum = 0;
        std::string tmp;
        std::vector<std::string> lines;
        lines.reserve(1000);

        while(!in.eof())
        {
            std::getline(in, tmp);
            lines.emplace_back(tmp);
        }


        std::vector<std::string> numbers {"one","two","three","four","five","six","seven","eight","nine"};
        //std::map<std::string, int> concatNumbers {{"oneight", 18},{"twone",21},{"threeight",38}, {"fiveight",58}, {"sevenine",79}, {"eightwo", 82}, {"eighthree", 83}, {"nineight", 98}};
        //parseLines(concatNumbers, lines);
        parseLines(numbers, lines);


        for(auto& line : lines)
        {
            sum += processLine(line);
        }

        std::cout << sum << std::endl;
    }
    else
    {
        std::cout<<"Cannot open file\n";
    }
    return 0;
}