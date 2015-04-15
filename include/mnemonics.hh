#ifndef MNEMONICS_HH
# define MNEMONICS_HH

# include <vector>
# include <string>

void handle_mnemonic( std::string& mnemonic,
                      std::istringstream& iss,
                      std::vector<char>& bytecode);

#endif /* end of include guard: MNEMONICS_HH */
