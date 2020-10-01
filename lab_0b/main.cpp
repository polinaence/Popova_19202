#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>

bool isAlphaNum(char c) {

	return (c >= '0' && c <= '9' || c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z');
}

int main(int argc, char** argv)
{
	if (argc != 3) {

		std::cerr << "Use 2 command-line args\n";
	}
	else {

		// 3 found (first is name of program)
		std::ifstream inFile(argv[1]);

		if (inFile.good()) {

			std::map<std::string, int> vault;
			int totalWords = 0;

			while (!inFile.eof()) {

				std::string s = "";
				std::getline(inFile, s);


				std::string word = "";
				for (size_t i = 0; i < s.size(); ++i) {
					if (isAlphaNum(s[i])) {
						word += s[i];
					}
					else {

						if (word.size() > 0) {


							// auto it = vault.find(word)
							std::map<std::string, int>::iterator it = vault.find(word);
							if (it == vault.end()) {
								vault[word] = 1;
							}
							else {
								++(it->second);
							}

							word = "";
							totalWords++;
						}
					}
				}
			}
			inFile.close();

			// sort by value
			std::vector<std::pair<std::string, int>> dataVector;
			dataVector.reserve(vault.size());
			for (std::map<std::string, int>::iterator it = vault.begin(); it != vault.end(); ++it) {
				dataVector.push_back(*it);
			}

			std::sort(dataVector.begin(), dataVector.end(), [](std::pair<std::string, int>& a, std::pair<std::string, int>& b) {return (a.second > b.second); });

			std::string csvFileName(argv[2]);
			if (csvFileName.size() >= 5 && csvFileName.substr(csvFileName.size() - 4, 4) == ".csv") {
				// create csv file
				std::ofstream outFile(argv[2]);
				//for(auto it = vault.begin(); it!=vault.end(); ++it)
				for (std::vector<std::pair<std::string, int>>::iterator it = dataVector.begin(); it != dataVector.end(); ++it) {
					outFile << it->first << " ; " << it->second << " ; " << 100.0 * it->second / totalWords << '\n';
				}
				outFile.close();
			}
			else {
				std::cerr << "Not a correct name for csv-file\n";
			}

		}
		else {
			std::cerr << "File " << argv[1] << " not found!\n";
		}
	}

	return 0;
}
