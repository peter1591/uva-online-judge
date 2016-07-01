#include <iostream>
#include <vector>

class Solution
{
private:
	struct Entry {
		int max_length;
		bool track_included;
	};

public:
	void SetTotal(int n) { total_ = n; }

	void AddTrack(int len) { tracks_.push_back(len); }

	void Solve()
	{
		Entry **dp = new Entry*[tracks_.size()];
		for (int i = 0; i < tracks_.size(); ++i) {
			dp[i] = new Entry[total_ + 1];
		}

		for (int i = 0; i < tracks_.size(); ++i) {

			for (int j = 0; j <= total_; ++j) {
				if (j == 0) dp[i][j] = { 0, false };
				else if (i == 0) {
					if (j >= tracks_[i]) {
						dp[i][j] = { tracks_[i], true };
					}
					else {
						dp[i][j] = { 0, false };
					}
				}
				else {
					int with_track_i = -1;
					if (j >= tracks_[i]) {
						with_track_i = dp[i - 1][j - tracks_[i]].max_length + tracks_[i];
					}
					int without_track_i = dp[i - 1][j].max_length;
					if (with_track_i > without_track_i) {
						dp[i][j] = { with_track_i, true };
					}
					else {
						dp[i][j] = { dp[i - 1][j].max_length, false };
					}
				}
			}
		}

		int max_length = dp[tracks_.size() - 1][total_].max_length;
		this->PrintTraceBack(dp, tracks_.size() - 1, total_);
		std::cout << "sum:" << max_length << std::endl;

		for (int i = 0; i < tracks_.size(); ++i) delete[] dp[i];
		delete[] dp;
	}

	void PrintTraceBack(Entry ** dp, int track, int n)
	{
		Entry const& entry = dp[track][n];

		if (track < 0) return;

		if (entry.track_included) {
			PrintTraceBack(dp, track-1, n - tracks_[track]);
			std::cout << tracks_[track] << " ";
		}
		else {
			PrintTraceBack(dp, track-1, n);
		}
	}

private:
	int total_;
	std::vector<int> tracks_;
};

int main(void)
{
	while (true) {
		Solution solution;

		int n;
		std::cin >> n;
		if (!std::cin) break;
		solution.SetTotal(n);

		int m;
		std::cin >> m;
		for (int i=0; i<m; ++i) {
			int track;
			std::cin >> track;
			solution.AddTrack(track);
		}

		solution.Solve();
	}
}
