#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <ctime>
using namespace std;

// ---------- Base Class ----------
class Animal {
public:
    string name, gender, color, origin, species, birthDate, uniqueID;
    int age, weight;

    string genBirthDay(int age, string season) {
        time_t now = time(0);
        tm *ltm = localtime(&now);
        int currentYear = 1900 + ltm->tm_year;
        int birthYear = currentYear - age;

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

// ---------- Helper ----------
string getCurrentDate() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    char buffer[11];
    strftime(buffer, 11, "%Y-%m-%d", ltm);
    return string(buffer);
}

// ---------- Main ----------
int main() {
    ifstream inFile("arrivingAnimals.txt");
    ifstream nameFile("animalNames.txt");
    ofstream outFile("zooPopulation.txt");

    vector<string> arrivingData;
    vector<string> names;

    // Fallback data if files missing
    if (!inFile.is_open()) {
        cout << " arrivingAnimals.txt\n";
        arrivingData = {
            "4 year old female hyena, born in spring, tan color, 70 pounds, from Friguia Park, Tunisia",
            "3 year old male lion, born in winter, golden color, 190 pounds, from Serengeti, Tanzania",
            "5 year old female tiger, born in fall, orange color, 220 pounds, from Ranthambore, India",
            "2 year old male bear, born in summer, brown color, 300 pounds, from Yellowstone, USA"
        };
    } else {
        string line;
        while (getline(inFile, line)) if (!line.empty()) arrivingData.push_back(line);
    }

    if (!nameFile.is_open()) {
        cout << " animalNames.txt \n";
        names = {"George", "Michael", "Sharon", "Jay"};
    } else {
        string line;
        while (getline(nameFile, line)) if (!line.empty()) names.push_back(line);
    }

    if (!outFile.is_open()) {
        cerr << "Error: could not open zooPopulation.txt for writing.\n";
        return 1;
    }

    vector<Animal> animals;
    map<string, int> speciesCount;
    int nameIndex = 0;

    for (string line : arrivingData) {
        Animal a;
        string temp, season;
        stringstream ss(line);
        ss >> a.age >> temp >> temp >> a.gender >> a.species; // "4 year old female hyena"
        ss >> temp >> temp >> season; // "born in spring"
        if (season.back() == ',') season.pop_back();
        ss >> a.color >> temp >> a.weight; // "tan color, 70"
        ss >> temp; // skip "pounds,"
        ss >> temp >> a.origin; // "from Friguia"
        string rest;
        getline(ss, rest);
        a.origin += rest;

        if (nameIndex < names.size()) a.name = names[nameIndex++];
        else a.name = "Unnamed";

        speciesCount[a.species]++;
        a.uniqueID = a.genUniqueID(a.species, speciesCount[a.species]);
        a.birthDate = a.genBirthDay(a.age, season);
        a.weight = a.weight;

        animals.push_back(a);
    }

    // Group by species
    map<string, vector<Animal>> habitats;
    for (auto &a : animals) habitats[a.species].push_back(a);

    string today = getCurrentDate();

    for (auto &h : habitats) {
        string species = h.first;
        string header;
        if (species == "hyena") header = "Hyena Habitat:";
        else if (species == "lion") header = "Lion Habitat:";
        else if (species == "tiger") header = "Tiger Habitat:";
        else if (species == "bear") header = "Bear Habitat:";
        else header = "Other Habitat:";

        outFile << header << "\n";
        for (auto &a : h.second) {
            outFile << a.uniqueID << "; " << a.name
                    << "; birth date " << a.birthDate
                    << "; " << a.color << " color; " << a.gender
                    << "; " << a.weight << " pounds; from " << a.origin
                    << "; arrived " << today << "\n\n";
        }
    }

    cout << " Zoo population report created: zooPopulation.txt\n";
    return 0;
}
