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
            int pointerpos = 0;
            Token currenttoken = next_token();
            while (currenttoken.type != END) {
                if (currenttoken.type == LOOPSTART) {
                    loopstarts.push_back(pos);
                    looppointers.push_back(pointerpos);
                }        
                if (currenttoken.type == LOOPEND) {
                    loopends.push_back(pos);
                }
                if (currenttoken.type == RIGHT) {
                    pointerpos += 1;
                }
                if (currenttoken.type == LEFT) {
                    pointerpos -= 1;
                }
                currenttoken = next_token();
            }
            currenttoken.type = "NULL";
            currenttoken.value = 0;
            pos = 0;
        }

        void expr(){
            findloops();
            reverse(loopends.begin(),loopends.end());
            string output,input;
            Token currenttoken = next_token();
            currenttoken = next_token();
            while (currenttoken.type != END) {      

                if (currenttoken.type == LOOPEND) {
                    if (find(loopends.begin(),loopends.end(), pos) != loopends.end()) {
                        auto it = find(loopends.begin(),loopends.end(), pos);
                        if (it != loopends.end()) {
                            int index = it - loopends.begin();
                            if (memory[looppointers[index]] != 0) {
                                pos = loopstarts[index];
                            }
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
