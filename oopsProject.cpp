#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <map>

using namespace std;
#define MIN_BALANCE 500

class InsufficientFunds
{
};

class Account
{
private:
    long accNumber;
    string firstName;
    string lastName;
    float balance;
    static long nextAccNumber;

public:
    Account() {}
    Account(string fname, string lname, float balance);
    long getAccNo()
    {
        return accNumber;
    }
    string getFirstName()
    {
        return firstName;
    }
    string getLastName()
    {
        return lastName;
    }
    float getBalance()
    {
        return balance;
    }
    void deposit(float amount);
    void withdraw(float amount);
    static void setLastAccNumber(long accountNumber);
    static long getLastAccNumber();
    friend ofstream &operator<<(ofstream &ofs, Account &acc);
    friend ifstream &operator>>(ifstream &ifs, Account &acc);
    friend ostream &operator<<(ostream &os, Account &acc);
};

long Account::nextAccNumber = 0;

class Bank
{
private:
    map<long, Account> accounts;

public:
    Bank();
    Account openAccount(string fname, string lname, float balance);
    Account balanceEnquiry(long accNumber);
    Account deposit(long accNumber, float amount);
    Account withdraw(long accNumber, float amount);
    void closeAccount(long accNumber);
    void showAllAccounts();
    ~Bank();
};

int main()
{
    Bank b;
    Account acc;
    int choice;
    string fname, lname;
    long accNumber;
    float balance;
    float amount;
    cout << "***Welcome to the Banking System***" << endl;

    do
    {
        cout << "\n\tPlease select an option:";
        cout << "\n\t1. Open an Account";
        cout << "\n\t2. Check Account Balance";
        cout << "\n\t3. Deposit Funds";
        cout << "\n\t4. Withdraw Funds";
        cout << "\n\t5. Close an Account";
        cout << "\n\t6. Show All Accounts";
        cout << "\n\t7. Quit";
        cout << "\nEnter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            cout << "Enter First Name: ";
            cin >> fname;
            cout << "Enter Last Name: ";
            cin >> lname;
            cout << "Enter the Initial Deposit Amount: ";
            cin >> balance;

            acc = b.openAccount(fname, lname, balance);
            cout << "\nCongratulations! Your account has been successfully created." << endl;
            cout << "Account Details:" << endl;
            cout << acc;
            break;

        case 2:
            cout << "Enter Account Number: ";
            cin >> accNumber;

            acc = b.balanceEnquiry(accNumber);
            cout << "\nAccount Details:" << endl;
            cout << acc;
            break;

        case 3:
            cout << "Enter Account Number: ";
            cin >> accNumber;
            cout << "Enter the Amount to Deposit: ";
            cin >> amount;

            acc = b.deposit(accNumber, amount);
            cout << "\nDeposit successful. Updated Account Details:" << endl;
            cout << acc;
            break;

        case 4:
            cout << "Enter Account Number: ";
            cin >> accNumber;
            cout << "Enter the Amount to Withdraw: ";
            cin >> amount;

            acc = b.withdraw(accNumber, amount);
            cout << "\nWithdrawal successful. Updated Account Details:" << endl;
            cout << acc;
            break;

        case 5:
            cout << "Enter Account Number: ";
            cin >> accNumber;

            b.closeAccount(accNumber);
            cout << "\nAccount successfully closed." << endl;
            break;

        case 6:
            b.showAllAccounts();
            break;

        case 7:
            break;

        default:
            cout << "\nPlease enter a valid choice.";
            exit(0);
        }
    } while (choice != 7);

    return 0;
}

Account::Account(string fname, string lname, float balance)
{
    nextAccNumber++;
    accNumber = nextAccNumber;
    firstName = fname;
    lastName = lname;
    this->balance = balance;
}

void Account::deposit(float amount)
{
    balance += amount;
}

void Account::withdraw(float amount)
{
    if (balance - amount < MIN_BALANCE)
        throw InsufficientFunds();
    balance -= amount;
}

void Account::setLastAccNumber(long accountNumber)
{
    nextAccNumber = accountNumber;
}

long Account::getLastAccNumber()
{
    return nextAccNumber;
}

ofstream &operator<<(ofstream &ofs, Account &acc)
{
    ofs << acc.accNumber << endl;
    ofs << acc.firstName << endl;
    ofs << acc.lastName << endl;
    ofs << acc.balance << endl;
    return ofs;
}

ifstream &operator>>(ifstream &ifs, Account &acc)
{
    ifs >> acc.accNumber;
    ifs >> acc.firstName;
    ifs >> acc.lastName;
    ifs >> acc.balance;
    return ifs;
}

ostream &operator<<(ostream &os, Account &acc)
{
    os << "First Name:" << acc.getFirstName() << endl;
    os << "Last Name:" << acc.getLastName() << endl;
    os << "Account Number:" << acc.getAccNo() << endl;
    os << "Balance:" << acc.getBalance() << endl;
    return os;
}

Bank::Bank()
{
    Account account;
    ifstream infile;
    infile.open("Bank.data");
    if (!infile)
    {
        cout << "Error in Opening! File Not Found!!" << endl;
        return;
    }
    while (!infile.eof())
    {
        infile >> account;
        accounts.insert(pair<long, Account>(account.getAccNo(), account));
    }
    Account::setLastAccNumber(account.getAccNo());
    infile.close();
}

Account Bank::openAccount(string fname, string lname, float balance)
{
    ofstream outfile;
    Account account(fname, lname, balance);
    accounts.insert(pair<long, Account>(account.getAccNo(), account));
    outfile.open("Bank.data", ios::trunc);
    map<long, Account>::iterator itr;
    for (itr = accounts.begin(); itr != accounts.end(); itr++)
    {
        outfile << itr->second;
    }
    outfile.close();
    return account;
}

Account Bank::balanceEnquiry(long accNumber)
{
    map<long, Account>::iterator itr = accounts.find(accNumber);
    return itr->second;
}

Account Bank::deposit(long accNumber, float amount)
{
    map<long, Account>::iterator itr = accounts.find(accNumber);
    itr->second.deposit(amount);
    return itr->second;
}

Account Bank::withdraw(long accNumber, float amount)
{
    map<long, Account>::iterator itr = accounts.find(accNumber);
    itr->second.withdraw(amount);
    return itr->second;
}

void Bank::closeAccount(long accNumber)
{
    map<long, Account>::iterator itr = accounts.find(accNumber);
    cout << "Account Deleted" << itr->second;
    accounts.erase(accNumber);
}

void Bank::showAllAccounts()
{
    map<long, Account>::iterator itr;
    for (itr = accounts.begin(); itr != accounts.end(); itr++)
    {
        cout << "Account " << itr->first << endl
             << itr->second << endl;
    }
}

Bank::~Bank()
{
    ofstream outfile;
    outfile.open("Bank.data", ios::trunc);
    map<long, Account>::iterator itr;
    for (itr = accounts.begin(); itr != accounts.end(); itr++)
    {
        outfile << itr->second;
    }
    outfile.close();
}
