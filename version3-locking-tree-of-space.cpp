#include <bits/stdc++.h>
#include <mutex> // Required for thread safety

using namespace std;

struct TreeNode {
    string name;
    bool isLocked;
    int lockedBy;
    TreeNode* parent;
    vector<TreeNode*> children;
    int lockedDescendantsCount;

    TreeNode(string s) : name(s), isLocked(false), lockedBy(0), 
                         parent(nullptr), lockedDescendantsCount(0) {}
};

class LockingTree {
private:
    unordered_map<string, TreeNode*> mp;
    
    // Mutex to ensure thread-safe operations
    std::mutex treeMutex;

    // Helper 1: Verify all locks in the subtree belong to the user
    // (Internal helpers do not need their own locks, they are called by public methods that already hold the lock)
    bool checkCanUpgrade(TreeNode* node, int uid) {
        if (node->isLocked && node->lockedBy != uid) return false;
        
        if (node->lockedDescendantsCount == 0) return true;

        for (TreeNode* child : node->children) {
            if (!checkCanUpgrade(child, uid)) return false;
        }
        return true;
    }

    // Helper 2: Unlock nodes and return the total number of nodes freed
    int unlockSubtree(TreeNode* node) {
        int freedCount = 0;
        
        if (node->isLocked) {
            node->isLocked = false;
            node->lockedBy = 0;
            freedCount++;
        }
        
        if (node->lockedDescendantsCount > 0) {
            for (TreeNode* child : node->children) {
                freedCount += unlockSubtree(child);
            }
            node->lockedDescendantsCount = 0; 
        }
        return freedCount;
    }

public:
    // Constructor
    LockingTree(const vector<string>& names) {
        mp.reserve(names.size());
        for (const string& s : names) {
            mp[s] = new TreeNode(s);
        }
    }

    // Destructor to prevent memory leaks
    ~LockingTree() {
        for (auto& pair : mp) {
            delete pair.second; // Free the dynamically allocated TreeNodes
        }
        mp.clear();
    }

    void buildEdge(const string& parent, const string& child) {
        // Optional lock here if you plan to build the tree concurrently
        std::lock_guard<std::mutex> lock(treeMutex); 
        
        TreeNode* p = mp[parent];
        TreeNode* c = mp[child];
        p->children.push_back(c);
        c->parent = p;
    }

    bool lockNode(const string& name, int uid) {
        // Acquire lock for the duration of this function
        std::lock_guard<std::mutex> lock(treeMutex);
        
        TreeNode* node = mp[name];
        
        if (node->isLocked || node->lockedDescendantsCount > 0) return false;

        TreeNode* temp = node->parent;
        while (temp != nullptr) {
            if (temp->isLocked) return false;
            temp = temp->parent;
        }

        node->isLocked = true;
        node->lockedBy = uid;

        temp = node->parent;
        while (temp != nullptr) {
            temp->lockedDescendantsCount++;
            temp = temp->parent;
        }
        return true;
    }

    bool unlockNode(const string& name, int uid) {
        // Acquire lock for the duration of this function
        std::lock_guard<std::mutex> lock(treeMutex);
        
        TreeNode* node = mp[name];

        if (!node->isLocked || node->lockedBy != uid) return false;

        node->isLocked = false;
        node->lockedBy = 0;

        TreeNode* temp = node->parent;
        while (temp != nullptr) {
            temp->lockedDescendantsCount--;
            temp = temp->parent;
        }
        return true;
    }

    bool upgradeNode(const string& name, int uid) {
        // Acquire lock for the duration of this function
        std::lock_guard<std::mutex> lock(treeMutex);
        
        TreeNode* node = mp[name];

        if (node->isLocked || node->lockedDescendantsCount == 0) return false;

        TreeNode* temp = node->parent;
        while (temp != nullptr) {
            if (temp->isLocked) return false;
            temp = temp->parent;
        }

        for (TreeNode* child : node->children) {
            if (!checkCanUpgrade(child, uid)) return false;
        }

        int totalFreed = 0;
        for (TreeNode* child : node->children) {
            totalFreed += unlockSubtree(child);
        }

        node->isLocked = true;
        node->lockedBy = uid;

        int netChange = 1 - totalFreed;
        temp = node->parent;
        while (temp != nullptr) {
            temp->lockedDescendantsCount += netChange;
            temp = temp->parent;
        }

        return true;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, m, Q;
    if (!(cin >> N >> m >> Q)) return 0;

    vector<string> names(N);
    for (int i = 0; i < N; i++) {
        cin >> names[i];
    }

    LockingTree tree(names);

    for (int i = 1; i < N; i++) {
        int parIndex = (i - 1) / m;
        tree.buildEdge(names[parIndex], names[i]);
    }

    while (Q--) {
        int type, user;
        string name;

        cin >> type >> name >> user;
        bool ans = false;

        if (type == 1) ans = tree.lockNode(name, user);
        else if (type == 2) ans = tree.unlockNode(name, user);
        else ans = tree.upgradeNode(name, user);

        cout << (ans ? "true" : "false") << "\n";
    }

    return 0;
}