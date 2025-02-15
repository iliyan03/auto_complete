#include <iostream>
#include <fstream>
#include "trie.hpp"
#include "file_watcher.hpp"

static void init_trie(trie& oTrie)
{
    auto add_to_trie = [&oTrie](std::string_view str, int n) {
        for (int i = 0; i < n; i++) {
            oTrie.add_entry(str);
        }
        };

    add_to_trie("tree", 10);
    add_to_trie("true", 35);
    add_to_trie("try", 29);
    add_to_trie("toy", 14);
    add_to_trie("ala_bala", 21);
    add_to_trie("alabalanica", 35);
    add_to_trie("aaaa", 23);
    add_to_trie("wish", 25);
    add_to_trie("win", 50);
}

trie oTrie;

void foo(std::string_view file_path)
{
    std::ifstream file(file_path.data());
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string prefix = buffer.str();

    std::vector<std::string> res;
    oTrie.get_top_k_by_prefix(prefix, 2, res);

    system("cls");
    for (std::string_view str : res)
    {
        std::cout << str << "\n";
    }
}

int main()
{
    init_trie(oTrie);
    
    constexpr std::string_view file_path = {text_file_path};
    using namespace std::chrono_literals;
    file_watcher watcher(file_path, 100ms);
    watcher.run(foo);
}