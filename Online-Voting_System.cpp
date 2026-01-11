#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// ================= Base Class =================
class User {
protected:
    string name;
    int id;

public:
    void setName(string n) { name = n; }
    void setID(int i) { id = i; }

    string getName() { return name; }
    int getID() { return id; }

    virtual void vote() = 0;

    virtual void display() {
        cout << "Name: " << name << endl;
        cout << "ID: " << id << endl;
    }

    virtual ~User() {}
};

// ================= Voter Class =================
class Voter : public User {
private:
    static int votes[3];
    int age;
    bool hasVoted;

public:
    Voter() {
        age = 0;
        hasVoted = false;
    }

    // -------- Setters / Getters --------
    void setAge(int a) { age = a; }
    int getAge() { return age; }

    // -------- Vote File Handling --------
    static void loadVotes() {
        ifstream fin("votes.txt");
        if (fin) {
            fin >> votes[0] >> votes[1] >> votes[2];
        }
        fin.close();
    }

    static void saveVotes() {
        ofstream fout("votes.txt");
        fout << votes[0] << " " << votes[1] << " " << votes[2];
        fout.close();
    }

    // -------- Voter Record Handling --------
    bool checkVoterRecord() {
        ifstream fin("voters.txt");
        int fid, fage, voted;
        string fname;

        while (fin >> fid >> fname >> fage >> voted) {
            if (fid == id) {
                name = fname;
                age = fage;
                hasVoted = voted;
                fin.close();
                return true;
            }
        }
        fin.close();
        return false;
    }

    void saveVoterRecord() {
        ofstream fout("voters.txt", ios::app);
        fout << id << " " << name << " " << age << " " << hasVoted << endl;
        fout.close();
    }

    void updateVoterStatus() {
        ifstream fin("voters.txt");
        ofstream fout("temp.txt");

        int fid, fage, voted;
        string fname;

        while (fin >> fid >> fname >> fage >> voted) {
            if (fid == id)
                fout << fid << " " << fname << " " << fage << " 1\n";
            else
                fout << fid << " " << fname << " " << fage << " " << voted << "\n";
        }

        fin.close();
        fout.close();

        remove("voters.txt");
        rename("temp.txt", "voters.txt");
    }

    bool isEligible() {
        return age >= 18;
    }

    // -------- Voting --------
    void vote() override {
        if (!isEligible()) {
            cout << "You are not eligible to vote (Age < 18).\n";
            return;
        }

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
            saveVotes();
            updateVoterStatus();
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

// Static initialization
int Voter::votes[3] = {0, 0, 0};

// ================= Admin Class =================
class Admin : public User {
public:
    void vote() override {
        cout << "Admin cannot vote.\n";
    }

    void viewResults() {
        Voter::showVotes();
    }

    void viewVoters() {
        ifstream fin("voters.txt");
        int id, age, voted;
        string name;

        cout << "\n--- Registered Voters ---\n";
        while (fin >> id >> name >> age >> voted) {
            cout << "ID: " << id
                 << " | Name: " << name
                 << " | Age: " << age
                 << " | Status: " << (voted ? "Voted" : "Not Voted") << endl;
        }
        fin.close();
    }
};

// ================= Main Function =================
int main() {
    Voter::loadVotes();

    int choice;
    do {
        cout << "\n===== Online Voting System =====\n";
        cout << "1. Voter Login\n";
        cout << "2. Admin Login\n";
        cout << "3. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            Voter v;
            int vid, age;
            string name;

            cout << "Enter Voter ID: ";
            cin >> vid;
            v.setID(vid);

            if (!v.checkVoterRecord()) {
                cout << "New Voter Registration\n";
                cout << "Enter Name: ";
                cin >> name;
                cout << "Enter Age: ";
                cin >> age;

                v.setName(name);
                v.setAge(age);
                v.saveVoterRecord();
            }

            v.display();
            v.vote();
        }
        else if (choice == 2) {
            Admin a;
            string name;
            int id;

            cout << "Enter Admin Name: ";
            cin >> name;
            cout << "Enter Admin ID: ";
            cin >> id;

            a.setName(name);
            a.setID(id);

            a.display();
            a.viewResults();
            a.viewVoters();
        }

    } while (choice != 3);

    cout << "Exiting system...\n";
    return 0;
}
