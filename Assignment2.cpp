// Movie Recommendation System Optimization A popular OTT platform, StreamFlix, offers
// personalized recommendations by sorting movies based on user preferences, such as IMDB rating,
// release year, or watch time popularity. However, during peak hours, sorting large datasets slows down
// the system. As a backend engineer, you must:
// ● Implement Quicksort to efficiently sort movies based on various user-selected parameters.
// ● Handle large datasets containing of movies while maintaining fast response times

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>
#include <algorithm>
using namespace std;
using namespace std::chrono;
struct Film {
string name;
float score;
int year_released;

int audience_count;
void print() const {
cout << name << " | Score: " << score
<< " | Year: " << year_released
<< " | Audience: " << audience_count << endl;
}
};
void customQuickSort(vector<Film> &collection, int low, int high, bool (*cmp)(const Film &, const Film
&)) {
if (low < high) {
int pivot_pos = low;
Film pivot_val = collection[high];
for (int i = low; i < high; i++) {
if (cmp(collection[i], pivot_val)) {
swap(collection[i], collection[pivot_pos]);
pivot_pos++;
}
}
swap(collection[pivot_pos], collection[high]);
customQuickSort(collection, low, pivot_pos - 1, cmp);
customQuickSort(collection, pivot_pos + 1, high, cmp);
}
}
bool cmpByScore(const Film &a, const Film &b) {
return a.score < b.score;
}
bool cmpByYear(const Film &a, const Film &b) {
return a.year_released < b.year_released;
}
bool cmpByAudience(const Film &a, const Film &b) {
return a.audience_count < b.audience_count;
}
vector<Film> readFilmsFromCSV(const string &filepath) {
vector<Film> collection;
ifstream fin(filepath);
if (!fin.is_open()) {
cerr << "Unable to open file: " << filepath << endl;
return collection;
}

string record;
getline(fin, record);
while (getline(fin, record)) {
stringstream ss(record);
string title_str, rating_str, year_str, crowd_str;
if (!getline(ss, title_str, ',') ||
!getline(ss, rating_str, ',') ||
!getline(ss, year_str, ',') ||
!getline(ss, crowd_str, ','))
{
continue;
}
try {
if (!title_str.empty()) {
Film f;
f.name = title_str;
f.score = stof(rating_str);
f.year_released = stoi(year_str);
f.audience_count = stoi(crowd_str);
collection.push_back(f);
}
} catch (const invalid_argument &) {
continue;
}
}
return collection;
}
int main() {
string file_name = "movies_real_titles.csv";
vector<Film> film_list = readFilmsFromCSV(file_name);
if (film_list.empty()) {
cout << "No films were loaded.\n";
return 1;
}
cout << "Sort films by (score/year/audience): ";
string sort_key;
cin >> sort_key;

transform(sort_key.begin(), sort_key.end(), sort_key.begin(), ::tolower);
bool (*cmp)(const Film &, const Film &);
if (sort_key == "score")
cmp = cmpByScore;
else if (sort_key == "year")
cmp = cmpByYear;
else if (sort_key == "audience")
cmp = cmpByAudience;
else {
cout << "Invalid input. Defaulting to score.\n";
cmp = cmpByScore;
}
auto start_t = high_resolution_clock::now();
customQuickSort(film_list, 0, film_list.size() - 1, cmp);
auto end_t = high_resolution_clock::now();
duration<double> elapsed = end_t - start_t;
cout << "\nTop 10 films sorted by " << sort_key << ":\n";
int top_index = max(0, static_cast<int>(film_list.size()) - 10);
for (int i = film_list.size() - 1; i >= top_index; --i) {
film_list[i].print();
}
cout << "\nSorted " << film_list.size() << " films in " << elapsed.count() << " seconds.\n";
return 0;
}
