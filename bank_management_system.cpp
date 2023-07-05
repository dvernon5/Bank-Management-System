#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <vector>
#include <map>

class InsufficientFunds{};

class Account
{
    public:
        Account() = default;
        Account(std::string firstName, std::string lastName, double balance);
        long getAccountNumber() { return accountNumber; }
        std::string getFirstName() { return firstName; }
        std::string getLastName() { return lastName; }
        double getBalance(){ return balance; }
        
        void Deposit(double amount);
        void Withdraw(double amount);
        static void setLastAccountNumber(long accountNumber);
        static long getLastAccountNumber();
        friend std::ofstream & operator << (std::ofstream& ofs, Account& account);
        friend std::ifstream & operator >> (std::ifstream& ifs, Account& account);
        friend std::ostream  & operator << (std::ostream&  os,  Account& account);
        
    private:
        long accountNumber;
        std::string firstName;
        std::string lastName;
        double balance;
        static long nextAccountNumber;
};

long Account::nextAccountNumber = 0;

class Bank
{
    public:
        Bank();
        Account OpenAccount(std::string firstName, std::string lastName, double balance);
        Account BalanceEnquiry(long accountNumber);
        Account Deposit(long accountNumber, double amount);
        Account Withdraw(long accountNumber, double amount);
        void closeAccount(long accountNumber);
        void showAllAccounts();
        ~Bank();
    
    private:
        std::map<long, Account> accounts;
};

int main(void)
{
    Bank bank;
    Account account;
    
    int selection = 0;
    std::string firstName, lastName;
    long accountNumber = 0;
    double balance = 0.0;
    double amount = 0.0;
    
    std::cout << "***Banking System***" << std::endl;
    
    do
    {
        std::cout << "\n\tSelect one option below" << std::endl;
        std::cout << "\t1. Open an Account" << std::endl;
        std::cout << "\t2. Balance Enquiry" << std::endl;
        std::cout << "\t3. Deposit" << std::endl;
        std::cout << "\t4. Withdrawl" << std::endl;
        std::cout << "\t5. Close an Account" << std::endl;
        std::cout << "\t6. Show All Accounts" << std::endl;
        std::cout << "\t7. Quit" << std::endl;
        
        std::cout << "Enter your choice: ";
        std::cin  >> selection;
        
        switch (selection) {
            case 1:
                std::cout << "Enter your first name: ";
                std::cin  >> firstName;
                
                std::cout << "Enter your last name: ";
                std::cin  >> lastName;
                
                std::cout << "Enter your initial amount: ";
                std::cin  >> balance;
                
                account = bank.OpenAccount(firstName, lastName, balance);
                std::cout << "\n" << "Congradulations account is Created" << std::endl;
                std::cout << account;
                break;
            
            case 2:
                std::cout << "Enter Account Number: ";
                std::cout << accountNumber << std::endl;
                
                account = bank.BalanceEnquiry(accountNumber);
                std::cout << "\n" << "Your account Details" << std::endl;
                std::cout << account;
                break;
                
            case 3:
                std::cout << "Enter Account Number: ";
                std::cin  >> accountNumber;
                
                std::cout << "Enter Balance: ";
                std::cin  >> amount;
                
                account = bank.Deposit(accountNumber, amount);
                std::cout << std::endl << "Amount is Deposited" << std::endl;
                std::cout << account;
                break;
                
            case 4:
                std::cout << "Enter Account Number: ";
                std::cin  >> accountNumber;
                
                std::cout << "Enter Balance: ";
                std::cin  >> amount;
                
                account = bank.Withdraw(accountNumber, amount);
                std::cout << "\n" << "Amount Withdrawn" << std::endl;
                std::cout << account;
                break;
                
            case 5:
                std::cout << "Enter Account Number: ";
                std::cin  >> accountNumber;
                
                bank.closeAccount(accountNumber);
                std::cout << "\n" << "Account is Closed" << std::endl;
                std::cout << account;
                break;
                
            case 6:
                bank.showAllAccounts();
                break;
                
            case 7:
                break;
                
            default:
                std::cout << "\nPlease select correct choice";
                exit(0);
        }
        
    } while (selection != 7);
    
    return 0;
}

Account::Account(std::string firstName, std::string lastName, double balance)
{
    ++nextAccountNumber;
    accountNumber = nextAccountNumber;
    this->firstName = firstName;
    this->lastName = lastName;
    this->balance = balance;
}

void Account::Deposit(double amount)
{
    balance += amount;  // balance = balance + amount
}

void Account::Withdraw(double amount)
{
    if (balance - amount < 0)
    {
        throw InsufficientFunds();
    }
    
    balance -= amount;  // balance = balance - amount
}

void Account::setLastAccountNumber(long accountNumber)
{
    nextAccountNumber = accountNumber;
}

long Account::getLastAccountNumber()
{
    return nextAccountNumber;
}

std::ofstream & operator << (std::ofstream& ofs, Account& account)
{
    ofs << account.firstName << std::endl;
    ofs << account.lastName << std::endl;
    ofs << account.accountNumber << std::endl;
    ofs << account.balance << std::endl;
    
    return ofs;
}

std::ifstream & operator >> (std::ifstream& ifs, Account& account)
{
    ifs >> account.firstName;
    ifs >> account.lastName;
    ifs >> account.accountNumber;
    ifs >> account.balance;
    
    return ifs;
}

std::ostream & operator << (std::ostream& os, Account& account)
{
    os << "First Name: " << account.firstName << std::endl;
    os << "Last Name: " << account.lastName << std::endl;
    os << "Account Number " << account.accountNumber << std::endl;
    os << "Balance " << account.balance << std::endl;
    
    return os;
}

Bank::Bank()
{
    Account account;
    std::ifstream infile;
    infile.open("Bank.data");
    
    if (!infile)
    {
        std::cout << "Error in Opening" << std::endl;
        return;
    }
    
    while (!infile.eof())
    {
        infile >> account;
        accounts.insert(std::pair<long, Account>(account.getAccountNumber(), account));
    }
    
    Account::setLastAccountNumber(account.getAccountNumber());
    
    infile.close();
}

Account Bank::OpenAccount(std::string firstName, std::string lastName, double balance)
{
    std::ofstream outfile;
    Account account(firstName, lastName, balance);
    accounts.insert(std::pair<long, Account>(account.getAccountNumber(), account));
    
    outfile.open("Bank.data", std::ios::trunc);
    
    std::map<long, Account>::iterator itr;
    
    for (itr = accounts.begin(); itr != accounts.end(); ++itr)
    {
        outfile << itr->second;
    }
    
    outfile.close();
    
    return account;
}

Account Bank::BalanceEnquiry(long accountNumber)
{
    std::map<long, Account>::iterator itr = accounts.find(accountNumber);
    
    return itr->second;
}

Account Bank::Deposit(long accountNumber, double amount)
{
    std::map<long, Account>::iterator itr = accounts.find(accountNumber);
    itr->second.Deposit(amount);
    
    return itr->second;
}

Account Bank::Withdraw(long accountNumber, double amount)
{
    std::map<long, Account>::iterator itr = accounts.find(accountNumber);
    itr->second.Withdraw(amount);
    
    return itr->second;
}

void Bank::closeAccount(long accountNumber)
{
    std::map<long, Account>::iterator itr = accounts.find(accountNumber);
    
    std::cout << "Account Deleted " << itr->second;
    accounts.erase(accountNumber);
}

void Bank::showAllAccounts()
{
    std::map<long, Account>::iterator itr;
    
    for (itr = accounts.begin(); itr != accounts.end(); ++itr)
    {
        std::cout << "Account " << itr->first << "\n" << itr->second << std::endl;
    }
}

Bank::~Bank()
{
    std::ofstream outfile;
    outfile.open("Bank.data", std::ios::trunc);
    
    std::map<long, Account>::iterator itr;
    
    for (itr = accounts.begin(); itr != accounts.end(); ++itr)
    {
        outfile << itr->second;
    }
    
    outfile.close();
}
