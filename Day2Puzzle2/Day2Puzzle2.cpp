#include <algorithm>
#include <fstream>
#include <iostream>
#include <ranges>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>

int main() {
	std::fstream input("input.txt");
	int sum = 0;
	std::unordered_map<std::string, int> maxCubes;
	auto calculateMax = [&maxCubes](const auto& _cubes) {
		std::string_view cubes(_cubes);
		int count;
		auto [ptr, err] = std::from_chars(cubes.data(), cubes.data() + cubes.size(), count);
		std::string color(ptr + 1, cubes.data() + cubes.size());
		maxCubes[color] = std::max(maxCubes[color], count);
	};
	std::string line;
	while (std::getline(input, line)) {
		for (const auto& game : line | std::views::split(std::string(": ")) | std::views::drop(1)) {
			for (const auto& set : game | std::views::split(std::string("; "))) {
				auto colors = set | std::views::split(std::string(", "));
				std::ranges::for_each(colors, calculateMax);
			}
			sum += maxCubes["red"] * maxCubes["green"] * maxCubes["blue"];
			maxCubes.clear();
		}
	}
	std::cout << sum << '\n';
}

