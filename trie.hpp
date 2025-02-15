#include <string>
#include <vector>
#include <algorithm>

class trie
{
private:
	struct node {
		char key;
		int frequency;
		std::vector<node> children;
		node(char c) : 
			key(c), 
			frequency(0)
		{}
	};

	node m_root;
public:
	trie() : 
		m_root(' ') 
	{
	}

	// steps:
	// find node with prefix, for example prefix = "tr", start from root -> "t" -> "r"
	// then traverse all descendants of that node
	// add every descendant with a frequency > 0 to the result
	// sort the result
	// return the k most frequent entries
	void get_top_k_by_prefix(std::string_view prefix, int k, std::vector<std::string>& res)
	{
		node* start_node = find_node(prefix, &m_root);
		if (start_node == &m_root)
		{
			return;
		}

		std::vector<std::pair<std::string, int>> descendants;
		std::string str(prefix);
		get_descendants(*start_node, str, descendants);

		std::sort(descendants.begin(), descendants.end(), [](auto& a, auto& b) {
			return a.second > b.second;
		});

		std::transform(
			descendants.begin(),
			descendants.begin() + std::min(k, static_cast<int>(descendants.size())),
			std::back_inserter(res),
			[](std::pair<std::string, int> a) { return a.first; }
		);
	}

	void add_entry(std::string_view str) noexcept
	{
		add_entry(str, m_root);
	}

private:
	void get_descendants(node& current_node, std::string& str, std::vector<std::pair<std::string, int>>& res)
	{
		if (current_node.children.empty())
		{
			if(current_node.frequency)
				res.emplace_back(str, current_node.frequency);
			
			return;
		}
		
		for (node& child : current_node.children)
		{
			str.push_back(child.key);
			get_descendants(child, str, res);
			str.pop_back();
		}
	}

	node* find_node(std::string_view str, node* current_node)
	{
		char c;
		while (!str.empty())
		{
			c = str.front();

			bool found = false;
			for (node& child : current_node->children)
			{
				if (child.key == c)
				{
					current_node = &child;
					found = true;
					break;
				}
			}

			if (!found)
				break;

			str = str.substr(1);
			if (str.empty())
			{
				return current_node;
			}
		}

		return &m_root;
	}

	void add_entry(std::string_view str, node& current_node) noexcept
	{
		if (str.empty())
		{
			++current_node.frequency;
			return;
		}

		const char c = str.front();
		if (c == ' ')
		{
			return;
		}

		for (node& child : current_node.children)
		{
			if (child.key == c)
			{
				add_entry(str.substr(1), child);
				return;
			}
		}

		node& child = current_node.children.emplace_back(c);
		add_entry(str.substr(1), child);
	}
};