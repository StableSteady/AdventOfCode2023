#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <ranges>
#include <set>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

auto toLongLong = [](const auto& s) {
	std::string_view num(s);
	long long val{};
	std::from_chars(num.data(), num.data() + num.size(), val);
	return val;
	};

int main() {
	std::ifstream input("input.txt");
	std::string line;
	std::getline(input, line);
	auto source = (line | std::views::split(std::string(": ")) | std::views::drop(1)).front() | std::views::split(std::string(" ")) | std::views::transform(toLongLong) | std::ranges::to<std::set>();
	std::set<long long> destination;
	std::getline(input, line); // skip empty line
	while (std::getline(input, line) or input.eof()) {
		if (std::ranges::contains(line, ':') or line.empty() or input.eof()) {
			std::ranges::move(source, std::inserter(destination, destination.begin()));
			std::swap(source, destination);
			destination.clear();
			if (input.eof()) break; else continue;
		}
		const auto& [destinationStart, sourceStart, length] = (line | std::views::split(std::string(" ")) | std::views::transform(toLongLong) | std::views::adjacent<3>).front();
		auto toDestination = [destinationStart, sourceStart](long long source) {
			return source + (destinationStart - sourceStart);
			};
		std::ranges::transform(source.lower_bound(sourceStart), source.lower_bound(sourceStart + length), std::inserter(destination, destination.begin()), toDestination);
		source.erase(source.lower_bound(sourceStart), source.lower_bound(sourceStart + length));
	}
	std::cout << *source.begin();
}