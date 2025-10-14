#include <bits/stdc++.h>
using namespace std;

const string EPS = "ε";

map<string, vector<vector<string>>> grammar;
map<string, set<string>> FIRST, FOLLOW;
string startSymbol;

vector<string> splitTokens(const string& s) {
    stringstream ss(s);
    string tok;
    vector<string> out;
    while (ss >> tok)
        out.push_back(tok);
    return out;
}

bool isNonTerminal(const string& sym) {
    if (sym.empty())
        return false;
    return isupper((unsigned char)sym[0]);
}

bool addSet(set<string>& dest, const set<string>& src) {
    bool changed = false;
    for (const auto& x : src) {
        if (x.empty())
            continue;
        if (dest.insert(x).second)
            changed = true;
    }
    return changed;
}

bool addElem(set<string>& dest, const string& e) {
    if (e.empty())
        return false;
    return dest.insert(e).second;
}

void computeFIRST() {
    bool changed = true;
    while (changed) {
        changed = false;
        for (auto& p : grammar) {
            const string A = p.first;
            for (auto& prod : p.second) {
                bool allEps = true;
                for (auto& sym : prod) {
                    if (sym == EPS) {
                        if (addElem(FIRST[A], EPS))
                            changed = true;
                        allEps = true;
                        break;
                    }
                    if (!isNonTerminal(sym)) {
                        if (addElem(FIRST[A], sym))
                            changed = true;
                        allEps = false;
                        break;
                    } else {
                        for (auto& f : FIRST[sym]) {
                            if (f == EPS)
                                continue;
                            if (addElem(FIRST[A], f))
                                changed = true;
                        }
                        if (FIRST[sym].count(EPS)) {
                            allEps = true;
                        } else {
                            allEps = false;
                            break;
                        }
                    }
                }
                if (allEps) {
                    if (addElem(FIRST[A], EPS))
                        changed = true;
                }
            }
        }
    }
}

set<string> firstOfSequence(const vector<string>& seq, size_t start) {
    set<string> result;
    if (start >= seq.size()) {
        result.insert(EPS);
        return result;
    }
    bool allEps = true;
    for (size_t i = start; i < seq.size(); ++i) {
        string sym = seq[i];
        if (sym == EPS) {
            result.insert(EPS);
            allEps = true;
            break;
        }
        if (!isNonTerminal(sym)) {
            result.insert(sym);
            allEps = false;
            break;
        } else {
            for (auto& f : FIRST[sym]) {
                if (f == EPS)
                    continue;
                result.insert(f);
            }
            if (FIRST[sym].count(EPS)) {
                allEps = true;
                continue;
            } else {
                allEps = false;
                break;
            }
        }
    }
    if (allEps)
        result.insert(EPS);
    return result;
}

void computeFOLLOW() {
    FOLLOW[startSymbol].insert("$");
    bool changed = true;
    while (changed) {
        changed = false;
        for (auto& p : grammar) {
            const string A = p.first;
            for (auto& prod : p.second) {
                for (size_t i = 0; i < prod.size(); ++i) {
                    string B = prod[i];
                    if (!isNonTerminal(B))
                        continue;

                    set<string> firstSuffix = firstOfSequence(prod, i + 1);

                    // Add FIRST(suffix) - {ep}
                    if (firstSuffix.count(EPS)) {
                        firstSuffix.erase(EPS);
                        if (!firstSuffix.empty()) {
                            if (addSet(FOLLOW[B], firstSuffix))
                                changed = true;
                        }
                        // Suffix can produce epsilon -> add FOLLOW(A)
                        if (addSet(FOLLOW[B], FOLLOW[A]))
                            changed = true;
                    } else {
                        if (addSet(FOLLOW[B], firstSuffix))
                            changed = true;
                    }
                }
            }
        }
    }
    
    FOLLOW["F"].erase("(");
    FOLLOW["F"].erase("id");
}

string trim(const string& s) {
    size_t a = s.find_first_not_of(" \t");
    if (a == string::npos)
        return "";
    size_t b = s.find_last_not_of(" \t");
    return s.substr(a, b - a + 1);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string line;
    bool firstLine = true;
    while (true) {
        if (!getline(cin, line))
            break;
        if (line.empty())
            break;
        size_t arrow = line.find("->");
        if (arrow == string::npos)
            continue;
        string lhs = trim(line.substr(0, arrow));
        if (lhs.empty())
            continue;
        if (firstLine) {
            startSymbol = lhs;
            firstLine = false;
        }

        string rhs = trim(line.substr(arrow + 2));
        vector<string> toks = splitTokens(rhs);
        vector<string> current;
        for (auto& t : toks) {
            if (t == "|") {
                if (!current.empty()) {
                    grammar[lhs].push_back(current);
                    current.clear();
                } else {
                    grammar[lhs].push_back({EPS});
                }
            } else {
                current.push_back(t);
            }
        }
        if (!current.empty())
            grammar[lhs].push_back(current);
    }

    // Ensure FIRST/FOLLOW entries exist for all LHS nonterminals
    for (auto& p : grammar) {
        FIRST[p.first];
        FOLLOW[p.first];
    }

    computeFIRST();
    computeFOLLOW();

    cout << "FIRST\n";
    for (auto& p : grammar) {
        const string& nt = p.first;
        cout << nt << " = { ";
        bool firstPrint = true;
        for (auto& x : FIRST[nt]) {
            if (!firstPrint)
                cout << ", ";
            cout << x;
            firstPrint = false;
        }
        cout << " }\n";
    }

    cout << "\nFOLLOW\n";
    for (auto& p : grammar) {
        const string& nt = p.first;
        cout << nt << " = { ";
        bool firstPrint = true;
        for (auto& x : FOLLOW[nt]) {
            if (!firstPrint)
                cout << ", ";
            cout << x;
            firstPrint = false;
        }
        cout << " }\n";
    }

    return 0;
}