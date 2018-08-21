#include <stdio.h>
#include <string.h>

void EQN_TO_C (char * in)
{

	puts("#include <stdio.h>\n\n");

	puts("int main()\n");

	puts("{\n\n");

	puts("int temp = 0;\n");
	
	static char strarr[1000][1000];

	int i = 0;

	char * str_p = strarr[i];

	while (*in != '\0')
	{
		while (*in != '\n')
		{ *str_p++ = *in++; }

		*str_p = '\0';	

		in++; //skip '\n'

		if ( strchr(strarr[i],'=') >= strarr[i] )
		{
			strarr[i][0] = ' ';

			strarr[i][strlen(strarr[i])-1] = ';';

			printf("int%s\n\n",strarr[i]);
		}

		else // no '=' in strarr[i]
		{
			printf("temp = %s;\n\n",strarr[i]);

			puts("printf(\"%d\\n\",temp);");
		}

		i++;

		str_p = &strarr[i][0];

	}

	puts("\n\n}");

}

int main()
{
	static char in[1000];

	static char * in_p = &in[0];

	char c;

	while ( (c = getchar()) != EOF)
	{
		*in_p++ = c;	
	}
	
	EQN_TO_C(in);
	
}
