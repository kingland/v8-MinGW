#include <stdio.h>

int main(){
	#if defined(__GNUC__)
	printf("GNU");
	#endif
	return 0;
}