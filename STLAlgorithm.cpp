#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

namespace stl_algorithm
{
	bool judge(const pair<T1, T2> a, const pair<T1, T2> b)
	{
		return a.second > b.second;
	}

	void Sort(vector<pair<T1, T2> >& vec)
	{
		std::sort(vec.begin(), vec.end(), judge);
	}

	void Insert(vector<T1>& v1, const vector<T1>& v2)
	{
		v1.insert(v1.end(), v2.begin(), v2.end());
	}

	void Random(vector<T>& vec)
	{
		random_shuffle(vec.begin(), vec.end());
	}

	void Release(vector<T>& vec)
	{
		vec.clear();
		vector<T> blank;
		vec.swap(blank);
	}

	bool parity_equal(T x, T y)  
	{  
	    return x==y; 
	}  

	vector<T>::iterator AdjacentFind(const vector<T>& vec)
	{
		vector<T>::iterator it = adjacent_find(vec.begin(), vec.end(), parity_equal);
		return it;
	}

	void Replace(vector<T> &vec, const T& old_value, const T& new_value)
	{
		replace(vec.begin(), vec.end(), old_value, new_value);
	}

	
}
