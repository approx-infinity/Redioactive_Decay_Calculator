#include<sstream>
#include<vector>
#include<string>
#include<fstream>
#include<iostream>
#include<iomanip>
#include<cmath>
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;
using namespace std;

struct Record {
    int z;
    int n;
    double time;
};

int main(int argc, char* argv[]){
    ifstream file("half_life.csv");
    if(!file.is_open()){
        cerr << "Error: Could not open half_life.csv.\n"
             << "Hint: Run from the build folder or ensure the CSV is next to the executable." << endl;
        return 2;
    }
    string line;
    vector<Record> data;

    getline(file, line); // Skip header

    while(getline(file, line)){
        if(line.empty()) continue;
        stringstream ss(line);
        Record r{};
        string temp;

        try{
            if(!getline(ss, temp, ',')) continue; r.z = stoi(temp);
            if(!getline(ss, temp, ',')) continue; r.n = stoi(temp);
            if(!getline(ss, temp, ',')) continue; r.time = stod(temp);
        }catch(const std::exception& e){
            // Skip malformed rows
            continue;
        }
        data.push_back(r);
    }

    // Taking inputs from user
    // Number of protons (Z) and neutrons (N)
    int Z, N;
    cout << "Enter number of protons (Z): ";
    cin >> Z;
    cout << "Enter number of neutrons (N): ";
    cin >> N;

    // Find the half-life for the given Z and N
    double half_life = -1;
    for(const auto& record : data){
        if(record.z == Z && record.n == N){
            half_life = record.time;
            break;
        }
    }

    if(half_life <= 0){
        cout << "Nuclide is stable or missing for Z=" << Z << " and N=" << N << endl;
        return 0;
    }

    cout << fixed << setprecision(6);
    cout << "\nHalf-life: " << half_life << " seconds, "<< half_life/(60*60*24.0) << " days, " << half_life/(60*60*24.0*365.0) << " years" << endl;

    // Initial number of nuclei
    int initial_nuclei;
    cout << "\nEnter initial number of nuclei: ";
    cin >> initial_nuclei;

    // Take time duration
    double time_duration;
    cout << "Enter time duration (in seconds): ";
    cin >> time_duration;

    // calculate decay constant
    double decay_constant = 0.693 / half_life;

    // Calculate time steps and time array
    int num = 10000;  // number of steps for simulation
    double time_step = time_duration / num;
    vector<double> time_array(num + 1);
    for(int i = 0; i <= num; ++i){
        time_array[i] = i * time_step;
    }

    // calculate remaining nuclei after the given time duration
    vector<double> remaining_nuclei(num + 1);
    remaining_nuclei[0] = initial_nuclei;

    for(int i = 1; i <= num; ++i){
        // Apply decay over the incremental time step to avoid compounding error
        remaining_nuclei[i] = remaining_nuclei[i - 1] * exp(-decay_constant * time_step);
    }

    cout << "\nRemaining nuclei after " << time_duration << " seconds: " << remaining_nuclei[num] << endl;

    // Plotting the results (headless-safe)
    //plt::backend("Agg");
    plt::figure();
    plt::plot(time_array, remaining_nuclei);
    plt::xlabel("T (seconds)");
    plt::ylabel("N");
    plt::title("Radioactive Decay");
    plt::grid(true);

    // Ask user if they want to save the plot
    string is_save;
    cout << "Do you want to save the plot? (y/n): ";
    cin >> is_save;
    if(is_save == "y" || is_save == "Y") {
        plt::save("decay.png");
        cout << "Saved plot to decay.png" << endl;
    }
    else if(is_save == "n" || is_save == "N") {
        plt::show();
    }

    return 0;
}