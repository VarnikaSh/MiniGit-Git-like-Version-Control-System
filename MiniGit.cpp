#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <ctime>
#include <sstream>
#include <algorithm>

using namespace std;

// =======================================
// HASHING
// =======================================

string hashString(const string& data) {
    hash<string> hasher;
    return to_string(hasher(data));
}

// =======================================
// MERKLE TREE
// =======================================

string buildMerkleRoot(vector<string> hashes) {
    if(hashes.empty())
      return "";
    while(hashes.size() > 1){
        vector<string> nextLevel;
        for(int i = 0; i < (int)hashes.size(); i += 2) {
            if(i + 1 < hashes.size()) {
                nextLevel.push_back(hashString(hashes[i] + hashes[i + 1]));
            }
            else {
                nextLevel.push_back(hashString(hashes[i] + hashes[i]));
            }
        }
        hashes = nextLevel;
    }
    return hashes[0];  
}

// ==========================================
// LCS DIFF
// ==========================================

void showDiff(string a, string b) {
    vector<string> A, B;
    string word;
    stringstream sa(a);
    while(getline(sa, word, '\n'))
      A.push_back(word);
    stringstream sb(b);
    while(getline(sb, word, '\n'))
      B.push_back(word);
    int n = A.size();
    int m = B.size();
    vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));
    for(int i = 1; i <= n; i++) {
        for(int j = 1; j <= m; j++) {
            if(A[i - 1] == B[j - 1])
              dp[i][j] = 1 + dp[i - 1][j - 1];
            else
              dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);  
        }
    }
    cout << "\n===== DIFF =====\n";
    int i = n; 
    int j = m;
    vector<string> result;
    while(i > 0 && j > 0) {
        if(A[i - 1] == B[j - 1]) {
            result.push_back("  " + A[i - 1]);
            i--;
            j--;
        }
        else if(dp[i - 1][j] > dp[i][j - 1]) {
            result.push_back("- " + A[i - 1]);
            i--;
        }
        else {
            result.push_back("+ " + B[j - 1]);
            j--;
        }
    }
    while(i > 0) {
        result.push_back("- " + A[i - 1]);
        i--;
    }    
    while(j > 0) {
        result.push_back("+ " + B[j - 1]);
        j--;
    }
    reverse(result.begin(), result.end());
    for(auto& line : result)
      cout << line << "\n";
}

// ===========================================
// COMMIT (DAG NODE)
// ===========================================

struct Commit {
    string id;
    string message;
    string merkleRoot;
    time_t timestamp;
    vector<string> parents;
    Commit() {}
    Commit(string msg, string root, vector<string> p) {
        message = msg;
        merkleRoot = root;
        parents = p;
        timestamp = time(nullptr);
        id = hashString(msg + root + to_string(timestamp));
    }
};

// =============================================
// VERSION CONTROL SYSTEM
// =============================================

class MiniGit {
    private:
    unordered_map<string, Commit> commits;
    unordered_map<string, string> branches;
    string currentBranch;

    public:
    MiniGit() {
        currentBranch = "main";
        branches["main"] = "";
    }

    string commit(string message, vector<string> files) {
        vector<string> hashes;
        for(auto& file : files)
          hashes.push_back(hashString(file));
        string merkleRoot = buildMerkleRoot(hashes);
        vector<string> parents;
        if(branches[currentBranch] != "") {
            parents.push_back(branches[currentBranch]);
        }  
        Commit c(message, merkleRoot, parents);
        commits[c.id] = c;
        branches[currentBranch] = c.id;
        cout << "\nCommit Created\n";
        cout << "ID: "
             << c.id.substr(0, 10)
             << "\n";
        return c.id;     
    }

    void createBranch(string name) {
        branches[name] = branches[currentBranch];
        cout << "\nBranch "
             << name
             << " created.\n";
    }

    void checkout(string branch) {
        if(!branches.count(branch)) {
            cout << "\nBranch not found.\n";
            return;
        }
        currentBranch = branch;
        cout << "\nSwitched to "
             << branch
             << "\n";
    }

    void merge(string branch) {
        if(!branches.count(branch)) {
            cout << "\nBranch not found.\n";
            return;
        }
        vector<string> parents;
        if(branches[currentBranch] != "")
          parents.push_back(branches[currentBranch]);
        if(branches[branch] != "")
          parents.push_back(branches[branch]);
        Commit mergeCommit("Merge " + branch, "merge_root", parents);
        commits[mergeCommit.id] = mergeCommit;
        branches[currentBranch] = mergeCommit.id;
        cout << "\nMerged "
             << branch
             << "\n";    
    }

    void log() {
        cout << "\n===== COMMIT LOG =====\n";
        string head = branches[currentBranch];
        while(head != "") {
            Commit& c = commits[head];
            cout << "\nCommit: "
                 << c.id.substr(0,10)
                 << "\n";
            cout << "Message: "
                 << c.message
                 << "\n";
            cout << "Merkle: "
                 << c.merkleRoot.substr(0,10)
                 << "\n";
            if(c.parents.empty())
              break;
            head = c.parents[0];                 
        }
    }
};

// =========================================
// MAIN
// =========================================

int main() {
    MiniGit vcs;
    vector<string> files1 = {"int main() {\nreturn 0;\n}"};
    vcs.commit("Initial Commit", files1);
    vcs.createBranch("feature");
    vcs.checkout("feature");
    vector<string> files2 = {"int main() {\ncout<<\"Hello\";\nreturn 0;\n}"};
    vcs.commit("Added Greeting", files2);
    vcs.checkout("main");
    vcs.merge("feature");
    vcs.log();
    string oldVersion = "Line1\nLine2\nLine3";
    string newVersion = "Line1\nLine2 Modified\nLine3\nLine4";
    showDiff(oldVersion, newVersion);
    return 0;
}