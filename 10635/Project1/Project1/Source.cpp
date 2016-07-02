#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>

class Solution
{
public:
	Solution(int size) : size_(size)
	{
		seq1_map_ = new int[size_*size_+1];
		for (int i = 0; i <= size_*size_; ++i) seq1_map_[i] = -1;
	}

	~Solution() {
		delete[] seq1_map_;
	}

	void AddNumberToLongerSequence(int v)
	{
		seq1_map_[v] = seq1_.size();
		seq1_.push_back(v);
	}

	void AddNumberToShorterSequence(int v)
	{
		seq2_.push_back(v);
	}

	void MakeShorterSequenceIndexMapping()
	{
		for (auto const& seq2_val : seq2_) {
			int idx = seq1_map_[seq2_val];
			if (idx >= 0) seq2_idx_map_.push_back(idx);
		}
	}

	int LongestIncreasingSubsequence(std::vector<int> const& seq)
	{
		// dp[i]: for the longest increasing sequence with length 'i', what is the smallest possible number for the last number
		
		int n = seq.size();
		int * dp = new int[n];
		for (int i = 0; i < n; ++i) dp[i] = -1;

		dp[0] = 0;
		int ans = 1;

		for (int i = 1; i < n; ++i)
		{
			// now consdier the i-th number in sequence 'seq'
			// (*) the dp[i] corresponds to an non-decreasing sequence
			// (*) only one element in 'dp' will be modified

			// use binary search to find the largest index 'idx' s.t. seq[dp[idx-1]] < seq[i]
			int min = 0;
			int max = n - 1;

			while (true) {
				if (min == max) {
					break;
				}

				int middle = (min + max + 1) / 2;

				if (dp[middle-1] < 0) {
					max = middle - 1;
				}
				else if (seq[dp[middle-1]] < seq[i]) {
					min = middle;
				}
				else {
					max = middle - 1;
				}
			}

			dp[min] = i;
			ans = std::max(min + 1, ans);
		}

		delete[] dp;

		return ans;
	}

	int Solve()
	{
		this->MakeShorterSequenceIndexMapping();

		// find longest increasing sequence in seq2_idx_map_
		return this->LongestIncreasingSubsequence(seq2_idx_map_);
	}

private:
	int size_;
	std::vector<int> seq1_; // longer
	std::vector<int> seq2_;

	int *seq1_map_;
	std::vector<int> seq2_idx_map_;
};

/*
int Solve(std::vector<int> const& seq1, std::vector<int> const& seq2)
{
	int n1 = seq1.size();
	int n2 = seq2.size();
	int ** dp = new int*[n1+1];
	for (int i = 0; i <= n1; ++i) dp[i] = new int[n2+1];

	// dp[i,j]: the longest common sequence for seq1[0~i-1] and seq2[0~j-1]
	// dp[i,j] = dp[i-1,j-1] + 1, if seq1[i] == seq2[j]
	//         = max(dp[i,j-1], dp[i-1,j]), if seq1[i] != seq2[j]

	for (int i = 0; i <= n1; ++i) {
		for (int j = 0; j <= n2; ++j) {
			if (i == 0 || j == 0) {
				dp[i][j] = 0;
				continue;
			}

			if (seq1[i - 1] == seq2[j - 1]) {
				dp[i][j] = dp[i - 1][j - 1] + 1;
			}
			else {
				dp[i][j] = std::max(dp[i][j - 1], dp[i - 1][j]);
			}
		}
	}

	return dp[n1][n2];

}

*/

int main(void)
{
	int cases = 0;
	std::cin >> cases;
	for (int i = 0; i < cases; ++i) {
		int size = 0;
		std::cin >> size;

		int n1 = 0, n2 = 0;
		std::cin >> n1 >> n2;

		Solution solution(size);

		bool reading_longer_sequence = n1 >= n2;

		for (int j = 0; j <= n1; ++j) {
			int v = 0;
			std::cin >> v;
			if (reading_longer_sequence) solution.AddNumberToLongerSequence(v);
			else solution.AddNumberToShorterSequence(v);
		}

		reading_longer_sequence = !reading_longer_sequence;
		for (int j = 0; j <= n2; ++j) {
			int v = 0;
			std::cin >> v;
			if (reading_longer_sequence) solution.AddNumberToLongerSequence(v);
			else solution.AddNumberToShorterSequence(v);
		}

		int ans = solution.Solve();
		std::cout << "Case " << i+1 << ": " << ans << std::endl;
	}
 }