#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class Car {
private:
    string carId;
    string brand;
    string model;
    double basePricePerDay;
    bool isAvailable;

public:
    Car(string carId, string brand, string model, double basePricePerDay) {
        this->carId = carId;
        this->brand = brand;
        this->model = model;
        this->basePricePerDay = basePricePerDay;
        this->isAvailable = true;
    }

    string getCarId() {
        return carId;
    }

    string getBrand() {
        return brand;
    }

    string getModel() {
        return model;
    }

    double calculatePrice(int rentalDays) {
        return basePricePerDay * rentalDays;
    }

    bool isAvailableForRent() {
        return isAvailable;
    }

    void rent() {
        isAvailable = false;
    }

    void returnCar() {
        isAvailable = true;
    }
};

class Customer {
private:
    string customerId;
    string name;

public:
    Customer(string customerId, string name) {
        this->customerId = customerId;
        this->name = name;
    }

    string getCustomerId() {
        return customerId;
    }

    string getName() {
        return name;
    }
};

class Rental {
private:
    Car* car;
    Customer* customer;
    int days;

public:
    Rental(Car* car, Customer* customer, int days) {
        this->car = car;
        this->customer = customer;
        this->days = days;
    }

    Car* getCar() {
        return car;
    }

    Customer* getCustomer() {
        return customer;
    }

    int getDays() {
        return days;
    }

    bool operator==(const Rental& other) const {
        return car == other.car && customer == other.customer && days == other.days;
    }
};

class CarRentalSystem {
private:
    vector<Car> cars;
    vector<Customer> customers;
    vector<Rental> rentals;

public:
    void addCar(Car car) {
        cars.push_back(car);
    }

    void addCustomer(Customer customer) {
        customers.push_back(customer);
    }

    void rentCar(Car& car, Customer& customer, int days) {
        if (car.isAvailableForRent()) {
            car.rent();
            rentals.push_back(Rental(&car, &customer, days));
        } else {
            cout << "Car is not available for rent." << endl;
        }
    }

    void returnCar(Car& car) {
        car.returnCar();
        Rental* rentalToRemove = nullptr;
        for (auto& rental : rentals) {
            if (rental.getCar() == &car) {
                rentalToRemove = &rental;
                break;
            }
        }
        if (rentalToRemove != nullptr) {
            rentals.erase(std::remove(rentals.begin(), rentals.end(), *rentalToRemove), rentals.end());
        } else {
            cout << "Car was not rented." << endl;
        }
    }

    void menu() {
        int choice;
        while (true) {
            cout << "===== Car Rental System =====" << endl;
            cout << "1. Rent a Car" << endl;
            cout << "2. Return a Car" << endl;
            cout << "3. Exit" << endl;
            cout << "Enter your choice: ";
            cin >> choice;
            cin.ignore();

            if (choice == 1) {
                cout << "\n== Rent a Car ==\n" << endl;
                string customerName;
                cout << "Enter your name: ";
                getline(cin, customerName);

                cout << "\nAvailable Cars:" << endl;
                for (auto& car : cars) {
                    if (car.isAvailableForRent()) {
                        cout << car.getCarId() << " - " << car.getBrand() << " " << car.getModel() << endl;
                    }
                }

                string carId;
                cout << "\nEnter the car ID you want to rent: ";
                getline(cin, carId);

                int rentalDays;
                cout << "Enter the number of days for rental: ";
                cin >> rentalDays;
                cin.ignore();

                Customer newCustomer("CUS" + to_string(customers.size() + 1), customerName);
                addCustomer(newCustomer);

                Car* selectedCar = nullptr;
                for (auto& car : cars) {
                    if (car.getCarId() == carId && car.isAvailableForRent()) {
                        selectedCar = &car;
                        break;
                    }
                }

                if (selectedCar != nullptr) {
                    double totalPrice = selectedCar->calculatePrice(rentalDays);
                    cout << "\n== Rental Information ==\n" << endl;
                    cout << "Customer ID: " << newCustomer.getCustomerId() << endl;
                    cout << "Customer Name: " << newCustomer.getName() << endl;
                    cout << "Car: " << selectedCar->getBrand() << " " << selectedCar->getModel() << endl;
                    cout << "Rental Days: " << rentalDays << endl;
                    cout << "Total Price: $" << totalPrice << endl;

                    char confirm;
                    cout << "\nConfirm rental (Y/N): ";
                    cin >> confirm;
                    cin.ignore();

                    if (confirm == 'Y' || confirm == 'y') {
                        rentCar(*selectedCar, newCustomer, rentalDays);
                        cout << "\nCar rented successfully." << endl;
                    } else {
                        cout << "\nRental canceled." << endl;
                    }
                } else {
                    cout << "\nInvalid car selection or car not available for rent." << endl;
                }
            } else if (choice == 2) {
                cout << "\n== Return a Car ==\n" << endl;
                string carId;
                cout << "Enter the car ID you want to return: ";
                getline(cin, carId);

                Car* carToReturn = nullptr;
                for (auto& car : cars) {
                    if (car.getCarId() == carId && !car.isAvailableForRent()) {
                        carToReturn = &car;
                        break;
                    }
                }

                if (carToReturn != nullptr) {
                    Customer* customer = nullptr;
                    for (auto& rental : rentals) {
                        if (rental.getCar() == carToReturn) {
                            customer = rental.getCustomer();
                            break;
                        }
                    }

                    if (customer != nullptr) {
                        returnCar(*carToReturn);
                        cout << "Car returned successfully by " << customer->getName() << endl;
                    } else {
                        cout << "Car was not rented or rental information is missing." << endl;
                    }
                } else {
                    cout << "Invalid car ID or car is not rented." << endl;
                }
            } else if (choice == 3) {
                break;
            } else {
                cout << "Invalid choice. Please enter a valid option." << endl;
            }
        }

        cout << "\nThank you for using the Car Rental System!" << endl;
    }
};

int main() {
    CarRentalSystem rentalSystem;

    Car car1("C001", "Toyota", "Camry", 60.0);
    Car car2("C002", "Honda", "Accord", 70.0);
    Car car3("C003", "Mahindra", "Thar", 150.0);
    rentalSystem.addCar(car1);
    rentalSystem.addCar(car2);
    rentalSystem.addCar(car3);

    rentalSystem.menu();

    return 0;
}
