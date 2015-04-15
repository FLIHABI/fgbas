#include <unordered_map>
#include <algorithm>
#include <iostream>

#include "mnemonics.hh"
#include "commons/asm/bytecodes.hh"

typedef void (*mnemonic_handler_type)(std::istringstream&, std::vector<char>&);

static void add(std::istringstream& iss, std::vector<char>& b)
{
  b.push_back(OP_ADD);
}

static void sub(std::istringstream& iss, std::vector<char>& b)
{
  b.push_back(OP_SUB);
}

static void mul(std::istringstream& iss, std::vector<char>& b)
{
  b.push_back(OP_MUL);
}

static void div(std::istringstream& iss, std::vector<char>& b)
{
  b.push_back(OP_DIV);
}

static void mod(std::istringstream& iss, std::vector<char>& b)
{
  b.push_back(OP_MOD);
}

static std::unordered_map<std::string, mnemonic_handler_type>& get_handlers()
{
  static std::unordered_map<std::string, mnemonic_handler_type> handlers =
  {
    {"add", add},
    {"sub", sub},
    {"mul", mul},
    {"div", div},
    {"mod", mod},
  };

  return handlers;
}

void handle_mnemonic( std::string& mnemonic,
                      std::istringstream& iss,
                      std::vector<char>& bytecode)
{
  std::transform(mnemonic.begin(), mnemonic.end(), mnemonic.begin(), tolower);

  if (get_handlers().find(mnemonic) == get_handlers().end())
  {
    std::cerr << "Unknown mnemonic '" << mnemonic << "'" << std::endl;
    std::exit(1);
  }

  get_handlers().at(mnemonic)(iss, bytecode);
}
