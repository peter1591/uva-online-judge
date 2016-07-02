#include <algorithm>
#include <climits>
#include <iostream>
#include <vector>

int FindIndex(int * dp, int dp_size, std::vector<int> const& numbers, int i)
{
	// numbers[dp[i]] is an increasing sequence
	// find largest index 'j' s.t. numbers[dp[j-1]] < numbers[i]

	int min = 0;
	int max = dp_size-1; // including

	while (true) {
		if (min >= max) {
			return min;
		}

		int middle = (min + max + 1) / 2; // ceil( (min+max)/2 )
		// we use ceiling, because we will divide into (min,middle-1) and (middle,max)
		// so the middle should be a slightly larger to make the two divisions equal

		int middle_idx = dp[middle - 1];
		if (middle_idx >= 0 && numbers[middle_idx] < numbers[i]) {
			min = middle; // this is a possible answer, but we need to find the largest index
		}
		else {
			max = middle - 1;
		}
	}
}

void PrintResult(std::vector<int> const& numbers, int * parent, int idx)
{
	if (idx < 0) return;
	PrintResult(numbers, parent, parent[idx]);
	std::cout << numbers[idx] << std::endl;
}

void Solve(std::vector<int> const& numbers)
{
	// dp[n] : index of the last number in the longest sequence with length 'n'
	// dp[n] = if dp[n-1] < numbers[n] < dp[n]

	int n = numbers.size();
	int * dp = new int[n];
	int * parent = new int[n];

	for (int i = 0; i < n; ++i) dp[i] = -1;

	dp[0] = 0;
	parent[0] = -1;
	int max_length = 1;

	for (int i = 1; i < n; ++i) {
		// Consider the new number: numbers[i]
		// (1) When considering this new number, only one of longest sequece will be changed
		//     I.e., only one entry in 'dp' will be modified

		int j = FindIndex(dp, n, numbers, i);

		dp[j] = i;
		max_length = std::max(j + 1, max_length);

		if (j == 0) parent[i] = -1;
		else parent[i] = dp[j - 1];
	}

	std::cout << max_length << std::endl;
	std::cout << "-" << std::endl;
	PrintResult(numbers, parent, dp[max_length - 1]);

	delete[] dp;
	delete[] parent;
}

int main(void)
{
	std::vector<int> numbers;
	while (true) {
		int n;
		std::cin >> n;
		if (!std::cin) break;
		numbers.push_back(n);
	}

	Solve(numbers);

	return 0;
}