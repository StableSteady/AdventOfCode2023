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
	std::vector<int> winningNumberCount, cardCount;
	while (std::getline(input, line)) {
		winningNumberCount.push_back(0);
		cardCount.push_back(1);
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
		for (int num : cards.back() 
								| std::views::split(std::string(" ")) 
								| std::views::filter(notEmpty) 
								| std::views::transform(toInt)) {
			if (isWinning[num]) {
				winningNumberCount.back()++;
			}
		}
	}
	for (const auto& [cardNumber, winningNumbers] : winningNumberCount | std::views::enumerate) {
		int currentCount = cardCount[cardNumber];
		auto addCurrentCount = [currentCount](int& count) { count += currentCount; };
		std::ranges::for_each_n(cardCount.begin() + cardNumber + 1, winningNumbers, addCurrentCount);
	}
	std::cout << std::ranges::fold_left_first(cardCount, std::plus<int>()).value() << '\n';
}