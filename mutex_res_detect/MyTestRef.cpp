

#include <scheduler/MyTestRef.h>

std::atomic_bool MyTestRef::s_blocked(false);

/*

int main(int argc, char** argv)
{
	MyTestRef * pMyTestRef = new MyTestRef ();
	sp<MyTestRef> spRefTest = pMyTestRef;
	return 0;
}

*/
