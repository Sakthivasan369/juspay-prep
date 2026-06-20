import random

def generate_tle_testcase(filename="tle_testcase.txt"):
    N = 10000    # Maximum nodes
    M = 1        # m=1 creates a skewed tree (Linked List), maximizing Height (H = 10,000)
    Q = 50000    # Maximum queries

    with open(filename, 'w') as f:
        # Write limits
        f.write(f"{N} {M} {Q}\n")
        
        # Write node names (Node0, Node1, ..., Node9999)
        names = [f"Node{i}" for i in range(N)]
        f.write(" ".join(names) + "\n")
        
        # Write Queries
        # We will repeatedly lock and unlock the very LAST node (Node9999).
        # This forces the pointer to walk up 10,000 ancestors every single query.
        deepest_node = f"Node{N-1}"
        uid = 1
        
        for i in range(Q):
            if i % 2 == 0:
                # Lock
                f.write(f"1 {deepest_node} {uid}\n")
            else:
                # Unlock
                f.write(f"2 {deepest_node} {uid}\n")

if __name__ == "__main__":
    generate_tle_testcase()
    print("TLE Test Case generated as 'tle_testcase.txt'")