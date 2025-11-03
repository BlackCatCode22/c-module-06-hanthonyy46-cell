#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <ctime>
using namespace std;

// ---------- Animal Class ----------
class Animal {
public:
    string name, gender, color, origin, species, birthDate, uniqueID;
    int age, weight;

    string genBirthDay(int age, string season) {
        time_t now = time(0);
        tm *ltm = localtime(&now);
        int birthYear = (1900 + ltm->tm_year) - age;

        string monthDay = "06-15";
        if (season == "spring") monthDay = "03-15";
        else if (season == "summer") monthDay = "06-15";
        else if (season == "fall") monthDay = "09-15";
        else if (season == "winter") monthDay = "12-15";

        return to_string(birthYear) + "-" + monthDay;
    }

    string genUniqueID(string species, int number) {
        string prefix;
        if (species == "hyena") prefix = "Hy";
        else if (species == "lion") prefix = "Li";
        else if (species == "tiger") prefix = "Ti";
        else if (species == "bear") prefix = "Be";
        if (number < 10) return prefix + "0" + to_string(number);
        return prefix + to_string(number);
    }
};

// ---------- Get Current Date ----------
string getCurrentDate() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    char buffer[11];
    strftime(buffer, 11, "%Y-%m-%d", ltm);
    return string(buffer);
}

// ---------- Main ----------
int main() {
    ifstream animalsFile("arrivingAnimals.txt");
    ifstream namesFile("animalNames.txt");
    ofstream outFile("zooPopulation.txt");

    vector<string> animalLines;
    vector<string> names;

    // Load animal data
    if (!animalsFile.is_open()) {
        cout << "arrivingAnimals.txt \n";
        animalLines = {
            "4 year old female hyena, born in spring, tan color, 70 pounds, from Friguia Park, Tunisia",
            "3 year old male lion, born in winter, golden color, 190 pounds, from Serengeti, Tanzania",
            "5 year old female tiger, born in fall, orange color, 220 pounds, from Ranthambore, India",
            "2 year old male bear, born in summer, brown color, 300 pounds, from Yellowstone, USA"
        };
    } else {
        string line;
        while (getline(animalsFile, line))
            if (!line.empty()) animalLines.push_back(line);
    }

    // Create animalNames.txt if it doesn't exist
    if (!namesFile.is_open()) {
        cout << "animalNames.txt .\n";
        ofstream createFile("animalNames.txt");
        names = {"George", "Michael", "Sharon", "Jay"};
        for (string n : names) createFile << n << "\n";
        createFile.close();
    } else {
        string line;
        while (getline(namesFile, line))
            if (!line.empty()) names.push_back(line);
    }

    if (!outFile.is_open()) {
        cerr << " Error: cannot open zooPopulation.txt for writing.\n";
        return 1;
    }

    vector<Animal> animals;
    map<string, int> speciesCount;
    int nameIndex = 0;

    // Read and process each animal line
    for (string line : animalLines) {
        Animal a;
        string temp, season;

        stringstream ss(line);
        ss >> a.age >> temp >> temp >> a.gender >> a.species;
        getline(ss, temp, ',');

        size_t pos = line.find("born in ");
        if (pos != string::npos) {
            size_t start = pos + 8;
            size_t end = line.find(',', start);
            season = line.substr(start, end - start);
        }

        pos = line.find("color");
        if (pos != string::npos) {
            size_t start = line.rfind(' ', pos - 2);
            a.color = line.substr(start + 1, pos - start - 2);
        }

        pos = line.find("pounds");
        if (pos != string::npos) {
            size_t start = line.rfind(' ', pos - 2);
            a.weight = stoi(line.substr(start + 1, pos - start - 1));
        }

        pos = line.find("from ");
        if (pos != string::npos)
            a.origin = line.substr(pos + 5);

        // Assign name
        if (nameIndex < names.size()) a.name = names[nameIndex++];
        else a.name = "Unnamed";

        speciesCount[a.species]++;
        a.uniqueID = a.genUniqueID(a.species, speciesCount[a.species]);
        a.birthDate = a.genBirthDay(a.age, season);

        animals.push_back(a);
    }

    // Group by species
    map<string, vector<Animal>> habitats;
    for (auto &a : animals) habitats[a.species].push_back(a);

    string today = getCurrentDate();

    // Output report
    outFile << "Zoo Population Report\n\n";
    for (auto &h : habitats) {
        string species = h.first;
        string header = species + " Habitat:";
        header[0] = toupper(header[0]);
        outFile << header << "\n";

        for (auto &a : h.second) {
            outFile << a.uniqueID << "; " << a.name
                    << "; birth date " << a.birthDate
                    << "; " << a.color << " color; " << a.gender
                    << "; " << a.weight << " pounds; from " << a.origin
                    << "; arrived " << today << "\n\n";
        }
    }

    cout << "Zoo population report created: zooPopulation.txt\n";
    return 0;
}
