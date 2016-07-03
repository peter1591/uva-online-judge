#include <algorithm>
#include <iostream>
#include <string>

class HourGlass
{
private:
	struct Entry
	{
		long long int ways;
		int next_row_idx;
		bool next_row_is_left;
	};

	void PrintRoute(int row, int col, int s, Entry *** dp)
	{
		int val = data_[row][col];

		if (row == 0) {
			std::cout << col << " ";
		}
		if (row == size_ * 2 - 2) {
			std::cout << std::endl;
			return; // the last row
		}

		if (dp[row][col][s].next_row_is_left) std::cout << "L";
		else std::cout << "R";

		PrintRoute(row + 1, dp[row][col][s].next_row_idx, s - val, dp);
	}

public:
	HourGlass(int size) : size_(size)
	{
		data_ = new int*[2 * size - 1];
		for (int i = 0; i < (2 * size - 1); ++i) {
			data_[i] = new int[size];
		}
	}

	~HourGlass()
	{
		for (int i = 0; i < (2 * size_ - 1); ++i) {
			delete[] data_[i];
		}
		delete[] data_;
	}

	HourGlass(HourGlass const&) = delete;
	HourGlass & operator=(HourGlass const&) = delete;

	int GetRowElementsCount(int row) const
	{
		// 0 is the top one; size_-1 is the middle one; size_*2-2 is the bottom one
		if (row <= size_ - 1) {
			// top half
			return size_ - row;
		}
		else {
			// bottom half
			row -= size_ - 1;
			return row + 1;
		}
	}

	void ReadInput()
	{
		for (int i = 0; i < (2 * size_ - 1); ++i) {
			for (int j = 0; j < GetRowElementsCount(i); ++j) {
				std::cin >> data_[i][j];
			}
		}
	}

	void GetNextRowAdjacentIndics(int row, int col, int * left_col, int * right_col)
	{
		if (row < size_ - 1) { // top half, excluding middle row
			*left_col = -1;
			*right_col = -1;
			if (col > 0) *left_col = col - 1;
			if (col < this->GetRowElementsCount(row + 1)) *right_col = col;
		}
		else {
			*left_col = col;
			*right_col = col + 1;
		}
	}

	void Solve(int s)
	{
		// dp[i][j][k]: the #-of-ways to use the rows (i~bottom), the j-th column, to form a sequence sum with k
		Entry *** dp = new Entry**[size_ * 2 - 1];
		for (int i = 0; i < size_ * 2 - 1; ++i) {
			dp[i] = new Entry*[this->GetRowElementsCount(i)];
			for (int j = 0; j < this->GetRowElementsCount(i); ++j) {
				dp[i][j] = new Entry[s + 1];
			}
		}

		for (int i = size_ * 2 - 2; i >= 0; --i) { // from last row to first
			for (int j = 0; j < this->GetRowElementsCount(i); ++j) {
				int cell_number = data_[i][j];
				if (i == size_ * 2 - 2) {
					for (int k = 0; k <= s; ++k) {
						dp[i][j][k] = { 0, -1, false };
					}
					dp[i][j][cell_number] = { 1, -1, false }; // itself
					continue;
				}

				int left_col = -1, right_col = -1;
				this->GetNextRowAdjacentIndics(i, j, &left_col, &right_col);

				for (int k = 0; k <= s; ++k) {
					if (k < cell_number) {
						dp[i][j][k] = { 0,-1, false };
						continue;
					}

					dp[i][j][k] = { 0, -1, false };

					if (left_col >= 0) {
						long long int ways = dp[i + 1][left_col][k - cell_number].ways;
						if (ways > 0) {
							if (dp[i][j][k].ways == 0) {
								dp[i][j][k].next_row_idx = left_col;
								dp[i][j][k].next_row_is_left = true;
							}
							dp[i][j][k].ways += ways;
						}
					}

					if (right_col >= 0) {
						long long int ways = dp[i + 1][right_col][k - cell_number].ways;
						if (ways > 0) {
							if (dp[i][j][k].ways == 0) {
								dp[i][j][k].next_row_idx = right_col;
								dp[i][j][k].next_row_is_left = false;
							}
							dp[i][j][k].ways += ways;
						}
					}
				}
			}
		}

		long long int ans = 0;
		int ans_start_col = -1;
		for (int i = 0; i < this->GetRowElementsCount(0); ++i) {
			if (dp[0][i][s].ways == 0) continue;
			if (ans_start_col < 0) ans_start_col = i;
			ans += dp[0][i][s].ways;
		}
		std::cout << ans << std::endl;

		if (ans_start_col >= 0) {
			this->PrintRoute(0, ans_start_col, s, dp);
		}
		else {
			std::cout << std::endl;
		}

		// Get runtime error on Uva online judge. Why?
		/*for (int i = 0; i < size_ * 2 - 1; ++i) {
			for (int j = 0; j < this->GetRowElementsCount(i); ++j) {
				delete[] dp[i][j];
			}
			delete[] dp[i];
		}
		delete[] dp;*/
	}

private:
	const int size_;
	int ** data_;
};

int main(void)
{
	while (true)
	{
		int n = 0, s = 0;
		std::cin >> n >> s;

		if (n == 0 && s == 0) break;

		HourGlass hour_glass(n);
		hour_glass.ReadInput();
		hour_glass.Solve(s);
	}
	return 0;
}