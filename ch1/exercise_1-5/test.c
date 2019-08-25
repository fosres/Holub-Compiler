#include <stdio.h>
int main(void)
{

	void (*func);

	void func2(int,int);

//	void func3(int,void);

	void func4(int func());

	void func5(int),func6(double);

	void func7(void (*func_2));
}
