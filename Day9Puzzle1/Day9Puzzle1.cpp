#include <algorithm>
#include <fstream>
#include <iostream>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

auto toInt = [](const auto& s) {
	std::string_view num(s);
	int val{};
	std::from_chars(num.data(), num.data() + num.size(), val);
	return val;
	};

auto toDifference = [](int first, int second) {
	return second - first;
	};

int predict(auto&& sequence) {
	if (std::ranges::all_of(sequence, [](int i) { return i == 0; }))
		return 0;
	else
		return sequence.back() + predict(sequence | std::views::pairwise_transform(toDifference) | std::ranges::to<std::vector>());
}

int main() {
	std::ifstream input("input.txt");
	std::string line;
	int sum = 0;
	while (std::getline(input, line)) {
		auto history = line | std::views::split(std::string(" ")) | std::views::transform(toInt) | std::ranges::to<std::vector>();
		sum += predict(history);
	}
	std::cout << sum;
}