#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <random>

using namespace std;
namespace fs = std::filesystem;

int GetRandomNumber(int min, int max)
{
	random_device r;
	default_random_engine e1(r());
	uniform_int_distribution<int> uniform_dist(min, max);
	return uniform_dist(e1);
}

static vector<string> GetItemNames(string inputPath) 
{
	vector<string> ItemNames = {};
	for (const auto& entry : fs::recursive_directory_iterator(inputPath)) 
	{
		if (entry.path().extension() != ".ymap") continue;
		string path = entry.path().string();
		string resourceName = "Unknown";
		while (path.find("\\") != string::npos) 
		{
			if (path.substr(path.find_last_of("\\") + 1) == "stream") 
			{
				path = path.substr(0, path.find_last_of("\\"));
				resourceName = path.substr(path.find_last_of("\\") + 1);
				break;
			} else {
				path = path.substr(0, path.find_last_of("\\"));
			}
		}
		string FileName = entry.path().filename().stem().generic_string();
		ItemNames.push_back(resourceName + ":" + FileName);
	}

	return ItemNames;
} 

static void WriteItemsToFile(vector<string> &ItemNames)
{
	ifstream infile("./data/interiorproxies.meta");
	ofstream outfile("./output/interiorproxies.meta");
	
	string line;
	string currentResource = "";
	while (getline(infile, line)) 
	{
		if (line.find("<startFrom value=") != string::npos) 
		{
			int random = GetRandomNumber(2000, 3000);
			cout << "Changing form number value to " << random << endl;
			line = "\t<startFrom value=\"" + to_string(random) + "\" />";
		}
		outfile << line << "\n";

		if (line.find("<proxies>") != string::npos) 
		{
			for (int i=0; i < ItemNames.size(); i++) 
			{
				string resourceName = ItemNames[i].substr(0, ItemNames[i].find(":"));
				string itemName = ItemNames[i].substr(ItemNames[i].find(":") + 1);
				if (resourceName != currentResource) 
				{
					currentResource = resourceName;
					outfile << (i > 0 ? "\n" : "") << "\t\t<!-- " << resourceName << " -->";
				}
				outfile << "\n" << "\t\t<Item>" << itemName << "</Item>";
				cout << "Writing '" << ItemNames[i] << "' to file." << (i != ItemNames.size() ? "\n" : "");
			}
		}
	}
}

int main()
{
	string inputPath = "./input/";
	if (int argc = __argc; argc > 1)
	{
		if (!fs::exists(__argv[1])) 
		{
			cout << "Path '" << __argv[1] << "' does not exist. Try with another path." << endl;
			return 1;
		} else {
			inputPath = __argv[1];
		}
	}
	cout << "Selected path: " << inputPath << endl;

	vector<string> ItemNames = GetItemNames(inputPath);
	WriteItemsToFile(ItemNames);

	cout << "\nInterior proxies has been successfully created. Total items: " << ItemNames.size() << endl;
	system("pause");

	return 0;
}