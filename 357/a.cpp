#include <iostream>

void PrintAnswer(int ans, int n)
{
	if (ans == 1) {
		std::cout << "There is only 1 way to produce " << n << " cents change." << std::endl;
	} else {
		std::cout << "There are " << ans << " ways to produce " << n << " cents change." <<std::endl;
	}
}

void Solve(int n)
{
	int coins[5] = {1, 5, 10, 25, 50};

	int *dp = new int[n];

	for (int i=0; i<n; ++i) dp[i] = 1;

	for (int coin=1; coin<5; ++coin) {
		int coin_value = coins[coin];
		for (int i=coin_value; i<n; ++i) {
			dp[i] += dp[i-coin_value];
		}
	}

	PrintAnswer(dp[n-1], n);

	delete [] dp;
}

int main(void)
{
	int n;

	while (true) {
		std::cin >> n;
		if (!std::cin) break;
		Solve(n);
	}
	return 0;
}
