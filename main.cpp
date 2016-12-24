#include ".\\Trie\\trie.h"

int build_main(int argc, char **argv)
{
    if (argc < 3) {
        return 0;
    }

    int nLineCnt = argc > 3 ? atoi(argv[3]) : 100;

    Trie trie;
    trie.read(argv[1], nLineCnt);
    trie.save(argv[2]);

    cout << "Node number: " << trie.size() << endl;
    cout << "Node size: " << trie.nodeSize() << endl;
    cout << "Total size: " << trie.size() * trie.nodeSize() << " bytes" << endl;
    cout << trie;

    return 0;
}

int correct_main(int argc, char **argv)
{
    if (argc < 2) {
        return 0;
    }
    Trie trie;
    trie.load(argv[1]);
    string query;
    while (std::cin >> query) {
        set<string> results = trie.correct(query, 2);
        for (string s : results) {
            cout << s << ", ";
        }
        cout << endl;
    }
    return 0;
}

int lookup_main(int argc, char **argv)
{
    if (argc < 2) {
        return 0;
    }
    Trie trie;
    trie.load(argv[1]);
    cout << trie;
    string query;
    while (cin >> query) {
        Index result = trie.lookup(query);
        cout << result << endl;
    }
    return 0;
}

int predict_main(int argc, char **argv)
{
    if (argc < 2) {
        return 0;
    }
    Trie trie;
    trie.load(argv[1]);
    string query;
    while (cin >> query) {
        vector<string> results = trie.predict(query);
        for (string s : results) {
            cout << s << ", ";
        }
        cout << endl;
    }
    return 0;
}

int traverse_main(int argc, char **argv)
{
    if (argc < 2) {
        return 0;
    }
    Trie trie;
    trie.load(argv[1]);
    int times = argc > 2 ? atoi(argv[2]) : 1;
    for (int i = 0; i < times; ++i) {
        trie.traverse();
    }
    return 0;
}

int main(int argc, char **argv)
{
    if (false)
    {
        char* argvalues[] = { "this", "count_1w.txt", "data.out" };
        build_main(sizeof(argvalues) / sizeof(char*), argvalues);
    }

    char* argvalues[] = { "this", "data.out" };
    traverse_main(sizeof(argvalues) / sizeof(char*), argvalues);

    if (false)
    {
        char* argvalues[] = { "this", "data.out" };
        predict_main(sizeof(argvalues) / sizeof(char*), argvalues);
    }
    if (false)
    {
        char* argvalues[] = { "this", "data.out" };
        lookup_main(sizeof(argvalues) / sizeof(char*), argvalues);
    }
    if (false)
    {
        char* argvalues[] = { "this", "data.out" };
        correct_main(sizeof(argvalues) / sizeof(char*), argvalues);
    }
    return 0;
}
