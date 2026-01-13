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
    static int votes[4];
    int age;
    bool hasVoted;

public:
    Voter() {
        age = 0;
        hasVoted = false;
    }

    void setAge(int a) { age = a; }

    // -------- Vote File Handling --------
    static void loadVotes() {
        ifstream fin("votes.txt");
        if (fin)
            fin >> votes[0] >> votes[1] >> votes[2] >> votes[3];
        fin.close();
    }

    static void saveVotes() {
        ofstream fout("votes.txt");
        fout << votes[0] << " " << votes[1] << " "
             << votes[2] << " " << votes[3];
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
            cout << "Not eligible to vote (Age < 18)\n";
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
        cout << "4. Candidate D\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice >= 1 && choice <= 4) {
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
        cout << "Candidate D: " << votes[3] << endl;
    }

    // ===== Feature 2: Winner =====
    static void showWinner() {
        int maxVotes = votes[0];
        int winner = 0;

        for (int i = 1; i < 4; i++) {
            if (votes[i] > maxVotes) {
                maxVotes = votes[i];
                winner = i;
            }
        }

        cout << "\nWinner: Candidate "
             << char('A' + winner)
             << " with " << maxVotes << " votes\n";
    }
};

// Static Initialization
int Voter::votes[4] = {0, 0, 0, 0};

// ================= Admin Class =================
class Admin : public User {
public:
    void vote() override {
        cout << "Admin cannot vote.\n";
    }

    void viewResults() {
        Voter::showVotes();
        Voter::showWinner();  
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

// ================= Main =================
int main() {
    Voter::loadVotes();

    const string ADMIN_PASS = "1234";
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
            string pass;
            cout << "Enter Admin Password: ";
            cin >> pass;

            if (pass != ADMIN_PASS) {
                cout << "Wrong password! Access denied.\n";
                continue;
            }

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

    return 0;
}
