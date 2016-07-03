#include <iostream>
#include <string>
#include <list>
#include <cassert>

class Graph
{
public:
	struct ResultPathElement {
		std::string word;
		int next_vertex;
	};

public:
	Graph() {
		for (int i = 0; i < max_vertics_; ++i) {
			vertics_info_[i].in_degree = 0;
			vertics_info_[i].out_degree = 0;
		}
	}

	void AddWord(std::string const& word) {
		int from = (int)(word.front() - 'a');
		int to = (int)(word.back() - 'a');

		OutEdge edge;
		edge.to = to;
		edge.s = word;
		vertics_out_edges_[from].push_back(edge);

		vertics_info_[from].out_degree++;
		vertics_info_[to].in_degree++;
	}

	std::list<ResultPathElement> FindEularPath()
	{
		std::list<ResultPathElement> ret;

		int start = -1, end = -1;
		if (!this->GetEularPathStartEnd(&start, &end)) {
			return ret;
		}

		if (start < 0) {
			// any point can be the start/end
			for (int i = 0; i < max_vertics_; ++i) {
				if (vertics_info_[i].out_degree > 0) {
					start = i;
					end = i;
					break;
				}
			}
			if (start < 0) {
				// no any edge in this graph
				return ret;
			}
		}

		// use DFS to build an initial path
		DFS(start, end, &ret, ret.end());

		// check if any vertex still have out-going edges
		while (true)
		{
			int current_vertex = start;
			std::list<ResultPathElement>::iterator it = ret.begin();
			bool found = false;
			while (true) {
				if (vertics_info_[current_vertex].out_degree > 0) {
					found = true;
					break;
				}
				if (it == ret.end()) break;
				current_vertex = it->next_vertex;
				++it;
			}
			if (!found) break; // done

			// find a circle from 'current_vertex' to 'current_vertex'
			DFS(current_vertex, current_vertex, &ret, it);
		}

		// check if any edge in graph
		for (int i = 0; i < max_vertics_; ++i) {
			if (vertics_info_[i].in_degree > 0 || vertics_info_[i].out_degree > 0) {
				ret.clear();
				return ret;
			}
		}

		return ret;
	}

private:
	// return false if no eular path exists
	// return true with 'start=-1' and 'end=-1' if any point can be a start/end
	bool GetEularPathStartEnd(int * start, int * end)
	{
		*start = -1;
		*end = -1;
		for (int i = 0; i < max_vertics_; ++i) {
			int in = vertics_info_[i].in_degree;
			int out = vertics_info_[i].out_degree;

			if (out == in + 1) {
				if (*start >= 0) return false;
				*start = i;
			}
			if (in == out + 1) {
				if (*end >= 0) return false;
				*end = i;
			}
		}
		return true;
	}

	void DFS(int start, int end, std::list<ResultPathElement> * result, std::list<ResultPathElement>::iterator insert_pos)
	{
		OutEdges & edges = vertics_out_edges_[start];

		assert(!edges.empty());

		OutEdge edge = edges.front();

		// remove edge from graph
		edges.pop_front();
		vertics_info_[start].out_degree--;
		assert(vertics_info_[start].out_degree >= 0);
		vertics_info_[edge.to].in_degree--;
		assert(vertics_info_[edge.to].in_degree >= 0);

		ResultPathElement path;
		path.next_vertex = edge.to;
		path.word = edge.s;
		result->insert(insert_pos, path);

		if (end == edge.to) return;

		DFS(edge.to, end, result, insert_pos);
	}

private:
	struct OutEdge {
		int to;
		std::string s;
	};

	typedef std::list<OutEdge> OutEdges;

	static constexpr int max_vertics_ = 26;
	OutEdges vertics_out_edges_[max_vertics_];

	struct VertexInfo {
		int out_degree;
		int in_degree;
	};
	VertexInfo vertics_info_[26];
};

int main(void)
{
	int cases = 0;
	std::cin >> cases;
	for (int case_no = 0; case_no < cases; ++case_no)
	{
		Graph graph;

		int n = 0;
		std::cin >> n;

		for (int i = 0; i < n; ++i) {
			std::string s;
			std::cin >> s;
			graph.AddWord(s);
		}

		auto result = graph.FindEularPath();
		if (result.empty()) {
			std::cout << "***" << std::endl;
			continue;
		}

		auto it = result.begin();
		while (true) {
			std::cout << it->word;
			++it;
			if (it == result.end()) break;
			std::cout << ".";
		}
		std::cout << std::endl;
	}
	return 0;
}