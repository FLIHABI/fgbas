#include <unordered_map>
#include <algorithm>
#include <sstream>
#include <stdexcept>

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

  if (!(iss >> value))
    throw std::invalid_argument("Invalid operand");

  push_long(value, b);
}

static inline void read_short_emit(std::istringstream& iss, std::vector<char>& b)
{
  short value;

  if (!(iss >> value))
    throw std::invalid_argument("Invalid operand");

  push_short(value, b);
}

static inline void read_byte_emit(std::istringstream& iss, std::vector<char>& b)
{
  unsigned value;

  if (!(iss >> value))
    throw std::invalid_argument("Invalid operand");

  b.push_back(value);
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

static void bit_and(std::istringstream& iss, std::vector<char>& b)
{
  b.push_back(OP_AND);
}

static void bit_or(std::istringstream& iss, std::vector<char>& b)
{
  b.push_back(OP_OR);
}

static void bit_xor(std::istringstream& iss, std::vector<char>& b)
{
  b.push_back(OP_XOR);
}

static void bit_not(std::istringstream& iss, std::vector<char>& b)
{
  b.push_back(OP_NOT);
}

static void bit_shr(std::istringstream& iss, std::vector<char>& b)
{
  b.push_back(OP_SHR);
  read_byte_emit(iss, b);
}

static void bit_shl(std::istringstream& iss, std::vector<char>& b)
{
  b.push_back(OP_SHL);
  read_byte_emit(iss, b);
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

static void jmp(std::istringstream& iss, std::vector<char>& b)
{
  b.push_back(OP_JMP);
  read_short_emit(iss, b);
}

static void je(std::istringstream& iss, std::vector<char>& b)
{
  b.push_back(OP_JE);
  read_short_emit(iss, b);
}

static void jl(std::istringstream& iss, std::vector<char>& b)
{
  b.push_back(OP_JL);
  read_short_emit(iss, b);
}

static void jg(std::istringstream& iss, std::vector<char>& b)
{
  b.push_back(OP_JG);
  read_short_emit(iss, b);
}

static void jne(std::istringstream& iss, std::vector<char>& b)
{
  b.push_back(OP_JNE);
  read_short_emit(iss, b);
}

static void jle(std::istringstream& iss, std::vector<char>& b)
{
  b.push_back(OP_JLE);
  read_short_emit(iss, b);
}

static void jge(std::istringstream& iss, std::vector<char>& b)
{
  b.push_back(OP_JGE);
  read_short_emit(iss, b);
}

static void obj_create(std::istringstream& iss, std::vector<char>& b)
{
  b.push_back(OP_CREATE);
  read_short_emit(iss, b);
}

static void obj_delete(std::istringstream& iss, std::vector<char>& b)
{
  b.push_back(OP_DELETE);
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
    {"and", bit_and},
    {"or", bit_or},
    {"xor", bit_xor},
    {"not", bit_not},
    {"shr", bit_shr},
    {"shl", bit_shl},
    {"cmp", cmp},
    {"call", call},
    {"callr", callr},
    {"ret", ret},
    {"jmp", jmp},
    {"je", je},
    {"jl", jl},
    {"jg", jg},
    {"jne", jne},
    {"jle", jle},
    {"jge", jge},
    {"create", obj_create},
    {"delete", obj_delete}
  };

  return handlers;
}

void handle_mnemonic( std::string& mnemonic,
                      std::istringstream& iss,
                      std::vector<char>& bytecode)
{
  std::transform(mnemonic.begin(), mnemonic.end(), mnemonic.begin(), tolower);

  auto handler = get_handlers().find(mnemonic);
  if (handler == get_handlers().end())
    throw std::invalid_argument("Invalid mnemonic '" + mnemonic + "'");

  handler->second(iss, bytecode);
}
