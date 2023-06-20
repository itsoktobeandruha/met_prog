#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

vector<int> sort_mass(vector<int>& mass)
{
	int temp;
	int negative = 0;

	for (int i = 0; i < mass.size(); i++)
	{
		if (mass[i] < 0)
		{
			negative++;
		}
		if (mass[i] >= 0)
		{
			for (int j = 0; j < i; j++)
			{
				if (mass[j] <= 0) 
				{ 
					temp = mass[i]; 
					mass[i] = mass[j]; 
					mass[j] = temp; 
				}
			}
		}
	}
	sort(mass.begin(), mass.end() - negative);
	return mass;
}

int main()
{
	vector<int> mass;
	int size;
	int element;
	cin >> size;
	for (int i = 0; i < size; i++)
	{
		cin >> element;
		mass.push_back(element);
	}
	sort_mass(mass);
	for (int i = 0; i < size; i++)
	{
		cout << mass[i] << ' ';
	}
	return 0;
}