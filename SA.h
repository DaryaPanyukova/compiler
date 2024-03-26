#ifndef SA_H_
#define SA_H_

#include <iostream>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <map>
#include <stack>
#include "Token.h"

class FIRSTConstructor {
public:
    FIRSTConstructor(const std::string& filename, std::map<std::string, std::vector<std::string>>& f);
};

class Semantic {
public:
    struct Type;
    struct FSignature;
    int row = 0;

    struct TID {
        std::map<std::string, std::pair<Type, int>> data;
        TID* par = nullptr;
    };

    struct Type {
        std::map<std::string, std::pair<Type, int>> fields;
        std::map<std::string, std::vector<FSignature>> ftid;
        std::vector <int> shape;
        std::string name;
        int ptr_num;
        bool is_ref;
        int size = 1;

        Type (std::string name0, int ptr_num0=0, bool is_ref0=false) {
            name = name0;
            ptr_num = ptr_num0;
            is_ref = is_ref0;
            size = get_size();
        }

        Type() = default;
        std::string GetName () {
            std::string res = name;
            for (int i = 0; i < ptr_num; ++i) {
                res += '*';
            }
            if (is_ref) {
                res += '&';
            }
            return res;
        }

        friend bool operator==(const Type& lhs, const Type& rhs) {
            return lhs.name == rhs.name && lhs.ptr_num == rhs.ptr_num; // сравнение s_ref?
        }

        size_t get_size() {
            int size_ = 0;
            if (ptr_num > 0 || is_ref) {
                size_ = 8;
            } else if (name == "int") {
                size_ = 8;
            } else if (name == "char") {
                size_ = 1;
            } else if (name == "bool") {
                size_ = 1;
            } else if (name == "float") {
                size_ = 8;
            }
            for (auto& el: fields) {
                size_ += el.second.first.get_size();
            }
            int pr = 1;
            for (auto& el: shape) {
                pr *= el;
            }
            size_ *= pr;
            if (size_ == 0) return 1;
            return size_;
        }

        void resize() {
            size = get_size();
        }

        bool check_method(const std::string& name) const;

        bool check_method(const std::string& name, const FSignature& sign);

        FSignature getSignature(const std::string& name, const FSignature& seg);

        Semantic::Type check_field(std::string name);

        bool isContainSignature(const std::vector<FSignature>& funcs, const FSignature& seg) const;

        void put_ftid(const std::pair<std::string, FSignature>& id);
    };

    struct FSignature {
        std::vector<Type> params;
        Type ret_type;
        friend bool operator==(const FSignature& a, const FSignature& b) {
            return a.params == b.params && a.ret_type == b.ret_type;
        }
        friend bool operator!=(const FSignature& a, const FSignature& b) {
            return !(a == b);
        }
    };

    void setAddr(const std::string& name, int addr);

    TID* getCurTid() const;

    std::map<std::string, Type> custom_type;

    bool isContainSignature(const std::vector<FSignature>& funcs, const FSignature& seg) const;

    FSignature getSignature(const std::vector<FSignature>& funcs, const FSignature& seg) const;

    FSignature getSignature(const std::string& name, const FSignature& seg);

    bool check_func(const std::string& name) const;

    bool check_func(const std::string& name, const FSignature& sign);

    bool check_struct(const std::string& name);

    void extend_tid();

    void put_ftid(const std::pair<std::string, FSignature>& id);

    void push_type(Type type);

    Type pop_type();

    void check_op(const std::string& op);

    void put_id(const std::string& id, Type type);

    void eq_type(Type type);

    Type check_id(const std::string& id) const;
    int check_addr(const std::string& id) const;

    void del_tid();

    static Semantic& inst();

    Type type;
    std::string id;
private:
    Semantic() = default;

    std::map<std::string, std::vector<FSignature>> ftid;

    TID* cur_tid = new TID;

private:
    std::stack<Type> st;
};

class Poliz {
public:
    void push_operand(const std::string& str, Semantic::Type type, bool rvalue = 0) {
        kim.push_back({str,0,0,0,0, rvalue, type});
        print_self();
    }

    void put_operand(int i, const std::string& str, Semantic::Type type, bool rvalue = 0) {
        kim[i] = {str, 0,0,0,0, rvalue, type};
        print_self();
    }

    void push_operation(const std::string& str, int prior, int cnt, bool is_right_ass);

    void finish_poliz() {
        while (!cock.empty()) {
            kim.push_back(cock.top());
            cock.pop();
        }
    }

    std::string calc(int start_point);

    int get_current_address() {
        return kim.size();
    }

    void print_self() {
        for (int i = 0; i < kim.size(); ++i) {
            std::cout << kim[i].lex << " ";
        }
        std::cout << std::endl;
        std::cout << kim.size() << std::endl;
    }

    Poliz get_sub_poliz(int lhs, int rhs) {
        Poliz poliz;
        for (int i = lhs; i < rhs; ++i) {
            poliz.kim.push_back(kim[i]);
        }
        return poliz;
    }


private:
    struct Token {
        std::string lex;
        bool is_operation = false;
        bool is_right_ass = false;
        int cnt = 0;
        int prior = 0;
        bool is_rvalue = false;
        Semantic::Type type;
    };


    std::stack<Token> cock;
    std::vector<Token> kim;
    std::vector<char> memory_stack;
    std::stack<int> ret_addr;
    int rbp = 0;


    void Create(std::stack<Token> &res);

    void Addition(std::stack<Token> &res);

    void Subtraction(std::stack<Token> &res);

    void Mult(std::stack<Token> &res);

    void Equality(std::stack<Token> &res);

    void Call(std::stack<Token> &res, int &i);

    void Ret(int& i);

    void Lower(std::stack<Token> &res, Token op1, Token op2);

    void Jmp(std::stack<Token> &res, int &i);

    void Jf(std::stack<Token> &res, int &i);

    void Pop(std::stack<Token> &res);

    void Sub(std::stack<Token> &res);

    void Dot(std::stack<Token> &res) const;

    void cast_lvalue(Token &op1);

};

class Generation {
public:
    Poliz poliz;
    std::map<std::string, int> got;
    int f_size = 0;

    void push_exp(const std::string& str, Semantic::Type type = Semantic::Type(), bool rvalue = 0);


    static Generation& inst() {
        static Generation res = Generation();
        return res;
    }

    void print_got() const {
        for (const auto& it: got) {
            std::cout << it.first << " " << it.second << std::endl;
        }
    }

};




class SA {
public:
	SA();
	void analize();

	void operator()() {
	    analize();
	}

private:
	Token cur;
	int ind = 0;
	int row = 1, col = 1;
    std::map<std::string, std::vector<std::string>> first_;
    bool first_equals(const std::string& str, const std::string& target);
    void GetToken();

    //Grammar
    void Program();

    void Struct();
    void StructBody(Semantic::Type& t);
    void Import();

    void Func();
    void Method(Semantic::Type& acc);
    void Type();
    void EType();
    void EEType();
    void EEEType();
    void Name();
    std::vector <Semantic::Type> Params();

    void Try();
    void CodeBlock();

    void Block();
    void Operator();
    void NonExtendedOperator();
    void For();
    void Definition();
    void If();
    void While();
    void Return();
    void Throw();

    void Prior1();
    void Prior1_dot();
    void Prior_dot();
    void Prior_dot_after();

    void Prior2();
    void Prior3();
    void Prior4();
    void Prior5();
    void Prior6();
    void Prior7();
    void Prior8();
    void Prior9();
    void Prior10();
    void Prior11();
    void Prior12();
    void Exp();

    std::vector<Semantic::Type> Enumeration();
};

#endif /* SA_H_ */
