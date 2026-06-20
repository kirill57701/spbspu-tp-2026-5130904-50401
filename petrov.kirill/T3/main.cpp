#include "polygon.hpp"
#include "funcs.hpp"
#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char** argv)
{
  if (argc != 2)
  {
    std::cerr << "Invalid arguments\n";
    return 1;
  }

  std::ifstream inputFile(argv[1]);
  if (!inputFile)
  {
    std::cerr << "Failed to open file\n";
    return 1;
  }

  std::vector< petrov::Polygon > storage;
  while (!inputFile.eof())
  {
    petrov::Polygon poly;
    if (inputFile >> poly)
    {
      storage.push_back(poly);
    }
    else
    {
      if (!inputFile.eof())
      {
        inputFile.clear();
        inputFile.ignore(1);
      }
    }
  }

  std::map< std::string, petrov::CommandAction > commands = petrov::createCommands();
  std::string currentCommand;

  while (std::cin >> currentCommand)
  {
    auto it = commands.find(currentCommand);
    if (it == commands.end())
    {
      std::cout << "<INVALID COMMAND>\n";
      petrov::skipLine(std::cin);
    }
    else
    {
      it->second(std::cin, std::cout, storage);
    }
  }

  return 0;
}
