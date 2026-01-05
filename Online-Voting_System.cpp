#include <iostream>
using namespace std;

// Base class
class User {
protected:
    string name;
    int id;

public:
    void setUser(string n, int i) {
        name = n;
        id = i;
    }

    virtual void vote() = 0;   // Abstraction (pure virtual function)

    virtual void display() {
        cout << "Name: " << name << endl;
        cout << "ID: " << id << endl;
    }
};

// Derived class: Voter
class Voter : public User {
private:
    static int votes[3];   // Encapsulation
    bool hasVoted;

public:
    Voter() {
        hasVoted = false;
    }

    void vote() {   // Polymorphism
        if (hasVoted) {
            cout << "You have already voted!\n";
            return;
        }

        int choice;
        cout << "\nVote for a candidate:\n";
        cout << "1. Candidate A\n";
        cout << "2. Candidate B\n";
        cout << "3. Candidate C\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice >= 1 && choice <= 3) {
            votes[choice - 1]++;
            hasVoted = true;
            cout << "Vote submitted successfully!\n";
        } else {
            cout << "Invalid choice!\n";
        }
    }

    static void showVotes() {
        cout << "\n--- Voting Results ---\n";
        cout << "Candidate A: " << votes[0] << endl;
        cout << "Candidate B: " << votes[1] << endl;
        cout << "Candidate C: " << votes[2] << endl;
    }
};

// Static member initialization
int Voter::votes[3] = {0, 0, 0};

// Derived class: Admin
class Admin : public User {
public:
    void vote() {   // Polymorphism
        cout << "\nAdmin cannot vote.\n";
    }

    void viewResults() {
        Voter::showVotes();
    }
};

int main() {
    int choice;
    cout << "Online Voting System (Simulation)\n";
    cout << "1. Voter\n";
    cout << "2. Admin\n";
    cout << "Enter choice: ";
    cin >> choice;

    if (choice == 1) {
        Voter v;
        string name;
        int id;

        cout << "Enter voter name: ";
        cin >> name;
        cout << "Enter voter ID: ";
        cin >> id;

        v.setUser(name, id);
        v.display();
        v.vote();
    }
    else if (choice == 2) {
        Admin a;
        string name;
        int id;

        cout << "Enter admin name: ";
        cin >> name;
        cout << "Enter admin ID: ";
        cin >> id;

        a.setUser(name, id);
        a.display();
        a.viewResults();
    }
    else {
        cout << "Invalid option!";
    }

    return 0;
}
