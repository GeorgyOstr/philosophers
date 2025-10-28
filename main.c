#include <unistd.h>
#include <pthread.h>
#include <stdio.h>

void	*routine(void *arg)
{
	write(1, "hi\n", 3);
	usleep(1000000);
}

int main(void)
{
	pthread_t		t1;
	int				s;
	pthread_attr_t	attr;
	void	*res;

	s = pthread_attr_init(&attr);
	s = pthread_create(&t1, &attr, routine, &s);
	s = pthread_attr_destroy(&attr);
	s = pthread_join(t1, &res);
	printf("%d\n", s);
	return (0);
}
