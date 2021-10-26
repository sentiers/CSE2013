// CSE2013-01 System Programming and Practice
// 
// Name: Hyunji Cho
// StudentID: 2019113581
// Email: sentiers@naver.com
// Date: 2021 October 26
//
// stringtohex.c

#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>

int main() {
	char str[30];
	int value;
	printf("input: ");
	scanf("%s", str);
	printf("hex  : ");
	for (int i = 0; i < strlen(str); i++) {
		// for exception
		if ((str[i] >= 48 && str[i] <= 57) || (str[i] >= 65 && str[i] <= 70)) {
			// convert and print
			value = str[i] - 48;
			if (value >= 10) // if the value is larger than 10, subtract 7
				value -= 7;
			printf("%X", value);
		}
	}
	return 0;
}