// Design and implement a sorting algorithm using Merge Sort to efficiently arrange customer
// orders based on their timestamps. The solution should handle a large dataset (up to 1 million
// orders) with minimal computational overhead. Additionally, analyze the time complexity and
// compare it with traditional sorting techniques.

#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include <sstream>
using namespace std;
struct Transaction {
string txn_code;
time_t txn_time;
};
void populate_random_transactions(vector<Transaction> &txn_list, int total_txns) {
tm start_point = {};
start_point.tm_year = 2025 - 1900;
start_point.tm_mon = 9; // June (0-based)
start_point.tm_mday = 2;
start_point.tm_hour = 22;
time_t base_time = mktime(&start_point);
for (int idx = 0; idx < total_txns; idx++) {
int rand_offset_min = rand() % 100000;
time_t txn_timestamp = base_time + (rand_offset_min * 60);
txn_list.push_back({ "TXN" + to_string(idx + 1), txn_timestamp });
}
}
void merge_chunks(vector<Transaction> &txn_list, int left_idx, int mid_idx, int right_idx) {
int left_size = mid_idx - left_idx + 1;
int right_size = right_idx - mid_idx;
vector<Transaction> left_vec(txn_list.begin() + left_idx, txn_list.begin() + mid_idx + 1);
vector<Transaction> right_vec(txn_list.begin() + mid_idx + 1, txn_list.begin() + right_idx + 1);
int i = 0, j = 0, k = left_idx;

while (i < left_size && j < right_size) {
if (left_vec[i].txn_time <= right_vec[j].txn_time)
txn_list[k++] = left_vec[i++];
else
txn_list[k++] = right_vec[j++];
}
while (i < left_size)
txn_list[k++] = left_vec[i++];
while (j < right_size)
txn_list[k++] = right_vec[j++];
}
void sort_transactions(vector<Transaction> &txn_list, int left_idx, int right_idx) {
if (left_idx < right_idx) {
int mid_idx = left_idx + (right_idx - left_idx) / 2;
sort_transactions(txn_list, left_idx, mid_idx);
sort_transactions(txn_list, mid_idx + 1, right_idx);
merge_chunks(txn_list, left_idx, mid_idx, right_idx);
}
}
void show_first_entries(const vector<Transaction> &txn_list, int num_display) {
for (int i = 0; i < num_display && i < (int)txn_list.size(); i++) {
tm *utc_time = gmtime(&txn_list[i].txn_time);
char formatted[30];
strftime(formatted, sizeof(formatted), "%Y-%m-%dT%H:%M:%SZ", utc_time);
cout << "Transaction Code: " << txn_list[i].txn_code
<< ", Time: " << formatted << '\n';
}
}
int main() {
srand(static_cast<unsigned>(time(nullptr)));
const int TOTAL_RECORDS = 1000000;
vector<Transaction> txn_records;
txn_records.reserve(TOTAL_RECORDS);
populate_random_transactions(txn_records, TOTAL_RECORDS);

sort_transactions(txn_records, 0, TOTAL_RECORDS - 1);
show_first_entries(txn_records, 10);
return 0;
}
