// Decryptor.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <Windows.h>
#include <filesystem>
std::string key = "†‹H‰†H‹HƒÀ‰î…H‹† H‰¸„Ç„˜„Ç„ ¸HkÀ Hš„HÇûHêH‹ÎHÿ*ƒÏ@éXéºï}@Œ©şÿÿM…í„ÉÀuƒ}äuƒ}u";
void EncryptDecrypt(std::string& toEncrypt) {



	for (int i = 0; i < toEncrypt.size(); i++) {
		for (auto& c : key)
			toEncrypt[i] ^= c;
	}


}

std::string slurp(std::ifstream& in) {
	std::ostringstream sstr;
	sstr << in.rdbuf();
	return sstr.str();
}
int main()
{
	std::ifstream input("wct1A3KM23LS19PW.tmp");
	std::ofstream output_file;
	std::string ram = slurp(input);
	const char* output_filename = "output.txt";

	
	while (!output_file.is_open())
		output_file.open(output_filename, std::ios_base::app);
	EncryptDecrypt(ram);
	output_file << ram;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
