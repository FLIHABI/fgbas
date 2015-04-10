#include <fstream>
#include <iostream>
#include <vector>

#include "assembler.hh"

std::vector<char> convert_instructions(std::ifstream& is)
{
  std::vector<char> bytecode;
  std::string current_line;

  while (std::getline(is, current_line))
  {
    //TODO: process mnemonics
  }

  return bytecode;
}

void assemble(const std::string& in_file, const std::string& out_file)
{
  std::ifstream is(in_file);
  if (!is)
  {
    std::cerr << "Cannot open file '" << in_file << "'" << std::endl;
    std::exit(1);
  }

  std::vector<char> bytecode = convert_instructions(is);
  is.close();

  std::ofstream os(out_file);
  if (!os)
  {
    std::cerr << "Cannot create file '" << out_file << "'" << std::endl;
    std::exit(1);
  }

  os.write(bytecode.data(), bytecode.size());
  os.close();
}
