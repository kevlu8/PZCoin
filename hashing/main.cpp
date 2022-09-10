#include "ec.hpp"
#include "ecoh.hpp"

int main() {
	std::string message;
	std::cout << "Enter message to hash";
	std::cin >> message;
	std::cout << "Hash of message: " << hash(message) << std::endl;
}
