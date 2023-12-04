#include <algorithm>
#include <fstream>
#include <iostream>
#include <ranges>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

auto notEmpty = [](const auto& s) {
	return !std::string_view(s).empty();
	};

auto toInt = [](const auto& s) {
	std::string_view num(s);
	int val{};
	std::from_chars(s.data(), s.data() + s.size(), val);
	return val;
	};

int main() {
	std::ifstream input("input.txt");
	std::string line;
	int sum = 0;
	while (std::getline(input, line)) {
		auto cards = (line 
						| std::views::split(std::string(": ")) 
						| std::views::drop(1)).front() 
						| std::views::split(std::string(" | ")) 
						| std::ranges::to<std::vector>();
		std::unordered_map<int, bool> isWinning;
		for (int num : cards.front() 
									| std::views::split(std::string(" ")) 
									| std::views::filter(notEmpty) 
									| std::views::transform(toInt)) 
		{
			isWinning[num] = true;
		}
		int points = 0;
		for (int num : cards.back() 
									| std::views::split(std::string(" ")) 
									| std::views::filter(notEmpty) 
									| std::views::transform(toInt)) 
		{
			if (isWinning[num]) {
				points <<= 1;
				points = std::max(points, 1);
			}
		}
		sum += points;
	}
	std::cout << sum << '\n';
}