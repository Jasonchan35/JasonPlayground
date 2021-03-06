#include "Knight_brute_force.h"
#include "Knight_Link.h"
#include "Knight2.h"

int main(){
    DWORD startTime = GetTickCount();

	const int n = 6;

	//auto* knight = new Knight_brute_force(n);
	//auto* knight = new Knight_Link(n);
	auto* knight = new Knight2(n);

	knight->resolveOne();
//	knight->resolveAll();

	DWORD endTime = GetTickCount();
	printf("time: %f seconds.\n", (endTime - startTime)/1000.0);
	return 0;
}

