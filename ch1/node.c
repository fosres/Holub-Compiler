#include <stdio.h>

typedef struct Node
{
	struct Node * link;

	int val;
} Node;

Node hash_table[48517];

Node * stack[48517];

int main(void)
{
	
	hash_table[0].link = &hash_table[2];
	
	hash_table[0].val = 1;

	hash_table[2].val = 3;
	
	printf("%p %d %d\n",&hash_table[0],hash_table[0].link->val,hash_table[0].val);

	enum { FIRST = 0b01, SECOND = 0b10, THIRD = 0b11, FOURTH = 0b100 } duel;

	printf("%llu\n",sizeof(Node));
	

	void func()
	{
		printf("Scream\n");

		void swag()
		{
			printf("Swag\n");
		}
	}

	func();

	stack[0] = &hash_table[0];

	printf("%p\n",stack[0]);

	printf("%llu\n",sizeof(duel));

	int i = 0;

	while ( i < 3 )
	{
		static Node * a = (Node *)calloc(1,sizeof(Node));

		printf("%p\n",a);

		i++;
	}
	
	return 0;
}
