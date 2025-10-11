#include <bits/stdc++.h>
using namespace std;

bool isNC(const char& ch) {
    return ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') ||
            (ch >= '0' && ch <= '9'));
}

string removeExtraSpaces(string s) {
    string ret = "";
    for (char c : s) {
        if (c == ' ')
            continue;
        ret += c;
    }
    return ret;
}

class NonTerminal {
    string name;
    vector<string> productionRules;

public:
    NonTerminal(string name) { this->name = name; }

    // Returns the head of the production rule
    string getName() { return name; }

    void setNewRules(vector<string> rules) {
        productionRules.clear();
        for (auto rule : rules) {
            productionRules.push_back(rule);
        }
    }

    vector<string> getRules() { return productionRules; }

    void addRule(string rule) { productionRules.push_back(rule); }

    // Prints the production rules
    void printRule() {
        string toPrint = "";
        toPrint += name + " ->";

        for (string s : productionRules) {
            toPrint += " " + s + " |";
        }

        toPrint.pop_back();
        cout << toPrint << endl;
    }
};

class Grammar {
    vector<NonTerminal> nonTerminals;

public:
    // Add rules to the grammar
    void addRule(string rule) {
        bool nt = 0;
        string parse = "";
        for (int i = 0; i < rule.size(); i++) {
            if (i && rule[i - 1] == '-' && rule[i] == '>') {
                while (!isNC(parse.back())) {
                    parse.pop_back();
                }
                NonTerminal newNonTerminal(parse);
                nonTerminals.push_back(newNonTerminal);
                parse = "";
                break;
            } else {
                parse += rule[i];
            }
        }
        //        cout << "Name : "<< nonTerminals.back().getName() << '\n';
        int i = 0;
        while (rule[i - 1] != '>')
            i++;
        for (; i < rule.size(); i++) {
            if (rule[i] == '|') {
                parse = removeExtraSpaces(parse);
                if (parse.size()) {
                    nonTerminals.back().addRule(parse);
                    parse = "";
                }
            } else {
                parse += rule[i];
            }
        }
        //        for(string &x : nonTerminals.back().getRules()){
        //          cout << x << ' ';
        //        }
        parse = removeExtraSpaces(parse);
        if (parse.size()) {
            nonTerminals.back().addRule(parse);
        }
    }

    void inputData() {
        string input = "";
        while (input != "end") {
            getline(cin, input);
            if (input == "end")
                break;
            addRule(input);
        }
    }

    // Algorithm for eliminating the non-Immediate Left Recursion
    void solveNonImmediateLR(NonTerminal& A, NonTerminal& B) {
        string nameA = A.getName();
        string nameB = B.getName();

        vector<string> rulesA, rulesB, newRulesA;
        rulesA = A.getRules();
        rulesB = B.getRules();

        for (auto rule : rulesA) {
            if (rule.substr(0, nameB.size()) == nameB) {
                for (auto rule1 : rulesB) {
                    newRulesA.push_back(rule1 + rule.substr(nameB.size()));
                }
            } else {
                newRulesA.push_back(rule);
            }
        }
        A.setNewRules(newRulesA);
    }

    // Algorithm for eliminating Immediate Left Recursion
    void solveImmediateLR(NonTerminal& A) {
        string name = A.getName();
        string newName = name + "'";

        vector<string> alphas, betas, rules, newRulesA, newRulesA1;
        rules = A.getRules();

        // Checks if there is left recursion or not
        for (auto rule : rules) {
            if (rule.substr(0, name.size()) == name) {
                alphas.push_back(rule.substr(name.size()));
            } else {
                betas.push_back(rule);
            }
        }

        // If no left recursion, exit
        if (!alphas.size())
            return;

        if (!betas.size())
            newRulesA.push_back(newName);

        for (auto beta : betas)
            newRulesA.push_back(beta + newName);

        for (auto alpha : alphas)
            newRulesA1.push_back(alpha + newName);

        // Amends the original rule
        A.setNewRules(newRulesA);
        newRulesA1.push_back("\u03B5");

        // Adds new production rule
        NonTerminal newNonTerminal(newName);
        newNonTerminal.setNewRules(newRulesA1);
        nonTerminals.push_back(newNonTerminal);
    }

    // Eliminates left recursion
    void applyAlgorithm() {
        int size = nonTerminals.size();
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < i; j++) {
                solveNonImmediateLR(nonTerminals[i], nonTerminals[j]);
            }
            solveImmediateLR(nonTerminals[i]);
        }
    }

    // Print all the rules of grammar
    void printRules() {
        for (auto nonTerminal : nonTerminals) {
            nonTerminal.printRule();
        }
    }
};

int main() {

    Grammar grammar;
    grammar.inputData();
    grammar.applyAlgorithm();
    grammar.printRules();

    return 0;
}
