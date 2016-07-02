#include <iostream>
#include <vector>

class Solution
{
public:
	Solution() : accumulated_sum_(nullptr) {

	}

	~Solution() {
		if (accumulated_sum_) delete[] accumulated_sum_;
	}

	void AddNumber(int number) { numbers_.push_back(number); }

	void PrepareAccumulatedSum()
	{
		int n = numbers_.size();
		accumulated_sum_ = new int[n];

		accumulated_sum_[0] = numbers_[0];
		for (int i = 1; i < n; ++i) {
			accumulated_sum_[i] = accumulated_sum_[i - 1] + numbers_[i];
		}
	}

	int CalculateRangeSum(int inclusive_left, int inclusive_right)
	{
		int sum = accumulated_sum_[inclusive_right];
		if (inclusive_left > 0) sum -= accumulated_sum_[inclusive_left - 1];
		return sum;
	}

	// all dp[n][m] with (m-n)<(j-i) are guarnteed to be calculated
	void CalculateDP(int ** dp, int i, int j)
	{
		int max_diff = 0;
		bool max_diff_set = false;

		for (int pick = i; pick <= j; ++pick) {
			// pick (i) to (pick)
			int score = this->CalculateRangeSum(i, pick);
			int max_diff_opponent = 0;
			if (pick < j) max_diff_opponent = dp[pick + 1][j]; // at least one number for opponent
			int diff = score - max_diff_opponent;

			if (!max_diff_set || diff > max_diff) {
				max_diff_set = true;
				max_diff = diff;
			}

			// pick (pick) to (j)
			score = this->CalculateRangeSum(pick, j);
			max_diff_opponent = 0;
			if (pick > i) max_diff_opponent = dp[i][pick - 1];
			diff = score - max_diff_opponent;
			if (!max_diff_set || diff > max_diff) {
				max_diff_set = true;
				max_diff = diff;
			}
		}

		dp[i][j] = max_diff;
	}

	void Solve()
	{
		this->PrepareAccumulatedSum();

		int n = numbers_.size();

		int ** dp = new int*[n];
		for (int i = 0; i < n; ++i) dp[i] = new int[n];

		for (int idx_diff = 0; idx_diff < n; ++idx_diff)
		{
			for (int i = 0; i < n; ++i) {
				int j = i + idx_diff;

				if (j >= n) break;

				if (idx_diff == 0) {
					// left only one number, the first player can have only one choice to choose it
					dp[i][j] = numbers_[i];
					continue;
				}

				CalculateDP(dp, i, j);
			}
		}

		std::cout << dp[0][n - 1] << std::endl;

		for (int i = 0; i < n; ++i) delete[] dp[i];
		delete[] dp;
	}

private:
	std::vector<int> numbers_;
	int * accumulated_sum_;
};

int main(void)
{
	while (true) {
		int n = 0;
		std::cin >> n;
		if (n == 0) break;

		Solution solution;

		for (int i = 0; i < n; ++i) {
			int v = 0;
			std::cin >> v;
			solution.AddNumber(v);
		}

		solution.Solve();
	}

	return 0;
}