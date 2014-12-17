#include <stdio.h>
#include <netinet/in.h>

union Test
{
	short value;
	char union_bytes[ sizeof(short) ];
} ;
void byteOrder(Test test) {
	
	if ((test.union_bytes[0] == 1) && (test.union_bytes[1] == 2)) {
		printf("%x :big endian\n", test.value);
	}
	else if((test.union_bytes[0] == 2) && (test.union_bytes[1] == 1)) {
		printf("%x :little endian\n", test.value);
	}
	else {
		printf("unknown\n");
	}
	
	return;

}

int main() {
	Test test1,test2;
	test1.value = 0x0102;
	byteOrder(test1);
	test2.value = htons(test1.value); /* host order to net order*/
	byteOrder(test2);
	return 0;
}