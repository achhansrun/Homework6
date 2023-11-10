#include "da6.hpp"
int main()
{
	const int SIZE = 1000;  // Size for loops
	SlowMap<int, int> m;
	for (int i = 0; i < SIZE * 2; ++i)
	{
		m.set(i, 2 * i);
	}
}