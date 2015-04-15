#include <unordered_map>
#include <algorithm>
#include <iostream>
#include <sstream>

#include "mnemonics.hh"
#include "commons/asm/bytecodes.hh"

typedef void (*mnemonic_handler_type)(std::istringstream&, std::vector<char>&);

static inline void push_long(long long value, std::vector<char>& v)
{
  for (int i = 0; i < 8; ++i, value >>= 8)
    v.push_back(value & 0xFF);
}

static inline void push_short(long long value, std::vector<char>& v)
{
  for (int i = 0; i < 2; ++i, value >>= 8)
    v.push_back(value & 0xFF);
}

static inline void read_long_emit(std::istringstream& iss, std::vector<char>& b)
{
  long long value;
  iss >> value;
  push_long(value, b);
}

static inline void read_short_emit(std::istringstream& iss, std::vector<char>& b)
{
  short value;
  iss >> value;
  push_short(value, b);
}

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

static void halt(std::istringstream& iss, std::vector<char>& b)
{
  b.push_back(OP_HALT);
}

static void pop(std::istringstream& iss, std::vector<char>& b)
{
  b.push_back(OP_POP);
}

static void push(std::istringstream& iss, std::vector<char>& b)
{
  b.push_back(OP_PUSH);
  read_long_emit(iss, b);
}

static void pushr(std::istringstream& iss, std::vector<char>& b)
{
  b.push_back(OP_PUSHR);
  read_short_emit(iss, b);
}

static void popr(std::istringstream& iss, std::vector<char>& b)
{
  b.push_back(OP_POPR);
  read_short_emit(iss, b);
}

static void cmp(std::istringstream& iss, std::vector<char>& b)
{
  b.push_back(OP_CMP);
}

static void call(std::istringstream& iss, std::vector<char>& b)
{
  b.push_back(OP_CALL);
  read_short_emit(iss, b);
}

static void callr(std::istringstream& iss, std::vector<char>& b)
{
  b.push_back(OP_CALLR);
  read_short_emit(iss, b);
}

static void ret(std::istringstream& iss, std::vector<char>& b)
{
  b.push_back(OP_RET);
}

static std::unordered_map<std::string, mnemonic_handler_type>& get_handlers()
{
  static std::unordered_map<std::string, mnemonic_handler_type> handlers =
  {
    {"halt", halt},
    {"pop", pop},
    {"push", push},
    {"pushr", pushr},
    {"popr", popr},
    {"add", add},
    {"sub", sub},
    {"mul", mul},
    {"div", div},
    {"mod", mod},
    {"cmp", cmp},
    {"call", call},
    {"callr", callr},
    {"ret", ret},
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
