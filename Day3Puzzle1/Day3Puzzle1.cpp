#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <print>
#include <ranges>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <array>

auto numberBoundary = [](const auto& lhs, const auto& rhs) {
	const auto& [lIndex, lVal] = lhs;
	const auto& [rIndex, rVal] = rhs;
	return std::isdigit(lVal) == std::isdigit(rVal);
	};

auto numberChunk = [](const auto& chunk) {
	const auto& [index, val] = chunk.front();
	return std::isdigit(val);
	};

auto symbolChunk = [](const auto& chunk) {
	const auto& [index, val] = chunk.front();
	return !std::isdigit(val);
	};

auto removeIndex = [](const auto& t) {
	const auto& [index, val] = t; return val;
	};

int main() {
	std::ifstream input("input.txt");
	std::string line;
	int lineNumber = 0;
	std::unordered_map<int, std::unordered_map<int, std::pair<int, bool>*>> numberMap;
	while (std::getline(input, line)) {
		auto chunks = line | std::views::enumerate | std::views::chunk_by(numberBoundary);
		for (const auto& chunk : chunks | std::views::filter(numberChunk)) {
			int val = std::stoi(chunk | std::views::transform(removeIndex) | std::ranges::to<std::string>());
			std::pair<int, bool>* numPair = new std::pair<int, bool>(val, false);
			for (const auto& [index, _] : chunk) {
				numberMap[lineNumber][index] = numPair;
			}
		}
		++lineNumber;
	}
	lineNumber = 0;
	input.clear();
	input.seekg(0);
	while (std::getline(input, line)) {
		auto chunks = line | std::views::enumerate | std::views::chunk_by(numberBoundary);
		for (const auto& chunk : chunks | std::views::filter(symbolChunk)) {
			auto symbols = chunk | std::views::filter([](const auto& t) { const auto& [index, symbol] = t; return symbol != '.'; });
			static std::array delta{ -1, 0, 1 };
			for (const auto& [index, symbol] : symbols) {
				for (const auto& [rowDelta, columnDelta] : std::views::cartesian_product(delta, delta)) {
					if (numberMap.contains(lineNumber + rowDelta) and numberMap[lineNumber + rowDelta].contains(index + columnDelta))
						numberMap[lineNumber + rowDelta][index + columnDelta]->second = true;
				}
			}
		}
		++lineNumber;
	}
	int sum{};
	std::unordered_map<std::pair<int, bool>*, bool> seen;
	for (const auto& [row, map] : numberMap) {
		for (const auto& [column, number] : map) {
			if (!seen[number] and number->second) {
				seen[number] = true;
				sum += number->first;
			}
		}
	}
	std::cout << sum << '\n';
}