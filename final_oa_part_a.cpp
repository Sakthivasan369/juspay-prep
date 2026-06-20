#include <bits/stdc++.h>
using namespace std;

struct TreeNode {
    string name;
    bool isLocked;
    int lockedBy;
    int lockedDescendantsCount;

    // uid -> count of locked descendants
    unordered_map<int, int> descendantUIDs;

    TreeNode* parent;
    vector<TreeNode*> children;

    TreeNode(string s)
        : name(s),
          isLocked(false),
          lockedBy(0),
          lockedDescendantsCount(0),
          parent(nullptr) {}
};

class LockingTree {
private:
    unordered_map<string, TreeNode*> mp;

    bool checkAncestors(TreeNode* node) {
        TreeNode* curr = node->parent;

        while (curr) {
            if (curr->isLocked)
                return false;

            curr = curr->parent;
        }

        return true;
    }

    void updateAncestors(TreeNode* node, int uid, int delta) {
        TreeNode* curr = node->parent;

        while (curr) {
            curr->lockedDescendantsCount += delta;

            curr->descendantUIDs[uid] += delta;

            if (curr->descendantUIDs[uid] == 0)
                curr->descendantUIDs.erase(uid);

            curr = curr->parent;
        }
    }

    int unlockSubtree(TreeNode* node) {
        int freedLocks = 0;

        if (node->isLocked) {
            node->isLocked = false;
            node->lockedBy = 0;
            freedLocks++;
        }

        if (node->lockedDescendantsCount > 0) {
            for (TreeNode* child : node->children) {
                freedLocks += unlockSubtree(child);
            }

            node->lockedDescendantsCount = 0;
            node->descendantUIDs.clear();
        }

        return freedLocks;
    }

public:
    LockingTree(const vector<string>& names) {
        mp.reserve(names.size());

        for (const string& name : names) {
            mp[name] = new TreeNode(name);
        }
    }

    ~LockingTree() {
        for (auto& p : mp) {
            delete p.second;
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

        if (node->isLocked)
            return false;

        if (node->lockedDescendantsCount > 0)
            return false;

        if (!checkAncestors(node))
            return false;

        node->isLocked = true;
        node->lockedBy = uid;

        updateAncestors(node, uid, 1);

        return true;
    }

    bool unlockNode(const string& name, int uid) {
        TreeNode* node = mp[name];

        if (!node->isLocked)
            return false;

        if (node->lockedBy != uid)
            return false;

        node->isLocked = false;
        node->lockedBy = 0;

        updateAncestors(node, uid, -1);

        return true;
    }

    bool upgradeNode(const string& name, int uid) {
        TreeNode* node = mp[name];

        if (node->isLocked)
            return false;

        if (node->lockedDescendantsCount == 0)
            return false;

        if (!checkAncestors(node))
            return false;

        // O(1) verification
        if (node->descendantUIDs.size() != 1)
            return false;

        if (node->descendantUIDs.begin()->first != uid)
            return false;

        int totalFreedLocks = unlockSubtree(node);

        node->isLocked = true;
        node->lockedBy = uid;

        int netChange = 1 - totalFreedLocks;

        updateAncestors(node, uid, netChange);

        return true;
    }
};

int main() {
    freopen("tle_testcase.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, m, Q;
    cin >> N >> m >> Q;

    vector<string> names(N);

    for (int i = 0; i < N; i++) {
        cin >> names[i];
    }

    LockingTree tree(names);

    for (int i = 1; i < N; i++) {
        int parentIndex = (i - 1) / m;
        tree.buildEdge(names[parentIndex], names[i]);
    }

    while (Q--) {
        int operation, uid;
        string nodeName;

        cin >> operation >> nodeName >> uid;

        bool result = false;

        if (operation == 1)
            result = tree.lockNode(nodeName, uid);
        else if (operation == 2)
            result = tree.unlockNode(nodeName, uid);
        else if (operation == 3)
            result = tree.upgradeNode(nodeName, uid);

        cout << (result ? "true" : "false") << '\n';
    }

    return 0;
}


