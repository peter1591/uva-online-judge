#include <string>
#include <vector>
#include <iostream>
#include <limits>
#include <cmath>

class StudentGroupAssignment
{
public:
	StudentGroupAssignment() : v_(0) {}
	StudentGroupAssignment(int v) : v_(v) {}
	StudentGroupAssignment(StudentGroupAssignment const& rhs): v_(rhs.v_) {}

	void AddStudent(int n)
	{
		int flag = 1;
		flag = flag << n;
		v_ = (v_ | flag);
	}

	void RemoveStudent(int n)
	{
		int flag = 1;
		flag = flag << n;
		flag = ~flag;
		v_ = v_ & flag;
	}

	bool HasStudent(int n) const
	{
		int flag = 1;
		flag = flag << n;
		if ((v_ & flag) != 0) return true;
		else return false;
	}

	bool IsEmpty() const { return v_ == 0; }

	int GetNumber() const { return v_; }

public:
	static int GetMaximumNumber(int students) {
		int n = 1;
		n = n << (students + 1);
		return n - 1;
	}
	
private:
	constexpr static int max_students_ = 16;
	int v_;
};

class Solution
{
private:
	struct Student {
		std::string name;
		int x;
		int y;
	};

public:
	Solution(int n) :n_(n), dp_(nullptr)
	{
		int size = StudentGroupAssignment::GetMaximumNumber(n_)+1;
		dp_ = new double[size];
		for (int i = 0; i < size; ++i) dp_[i] = -2.0;
	}

	~Solution() {
		if (dp_) {
			delete[] dp_;
		}
	}

	void AddStudent(std::string const& name, int x, int y)
	{
		students_.push_back({ name, x, y });
	}

	double Solve()
	{
		StudentGroupAssignment group;
		for (int i = 0; i < n_; ++i) {
			group.AddStudent(i);
		}
		return this->GetAnswer(group);
	}

	double GetAnswer(StudentGroupAssignment group)
	{
		int dp_idx = group.GetNumber();

		if (group.IsEmpty()) 
			return 0.0;

		if (dp_[dp_idx] > -1.0) return dp_[dp_idx];

		// pick any two students to form a new group
		double min_dist = std::numeric_limits<double>::max();
		for (int i = 0; i < n_; ++i) {
			if (!group.HasStudent(i)) continue;
			for (int j = i+1; j < n_; ++j) {
				if (!group.HasStudent(j)) continue;

				StudentGroupAssignment new_group(group);
				new_group.RemoveStudent(i);
				new_group.RemoveStudent(j);

				double dist = this->GetDistance(i, j);
				double cost = dist + this->GetAnswer(new_group);
				if (cost < min_dist) {
					min_dist = cost;
				}
			}
		}

		dp_[dp_idx] = min_dist;

		return dp_[dp_idx];
	}

	double GetDistance(int i1, int i2)
	{
		Student const& s1 = students_[i1];
		Student const& s2 = students_[i2];
		double v = (s1.x - s2.x)*(s1.x - s2.x);
		v += (s1.y - s2.y) * (s1.y - s2.y);
		return std::sqrt(v);
	}

private:
	int n_;
	double * dp_;
	std::vector<Student> students_;
};

int main(void)
{
	int case_no = 0;
	while (true) {
		case_no++;

		int n = 0;
		std::cin >> n;
		if (n == 0) break;

		Solution solution(n*2);
		for (int i = 0; i < n * 2; ++i) {
			std::string name;
			std::cin >> name;
			int x = 0, y = 0;
			std::cin >> x >> y;
			solution.AddStudent(name, x, y);
		}

		double ans = solution.Solve();
		std::cout << "Case " << case_no << ": ";
		printf("%.2f", ans);
		std::cout << std::endl;
	}
}