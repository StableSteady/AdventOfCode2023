#include <algorithm>
#include <charconv>
#include <cmath>
#include <fstream>
#include <iostream>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

auto notEmpty = [](const auto& s) {
	return !std::string_view(s).empty();
	};

auto toInt = [](const auto& s) {
	std::string_view num(s);
	int val{};
	std::from_chars(num.data(), num.data() + num.size(), val);
	return val;
	};

auto toWinCount = [](const auto& tuple) {
	const auto& [time, distance] = tuple;
	double discriminant = time * time - 4 * distance;
	if (discriminant <= 0)
		return 0;
	return 1 + static_cast<int>(std::floor((time + std::sqrt(discriminant)) / 2) - std::ceil((time - std::sqrt(discriminant)) / 2));
	};

int main() {
	std::ifstream input("input.txt");
	std::string timeLine, distanceLine;
	std::getline(input, timeLine);
	std::getline(input, distanceLine);
	auto times = (timeLine | std::views::split(std::string(": ")) | std::views::drop(1)).front() | std::views::split(std::string(" ")) | std::views::filter(notEmpty) | std::views::transform(toInt) | std::ranges::to<std::vector>();
	auto distances = (distanceLine | std::views::split(std::string(": ")) | std::views::drop(1)).front() | std::views::split(std::string(" ")) | std::views::filter(notEmpty) | std::views::transform(toInt) | std::ranges::to<std::vector>();
	auto winCounts = std::views::zip(times, distances) | std::views::transform(toWinCount);
	std::cout << std::ranges::fold_left_first(winCounts, std::multiplies<int>()).value();
}