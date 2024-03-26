#include "LA.h"

LA::LA() {
	load_input();
//	clear_comments();
	load_reserved_words();
	sfm = DFSMConstructor("DFSM.txt").getRoot();
}

void LA::fact_to_tokens() {
    int pos = 0;
    while (pos < input.size()) {
        auto tok = read_token(pos);
        if (tok.data.empty() || tok.data[0] == 0 || tok.data == "\t" || tok.data == "\r") continue;
        Token::mutex.lock();
        Token::tokens.push_back(tok);
        Token::state = Go;
        Token::mutex.unlock();
    }
    Token::mutex.lock();
    Token::state = End;
    Token::mutex.unlock();
}

void LA::print_tokens(std::ostream &out, const std::vector<Token> &vec) const {
    for (Token tok: vec) {
        out << tok.data << " - " << get_type_name(tok.type) << '\n';
    }
}

Token LA::read_token(int &pos) {
    pos = std::max(0, pos);
    Token token;
    Statement* ptr = sfm;
    while (pos < input.size() && ptr && ptr->term == NotTerm) {
        token.data += input[pos];
        ptr = ptr->next[input[pos]];
        ++pos;
    }
    if (ptr && ptr->term == OneLetter) {
        --pos;
        token.data.pop_back();
    }
    if (ptr) token.type = ptr->ret_type;
    if (token.type == Name && reserved_words.count(token.data)) {
        token.type = Res;
    }
    return token;
}

std::string LA::get_type_name(int type) const {
    switch (type) {
        case Res:
            return "Reserved word";
        case Name:
            return "Name";
        case Float:
            return "Float";
        case Integer:
            return "Integer";
        case Oper:
            return "Operator";
        case Punct:
            return "Punctuation";
        case String:
            return "String";
        case Other:
            return "Other";
        case Comment:
            return "Comment";
        case Bool:
            return "Bool";
        case Char:
            return "Char";
        default:
            break;
    }
    return "Unknown";
}

void LA::read_code(std::istream &in) {
    unsigned long long size = in.tellg();
    input.assign(size + 1, '\0');
    in.seekg(0);
    if (!in.read(&input[0], size)) {
        throw std::runtime_error("Can't open input file");
    }
}

void LA::clear_comments() {
    std::regex r = std::regex(R"((?://.*)|(/\*(?:.|[\n\r])*?\*/))");
    std::string out(input.size(), '\0');
    out[out.size() - 1] = '\n';
    std::regex_replace(&out[0], input.begin(), input.end(), r, "\n");
    input = out;
}

void LA::load_input() {
    std::ifstream ifile("input.txt", std::ios::binary | std::ios::ate);
    read_code(ifile);
}

void LA::load_reserved_words() {
    std::ifstream ifile("reserved.txt");
    std::string word;
    while (ifile >> word) {
        reserved_words.insert(word);
    }
}

DFSMConstructor::DFSMConstructor(const std::string& filename) {
    std::ifstream in(filename, std::ios::binary | std::ios::ate);
    auto size = in.tellg();
    std::string input(size, '\0');
    in.seekg(0);
    if (!in.read(&input[0], size)) {
        throw std::runtime_error("Can't open input file");
    }
    std::map<std::string, Statement*> m;
    std::regex r(R"(\{([\w\?]+)\s?->\s?([\w\?]+)\s(.+)\})");
    auto beg = std::sregex_iterator(input.begin(), input.end(), r);
    auto end = std::sregex_iterator();
    for (auto it = beg; it != end; ++it) {
        std::smatch match = *it;
        std::string from = match[1].str();
        std::string to = match[2].str();
        std::string syms = match[3].str();
        if (!m[from]) m[from] = new Statement;
        if (!m[to]) m[to] = new Statement;
        Statement*& f = m[from];
        Statement*& t = m[to];
        if (syms == "ALL") {
            for (int i = 0; i < 256; ++i) {
                f->next[i] = t;
            }
        } else if (syms == "ENDL") {
            f->next['\n'] = t;
        } else {
            for (unsigned char ch: syms) {
                f->next[ch] = t;
            }
        }
    }

    r = std::regex(R"(\{(\w+)\s-\s(\w+)\s-\s(\w+)\})");
    beg = std::sregex_iterator(input.begin(), input.end(), r);
    end = std::sregex_iterator();
    for (auto it = beg; it != end; ++it) {
        std::smatch match = *it;
        std::string state = match[1].str();
        std::string term = match[2].str();
        std::string ret_type = match[3].str();
        if (!m[state]) {
            throw std::runtime_error("Undefined state " + state);
        }
        Statement*& st = m[state];
        if (term == "OneLetter") {
            st->term = OneLetter;
            st->ret_type = getTypeByName(ret_type);
        } else if (term == "Nothing") {
            st->term = Nothing;
            st->ret_type = getTypeByName(ret_type);
        } else {
            continue;
        }
    }


    root = m["ROOT"];
}

Statement *DFSMConstructor::getRoot() const {
    return root;
}

int DFSMConstructor::getTypeByName(const std::string &name) {
    if (name == "Res") return Res;
    if (name == "Var") return Name;
    if (name == "Float") return Float;
    if (name == "Integer") return Integer;
    if (name == "Oper") return Oper;
    if (name == "Punct") return Punct;
    if (name == "Other") return Other;
    if (name == "String") return String;
    if (name == "Comment") return Comment;
    if (name == "Bool") return Bool;
    if (name == "Char") return Char;
    return Other;
}

