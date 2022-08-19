
#include "print.hpp"

void	print(char *const *envs, size_t size)
{
	std::cout << "The environment variables are:" << std::endl;
	for (size_t i = 0; i < size; i++) {
		std::cout << envs[i] << std::endl;
	}
}
