#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "assembler.hh"
#include "mnemonics.hh"

std::vector<char> convert_instructions(std::ifstream& is)
{
  std::vector<char> bytecode;
  std::string current_line;
  int line = 1;

  while (std::getline(is, current_line))
  {
    std::string mnemonic;
    std::istringstream iss(current_line);

    if ((iss >> mnemonic))
    {
      try
      {
        handle_mnemonic(mnemonic, iss, bytecode);
      }
      catch(std::exception& e)
      {
        std::cerr << "Parse error line " << line << ": " << e.what() << std::endl;
        is.close();
        std::exit(1);
      }
    }

    line++;
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
