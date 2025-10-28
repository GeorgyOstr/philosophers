#include <unistd.h>

int main(void)
{
	pid_t pid;

	pid = fork();
	if (!pid)
	{
		write(1, "hello\n", 6);
	}
	else
	{
		write(1, "bye!!\n", 6);
	}
	return 0;
}
