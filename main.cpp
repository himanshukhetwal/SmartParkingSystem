#include <iostream>
#include <vector>
#include <fstream>
#include <ctime>

using namespace std;

/* =======================
   Vehicle Class
   ======================= */
class Vehicle {
public:
    string number;
    string type;
    size_t slot;
    time_t entryTime;

    Vehicle(string n, string t, size_t s, time_t e)
        : number(n), type(t), slot(s), entryTime(e) {}
};

/* =======================
   Parking Lot Class
   ======================= */
class ParkingLot {
private:
    vector<Vehicle> vehicles;
    size_t maxSlots;

public:
    ParkingLot(size_t slots) : maxSlots(slots) {
        loadFromFile();
    }

    ~ParkingLot() {
        saveToFile();
    }

    void parkVehicle() {
        if (vehicles.size() >= maxSlots) {
            cout << "Parking Full!\n";
            return;
        }

        string number, type;
        cout << "Enter vehicle number: ";
        cin >> number;
        cout << "Enter vehicle type (Car/Bike): ";
        cin >> type;

        size_t slot = getFreeSlot();
        vehicles.emplace_back(number, type, slot, time(nullptr));

        cout << "Vehicle parked successfully at slot " << slot << "\n";
    }

    void removeVehicle() {
        string number;
        cout << "Enter vehicle number to exit: ";
        cin >> number;

        for (size_t i = 0; i < vehicles.size(); i++) {
            if (vehicles[i].number == number) {
                time_t exitTime = time(nullptr);
                double hours =
                    difftime(exitTime, vehicles[i].entryTime) / 3600.0;
                if (hours < 1) hours = 1;

                int rate = (vehicles[i].type == "Car") ? 40 : 20;

                cout << "Slot Freed: " << vehicles[i].slot << "\n";
                cout << "Parking Fee: ₹" << hours * rate << "\n";

                vehicles.erase(vehicles.begin() + i);
                return;
            }
        }

        cout << "Vehicle not found!\n";
    }

    void displayStatus() const {
        cout << "\n--- Parking Status ---\n";

        if (vehicles.empty()) {
            cout << "No vehicles parked.\n";
        } else {
            for (const auto &v : vehicles) {
                cout << "Slot: " << v.slot
                     << " | Number: " << v.number
                     << " | Type: " << v.type << "\n";
            }
        }

        cout << "Available Slots: "
             << maxSlots - vehicles.size() << "\n";
    }

private:
    size_t getFreeSlot() const {
        vector<bool> used(maxSlots + 1, false);

        for (const auto &v : vehicles) {
            used[v.slot] = true;
        }

        for (size_t i = 1; i <= maxSlots; i++) {
            if (!used[i]) return i;
        }

        return 0; // should never happen
    }

    void saveToFile() const {
        ofstream file("parking_data.txt");

        for (const auto &v : vehicles) {
            file << v.number << " "
                 << v.type << " "
                 << v.slot << " "
                 << v.entryTime << "\n";
        }
    }

    void loadFromFile() {
        ifstream file("parking_data.txt");
        if (!file) return;

        string number, type;
        size_t slot;
        time_t entry;

        while (file >> number >> type >> slot >> entry) {
            vehicles.emplace_back(number, type, slot, entry);
        }
    }
};

/* =======================
   MAIN FUNCTION
   ======================= */
int main() {
    ParkingLot lot(10);
    int choice;

    while (true) {
        cout << "\n--- Smart Parking System ---\n";
        cout << "1. Park Vehicle\n";
        cout << "2. Remove Vehicle\n";
        cout << "3. Display Status\n";
        cout << "4. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                lot.parkVehicle();
                break;
            case 2:
                lot.removeVehicle();
                break;
            case 3:
                lot.displayStatus();
                break;
            case 4:
                cout << "Exiting system...\n";
                return 0;
            default:
                cout << "Invalid choice!\n";
        }
    }
}
