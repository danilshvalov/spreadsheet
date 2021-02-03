#include "../common.h"

#include <iostream>
#include <charconv>

using namespace std;

std::string ConvertToTableNumberSystem(int number) {
	string result;
	while (number >= 0) {
		result += static_cast<char>('A' + number % 26);
		number /= 26;
		--number;
	}
	reverse(result.begin(), result.end());
	return result;
}
int ConvertFromTableNumberSystem(std::string_view number) {
	int result = 0;
	for (size_t pos = number.size(); pos > 0; --pos) {
		result += (number[pos - 1] - 'A' + (pos == number.size() ? 0 : 1)) * pow(26, number.size() - pos);
	}
	return result;
}

bool Position::operator==(const Position& rhs) const {
	return tie(row, col) == tie(rhs.row, rhs.col);
}

bool Position::operator<(const Position& rhs) const {
	return tie(row, col) == tie(rhs.row, rhs.col);
}

bool Position::IsValid() const {
	return col > 0 && col < kMaxCols&& row > 0 && row < kMaxRows;
}

string Position::ToString() const {
	return ConvertToTableNumberSystem(col) + to_string(row + 1);
}

Position Position::FromString(string_view str) {
	string_view col_view = ReadCols(str);
	str.remove_prefix(col_view.size());

	string_view row_view = ReadRows(str);
	str.remove_prefix(row_view.size());

	if (!str.empty() || col_view.empty() || row_view.empty()) {
		return INVALID_POSITION;
	}

	int row;
	auto convert_result = from_chars(row_view.data(), row_view.data() + row_view.size(), row);

	if (convert_result.ec == std::errc::invalid_argument ||
			convert_result.ec == std::errc::result_out_of_range) {
		return INVALID_POSITION;
	}

	return { .row = row - 1, .col = ConvertFromTableNumberSystem(col_view) };
}

bool Size::operator==(const Size& rhs) const {
	return tie(rows, cols) == tie(rhs.rows, rhs.cols);
}
