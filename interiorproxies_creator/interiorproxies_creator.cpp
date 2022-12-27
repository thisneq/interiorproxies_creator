#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

using namespace std;
namespace fs = std::filesystem;

static vector<string> GetItemNames(string inputPath) 
{
	vector<string> ItemNames = {};
	for (const auto& entry : fs::recursive_directory_iterator(inputPath)) 
	{
		if (entry.path().extension() != ".ymap") continue;

		string FileName = entry.path().filename().stem().generic_string();
		ItemNames.push_back(FileName);
	}

	return ItemNames;
} 

static void WriteItemsToFile(vector<string> &ItemNames)
{
	ifstream infile("./data/interiorproxies.meta");
	ofstream outfile("./output/interiorproxies.meta");
	
	string line;
	while (getline(infile, line)) 
	{
		outfile << line << "\n";

		if (line.find("<proxies>") != string::npos) 
		{
			for (int i=0; i < ItemNames.size(); i++) 
			{
				outfile << (i > 0 ? "\n" : "") << "\t\t<Item>" << ItemNames[i] << "</Item>";
				cout << "Writing '" << ItemNames[i] << "' to file." << (i != ItemNames.size() ? "\n" : "");
			}
		}
	}
}

int main()
{
	vector<string> ItemNames = GetItemNames("./input/");
	WriteItemsToFile(ItemNames);

	cout << "\nInterior proxies has been successfuly created. Total items: " << ItemNames.size() << "\n";
	system("pause");

	return 0;
}