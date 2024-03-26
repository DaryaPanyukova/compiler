#ifndef TOKEN_H_
#define TOKEN_H_

#include <string>
#include <queue>
#include <thread>
#include <vector>

enum {
	Res,
	Name = 1,
    Char,
	Float,
	Integer,
	Bool,
	Oper,
	Punct,
	String,
	Comment,
	Other
};
//
//std::vector <std::string> TYPENAME_TABLE{"", "", "char", "float", "int", "bool",
//                                         "", "", "char*", "", ""};

enum {
	Nothing,
	OneLetter,
	NotTerm
};

enum  {
    Wait,
    Go,
    End
};

class Token {
public:
	std::string data = "";
	int type = Other;

    static std::vector<Token> tokens;
    static int state;
    static std::mutex mutex;
};




#endif /* TOKEN_H_ */
