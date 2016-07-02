#include <algorithm>
#include <iostream>
#include <string>

int Solve(std::string const& s)
{
	// dp[i,j]: the longest palindrome length of the substring from i to j
	// dp[i,j] = dp[i+1,j-1]+1, if string[i] = string[j]
	// dp[i,j] = min(dp[i,j-1], dp[i+1,j]), if string[i] != string[j]

	int n = s.length();

	if (n == 0) return 0;

	int ** dp = new int*[n];
	for (int i = 0; i < n; ++i) dp[i] = new int[n];

	for (int diff = 0; diff < n; ++diff)
	{
		for (int i = 0; i < n; ++i) {
			int j = i + diff;

			if (j < 0 || j >= n) break;

			if (diff == 0) {
				dp[i][j] = 1; // only one character
				continue;
			}

			if (s[i] == s[j]) {
				dp[i][j] = 2;
				if (i + 1 <= j - 1) dp[i][j] += dp[i + 1][j - 1];
			}
			else {
				dp[i][j] = std::max(dp[i][j - 1], dp[i + 1][j]);
			}
		}
	}

	int ans = dp[0][n - 1];

	for (int i = 0; i < n; ++i) delete[] dp[i];
	delete[] dp;

	return ans;
}

int main(void)
{
	int n = 0;
	std::cin >> n;

	std::string dummy;
	std::getline(std::cin, dummy); // to consume the last newline character

	for (int i = 0; i < n; ++i) {
		std::string s;
		std::getline(std::cin, s);
		int ans = Solve(s);
		std::cout << ans << std::endl;
	}
	return 0;
}