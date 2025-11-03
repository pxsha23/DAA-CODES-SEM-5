// A massive earthquake has struck a remote region, and a relief organization is transporting
// essential supplies to the affected area. The organization has a limited-capacity relief truck
// that can carry a maximum weight of W kg. They have N different types of essential items,
// each with a specific weight and an associated utility value (importance in saving lives and
// meeting urgent needs). Since the truck has limited capacity, you must decide which items to
// include to maximize the total utility value while ensuring the total weight does not exceed
// the truck's limit.

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct Item {
int weight;
int utility;
bool perishable;
};

bool cmpRatio(const Item &a, const Item &b) {
double r1 = (double)a.utility / a.weight;
double r2 = (double)b.utility / b.weight;
return r1 > r2;
}

int knapsackDP(const vector<Item>& items, int W, vector<int>& chosenItems) {
int N = items.size();
vector<vector<int>> dp(N + 1, vector<int>(W + 1, 0));

for (int i = 1; i <= N; ++i) {
int wt = items[i-1].weight;
int val = items[i-1].utility;
for (int w = 0; w <= W; ++w) {
if (wt <= w)
dp[i][w] = max(dp[i-1][w], dp[i-1][w - wt] + val);
else
dp[i][w] = dp[i-1][w];
}
}

chosenItems.clear();
int w = W;

for (int i = N; i > 0; --i) {
if (dp[i][w] != dp[i-1][w]) {
chosenItems.push_back(i-1);
w -= items[i-1].weight;
}
}
reverse(chosenItems.begin(), chosenItems.end());

return dp[N][W];
}

int knapsackGreedy(const vector<Item>& items, int W, vector<int>& chosenItems) {
int totalWeight = 0, totalUtility = 0;
chosenItems.clear();
vector<int> indices(items.size());
for (int i = 0; i < (int)items.size(); ++i) indices[i] = i;

sort(indices.begin(), indices.end(), [&](int a, int b){
return (double)items[a].utility / items[a].weight > (double)items[b].utility /
items[b].weight;
});

for (int i : indices) {
if (totalWeight + items[i].weight <= W) {
totalWeight += items[i].weight;
totalUtility += items[i].utility;
chosenItems.push_back(i);
}
}
return totalUtility;
}

int main() {
int N, W;
cout << "Enter number of item types and truck capacity (weight): ";
cin >> N >> W;

vector<Item> items(N);
cout << "Enter weight, utility and perishable(1/0) for each item:\n";
for (int i = 0; i < N; ++i) {
int w, u, p;
cin >> w >> u >> p;
items[i] = {w, u, (bool)p};
}

vector<Item> prioritizedItems = items;
for (auto &item : prioritizedItems) {
if (item.perishable) {
item.utility = int(item.utility * 1.2);
}
}

vector<int> chosenGreedy, chosenDP;
int greedyUtility = knapsackGreedy(prioritizedItems, W, chosenGreedy);
int dpUtility = knapsackDP(items, W, chosenDP);

cout << "\nGreedy approach utility: " << greedyUtility << "\nChosen items (indexes): ";
for (int i : chosenGreedy) cout << i << " ";
cout << "\n";

cout << "\nDP approach utility (optimal): " << dpUtility << "\nChosen items (indexes): ";
for (int i : chosenDP) cout << i << " ";

cout << "\n";

return 0;
}
