#include <iostream>
#include <list>
#include <memory>
#include <sstream>

struct OutEdge {
	long long int weight;
	int to;
};

class Graph
{
public:
	typedef std::list<OutEdge> OutEdges;

public:
	Graph(int vertex_count) : vertex_count_(vertex_count)
	{
		vertics_out_edges_ = new OutEdges[vertex_count_];
	}

	~Graph() {
		delete[] vertics_out_edges_;
	}

	Graph(Graph const&) = delete;
	Graph & operator=(Graph const&) = delete;

	void AddDirectedEdge(int from, int to, long long int weight)
	{
		OutEdge edge;
		edge.to = to;
		edge.weight = weight;
		vertics_out_edges_[from].push_back(edge);
	}

	long long int Solve(long long int path_min, long long int path_max)
	{
		return DFS(0, 1, path_min, path_max);
	}

	// return the 'best' possible value from the given vertex 'vertex'
	long long int DFS(int vertex, int level, long long int min, long long int max)
	{
		if (vertics_out_edges_[vertex].empty()) {
			// leaf
			if (min <= 0 && max >= 0) return 0; // a path satisfying the value constraint
			else return -1; // a path not satifying the value constraint
		}

		auto it_child = vertics_out_edges_[vertex].begin();
		long long int best_child_val = -1; // all weights are positive
		for (auto it_child = vertics_out_edges_[vertex].begin(); it_child != vertics_out_edges_[vertex].end(); ++it_child)
		{
			long long int child_val = DFS(it_child->to, level + 1, min - it_child->weight, max - it_child->weight);
			if (child_val < 0) continue;

			child_val += it_child->weight;

			if (level % 2 == 0) {
				// Choose the minimum one
				if (best_child_val<0 || child_val < best_child_val) best_child_val = child_val;
			}
			else {
				// Choose the maximum one
				if (best_child_val<0 || child_val > best_child_val) best_child_val = child_val;
			}
		}

		return best_child_val;
	}

private:
	int vertex_count_;
	OutEdges *vertics_out_edges_;
};

int main(void)
{
	while (true)
	{
		std::string line;
		if (!std::getline(std::cin, line)) break;

		std::stringstream ss(line);
		int n = 0;
		long long int l = 0, r = 0;
		ss >> n >> l >> r;

		Graph graph(n);

		while (true) {
			if (!std::cin) break;
			if (!std::getline(std::cin, line)) break;

			if (line.empty()) break;

			int from = -1, to = -1;
			long long int weight = -1;
			std::stringstream ss2(line);
			ss2 >> from >> to >> weight;

			graph.AddDirectedEdge(from, to, weight);
		}

		long long int ans = graph.Solve(l, r);
		if (ans < 0) {
			std::cout << "Oh, my god!" << std::endl;
		}
		else {
			std::cout << ans << std::endl;
		}
	}
}