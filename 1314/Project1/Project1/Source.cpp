#include <iostream>
#include <string>

int Solve(std::string const& s)
{
	if (s.empty()) return 0;

	int best_idx = 0;

	int n = s.length();

	int compared_len = 0;

	for (int current_idx = 1; ; ++current_idx)
		//for (int current_idx = 1; current_idx < 2*n; ++current_idx)
	{
		if (compared_len >= n) return best_idx;

		char ch_best = s[(best_idx + compared_len) % n];
		char ch_current = s[current_idx%n];

		if (ch_current == ch_best) {
			compared_len++;
		}
		else if (ch_current < ch_best) {
			// this is the new best
			best_idx = current_idx - compared_len;
			compared_len = 0;
		}
		else {
			// beat by the current best
			compared_len = 0;
		}
	}

	return best_idx;
}

int main(void)
{
	int cases = 0;
	std::cin >> cases;
	for (int i = 0; i < cases; ++i) {
		int len = -1;
		std::cin >> len;

		std::string s;
		s.reserve(len+1);

		std::cin >> s;

		std::cout << Solve(s) << std::endl;
	}
	return 0;
}