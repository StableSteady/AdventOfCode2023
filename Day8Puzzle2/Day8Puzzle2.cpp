#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <ranges>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

struct Node {
	std::string name, left, right;
	bool isStart() { return name[2] == 'A'; };
	bool isEnd() { return name[2] == 'Z'; };
};

std::istream& operator>>(std::istream& is, Node& node) {
	std::string temp;
	is >> node.name >> temp >> node.left >> node.right;
	node.left = node.left.substr(1, 3);
	node.right = node.right.substr(0, 3);
	return is;
}

int main() {
	std::ifstream input("input.txt");
	std::string instructions;
	std::getline(input, instructions);
	std::unordered_map<std::string, Node> nodes;
	std::vector<std::string> startNodes;
	for (Node& node : std::views::istream<Node>(input)) {
		if (node.isStart())
			startNodes.push_back(node.name);
		nodes.insert(std::make_pair(node.name, std::move(node)));
		if (input.eof()) break;
	}
	auto toSteps = [&nodes, &instructions](std::string node) {
		int steps = 0, instructionCount = instructions.size();
		while (!nodes[node].isEnd()) {
			node = instructions[steps % instructionCount] == 'L' ? nodes[node].left : nodes[node].right;
			++steps;
		}
		return steps;
		};
	auto steps = startNodes | std::views::transform(toSteps);
	std::cout << *std::ranges::fold_left_first(steps, [](long long lhs, long long rhs) { return std::lcm(lhs, rhs); });
}