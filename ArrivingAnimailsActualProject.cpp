#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <regex>
#include <memory>
using namespace std;

// Animal Class
class Animal {
protected:
    // Common properties for all animals
    string name;
    int age;
    string gender;
    string species;
    string birthSeason;
    string color;
    string origin;

public:
    // Build an animal using provided data
    Animal(int a, string g, string s, string b, string c, string o)
        : age(a), gender(g), species(s), birthSeason(b), color(c), origin(o) {}
    // Virtual destructor for safe cleanup
    virtual ~Animal() {}
    // Returns the animal's species (like "Lion")
    string getSpecies() const { return species; }
    // Prints the animal's information to an output file
    virtual void displayInfo(ofstream& out) const {
        out << species << " (" << gender << ", " << age << " yrs, born in "
            << birthSeason << ", " << color << ", from " << origin << ")\n";
    }
};
// Class: Each of these inherits from Animal and just sets its own species name
class Hyena : public Animal {
public:
    Hyena(int a, string g, string b, string c, string o)
        : Animal(a, g, "Hyena", b, c, o) {}
};

class Lion : public Animal {
public:
    Lion(int a, string g, string b, string c, string o)
        : Animal(a, g, "Lion", b, c, o) {}
};

class Tiger : public Animal {
public:
    Tiger(int a, string g, string b, string c, string o)
        : Animal(a, g, "Tiger", b, c, o) {}
};

class Bear : public Animal {
public:
    Bear(int a, string g, string b, string c, string o)
        : Animal(a, g, "Bear", b, c, o) {}
};
// This function reads one line from the file and extracts info using regex
shared_ptr<Animal> parseLine(const string& line) {
    // Regex pattern to match text like:
    // "4 year old female hyena, born in spring, tan color, 70 pounds, from Friguia Park, Tunisia"
    regex pattern(R"((\d+)\s+year\s+old\s+(\w+)\s+(\w+),\s+born\s+in\s+(\w+),\s+(.+?)\s+color,\s+[\d\s\w]+,\s+from\s+(.+))");
    // to store the found parts of the line
    smatch match;

    // Try to match the pattern to the line
    if (regex_search(line, match, pattern)) {
        int age = stoi(match[1]);
        string gender = match[2];
        string species = match[3];
        string birthSeason = match[4];
        string color = match[5];
        string origin = match[6];

        // Normalize capitalization
        for (auto& ch : species) ch = tolower(ch);

        // Create subclass
        if (species == "hyena") return make_shared<Hyena>(age, gender, birthSeason, color, origin);
        if (species == "lion")  return make_shared<Lion>(age, gender, birthSeason, color, origin);
        if (species == "tiger") return make_shared<Tiger>(age, gender, birthSeason, color, origin);
        if (species == "bear")  return make_shared<Bear>(age, gender, birthSeason, color, origin);
    }
    return nullptr;
}
int main() {
    // Open the input file (animals arriving)
    ifstream inFile("arrivingAnimals.txt");
    // Open the output file (new report)
    ofstream outFile("newAnimals.txt");

    // Check if the input file opened correctly
    if (!inFile.is_open()) {
        cerr << "Error: could not open arrivingAnimals.txt\n";
        return 1;
    }
    // Vector to hold all animals in the zoo
    vector<shared_ptr<Animal>> zoo;
    // Map to count how many animals per species
    map<string, int> speciesCount;

    string line;
    // Read the input file line by line
    while (getline(inFile, line)) {
        if (auto animal = parseLine(line)) {
            zoo.push_back(animal);
            speciesCount[animal->getSpecies()]++;
        }
    }
    // Write the report header
    outFile << "Zoo Arrival Report \n\n";
    // Loop through each species in the map
    for (auto& pair : speciesCount) {
        outFile << pair.first << "s (" << pair.second << " total):\n";
        // Go through all animals and print the ones that match this species
        for (auto& a : zoo) {
            if (a->getSpecies() == pair.first) {
                a->displayInfo(outFile);
            }
        }
        // blank line between species groups
        outFile << "\n";
    }
    // Tell the program to finish
    cout << "Report generated successfully: newAnimals.txt\n";
    return 0;
}
