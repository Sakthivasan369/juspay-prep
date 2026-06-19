#include <bits/stdc++.h>
using namespace std;

struct TreeNode {
    string name;
    bool isLocked;
    int lockedBy;
    TreeNode* parent;
    vector<TreeNode*> children;
    int lockedDescendantsCount;

    TreeNode(string s) {
        name = s;
        isLocked = false;
        lockedBy = 0;
        parent = nullptr;
        lockedDescendantsCount = 0;
    }
};

class LockingTree {
private:
    unordered_map<string, TreeNode*> mp;

public:
    LockingTree(const vector<string>& names) {
       && for (auto &s : names)
            mp[s] = new TreeNode(s);
    }

    void buildEdge(const string& parent, const string& child) {
        TreeNode* p = mp[parent];
        TreeNode* c = mp[child];
        p->children.push_back(c);
        c->parent = p;
    }

    bool lockNode(const string& name, int uid) {
        TreeNode* node = mp[name];
        if (node->isLocked) return false;

        // Check ancestors
        TreeNode* temp = node->parent;
        while (temp != nullptr) {
            if (temp->isLocked) return false;
            temp = temp->parent;
        }

        // Check descendants
        if (node->lockedDescendantsCount > 0) return false;

        node->isLocked = true;
        node->lockedBy = uid;

        // Update ancestors
        temp = node->parent;
        while (temp != nullptr) {
            temp->lockedDescendantsCount++;
            temp = temp->parent;
        }
        return true;
    }

    bool unlockNode(const string& name, int uid) {
        TreeNode* node = mp[name];

        if (!node->isLocked || node->lockedBy != uid)
            return false;

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
        TreeNode* node = mp[name];

        if (node->isLocked) return false;
        if (node->lockedDescendantsCount == 0) return false;

        // Check ancestors are not locked
        TreeNode* temp = node->parent;
        while (temp != nullptr) {
            if (temp->isLocked) return false;
            temp = temp->parent;
        }

        // BFS → find locked descendants
        queue<TreeNode*> q;
        for (auto child : node->children) q.push(child);

        vector<TreeNode*> lockedList;

        while (!q.empty()) {
            TreeNode* cur = q.front(); q.pop();

            if (cur->isLocked) {
                if (cur->lockedBy != uid) return false;
                lockedList.push_back(cur);
            }
            for (auto child : cur->children)
                q.push(child);
        }

        if (lockedList.empty()) return false;

        // Unlock all locked descendants
        for (TreeNode* desc : lockedList)
            unlockNode(desc->name, uid);

        // Lock the current node
        node->isLocked = true;
        node->lockedBy = uid;

        // update ancestors
        temp = node->parent;
        while (temp != nullptr) {
            temp->lockedDescendantsCount++;
            temp = temp->parent;
        }

        return true;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, m, Q;
    cin >> N >> m >> Q;

    vector<string> names(N);
    for (int i = 0; i < N; i++)
        cin >> names[i];

    LockingTree tree(names);

    // Build m-ary tree (same as your Java logic)
    for (int i = 1; i < N; i++) {
        int parIndex = (i - 1) / m;
        tree.buildEdge(names[parIndex], names[i]);
    }

    while (Q--) {
        int type, user;
        string name;

        cin >> type >> name >> user;
        bool ans = false;

        if (type == 1)
            ans = tree.lockNode(name, user);
        else if (type == 2)
            ans = tree.unlockNode(name, user);
        else
            ans = tree.upgradeNode(name, user);

        cout << (ans ? "true" : "false") << "\n";
    }

    return 0;
