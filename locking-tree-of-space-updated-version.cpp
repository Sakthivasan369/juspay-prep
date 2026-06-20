#include <bits/stdc++.h>
using namespace std;

struct TreeNode {
    string name;
    bool isLocked;
    int lockedBy;
    TreeNode* parent;
    vector<TreeNode*> children;
    int lockedDescendantsCount;

    // Optimization: Initialize using an initialization list for slight performance gain
    TreeNode(string s) : name(s), isLocked(false), lockedBy(0), 
                         parent(nullptr) , lockedDescendantsCount(0) {}
};

class LockingTree {
private:
    unordered_map<string, TreeNode*> mp;

    // Helper 1: Verify all locks in the subtree belong to the user
    bool checkCanUpgrade(TreeNode* node, int uid) {
        if (node->isLocked && node->lockedBy != uid) return false;
        
        // PRUNING OPTIMIZATION: If this branch has no locked descendants, stop searching it
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
        
        // PRUNING OPTIMIZATION: Only traverse deeper if locks exist below
        if (node->lockedDescendantsCount > 0) {
            for (TreeNode* child : node->children) {
                freedCount += unlockSubtree(child);
            }
            node->lockedDescendantsCount = 0; // Reset count since all are now unlocked
        }
        return freedCount;
    }
    void update_ancestors(TreeNode* node,int offset){
       TreeNode* temp = node->parent;
        while (temp != nullptr) {
            temp->lockedDescendantsCount += offset;
            temp = temp->parent;
        }
    }

public:
    LockingTree(const vector<string>& names) {
        mp.reserve(names.size()); // Prevent hash map rehashing during insertions
        for (const string& s : names) {
            mp[s] = new TreeNode(s);
        }
    }

    void buildEdge(const string& parent, const string& child) {
        TreeNode* p = mp[parent];
        TreeNode* c = mp[child];
        p->children.push_back(c);
        c->parent = p;
    }

    bool lockNode(const string& name, int uid) {
        TreeNode* node = mp[name];
        
        // 1. Check self and descendants
        if (node->isLocked || node->lockedDescendantsCount > 0) return false;

        // 2. Check ancestors
        TreeNode* temp = node->parent;
        while (temp != nullptr) {
            if (temp->isLocked) return false;
            temp = temp->parent;
        }

        // 3. Lock the node
        node->isLocked = true;
        node->lockedBy = uid;

        // 4. Update ancestors
        update_ancestors(node,1);
        return true;
    }

    bool unlockNode(const string& name, int uid) {
        TreeNode* node = mp[name];

        if (!node->isLocked || node->lockedBy != uid) return false;

        node->isLocked = false;
        node->lockedBy = 

        update_ancestors(node,-1);
        return true;
    }

    bool upgradeNode(const string& name, int uid) {
        TreeNode* node = mp[name];

        // 1. Check self and immediate requirements
        if (node->isLocked || node->lockedDescendantsCount == 0) return false;

        // 2. Check ancestors are not locked
        TreeNode* temp = node->parent;
        while (temp != nullptr) {
            if (temp->isLocked) return false;
            temp = temp->parent;
        }

        // 3. Verify all locked descendants belong to the user (Fast DFS instead of BFS)
        for (TreeNode* child : node->children) {
            if (!checkCanUpgrade(child, uid)) return false;
        }

        // 4. Unlock subtree and count how many nodes were freed
        int totalFreed = 0;
        for (TreeNode* child : node->children) {
            totalFreed += unlockSubtree(child);
        }

        // 5. Lock the current node
        node->isLocked = true;
        node->lockedBy = uid;

        // 6. BATCH UPDATE: Update ancestors EXACTLY ONCE with the net change
        // Net change = +1 (current node locked) - totalFreed (children unlocked)
        int netChange = 1 - totalFreed;
        update_ancestors(node,netChange);

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