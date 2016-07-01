#include <algorithm>
#include <array>
#include <iostream>
#include <vector>

struct Turtle
{
	int weight;
	int strength;
};

bool TurtleComparator(Turtle const& lhs, Turtle const& rhs)
{
	return lhs.strength < rhs.strength;
}

class Solution
{
public:
	void Add(int weight, int strength)
	{
		turtles_.push_back({weight, strength});
	}

	int Solve()
	{
		// It is proved that an optimal turtle pile has the following property:
		//    (*) A turtle with a larger strength is always nearer to the bottom
		//        If not, it can be swapped to be nearer to the bottom
		std::sort(turtles_.begin(), turtles_.end(), TurtleComparator);

		if (turtles_.empty()) return 0;

		int n = (int)turtles_.size();
		int *dp = new int[n];

		for (int i=0; i<n; ++i) {
			dp[i] = -1;
		}
		int maximum_height = 0;

		for (int i=0; i<n; ++i) {
			// add i-th turtle
			int load = turtles_[i].strength - turtles_[i].weight;

			for (int height=n-1; height>=0; --height) {
				if (height == 0) {
					if (dp[0] < 0) dp[0] = turtles_[i].weight;
					else dp[0] = std::min(dp[0], turtles_[i].weight);
					continue;
				}

				if (dp[height-1] < 0) {
					// the previous (i-1) turtles cannot form a pile with height=(height-1)
					// so, it's impossible to form a pile with height=(height) with i-th turtle
					// dp[height] not changed
				} else {
					if (load >= dp[height-1]) {
						int new_weight = dp[height-1] + turtles_[i].weight;

						if (dp[height] < 0) dp[height] = new_weight;
						else dp[height] = std::min(new_weight, dp[height]);

						maximum_height = std::max(height+1, maximum_height);
					} else {
						// dp[height] not changed
					}
				}
			}
		}

		delete [] dp;

		return maximum_height;
	}

private:
	std::vector<Turtle> turtles_;
};

int main(void)
{
	Solution solution;

	while (true)
	{
		int weight;
		int strength;

		std::cin >> weight;
		std::cin >> strength;

		if (!std::cin) break;

		solution.Add(weight, strength);
	}

	std::cout << solution.Solve() << std::endl;
	return 0;
}
