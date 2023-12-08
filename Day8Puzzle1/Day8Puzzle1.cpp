#include <fstream>
#include <iostream>
#include <ranges>
#include <string>
#include <unordered_map>
#include <utility>

struct Node {
	std::string name, left, right;
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
	for (Node& node : std::views::istream<Node>(input)) {
		nodes.insert(std::make_pair(node.name, std::move(node)));
		if (input.eof()) break;
	}
	int steps = 0, instructionCount = instructions.size();
	std::string currentNode = "AAA";
	while (currentNode != "ZZZ") {
		currentNode = instructions[steps % instructionCount] == 'L' ? nodes[currentNode].left : nodes[currentNode].right;
		++steps;
	}
	std::cout << steps;
}