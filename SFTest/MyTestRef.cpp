

#include "include/MyTestRef.h"



int main(int argc, char** argv)
{
	MyTestRef * pMyTestRef = new MyTestRef ();
	sp<MyTestRef> spRefTest = pMyTestRef;
	return 0;
}


