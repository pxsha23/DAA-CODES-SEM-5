// A devastating flood has hit multiple villages in a remote area, and the government, along with
// NGOs, is organizing an emergency relief operation. A rescue team has a limited-capacity boat that
// can carry a maximum weight of W kilograms. The boat must transport critical supplies, including
// food, medicine, and drinking water, from a relief center to the affected villages.
// Each type of relief item has:
// ● A weight (wi) in kilograms.
// ● Utility value (vi) indicating its importance (e.g., medicine has higher value than food).
// ● Some items can be divided into smaller portions (e.g., food and water), while others must
// be taken as a whole (e.g., medical kits).

#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

struct Item {
string name;
double weight;
double value;
bool divisible;
int priority;

Item(string n, double w, double v, bool d, int p)
: name(n), weight(w), value(v), divisible(d), priority(p) {}

6

double valuePerWeight() const {
return value / weight;
}
};
bool compare(const Item& a, const Item& b) {
if (a.priority == b.priority)
return a.valuePerWeight() > b.valuePerWeight();
return a.priority < b.priority;
}
double fractionalKnapsack(vector<Item> &items, double capacity, double
&totalWeightCarried) {
sort(items.begin(), items.end(), compare);
cout << "\nSorted Items (by Priority, then Value/Weight): \n";
cout << left << setw(20) << "Item"
<< setw(10) << "Weight"
<< setw(10) << "Value"
<< setw(12) << "Priority"
<< setw(15) << "Value/Weight"
<< setw(15) << "Type" << "\n";
for (const auto& item : items) {
cout << left << setw(20) << item.name
<< setw(10) << item.weight
<< setw(10) << item.value
<< setw(12) << item.priority

7

<< setw(15) << fixed << setprecision(2) << item.valuePerWeight()
<< setw(15) << (item.divisible ? "Divisible" : "Indivisible")
<< "\n";
}
double totalValue = 0.0;
totalWeightCarried = 0.0;
cout << "\nItems selected for transport:\n";
for (const auto& item : items) {
if (capacity <= 0) break;
if (item.divisible) {
double takenWeight = min(item.weight, capacity);
double takenValue = item.valuePerWeight() * takenWeight;
totalValue += takenValue;
capacity -= takenWeight;
totalWeightCarried += takenWeight;

cout << "- " << item.name << ": " << takenWeight << " kg, Utility = " << takenValue
<< ", Priority = " << item.priority << ", Type = Divisible\n";
} else {
if (item.weight <= capacity) {
totalValue += item.value;
capacity -= item.weight;
totalWeightCarried += item.weight;

8
cout << "- " << item.name << ": " << item.weight << " kg, Utility = " <<

item.value

<< ", Priority = " << item.priority << ", Type = Indivisible\n";
}
}
}
return totalValue;
}
int main() {
int n;
cout << "Enter number of items to load: ";
cin >> n;
vector<Item> items;
for (int i = 0; i < n; i++) {
string name;
double weight, value;
int priority;
char divisibleChar;
bool divisible;
cout << "Enter details for item " << i + 1 << ":\n";
cout << "Name: ";
cin.ignore();
getline(cin, name);

9

cout << "Weight (kg): ";
cin >> weight;
cout << "Value (utility): ";
cin >> value;
cout << "Priority (lower number = higher priority): ";
cin >> priority;
cout << "Is divisible? (y/n): ";
cin >> divisibleChar;
divisible = (divisibleChar == 'y' || divisibleChar == 'Y');

items.push_back(Item(name, weight, value, divisible, priority));
}
double capacity;
cout << "Enter maximum weight capacity of the rescue boat (in kg): ";
cin >> capacity;

double totalWeightCarried;
double maxValue = fractionalKnapsack(items, capacity, totalWeightCarried);

cout << "\n****-----***** Flood Relief Summary ****-----****\n";
cout << "Total weight carried: " << fixed << setprecision(2) << totalWeightCarried <<
" kg\n";
cout << "Total utility value carried: " << fixed << setprecision(2) << maxValue << "
units\n";

10

return 0;
}
