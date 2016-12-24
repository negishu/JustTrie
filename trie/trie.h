#ifndef TRIE_HPP
#define TRIE_HPP

#include <vector>
#include <queue>
#include <set>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

typedef __int32           int32_t;
typedef unsigned __int32 uint32_t;

typedef uint32_t Index;
const Index NOP = 0xFFFFFFFF;

struct Node {
    Index next;
    Index child;
    Node() : next(NOP), child(NOP) {}
    Node(Index next, Index child) : next(next), child(child) {}
    bool operator==(const Node &node) const
    {
        return next == node.next && child == node.child;
    }
    bool operator!=(const Node &node) const
    {
        return !(*this == node);
    }
};
inline std::ostream& operator << (std::ostream& out, const Node& node)
{
    out << node.next << ", " << node.child;
    return out;
}

typedef char Label;
const Label LEAF = '\0';

class Trie {
public:
    struct Path {
        string str;
        string::const_iterator position;
        Index current;
        int cost;
        Path(string str, string::const_iterator position, Index current, int cost) : str(str), position(position), current(current), cost(cost){}
    };
    struct Compare {
        bool operator()(const Path &a, const Path &b) {
            return a.cost < b.cost;
        }
    };
    vector<Node> nodes;
    vector<Label> labels;
    Trie();
    size_t size()     const;
    size_t nodeSize() const;
    void addNode(Label label, Index next, Index child);
    bool insert(const string &key);
    bool insert(string::const_iterator begin, string::const_iterator end, Index current);
    Index lookup(const string &key);
    Index lookup(string::const_iterator begin, string::const_iterator end, Index current);
    vector<string> predict(const string &key);
    vector<string> predict(string::const_iterator begin, string::const_iterator end, Index current);
    vector<string> predict(Index current);
    void traverse(Index current = 0, string str = "");
    set<string> correct(const string &key, int threshold);
    void read(const string &filename, const unsigned int nLineCnt = -1);
    void save(const string &filename);
    void load(const string &filename);
    bool operator==(const Trie &trie) const;
};
inline ostream& operator<<(ostream& out, const Trie& trie)
{
    out << "size: " << trie.size() << endl;
    for (size_t i = 0; i < trie.size(); ++i) {
        out << i << ": " << trie.labels[i] << " : " << trie.nodes[i] << endl;
    }
    return out;
}
#endif
