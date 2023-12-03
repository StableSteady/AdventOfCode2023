#include <algorithm>
#include <array>
#include <cctype>
#include <fstream>
#include <iostream>
#include <map>
#include <ranges>
#include <string>
#include <utility>
#include <vector>

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

auto removeVal = [](const auto& t) {
	const auto& [index, val] = t; return index;
	};

int main() {
	std::ifstream input("input.txt");
	std::string line;
	int lineCount = 1;
	getline(input, line);
	int lineWidth = line.size();
	while (getline(input, line)) {
		++lineCount;
	}

	const int rows = lineCount, columns = lineWidth;
	std::vector <std::vector<std::pair<int, int>>> products(rows, std::vector<std::pair<int, int>>(columns, {1, 0})); // first = val, second = count
	lineCount = 0;
	input.clear();
	input.seekg(0);

	while (std::getline(input, line)) {
		auto chunks = line | std::views::enumerate | std::views::chunk_by(numberBoundary);
		for (const auto& chunk : chunks | std::views::filter(numberChunk)) {
			int val = std::stoi(chunk | std::views::transform(removeIndex) | std::ranges::to<std::string>());
			static std::array delta{ -1, 0, 1 };
			auto indices = std::views::cartesian_product(std::array{ lineCount }, chunk | std::views::transform(removeVal), delta, delta) | std::views::transform([](const auto& tuple) {
				const auto& [row, column, rowDelta, columnDelta] = tuple;
				return std::pair<int, int>(row + rowDelta, column + columnDelta);
				}) | std::views::filter([rows, columns](const auto& tuple) {
					const auto& [row, column] = tuple;
					return row >= 0 and row < rows and column >= 0 and column < columns;
					});
				std::map<std::pair<int, int>, bool> seen;
			for (const auto& tuple : indices) {
				if (seen[tuple])
					continue;
				const auto& [row, column] = tuple;
				seen[tuple] = true;
				products[row][column].first *= val;
				++products[row][column].second;
			}
		}
		++lineCount;
	}

	lineCount = 0;
	input.clear();
	input.seekg(0);
	int sum = 0;
	while (std::getline(input, line)) {
		auto chunks = line | std::views::enumerate | std::views::chunk_by(numberBoundary);
		for (const auto& chunk : chunks | std::views::filter(symbolChunk)) {
			auto gears = chunk | std::views::filter([](const auto& t) { const auto& [index, symbol] = t; return symbol == '*'; }) | std::views::transform(removeVal);
			for (int column : gears) {
				if (products[lineCount][column].second == 2) {
					sum += products[lineCount][column].first;
				}
			}
		}
		++lineCount;
	}
	std::cout << sum << '\n';
}