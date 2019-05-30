///
///
/// this is cgi algorithm test program.
///
///

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
	char aa[100] = "total.cgi?from=1&to=10";
	int n;
	int m;
	int i;
	int sum = 0;

	strtok(aa,"=");
	n = atoi(strtok(NULL,"&"));
	strtok(NULL,"=");
	m = atoi(strtok(NULL," "));

	for(i = n; i <= m ; i++)
		sum += i;
	printf("%d\n",sum);

	return 0;
}
