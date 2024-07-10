#include <iostream>
#include <fstream>
#include <vector> 
#include <map>
#include <algorithm>

using namespace std;

string END = "EOF", PLUS = "PLUS", MINUS = "MINUS", RIGHT = "RIGHT", LEFT = "LEFT", LOOPSTART = "LOOPSTART", LOOPEND = "LOOPEND", INPUT = "INPUT", OUTPUT = "OUTPUT";


class Token {
    public:
        string type;
        char value;
};

ostream& operator<<(ostream& out, const Token& token) {
    out << "Token" << token.type << ":" << token.value << "\n";
    return out;
};


class Interpreter {
    public:
        string text;
        int pos = 0;
        int currentpointer = 0;
        Token currenttoken;
        vector<int> loopstarts = {}, loopends = {}, looppointers = {};
        map <int,int> memory = {
            {0,1}
        };
        
        map <int, pair<int,int>> loops;

        Token next_token(){
            char currentchar = text[pos];
            Token token;
            if(pos >= text.length()){token.type = END; token.value=' '; return token;}
            switch (currentchar)
            {
            case '+':
                token.type = PLUS;
                token.value = '+';
                pos++;
                return token;
            
            case '-':
                token.type = MINUS;
                token.value = currentchar;
                pos++;
                return token;
            
            case '<':
                token.type = LEFT;
                token.value = currentchar;
                pos++;
                return token;

            case '>':
                token.type = RIGHT;
                token.value = currentchar;
                pos++;
                return token;
            
            case '.':
                token.type = OUTPUT;
                token.value = currentchar;
                pos++;
                return token;

            case ',':
                token.type = INPUT;
                token.value = currentchar;
                pos++;
                return token;
            
            case '[':
                token.type = LOOPSTART;
                token.value = currentchar;
                pos++;
                return token;

            case ']':
                token.type = LOOPEND;
                token.value = currentchar;
                pos++;
                return token;

            default:
                cout << "Error! " << currenttoken;
                break;
            }
            
        }

        void findloops() {
            vector<int> used = {};
            int pointer = 0;
            //cout << text.length();
            for (int i = 0; i<text.length(); i++) {
                int starts =0,ends = 0;

                if (text[i] == '>') {
                    pointer += 1;
                }

                if (text[i] == '<') {
                    pointer -= 1;
                }

                if (text[i] == '[')  {
                    
                    if (find(used.begin(), used.end(), i) == used.end()) {
                        for (int b = i; b < text.length(); b++) {
                            if (find(used.begin(), used.end(), b) != used.end()) {
                                break;
                            } 
                            if (text[b] == '[') {
                                starts += 1;
                            }

                            if (text[b] == ']') {
                                ends += 1;
                            }
                            if (starts == ends) {
                                loops[b] = make_pair(i,pointer);
                                used.push_back(i);
                                used.push_back(b);
                                break;
                            }

                        }
                    }
                }
            }
        }

        void expr(){
            findloops();
            string output,input;
            Token currenttoken = next_token();
            currenttoken = next_token();
            while (currenttoken.type != END) {      
                if (currenttoken.type == LOOPEND) {
                    if (loops.find(pos) == loops.end()) {
                        if (memory[loops[pos-1].second] != 0) {
                            pos = loops[pos-1].first;
                        }      
                    }
                }
                
                
                if (currenttoken.type == PLUS){
                    memory[currentpointer] += 1;
                }

                if (currenttoken.type == MINUS) {
                    memory[currentpointer] -= 1;
                }
                
                if (currenttoken.type == OUTPUT) {
                    auto out = static_cast<char>(memory[currentpointer]);
                    cout << out;
                }
                
                if (currenttoken.type == INPUT) {
                    memory[currentpointer] = getchar();
                }

                if (currenttoken.type == LEFT) {
                    currentpointer -= 1;
                }

                if (currenttoken.type == RIGHT) {
                    currentpointer += 1;
                }
                
                currenttoken = next_token();
            };
            
        };
};

int main() {

    Interpreter Interpreter;
    ifstream file("brainfuck.bf");
    string str, filetext; 
    while (std::getline(file, str)){filetext+=str;};
    Interpreter.text = filetext;
    
    Interpreter.currenttoken.type = "NULL";
    Interpreter.currenttoken.value = 0;
    Interpreter.expr();
    return 0;
}
