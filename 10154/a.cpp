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

		// consider only the first turtle
		dp[0] = turtles_[0].weight;
		for (int i=1; i<n; ++i) {
			dp[i] = 0;
		}
		int maximum_height = 1;

		for (int i=1; i<n; ++i) {
			// add i-th turtle
			int load = turtles_[i].strength - turtles_[i].weight;

			for (int height=n-1; height>=0; --height) {
				if (dp[i-1] < 0) {
					// the previous (i-1) turtles cannot form a pile with height=(height-1)
					// so, it's impossible to form a pile with height=(height) with i-th turtle
					// dp[i] = -1; (not changed)
				} else {
					if (load >= dp[i-1]) {
						dp[i] += turtles_[i].weight;
						maximum_height = std::max(height, maximum_height);
					} else {
						dp[i] = -1;
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
