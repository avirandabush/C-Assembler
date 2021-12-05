/*assembler project*/
/*maman 14*/
/*the main program*/

#include "info.h"

/*start*/
int main(int argc, char *argv[])
{
	int i;
	
	if(argc == 1)			/*check if there are no arguments*/
	{
		fprintf(stderr, "you didnt send any files\n");
		exit(0);
	}

	for(i = 1;i < argc;i++)		/*send each file*/
	{
		assembler(argv[i]);	/*call the parsing function*/
	}
	return 0;			/*end*/
}

