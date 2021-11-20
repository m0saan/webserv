
#include <iostream>

int main()

{
	char *ptr = (char *)malloc(sizeof(char ) * 10);
	std::cout << (sizeof(ptr)/sizeof(ptr[0])) << std::endl;
}