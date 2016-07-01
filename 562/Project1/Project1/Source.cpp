#include <iostream>
#include <vector>

void Solve(std::vector<int> const& coins)
{
	int count = 0;
	for (auto const& coin : coins) count += coin;

	int table_size = count; // TODO: count/2 should be okay

	bool * dp = new bool[table_size + 1];
	dp[0] = true;
	for (int i = 1; i <= table_size; ++i) dp[i] = false;

	for (auto const& coin : coins) {
		for (int i = table_size; i >= coin; --i) {
			// dp[i] = dp[i] OR dp[i-coin_value]
			if (dp[i - coin]) dp[i] = true;
		}
	}

	int min_diff = count;
	for (int i = count / 2; i >= 0; --i) {
		if (!dp[i]) continue;

		int diff = (count - i) - i;
		if (diff < min_diff) min_diff = diff;
	}

	std::cout << min_diff << std::endl;
}

int main(void)
{
	int cases = 0;
	std::cin >> cases;

	for (int i = 0; i < cases; ++i) {
		int count = 0;
		std::cin >> count;

		std::vector<int> coins;

		for (int j = 0; j < count; ++j) {
			int coin = 0;
			std::cin >> coin;
			coins.push_back(coin);
		}

		Solve(coins);
	}
}