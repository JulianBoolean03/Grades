
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <numeric>
#include <string>
#include <vector>
using namespace std;
double mean(const std::vector<int> &v) {
  if (v.empty())
    return 0.0;
  double sum = std::accumulate(v.begin(), v.end(), 0.0);
  return sum / v.size();
}

class Student {
private:
  std::string Name;
  std::vector<int> Grades;

public:
  Student() = default;
  explicit Student(std::string name) : Name(std::move(name)) {}

  // Basic accessors
  void setName(const std::string &name) { Name = name; }
  const std::string &getName() const { return Name; }

  // Grades
  void addGrade(int grade) { Grades.push_back(grade); }
  const std::vector<int> &getGradesVec() const { return Grades; }

  void printGrades() const {
    cout << getName() << "'s grades are: ";
    for (auto it = Grades.begin(); it != Grades.end(); ++it) {
      if (it != Grades.begin())
        cout << ", ";
      cout << *it;
    }
    cout << '\n';
  }

  void dropLowestGrade() {
    if (Grades.empty()) {
      std::cout << "Cannot drop lowest grade from an empty vector.\n";
      return;
    }
    auto smallest = std::min_element(Grades.begin(), Grades.end());
    Grades.erase(smallest);
  }

  double getAverage() const { return mean(Grades); }

  double getMedian() const {
    if (Grades.empty())
      return 0.0;
    std::vector<int> v = Grades;
    std::sort(v.begin(), v.end());
    size_t n = v.size();
    if (n % 2 == 1)
      return static_cast<double>(v[n / 2]);
    return (v[n / 2 - 1] + v[n / 2]) / 2.0;
  }

  double getStdev() const {
    if (Grades.size() < 2)
      return 0.0;
    double m = getAverage();
    double sum_sq = 0.0;
    for (int x : Grades)
      sum_sq += (x - m) * (x - m);
    return std::sqrt(sum_sq / (Grades.size() - 1));
  }

  void computeStatistics() const {
    cout << fixed << setprecision(2) << "Mean: " << getAverage()
         << "  Median: " << getMedian() << "  StdDev: " << getStdev() << '\n';
  }
};

void list_students(const vector<Student> &students) {
  for (const auto &s : students)
    cout << s.getName() << '\n';
}

void add_student(vector<Student> &students, const Student &s) {
  students.push_back(s);
}

void remove_students(vector<Student> &students, const std::string &Name) {
  for (auto it = students.begin(); it != students.end(); ++it) {
    if (it->getName() == Name) {
      cout << it->getName() << " has been removed\n\n";
      students.erase(it);
      return;
    }
  }
  cout << "No student with name " << Name << " found.\n";
}

int partition_by_avg(vector<Student> &students, int low, int high) {
  double pivot = students[high].getAverage();
  int i = low - 1;
  for (int j = low; j < high; ++j) {
    if (students[j].getAverage() <= pivot) {
      ++i;
      std::swap(students[i], students[j]);
    }
  }
  std::swap(students[i + 1], students[high]);
  return i + 1;
}

void quickSort_by_avg(vector<Student> &students, int low, int high) {
  if (low < high) {
    int pi = partition_by_avg(students, low, high);
    quickSort_by_avg(students, low, pi - 1);
    quickSort_by_avg(students, pi + 1, high);
  }
}

void sort_by_average_quick(vector<Student> &students) {
  if (!students.empty())
    quickSort_by_avg(students, 0, (int)students.size() - 1);
}

void sort_by_average(vector<Student> &students, bool descending = false) {
  std::sort(students.begin(), students.end(),
            [&](const Student &a, const Student &b) {
              return descending ? (a.getAverage() > b.getAverage())
                                : (a.getAverage() < b.getAverage());
            });
}

void search_by_name(const vector<Student> &students,
                    const std::string &needle) {
  bool found = false;
  for (const auto &s : students) {
    if (s.getName() == needle) {
      cout << "Found: " << s.getName() << '\n';
      found = true;
    }
  }
  if (!found)
    cout << "No match for \"" << needle << "\"\n";
}

void class_stats(const vector<Student> &students) {

  vector<int> all;
  for (const auto &s : students) {
    const auto &g = s.getGradesVec();
    all.insert(all.end(), g.begin(), g.end());
  }

  cout << fixed << setprecision(2);
  cout << "Class grade count: " << all.size() << '\n';
  cout << "Class mean: " << mean(all) << '\n';

  // median
  double med = 0.0;
  if (!all.empty()) {
    sort(all.begin(), all.end());
    size_t n = all.size();
    med = (n % 2) ? all[n / 2] : (all[n / 2 - 1] + all[n / 2]) / 2.0;
  }
  cout << "Class median: " << med << '\n';

  // sample stdev
  double sd = 0.0;
  if (all.size() >= 2) {
    double m = mean(all);
    double sumsq = 0.0;
    for (int x : all)
      sumsq += (x - m) * (x - m);
    sd = std::sqrt(sumsq / (all.size() - 1));
  }
  cout << "Class stdev: " << sd << "\n";
}

int main() {
  vector<Student> roster;
  Student a("Alice");
  a.addGrade(90);
  a.addGrade(80);
  a.addGrade(100);
  Student b("Bob");
  b.addGrade(70);
  b.addGrade(75);
  b.addGrade(85);
  Student c("Cara");
  c.addGrade(95);
  c.addGrade(92);

  add_student(roster, a);
  add_student(roster, b);
  add_student(roster, c);

  cout << "== Students ==\n";
  list_students(roster);

  cout << "\n== Sort by average (ascending, STL) ==\n";
  sort_by_average(roster);
  for (const auto &s : roster) {
    cout << s.getName() << " avg=" << fixed << setprecision(2) << s.getAverage()
         << '\n';
  }

  cout << "\n== Remove Bob ==\n";
  remove_students(roster, "Bob");
  list_students(roster);

  cout << "\n== Class stats ==\n";
  class_stats(roster);

  return 0;
}
