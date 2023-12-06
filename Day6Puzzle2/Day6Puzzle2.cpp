#include <algorithm>
#include <charconv>
#include <fstream>
#include <iostream>
#include <ranges>
#include <string>
#include <string_view>

auto notEmpty = [](const auto& s) {
	return !std::string_view(s).empty();
	};

auto toInt = [](const auto& s) {
	long long val{};
	std::from_chars(s.data(), s.data() + s.size(), val);
	return val;
	};

int main() {
	std::ifstream input("input.txt");
	std::string timeLine, distanceLine;
	std::getline(input, timeLine);
	std::getline(input, distanceLine);
	auto maxTime = toInt((timeLine | std::views::split(std::string(": ")) | std::views::drop(1)).front() | std::views::split(std::string(" ")) | std::views::filter(notEmpty) | std::views::join | std::ranges::to<std::string>());
	auto maxDistance = toInt((distanceLine | std::views::split(std::string(": ")) | std::views::drop(1)).front() | std::views::split(std::string(" ")) | std::views::filter(notEmpty) | std::views::join | std::ranges::to<std::string>());
	auto toDistance = [maxTime](long long time) {
		return (maxTime - time) * time;
		};
	auto winningDistance = [maxDistance](long long distance) {
		return distance > maxDistance;
		};
	auto winCount = std::ranges::count_if(std::views::iota(0, maxTime) | std::views::transform(toDistance), winningDistance);
	std::cout << winCount;
}