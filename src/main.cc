#include <iostream>
#include <string>

#include "assembler.hh"

void check_args(int argc, char* bin_name)
{
  if (argc == 2 || argc == 3)
    return;

  std::cerr << "Usage: " << bin_name << " <file> [<output>]" << std::endl;
  std::exit(1);
}

int main(int argc, char *argv[])
{
  check_args(argc, argv[0]);

  std::string output = argc == 3 ? argv[2] : "out.bin";
  std::string input = argv[1];

  assemble(input, output);

  return 0;
}
