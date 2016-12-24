#include <vector>
#include <queue>
#include <set>
#include <fstream>
#include <iostream>
#include <string>
#include <iterator>

int spliter(std::string& instring, const int c, std::vector<std::string> *output)
{
    std::back_insert_iterator<std::vector<std::string>> it(*output);
    const char *p = instring.data();
    const char *end = instring.size() + p;
    while (p != end) {
        if (*p == c) {
            ++p;
        }
        else {
            const char *start = p;
            while (++p != end && *p != c);
            it++ = std::string(start, p - start);
        }
    }
    return output->size();
}

#include "trie.h"
Trie::Trie()
{
    nodes.push_back(Node());
    labels.push_back(LEAF);
}
size_t Trie::size() const
{
    return nodes.size();
}
size_t Trie::nodeSize() const
{
    return sizeof(Node)+sizeof(Label);
}
void Trie::addNode(Label label, Index next, Index child)
{
    labels.push_back(label);
    nodes.push_back(Node(next, child));
}
bool Trie::insert(const string &key)
{
    string key_(key);
    key_.push_back(LEAF);
    return insert(key_.begin(), key_.end(), 0);
}
bool Trie::insert(string::const_iterator begin, string::const_iterator end, Index current)
{
    while (begin != end) {
        while (true) {
            if (labels[current] == *begin) {
                break;
            }
            if (nodes[current].next == NOP) {
                goto NEXT;
            }
            current = nodes[current].next;
        }
        begin++;
        current = nodes[current].child;
    }
    NEXT:
    if (begin == end) {
        return false;
    }
    if ((size_t)current < nodes.size()) {
        addNode(*begin, nodes[current].next, NOP);
        nodes[current].next = nodes.size() - 1;
        current = nodes.size() - 1;
        begin++;
    }
    while (begin != end) {
        addNode(*begin, NOP, NOP);
        if ((size_t)current < nodes.size()) {
            nodes[current].child = nodes.size() - 1;
        }
        current = nodes.size() - 1;
        begin++;
    }
    return true;
}
Index Trie::lookup(const string &key)
{
    string key_(key);
    key_.push_back(LEAF);
    return lookup(key_.begin(), key_.end(), 0);
}
Index Trie::lookup(string::const_iterator begin, string::const_iterator end, Index current)
{
    while (begin != end) {
        while (true) {
            if (labels[current] == *begin) {
                break;
            }
            if (nodes[current].next == NOP) {
                return NOP;
            }
            current = nodes[current].next;
        }
        begin++;
        if (begin == end) {
            return current;
        }
        current = nodes[current].child;
    }
    return NOP;
}
vector<string> Trie::predict(const string &key)
{
    vector<string> results = predict(key.begin(), key.end(), 0);
    for (string &result : results) {
        result.insert(result.begin(), key.begin(), key.end());
    }
    return results;
}
vector<string> Trie::predict(string::const_iterator begin, string::const_iterator end, Index current)
{
    while (begin != end) {
        while (true) {
            if (labels[current] == *begin) {
                break;
            }
            if (nodes[current].next == NOP) {
                return vector<string>();
            }
            current = nodes[current].next;
        }
        begin++;
        current = nodes[current].child;
    }
    return predict(current);
}
vector<string> Trie::predict(Index current)
{
    vector<string> results;
    if (nodes[current].child == NOP) {
        results.push_back("");
    }
    if (nodes[current].child != NOP) {
        vector<string> temp = predict(nodes[current].child);
        for (auto s : temp) {
            s.insert(s.begin(), labels[current]);
            results.push_back(s);
        }
    }
    if (nodes[current].next != NOP) {
        vector<string>temp = predict(nodes[current].next);
        results.insert(results.end(), temp.begin(), temp.end());
    }
    return results;
}
void Trie::traverse(Index current, string str)
{
    char label = labels.at(current);
    if (label == LEAF) {
        cout << str.c_str() << endl;
    }
    if (nodes.at(current).child != NOP) {
        traverse(nodes.at(current).child, str + label);
    }
    if (nodes.at(current).next != NOP) {
        traverse(nodes.at(current).next, str);
    }
}
set<string> Trie::correct(const string &key, int threshold)
{
    set<string> results;
    priority_queue<Path, vector<Path>, Compare> que;
    que.push(Path("", key.begin(), 0, 0));
    while (!que.empty()) {
        Path path = que.top();
        que.pop();
        Index i = path.current;
        while (i != NOP) {
            if (path.position == key.end()) {
                if (nodes[i].child == NOP) {
                    results.insert(path.str);
                }
                i = nodes[i].next;
                continue;
            }
            // Replace
            {
                Path new_path(path);
                new_path.cost += (labels[i] != *path.position);
                new_path.position++;
                new_path.current = nodes[i].child;
                new_path.str.push_back(labels[i]);

                if (new_path.cost <= threshold) {
                    que.push(new_path);
                }
            }
            // Insert
            {
                Path new_path(path);
                new_path.cost += 1;
                new_path.current = nodes[i].child;
                new_path.str.push_back(labels[i]);

                if (new_path.cost <= threshold) {
                    que.push(new_path);
                }
            }
            i = nodes[i].next;
        }
        // Delete
        Path new_path(path);
        if (new_path.position != key.end()) {
            new_path.cost += 1;
            new_path.position++;
            if (new_path.cost <= threshold) {
                que.push(new_path);
            }
        }
    }
    return results;
}
void Trie::read(const string &filename, const unsigned int nLineCnt)
{
    unsigned int nCnt = 0;
    ifstream ifs(filename);
    string line;
    while (getline(ifs, line)) {
        std::vector<std::string> segs;
        spliter(line, '\t', &segs);
        insert(segs[0]);
        nCnt++;

        if (nCnt == nLineCnt) break;
    }
    ifs.close();
}
void Trie::save(const string &filename)
{
    ofstream ofs(filename, ios::binary);
    ofs.write(labels.data(), labels.size() * sizeof(Label));
    ofs.write((Label*)nodes.data(), nodes.size() * sizeof(Node));
    ofs.close();
}
void Trie::load(const string &filename)
{
    labels.clear();
    nodes.clear();

    ifstream ifs(filename, ios::binary);
    ifs.seekg(0, ifs.end);
    size_t size = (size_t)(ifs.tellg() / nodeSize());
    ifs.seekg(0, ifs.beg);

    labels.resize(size);
    nodes.resize(size);

    ifs.read(labels.data(), size * sizeof(Label));
    ifs.read((Label*)nodes.data(), size * sizeof(Node));

    ifs.close();
}
bool Trie::operator==(const Trie &trie) const
{
    if (size() != trie.size()) {
        return false;
    }
    for (size_t i = 0; i < size(); ++i) {
        if (labels[i] != trie.labels[i] || nodes[i] != trie.nodes[i]) {
            return false;
        }
    }
    return true;
}
