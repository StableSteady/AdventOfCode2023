#include <algorithm>
#include <fstream>
#include <iostream>
#include <ranges>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

auto notPossible = [](const auto& _color) {
	static std::unordered_map<std::string, int> maxCubes{ {"red", 12}, {"green", 13}, {"blue", 14} };
	std::string_view color(_color);
	int count{};
	auto [ptr, err] = std::from_chars(color.data(), color.data() + color.size(), count);
	return count > maxCubes[std::string(ptr + 1, color.data() + color.size())];
};

int main() {
	std::fstream input("input.txt");
	int sum = 0, id = 0;
	std::string line;
	while (std::getline(input, line)) {
		++id;
		for (const auto& game : line | std::views::split(std::string(": ")) | std::views::drop(1)) {
			bool validGame = true;
			for (const auto& set : game | std::views::split(std::string("; "))) {
				auto colors = set | std::views::split(std::string(", "));
				if (std::ranges::any_of(colors, notPossible)) {
					validGame = false;
					break;
				}
			}
			if (validGame) {
				sum += id;
			}
		}
	}
	std::cout << sum << '\n';
}

