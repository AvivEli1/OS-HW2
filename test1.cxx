#include "hw2_test.h"
#include <stdio.h>
#include <iostream>
#include <cassert>

using namespace std;

int main() {
	long x = get_ban('g');
	cout << "getpid() should not be banned initially (0) -> " << x << endl;
	assert(x == 0);

	x = set_ban(1,1,0,0);
	cout << "set_ban(1100) returned: " << x;
	if(x == -1) cout << " (did you use sudo?)";
	cout << endl;

	assert(x == 0);
	x = get_ban('g');
	cout << "getpid() should be banned (1) -> " << x << endl;
	assert(x == 1);

	x = flip_ban_branch(1, 'k');
	cout << "set_flip_branch returned: " << x << endl;
	assert(x >= 0);

	x = check_ban(pid_t(getppid()), 'k');
	cout << "Our parent should be banned from using kill() (1) -> " << x << endl;
	assert(x == 1);
	
	x = getpid();
	cout << "getpid() returned: " << x << endl;
	assert(x == -1);

	cout << "===== SUCCESS =====" << endl;
	return 0;
}

