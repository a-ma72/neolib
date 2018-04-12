#include <iostream>
#include <fstream>
#include <neolib/json.hpp>

template class neolib::basic_quick_string<char>;

int main(int argc, char** argv)
{
	try
	{
		std::string input;
		if (argc < 2)
		{
			std::cout << "Input: ";
			std::cin >> input;
		}
		else
			input = argv[1];

		neolib::json json{ input };

		std::string output;
		if (argc < 3)
		{
			std::cout << "Output: ";
			std::cin >> output;
		}
		else
			output = argv[2];

		json.write(output);
	}
	catch (std::exception& e)
	{
		std::cerr << "\nError: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}
}

