
#include "print.hpp"

void	print_env_variables(char *const *envs, size_t size)
{
	std::cout << "The environment variables are:" << std::endl;
	for (size_t i = 0; i < size; i++) {
		std::cout << envs[i] << std::endl;
	}
}

void	print_address(const std::string &name, struct sockaddr *address)
{
	std::vector<char> address_str(30);
	getnameinfo((struct sockaddr *)address, sizeof(struct sockaddr), address_str.data(), 30, NULL, 0, NI_NUMERICHOST);
	std::cout << name << " address is :" << address_str.data() << std::endl;
}
