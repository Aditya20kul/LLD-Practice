//============================================================================
// Name        : Splitwise.cpp
// Author      : Aditya
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include<utility>
#include<vector>
#include<unordered_map>
#include<unordered_set>
#include<algorithm>
#include<cmath>
#include<numeric>
using namespace std;

enum Split {EQUAL, PERCENT, EXACT};

class User{
public:
	User(string &);
	const string& getName() const;
	double getTotalExpenseSoFar() const;
	const vector<pair<User, double> > &getUserExpenseSheet() const;
	bool operator==(const User& user) const;
	int getUserId() const;
	void addToUserExpenseSheet(User &, double);
	void printTotalBalance();
private:
	int userId;
	string name;
	vector<pair<User, double>> userExpenseSheet;
	double totalExpenseSoFar;
	int getUniqueId();
};

class Expense{
public:
	Expense(User&, Split, vector<User>&, double);
	const string& getDescription() const;
	void setDescription(const string &description);
	const vector<double>& getExactDistribution() const;
	void setExactDistribution(const vector<double> &exactDistribution);
	int getExpenseId() const;
	const vector<double>& getPercentDistribution() const;
	void setPercentDistribution(const vector<double> &percentDistribution);
	Split getSplit() const;
	void setSplit(Split split);
	const User* getCreditor() const;
	const vector<User>& getDefaulters() const;
	double getExactTotalAmount() const;

private:
	int expenseId;
	string description;
	Split split;
	vector<double> percentDistribution;
	vector<double> exactDistribution;
	int getUniqueExpenseId();
	User *creditor;
	vector<User> defaulters;
	double exactTotalAmount;
};

class Splitwise{
public:
	Splitwise(){}
	void registerUser(User&);
	void addExpense(Expense&);
	void printBalanceForAllUsers();
	const vector<User>& getUsers() const;

private:
	vector<User>users;
	unordered_set<int> userIds;
	bool calculateExpenses(Expense &);
	bool verifyUsers(User, vector<User>);
	vector<double> divideEqually(double, size_t);
};

// Constructors

User::User(string &name){
	this->name = name;
	this->userId = getUniqueId();
	this->totalExpenseSoFar = 0;
	this->userExpenseSheet.clear();
}

Expense::Expense(User& creditor, Split split, vector<User> &defaulters, double exactTotalAmount){
	this->creditor = &creditor;
	this->split = split;
	this->defaulters = defaulters;
	this->exactTotalAmount = exactTotalAmount;
	this->expenseId = getUniqueExpenseId();
}



// End of Constructors

// Getters and Setters

const string& User::getName() const {
	return name;
}

double User::getTotalExpenseSoFar() const {
	return totalExpenseSoFar;
}

const vector<pair<User, double>>& User::getUserExpenseSheet() const {
	return userExpenseSheet;
}

int User::getUserId() const {
	return userId;
}

const string& Expense::getDescription() const {
	return description;
}

void Expense::setDescription(const string &description) {
	this->description = description;
}

const vector<double>& Expense::getExactDistribution() const {
	return exactDistribution;
}

void Expense::setExactDistribution(const vector<double> &exactDistribution) {
	this->exactDistribution = exactDistribution;
}

int Expense::getExpenseId() const {
	return expenseId;
}

const vector<double>& Expense::getPercentDistribution() const {
	return percentDistribution;
}

void Expense::setPercentDistribution(
		const vector<double> &percentDistribution) {
	this->percentDistribution = percentDistribution;
}

Split Expense::getSplit() const {
	return split;
}

void Expense::setSplit(Split split) {
	this->split = split;
}

const User* Expense::getCreditor() const {
	return creditor;
}

const vector<User>& Expense::getDefaulters() const {
	return defaulters;
}

double Expense::getExactTotalAmount() const {
	return exactTotalAmount;
}

const vector<User>& Splitwise::getUsers() const {
	return users;
}

// Getter and Setters End

// Utils Methods

inline bool User:: operator==(const User& user) const
{
	return this->userId == user.getUserId();
}
void User :: addToUserExpenseSheet(User& user, double value){
	if(*this == user)return;
	this->totalExpenseSoFar += value;
	for(auto &newExpense: userExpenseSheet){
		if(newExpense.first == user){
			newExpense.second += value;
			return;
		}
	}
	userExpenseSheet.push_back({user, value});
	return;
}

void User::printTotalBalance(){
	if(totalExpenseSoFar > 0){
		cout<<this->getName()<<" owes a total sum of "<<this->totalExpenseSoFar<<" Rupees\n";
	}
	else{
		cout<<this->getName()<<" gets back a total sum of "<<this->totalExpenseSoFar*(-1)<<" Rupees\n";
	}
}

int User::getUniqueId(){
	static int uniqueId = 1;
	return uniqueId++;
}

int Expense::getUniqueExpenseId(){
	static int expenseId = 1;
	return expenseId++;
}

vector<double> Splitwise::divideEqually(double totalAmount, size_t memberCount){
	vector<double> parts;
	for(size_t i=0; i<memberCount; i++){
		double part = trunc((100 * totalAmount)/(memberCount-i))/100.0;
		parts.push_back(part);
		totalAmount -= part;
	}
	return parts;
}

void Splitwise :: registerUser(User &user){
	if(userIds.find(user.getUserId()) == userIds.end()){
		users.push_back(user);
		userIds.insert(user.getUserId());
	}
}

void Splitwise::addExpense(Expense& expense){
	if(verifyUsers(*expense.getCreditor(), expense.getDefaulters())	 == false)
	{
		cout<<"Can't process expense. Kindly register all users and retry\n";
	}
	calculateExpenses(expense);
}

bool Splitwise :: verifyUsers(User user, vector<User> users){
	if(find(users.begin(), users.end(), user) == users.end()){
		users.push_back(user);
	}
	for(User usr: users){
		if(userIds.find(usr.getUserId()) == userIds.end()){
			return false;
		}
	}
	return true;
}

void Splitwise::printBalanceForAllUsers(){
	for(User usr: users){
		usr.printTotalBalance();
	}
}

bool Splitwise::calculateExpenses(Expense& expense){
	User creditor = *expense.getCreditor();
	vector<User> defaulters = expense.getDefaulters();
	vector<double> amountPerHead;
	switch(expense.getSplit()){
		case Split::EQUAL:
			amountPerHead = divideEqually(expense.getExactTotalAmount(), defaulters.size());
			for(size_t i=0; i<defaulters.size(); i++){
				auto iter = find(users.begin(), users.end(), creditor);
				if(iter != users.end()){
					iter->addToUserExpenseSheet(defaulters[i], (-1)*amountPerHead[i]);
				}
				auto iter1 = find(users.begin(), users.end(), defaulters[i]);
				if(iter1 != users.end()){
					iter1->addToUserExpenseSheet(creditor, amountPerHead[i]);
				}
			}
			break;

		case Split::EXACT:
			amountPerHead = expense.getExactDistribution();
			if(expense.getExactTotalAmount() != accumulate(amountPerHead.begin(), amountPerHead.end(), 0)){
				cout<<"Can't create expense, Total Amount not equal to sum of individual amounts, Please Try again\n";
				return false;
			}
			if(amountPerHead.size() != defaulters.size()){
				cout<<"Can't create expense, The amounts and the value numbers don't match\n";
				return false;
			}
			for(size_t i=0; i<defaulters.size(); i++){
				auto iter2 = find(users.begin(), users.end(), creditor);
				if(iter2 != users.end()){
					iter2->addToUserExpenseSheet(defaulters[i], (-1)*amountPerHead[i]);
				}
				auto iter3 = find(users.begin(), users.end(), defaulters[i]);
				if(iter3 != users.end()){
					iter3->addToUserExpenseSheet(creditor, amountPerHead[i]);
				}
			}
			break;

		case Split::PERCENT:
			amountPerHead = expense.getPercentDistribution();
					if(100 != accumulate(amountPerHead.begin(), amountPerHead.end(), 0))
					{
						cout << "Can't create expense. All percentages don't add to 100. "
				                "Please try again!\n";
						return false;
					}

					if(amountPerHead.size() != defaulters.size())
					{
						cout << "The percents and value numbers don't match. Expense can't be created. "
						                "Please try again!\n";
						return false;
					}
					for(size_t i=0; i < defaulters.size(); ++i)
					{
						double amount = (amountPerHead[i] * expense.getExactTotalAmount()) / 100.0;
						amount = floor((amount * 100.0) + 0.5) / 100.0;
						auto iter4 = find(users.begin(), users.end(), creditor);
						if(iter4 != users.end()){
							iter4->addToUserExpenseSheet(defaulters[i], (-1)*amount);
						}
						auto iter5 = find(users.begin(), users.end(), defaulters[i]);
						if(iter5 != users.end()){
							iter5->addToUserExpenseSheet(creditor, amount);
						}
					}
			break;
		default:
			break;
	}
}

// Utils Method Ends

int main() {
	 string s1 = "Jitu";
	    string s2 = "Navin";
	    string s3 = "Yogi";
	    string s4 = "Mandal";
	    User u1(s1);
	    User u2(s2);
	    User u3(s3);
	    User u4(s4);

	    vector<User> users;
	    users.push_back(u1);
	    users.push_back(u2);
	    users.push_back(u3);
	    users.push_back(u4);

	    Splitwise sp;
	    sp.registerUser(u1);
	    sp.registerUser(u2);
	    sp.registerUser(u3);
	    sp.registerUser(u4);

	    Expense expense(u1, Split::EQUAL, users, 2000);
	    sp.addExpense(expense);

	    sp.printBalanceForAllUsers();

	    vector<User> users2;
	    users2.push_back(u2);
	    users2.push_back(u3);

	    Expense expense2(u1, Split::EXACT, users2, 1400);
	    sp.addExpense(expense2);

	    vector<double> db = {500, 900};
	    expense2.setExactDistribution(db);
	    sp.addExpense(expense2);
	    sp.printBalanceForAllUsers();

	    vector<double> db2 = { 40,20,20,20 };
	    vector<User> users3 = { u1, u2, u3, u4 };

	    Expense expense3(u4, Split::PERCENT, users3, 1200);
	    expense3.setPercentDistribution(db2);
	    sp.addExpense(expense3);
	    sp.printBalanceForAllUsers();
	    cout << endl;
	    cout << endl;

	    for (User user : sp.getUsers())
	    {
	        for (pair<User, double> p : user.getUserExpenseSheet())
	        {
	            if (p.second > 0)
	                cout << user.getName().c_str() << " owes a total of " << p.second << " to " << p.first.getName().c_str() << "\n";
	            else
	                cout << user.getName().c_str() << " gets back a total of " << p.second * (-1) << " from " << p.first.getName().c_str() << "\n";

	        }
	    }

	cout << endl;
	return 0;
}


