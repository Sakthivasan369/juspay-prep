#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <queue>

using namespace std;

// ---------------------------------------------------------
// NODE STRUCTURE
// ---------------------------------------------------------
struct TreeNode {
    string name;
    bool isLocked;
    int lockedByUid;
    
    // Total count of all locked nodes in the subtree
    int lockedDescendantsCount; 
    
    // Optimization Map: Maps uid -> number of locked descendants belonging to this user
    unordered_map<int, int> userLockCounts; 

    TreeNode* parent;
    vector<TreeNode*> children;

    TreeNode(string n, TreeNode* p) : 
        name(n), isLocked(false), lockedByUid(-1), 
        lockedDescendantsCount(0), parent(p) {}
};


// ---------------------------------------------------------
// LOCKING TREE CLASS
// ---------------------------------------------------------
class LockingTree {
private:
    TreeNode* root;
    unordered_map<string, TreeNode*> nameToNode;

    // Helper: Check if any ancestor is locked O(H)
    bool hasLockedAncestor(TreeNode* node) {
        TreeNode* curr = node->parent;
        while (curr != nullptr) {
            if (curr->isLocked) return true;
            curr = curr->parent;
        }
        return false;
    }

    // Helper: Propagate lock counts up the tree O(H)
    void updateAncestors(TreeNode* node, int uid, int netChange) {
        TreeNode* curr = node->parent;
        while (curr != nullptr) {
            curr->lockedDescendantsCount += netChange;
            curr->userLockCounts[uid] += netChange;
            
            // Memory optimization: remove key if count drops to 0
            if (curr->userLockCounts[uid] == 0) {
                curr->userLockCounts.erase(uid);
            }
            curr = curr->parent;
        }
    }

    // Helper: Unlock all descendants in O(K) where K is number of locked nodes
    void unlockDescendants(TreeNode* node, int uid, int& unlockedCount) {
        // PRUNING: If this user has no locks in this branch, skip entirely.
        if (node->userLockCounts.find(uid) == node->userLockCounts.end()) {
            return; 
        }

        for (TreeNode* child : node->children) {
            if (child->isLocked && child->lockedByUid == uid) {
                child->isLocked = false;
                child->lockedByUid = -1;
                unlockedCount++;
            }
            
            // Recurse down
            unlockDescendants(child, uid, unlockedCount);
            
            // Because the O(1) check passed earlier, we know ONLY this user 
            // held locks in this subtree. Since we removed them all, we can 
            // safely zero out the intermediate descendant counts.
            child->lockedDescendantsCount = 0;
            child->userLockCounts.clear();
        }
    }

public:
    // Build tree from list of names and m-ary degree
    LockingTree(vector<string>& nodes, int m) {
        root = new TreeNode(nodes[0], nullptr);
        nameToNode[nodes[0]] = root;
        
        queue<TreeNode*> q;
        q.push(root);
        
        int index = 1;
        while (index < nodes.size()) {
            TreeNode* curr = q.front();
            q.pop();
            
            for (int i = 0; i < m && index < nodes.size(); i++, index++) {
                TreeNode* child = new TreeNode(nodes[index], curr);
                curr->children.push_back(child);
                nameToNode[nodes[index]] = child;
                q.push(child);
            }
        }
    }

    // ---------------------------------------------------------
    // LOCK: O(H)
    // ---------------------------------------------------------
    bool lock(string name, int uid) {
        TreeNode* node = nameToNode[name];
        
        if (node->isLocked) return false;
        if (node->lockedDescendantsCount > 0) return false;
        if (hasLockedAncestor(node)) return false;

        node->isLocked = true;
        node->lockedByUid = uid;
        
        // Update ancestors with +1 lock
        updateAncestors(node, uid, 1);
        return true;
    }

    // ---------------------------------------------------------
    // UNLOCK: O(H)
    // ---------------------------------------------------------
    bool unlock(string name, int uid) {
        TreeNode* node = nameToNode[name];
        
        if (!node->isLocked || node->lockedByUid != uid) return false;

        node->isLocked = false;
        node->lockedByUid = -1;
        
        // Update ancestors with -1 lock
        updateAncestors(node, uid, -1);
        return true;
    }

    // ---------------------------------------------------------
    // UPGRADE: O(K + H) where K is number of locks removed
    // ---------------------------------------------------------
    bool upgrade(string name, int uid) {
        TreeNode* node = nameToNode[name];

        if (node->isLocked) return false;
        if (node->lockedDescendantsCount == 0) return false;
        if (hasLockedAncestor(node)) return false;

        // ==========================================
        // THE O(1) VERIFICATION CHECK
        // ==========================================
        // If the number of locks held by 'uid' in this subtree does NOT 
        // exactly equal the total number of locks in the subtree, it means 
        // either another user holds a lock, or this user holds no locks.
        auto it = node->userLockCounts.find(uid);
        if (it == node->userLockCounts.end() || it->second != node->lockedDescendantsCount) {
            return false;
        }

        // ==========================================
        // FAST UNLOCKING
        // ==========================================
        int unlockedCount = 0;
        unlockDescendants(node, uid, unlockedCount);

        // Lock the current node
        node->isLocked = true;
        node->lockedByUid = uid;

        // Rebalance the current node's state
        node->lockedDescendantsCount = 0;
        node->userLockCounts.clear();

        // Rebalance the ancestors. 
        // The subtree lost `unlockedCount` locks, but gained 1 lock (the current node).
        // Net change for all ancestors above this node: (1 - unlockedCount)
        updateAncestors(node, uid, 1 - unlockedCount);

        return true;
    }
};

// ---------------------------------------------------------
// DRIVER FUNCTION
// ---------------------------------------------------------
int main() {
    // 1. Build the tree
    // Structure (m=2):
    //          World
    //       /         \
    //    Asia        Africa
    //   /    \      /      \
    // China India Egypt  SouthAfrica
    vector<string> nodes = {
        "World", "Asia", "Africa", 
        "China", "India", "Egypt", "SouthAfrica"
    };
    
    LockingTree tree(nodes, 2);

    cout << "--- Testing Standard Locking ---\n";
    cout << "Lock China by User 1 (Expect 1): " << tree.lock("China", 1) << "\n";
    cout << "Lock India by User 1 (Expect 1): " << tree.lock("India", 1) << "\n";
    
    // Africa has no locked descendants, should succeed
    cout << "Lock Africa by User 2 (Expect 1): " << tree.lock("Africa", 2) << "\n";

    cout << "\n--- Testing Ancestor/Descendant Conflict ---\n";
    // Asia cannot be locked because China and India are locked
    cout << "Lock Asia by User 2 (Expect 0): " << tree.lock("Asia", 2) << "\n";

    cout << "\n--- Testing Upgrade Constraints ---\n";
    // User 2 tries to upgrade Asia, but User 1 holds the locks below.
    cout << "Upgrade Asia by User 2 (Expect 0): " << tree.upgrade("Asia", 2) << "\n";
    
    // User 1 tries to upgrade World, but User 2 holds Africa.
    cout << "Upgrade World by User 1 (Expect 0): " << tree.upgrade("World", 1) << "\n";

    cout << "\n--- Testing Successful Upgrade ---\n";
    // User 1 tries to upgrade Asia. User 1 holds ALL locks below Asia (China & India).
    // This perfectly triggers the O(1) verification map check.
    cout << "Upgrade Asia by User 1 (Expect 1): " << tree.upgrade("Asia", 1) << "\n";

    cout << "\n--- Verifying Post-Upgrade State ---\n";
    // China and India should now be unlocked, so attempting to unlock them should fail.
    cout << "Unlock China by User 1 (Expect 0, it was auto-unlocked): " << tree.unlock("China", 1) << "\n";
    
    // Asia should now be locked by User 1.
    cout << "Unlock Asia by User 1 (Expect 1): " << tree.unlock("Asia", 1) << "\n";

    return 0;
}