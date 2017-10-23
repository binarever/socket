#include <stdlib.h>
#include <iostream>
#include <stdio.h>

main()
{
	FILE *fp;
	char path[1025];
	fp=popen("date","r");
	if(fp==NULL)
	{
		printf("ERROR");
	}
	while (fgets(path,sizeof(path)-1,fp)!=NULL)
	{
		printf("%s",path);
	}

	pclose(fp);
}
