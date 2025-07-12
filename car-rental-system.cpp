#include <iostream>
#include <fstream>
#include <string>
#include <limits>
using namespace std;

class Vehicle {
public:
    int id;
    string brand;
    string model;
    float pricePerDay;
    bool isAvailable;

    Vehicle() : id(0), brand(""), model(""), pricePerDay(0), isAvailable(true) {}

    virtual void inputDetails(int vid) {
        id = vid;
        cout << "Enter brand: ";
        cin >> brand;
        cout << "Enter model: ";
        cin >> model;
        cout << "Enter price per day: ";
        cin >> pricePerDay;
        isAvailable = true;
    }

    virtual void displayDetails() {
        cout << "ID: " << id << " | Brand: " << brand
             << " | Model: " << model
             << " | Price/Day: Rs " << pricePerDay
             << " | Available: " << (isAvailable ? "Yes" : "No") << endl;
    }

    virtual void saveToFile(ofstream& out) {
        out << "Vehicle " << id << " " << brand << " " << model << " "
            << pricePerDay << " " << isAvailable << endl;
    }

    virtual void loadFromFile(ifstream& in) {
        in >> id >> brand >> model >> pricePerDay >> isAvailable;
    }

    virtual ~Vehicle() {}
};

class Car : public Vehicle {
public:
    int seats;

    Car() : Vehicle(), seats(4) {}

    void inputDetails(int vid) override {
        Vehicle::inputDetails(vid);
        cout << "Enter number of seats: ";
        cin >> seats;
    }

    void displayDetails() override {
        cout << "Car - ID: " << id << " | Brand: " << brand
             << " | Model: " << model
             << " | Seats: " << seats
             << " | Price/Day: Rs " << pricePerDay
             << " | Available: " << (isAvailable ? "Yes" : "No") << endl;
    }

    void saveToFile(ofstream& out) override {
        out << "Car " << id << " " << brand << " " << model << " "
            << pricePerDay << " " << isAvailable << " " << seats << endl;
    }

    void loadFromFile(ifstream& in) override {
        in >> id >> brand >> model >> pricePerDay >> isAvailable >> seats;
    }
};

class Bike : public Vehicle {
public:
    bool hasCarrier;

    Bike() : Vehicle(), hasCarrier(false) {}

    void inputDetails(int vid) override {
        Vehicle::inputDetails(vid);
        cout << "Does it have a carrier? (1 for yes, 0 for no): ";
        cin >> hasCarrier;
    }

    void displayDetails() override {
        cout << "Bike - ID: " << id << " | Brand: " << brand
             << " | Model: " << model
             << " | Carrier: " << (hasCarrier ? "Yes" : "No")
             << " | Price/Day: Rs " << pricePerDay
             << " | Available: " << (isAvailable ? "Yes" : "No") << endl;
    }

    void saveToFile(ofstream& out) override {
        out << "Bike " << id << " " << brand << " " << model << " "
            << pricePerDay << " " << isAvailable << " " << hasCarrier << endl;
    }

    void loadFromFile(ifstream& in) override {
        in >> id >> brand >> model >> pricePerDay >> isAvailable >> hasCarrier;
    }
};

class Customer {
public:
    string name;
    int rentedVehicleId;
    string rentDate;
    int rentDays;

    Customer() : name(""), rentedVehicleId(-1), rentDate(""), rentDays(0) {}

    void inputDetails() {
        cout << "Enter customer name: ";
        cin.ignore();
        getline(cin, name);
    }

    void display() {
        cout << "Name: " << name << " | Rented Vehicle ID: ";
        if (rentedVehicleId == -1)
            cout << "None";
        else
            cout << rentedVehicleId;
        cout << " | Rent Date: " << rentDate
             << " | Rent Days: " << rentDays << endl;
    }

    void saveToFile(ofstream& out) {
        out << name << "\n" << rentedVehicleId << " " << rentDate << " " << rentDays << "\n";
    }

    void loadFromFile(ifstream& in) {
        in.ignore();
        getline(in, name);
        in >> rentedVehicleId >> rentDate >> rentDays;
    }
};

class RentalShop {
private:
    Vehicle* vehicles[100];
    Customer customers[100];
    int vehicleCount = 0;
    int customerCount = 0;

    bool isAdminLoggedIn = false;
    int loggedInCustomerIndex = -1;

public:
    RentalShop() {
        for (int i = 0; i < 100; i++) vehicles[i] = nullptr;
    }

    ~RentalShop() {
        for (int i = 0; i < vehicleCount; i++) {
            delete vehicles[i];
        }
    }

    bool customerLogin() {
        string name;
        cout << "Enter Customer Name: ";
        cin.ignore();
        getline(cin, name);

        for (int i = 0; i < customerCount; i++) {
            if (customers[i].name == name) {
                loggedInCustomerIndex = i;
                cout << "Customer login successful!\n";
                return true;
            }
        }
        cout << "Customer not found! Please register first.\n";
        return false;
    }

    void addVehicle() {
        if (vehicleCount >= 100) {
            cout << "Vehicle capacity full!\n";
            return;
        }
        int vid = vehicleCount + 1;

        int vtype;
        cout << "Enter Vehicle Type (1-Car, 2-Bike): ";
        cin >> vtype;

        if (vtype == 1) {
            vehicles[vehicleCount] = new Car();
        } else if (vtype == 2) {
            vehicles[vehicleCount] = new Bike();
        } else {
            cout << "Invalid vehicle type!\n";
            return;
        }

        vehicles[vehicleCount]->inputDetails(vid);
        vehicleCount++;
    }

    void registerCustomer() {
        if (customerCount >= 100) {
            cout << "Customer capacity full!\n";
            return;
        }

        string newName;
        cout << "Enter new customer name: ";
        cin.ignore();
        getline(cin, newName);

        for (int i = 0; i < customerCount; i++) {
            if (customers[i].name == newName) {
                cout << "Customer already registered.\n";
                return;
            }
        }

        customers[customerCount].name = newName;
        customers[customerCount].rentedVehicleId = -1;
        customers[customerCount].rentDate = "";
        customers[customerCount].rentDays = 0;
        customerCount++;

        cout << "Customer registered successfully.\n";
    }

    void showVehicles() {
        cout << "\n--- Vehicle List ---\n";
        for (int i = 0; i < vehicleCount; i++) {
            vehicles[i]->displayDetails();
        }
    }

    void showCustomers() {
        cout << "\n--- Customer List ---\n";
        for (int i = 0; i < customerCount; i++) {
            customers[i].display();
        }
    }

    void rentVehicle(int custIndex) {
        if (custIndex == -1) {
            cout << "Please login as customer first.\n";
            return;
        }
        if (customers[custIndex].rentedVehicleId != -1) {
            cout << "You already have a rented vehicle.\n";
            return;
        }
        int vid, days;
        string date;
        cout << "Enter Vehicle ID to rent: ";
        cin >> vid;
        cout << "Enter rental date (dd-mm-yyyy): ";
        cin >> date;
        cout << "Enter number of days to rent: ";
        cin >> days;

        for (int i = 0; i < vehicleCount; i++) {
            if (vehicles[i]->id == vid && vehicles[i]->isAvailable) {
                vehicles[i]->isAvailable = false;
                customers[custIndex].rentedVehicleId = vid;
                customers[custIndex].rentDate = date;
                customers[custIndex].rentDays = days;
                float total = vehicles[i]->pricePerDay * days;
                cout << "Vehicle rented successfully! Total price: Rs " << total << endl;
                return;
            }
        }
        cout << "Vehicle not available.\n";
    }

    void returnVehicle(int custIndex) {
        if (custIndex == -1) {
            cout << "Please login as customer first.\n";
            return;
        }
        int vid = customers[custIndex].rentedVehicleId;
        if (vid == -1) {
            cout << "No active rental found.\n";
            return;
        }
        for (int i = 0; i < vehicleCount; i++) {
            if (vehicles[i]->id == vid) {
                vehicles[i]->isAvailable = true;
                customers[custIndex].rentedVehicleId = -1;
                customers[custIndex].rentDate = "";
                customers[custIndex].rentDays = 0;
                cout << "Vehicle returned successfully.\n";
                return;
            }
        }
    }

    void saveData() {
        ofstream vout("vehicles.txt");
        ofstream coutf("customers.txt");
        if (!vout || !coutf) {
            cout << "Error opening files for saving.\n";
            return;
        }
        vout << vehicleCount << endl;
        coutf << customerCount << endl;
        for (int i = 0; i < vehicleCount; i++)
            vehicles[i]->saveToFile(vout);
        for (int i = 0; i < customerCount; i++)
            customers[i].saveToFile(coutf);
        vout.close();
        coutf.close();
        cout << "Data saved successfully.\n";
    }

    void loadData() {
        ifstream vin("vehicles.txt");
        ifstream cinf("customers.txt");
        if (!vin || !cinf) return;

        vin >> vehicleCount;
        for (int i = 0; i < vehicleCount; i++) {
            string type;
            vin >> type;
            if (type == "Car") {
                vehicles[i] = new Car();
            } else if (type == "Bike") {
                vehicles[i] = new Bike();
            } else {
                vehicles[i] = new Vehicle();
            }
            vehicles[i]->loadFromFile(vin);
        }

        cinf >> customerCount;
        cinf.ignore();
        for (int i = 0; i < customerCount; i++)
            customers[i].loadFromFile(cinf);

        vin.close();
        cinf.close();
    }

    void adminMenu() {
        string uname, pwd;
        cout << "\nAdmin Login\nUsername: ";
        cin >> uname;
        cout << "Password: ";
        cin >> pwd;

        if (uname == "admin" && pwd == "admin") {
            int choice;
            do {
                cout << "\n--- Admin Menu ---\n";
                cout << "1. Add Vehicle\n";
                cout << "2. Show All Vehicles\n";
                cout << "3. Show All Customers\n";
                cout << "4. Save Data\n";
                cout << "5. Exit\n";
                cout << "Enter your choice: ";
                cin >> choice;

                switch (choice) {
                    case 1: addVehicle(); break;
                    case 2: showVehicles(); break;
                    case 3: showCustomers(); break;
                    case 4: saveData(); break;
                    case 5: cout << "Exiting Admin Menu.\n"; break;
                    default: cout << "Invalid choice!\n";
                }
            } while (choice != 5);
        } else {
            cout << "Incorrect admin credentials.\n";
        }
    }

    void customerMenu() {
        if (!customerLogin()) return;
        int choice;
        do {
            cout << "\n--- Customer Menu ---\n";
            cout << "1. View Vehicles\n";
            cout << "2. Rent Vehicle\n";
            cout << "3. Return Vehicle\n";
            cout << "4. View My Info\n";
            cout << "5. Exit\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1: showVehicles(); break;
                case 2: rentVehicle(loggedInCustomerIndex); break;
                case 3: returnVehicle(loggedInCustomerIndex); break;
                case 4: customers[loggedInCustomerIndex].display(); break;
                case 5: cout << "Logging out.\n"; break;
                default: cout << "Invalid choice!\n";
            }
        } while (choice != 5);
    }
};

int main() {
    RentalShop shop;
    shop.loadData();
    int choice;

    do {
        cout << "\n--- Vehicle Rental System ---\n";
        cout << "1. Admin\n";
        cout << "2. Register Customer\n";
        cout << "3. Customer Login\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: shop.adminMenu(); break;
            case 2: shop.registerCustomer(); break;
            case 3: shop.customerMenu(); break;
            case 4: shop.saveData(); cout << "Thank you! Exiting.\n"; break;
            default: cout << "Invalid choice!\n";
        }
    } while (choice != 4);

    return 0;
}