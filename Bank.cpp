// Bank.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include<vector>
#include<string>
#include<fstream>
#include <iomanip>
using namespace std;
const string FileName = "MyFile.txt";
enum enMainMenue {
    ShowClientLists = 1,
    NewClient,
    DeleteClient,
    UpdateClientInfo,
    FindClient,
    Transaction,
    Exit
};
enum enTransAction {
    Deposit = 1,
    Wthdraw,
    TotalBalances,
    MainMenue
};
struct stClient {
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
    bool MarkDelete = false;
    double totalBalances;
    bool Add = false;
};
bool UpdateRecordDataFromFile(string, vector<stClient>&);
void ShowClientsInformation(vector <stClient>);
void ChoosingOperation(enMainMenue&);
void PrintClientRecord(stClient);
bool UpdateRecordDataFromFile(string, vector<stClient>&);
//void PrintResult();
string ReadClientAccountNumber();
bool ClientExitsByAccountNumber(string, string);
stClient AddClientData();
vector <stClient> SaveClientToFile(string, vector<stClient>);
stClient UpdateClientData(string&, stClient&);
vector <stClient> DeleteClientFromFile(string, vector<stClient>);
bool MarkClientForDeleteByAccountNumber(string, vector<stClient>&);
void MainMenueScreen();
void ShowTransactionsScreen();
void AddDepositOperation(vector<stClient>& ,enTransAction&);
double AddMoneyAmmount(vector<stClient>& , string);
bool FindClientByAccountNumber(string , vector<stClient> , stClient& );
vector<string> SplitString(string sentence, string delim = "#//#") {
    size_t pos;
    string sWord = "";
    vector<string>vSentence;
    //use find() function to get the position of delimiter
    while ((pos = sentence.find(delim)) != string::npos) {
        sWord = sentence.substr(0, pos);
        if (sWord != delim) {
            vSentence.push_back(sWord);
        }
        //erase() until position and move to the next word
        sentence.erase(0, pos + delim.length());
    }
    if (sentence != delim) {
        vSentence.push_back(sentence);
    }
    return vSentence;
}
string  ConvertRecordToLine(stClient Client, string seperator = "#//#") {
    string stClientRecord = "";
    vector<stClient>vClient;
    stClientRecord += Client.AccountNumber + seperator;
    stClientRecord += Client.PinCode + seperator;
    stClientRecord += Client.Name + seperator;
    stClientRecord += Client.Phone + seperator;
    stClientRecord += to_string(Client.AccountBalance);
    return stClientRecord;
}
string  ConvertRecordToLineUpdate(stClient Client, string seperator = "#//#") {
    string stClientRecord = "";
    stClientRecord += Client.AccountNumber + seperator;
    stClientRecord += Client.PinCode + seperator;
    stClientRecord += Client.Name + seperator;
    stClientRecord += Client.Phone + seperator;
    stClientRecord += to_string(Client.AccountBalance);
    return stClientRecord;
}
stClient ConvertLineToRecord(string Line, string seperator = "#//#") {
    vector<string>vSentence;
    stClient Client;
    vSentence = SplitString(Line, seperator);
    Client.AccountNumber = vSentence[0];
    Client.PinCode = vSentence[1];
    Client.Name = vSentence[2];
    Client.Phone = vSentence[3];
    Client.AccountBalance = stod(vSentence[4]);
    return Client;
}
vector<stClient> LoadClientDataFromFile(string FileName) {
    vector <stClient> vClientRecords;
    fstream MyFile;
    short counter = 0;
    MyFile.open(FileName, ios::in);
    if (MyFile.is_open()) {
        string Line;
        stClient Client;
        while (getline(MyFile, Line)) {
            Client = ConvertLineToRecord(Line, "#//#");
            vClientRecords.push_back(Client);
        }
        MyFile.close();
    }
    return vClientRecords;
}
bool FindClientByAccountNumber(string accountnumber, vector<stClient> vClient, stClient& Client) {
    for (stClient &c : vClient) {
        if (c.AccountNumber == accountnumber) {
            Client = c;
            return true;
        }
    }
    return false;
}
bool MarkClientForUpdateByAccountNumber(string accountnumber, vector<stClient>& vClient) {
    for (stClient& c : vClient) {
        if (c.AccountNumber == accountnumber) {
            c.MarkDelete = true;
            return true;
        }
    }
    return false;
}
vector <stClient> SaveClientToFile(string FileName, vector<stClient> vClient) {
    fstream MyFile;
    MyFile.open(FileName, ios::out);
    string DataLine;
    if (MyFile.is_open()) {
        for (stClient c : vClient) {
            if (c.MarkDelete == false) {
                DataLine = ConvertRecordToLine(c);
                MyFile << DataLine << endl;
            }
        }
        MyFile.close();
    }
    return vClient;
}
stClient UpdateClientData(string& accountnumber, stClient& ClientData) {
    ClientData.AccountNumber = accountnumber;
    cout << "Enter Name?";
    getline(cin >> ws, ClientData.Name);
    cout << "Enter Phone?";
    getline(cin, ClientData.Phone);
    cout << "Enter Pin Code?";
    getline(cin, ClientData.PinCode);
    cout << "Enter Account Balance?";
    cin >> ClientData.AccountBalance;
    return ClientData;
}
vector <stClient> UpdateClientToFile(string FileName, vector<stClient> vClient) {
    fstream MyFile;
    stClient Client;
    MyFile.open(FileName, ios::in);
    string DataLine;
    if (MyFile.is_open()) {
        for (stClient c : vClient) {
            if (Client.MarkDelete == false) {
                DataLine = ConvertRecordToLine(c);
                MyFile << DataLine << endl;
            }
        }
        MyFile.close();
    }
    return vClient;
}
bool UpdateRecordDataFromFile(string AccountNumber, vector<stClient>& vContent) {
    stClient Client;
    char Answer = 'n';
    if (FindClientByAccountNumber(AccountNumber, vContent, Client)) {
        PrintClientRecord(Client);
        cout << "\nare you sure want to update client\n";
        cin >> Answer;
        if (Answer == 'Y' || Answer == 'y') {
            for (stClient& c : vContent) {
                if (c.AccountNumber == AccountNumber) {
                    c = UpdateClientData(c.AccountNumber, Client);
                    break;
                }
            }
            SaveClientToFile(FileName, vContent);
            cout << "\nClient updated succesfully from client\n";
            return true;
        }
    }
    else {
        cout << "\nClient with account number (" << AccountNumber << ") isn't avaiable";
        return false;
    }
}
bool ClientExitsByAccountNumber(string AccountNumber, string FileName) {
    vector <stClient>vClients;
    fstream MyFile;
    MyFile.open(FileName, ios::in);
    if (MyFile.is_open()) {
        string Line;
        stClient Client;
        while (getline(MyFile, Line)) {
            Client = ConvertLineToRecord(Line);
            if (Client.AccountNumber == AccountNumber) {
                MyFile.close();
                return true;
            }
            vClients.push_back(Client);
        }
        MyFile.close();
    }
    return false;
}
stClient AddClientData() {
    stClient ClientData;
    vector <stClient> vClient;
    cout << "Enter Account Number?";
    getline(cin>>ws, ClientData.AccountNumber);
    while (ClientExitsByAccountNumber(ClientData.AccountNumber, FileName)) {
        cout << "no account number[" << ClientData.AccountNumber << "]is available add another account number\n";
        cout << "Enter Account Number?";
        getline(cin>>ws, ClientData.AccountNumber);
    }
    cout << "Enter Name?";
    getline(cin >> ws, ClientData.Name);
    cout << "Enter Phone?";
    getline(cin, ClientData.Phone);
    cout << "Enter Pin Code?";
    getline(cin, ClientData.PinCode);
    cout << "Enter Account Balance?";
    cin >> ClientData.AccountBalance;
    return ClientData;

}
string ReadClientAccountNumber() {
    stClient Client;
    cout << "Enter Account Number\n";
    cin >> Client.AccountNumber;
    return Client.AccountNumber;
}
string  ConvertRecordToLineAdd(stClient& Client, string seperator = "#//#") {
    string stClientRecord = "";
    Client = AddClientData();
    vector<stClient>vClient;
    stClientRecord += Client.AccountNumber + seperator;
    stClientRecord += Client.PinCode + seperator;
    stClientRecord += Client.Name + seperator;
    stClientRecord += Client.Phone + seperator;
    stClientRecord += to_string(Client.AccountBalance);
    return stClientRecord;
}
bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector<stClient>& vClient) {
    for (stClient &c : vClient) {
        if (c.AccountNumber == AccountNumber) {
            c.MarkDelete = true;
            return true;
        }
    }
    return false;
}
void PrintClientRecord(stClient Client) {
    cout << "\nAccount Number: " << Client.AccountNumber;
    cout << "\nPin Code: " << Client.PinCode;
    cout << "\nName: " << Client.Name;
    cout << "\nPhone: " << Client.Phone;
    cout << "\nAccount Balance: " << Client.AccountBalance;
}
void AddClientToFile(string FileName, string DataLine) {
    fstream MyFile;
    stClient Client;
    MyFile.open(FileName, ios::out | ios::app);
    if (MyFile.is_open()) {
        MyFile << DataLine << endl;
    }
    MyFile.close();

}
vector <stClient> DeleteClientFromFile(string FileName, vector<stClient> vClient) {
    fstream MyFile;
    stClient Client;
    MyFile.open(FileName, ios::out);
    string DataLine;
    if (MyFile.is_open()) {
        for (stClient& c : vClient) {
            if (c.MarkDelete == false) {
                DataLine = ConvertRecordToLine(c);
                MyFile << DataLine << endl;
            }
        }
        MyFile.close();
    }
    return vClient;
}
void AddNewClient() {
    stClient Client = AddClientData();
    vector <stClient> vClientRecords = LoadClientDataFromFile(FileName);
    AddClientToFile(FileName, ConvertRecordToLine(Client));
}
void AddNewClients() {
    char AddMore = 'y';
    do {
        cout << "Add new Client\n\n";
        AddNewClient();
        cout << "\nClient Added Successfully,do you want to add more clients?";
        cin >> AddMore;
    } while (toupper(AddMore) == 'Y');
}
void ShowAddNewClientsScreen() {
    stClient ClientData;
    cout << "_______________________________________\n";
    cout << "Add new clients screen\n";
    cout << "_______________________________________\n";
    AddNewClients();
}
bool DeleteRecordDataFromFile(string AccountNumber, vector<stClient>& vContent) {
    stClient Client;
    char Answer = 'n';
    if (FindClientByAccountNumber(AccountNumber, vContent, Client)) {
        PrintClientRecord(Client);
        cout << "\nare you sure want to delete client\n";
        cin >> Answer;
        if (Answer == 'Y' || Answer == 'y') {
            MarkClientForDeleteByAccountNumber(AccountNumber, vContent);
            SaveClientToFile(FileName, vContent);
            vContent = LoadClientDataFromFile(FileName);
            cout << "\nClient deleted succesfully from client\n";
            return true;
        }
    }
    else {
        cout << "\nClient with account number (" << AccountNumber << ") isn't avaiable";
        return false;
    }
}
void FindDataFromFile(string AccountNumber) {
    stClient Client;
    vector<stClient> vContent;
    char Answer = 'n';
    system("cls");
    if (FindClientByAccountNumber(AccountNumber, vContent, Client)) {
        PrintClientRecord(Client);
        cout << "\nClient with account number is available\n";
    }
    else {
        cout << "\nClient with account number (" << AccountNumber << ") isn't avaiable";
    }
}
short EnterOperationNumber() {
    short number;
    cout << "Choose what do you want to do?[" << enMainMenue::ShowClientLists << " to " << enMainMenue::Exit << "]\n";
    do {
        cin >> number;
    } while (number < 1 && number>6);
    return (number);
}
void ShowDeleteClientScreen() {
    cout << "_______________________________________\n";
    cout << "Delete client screen\n";
    cout << "_______________________________________\n";
    vector <stClient> vClientRecords = LoadClientDataFromFile(FileName);
    string AccountNumber = ReadClientAccountNumber();
    DeleteRecordDataFromFile(AccountNumber, vClientRecords);
}
void ShowUpdateClientScreen() {
    cout << "_______________________________________\n";
    cout << "Update client screen\n";
    cout << "_______________________________________\n";
    vector <stClient> vClientRecords = LoadClientDataFromFile(FileName);
    string AccountNumber = ReadClientAccountNumber();
    UpdateRecordDataFromFile(AccountNumber, vClientRecords);
}
void ShowFindClientScreen() {
    cout << "_______________________________________\n";
    cout << "Find client screen\n";
    cout << "_______________________________________\n";
    vector <stClient> vClientRecords = LoadClientDataFromFile(FileName);
    string AccountNumber = ReadClientAccountNumber();
    FindDataFromFile(AccountNumber);
}
void EndProgram() {
    cout << "-----------------------\n";
    cout << "The program is end\n";
    cout << "-----------------------\n";
}
void GoBackToMaiMenue() {
    cout << "\n\n Press Any Key to go back to MainMenue...";
    system("pause>0");
    MainMenueScreen();
}
void PrintClientRecordLine(stClient &Client) {
    vector<stClient>vClientRecords;
    /*double addi = AddMoneyAmmount(vClientRecords, Client.AccountNumber);*/
    cout << "|" << left << setw(15) << Client.AccountNumber;
    cout << "|" << left << setw(10) << Client.PinCode;
    cout << "|" << left << setw(40) << Client.Name;
    cout << "|" << left << setw(12) << Client.Phone;
    cout << "|" << left << setw(12) << Client.AccountBalance <<endl;
}
void ShowClientsInformation(vector <stClient> vClientRecords) {
    vClientRecords = LoadClientDataFromFile(FileName);
    cout << "\t\t\t\t\tClient List (" << vClientRecords.size() << ") Clients(s).\n";
    cout << "_______________________________________________________________________________________________________________________________________________________\n";
    cout << "|" << left << setw(15) << "Account Number";
    cout << "|" << left << setw(10) << "Pin Code";
    cout << "|" << left << setw(40) << "Client Name";
    cout << "|" << left << setw(12) << "Phone";
    cout << "|" << left << setw(12) << "Balance";
    cout << "_______________________________________________________________________________________________________________________________________________________\n";
    string Data;
    for (stClient c : vClientRecords) {
        PrintClientRecordLine(c);
    }
    
    cout << "_______________________________________________________________________________________________________________________________________________________\n";
}
void ShowTotalBalancesClientsInformation(vector <stClient> vClientRecords) {
    vClientRecords = LoadClientDataFromFile(FileName);
    cout << "\t\t\t\t\tClient List (" << vClientRecords.size() << ") Clients(s).\n";
    cout << "_______________________________________________________________________________________________________________________________________________________\n";
    cout << "|" << left << setw(15) << "Account Number";
    cout << "|" << left << setw(10) << "Pin Code";
    cout << "|" << left << setw(40) << "Client Name";
    cout << "|" << left << setw(12) << "Phone";
    cout << "|" << left << setw(12) << "Balance";
    cout << "_______________________________________________________________________________________________________________________________________________________\n";
    string Data;
    double total = 0;
    for (stClient c : vClientRecords) {
        PrintClientRecordLine(c);
        total += c.AccountBalance;
    }
    cout << "_______________________________________________________________________________________________________________________________________________________\n";
    cout << total;
}
void ChoosingOperation(enMainMenue  Menue) {
    vector <stClient> vClientRecords;
    switch (Menue) {
    case enMainMenue::ShowClientLists:
        system("cls");
        ShowClientsInformation(vClientRecords);
        GoBackToMaiMenue();
        break;
    case enMainMenue::NewClient:
        system("cls");
        ShowAddNewClientsScreen();
        GoBackToMaiMenue();
        break;
    case enMainMenue::DeleteClient:
        system("cls");
        ShowDeleteClientScreen();
        GoBackToMaiMenue();
        break;
    case enMainMenue::UpdateClientInfo:
        system("cls");
        ShowUpdateClientScreen();
        GoBackToMaiMenue();
        break;
    case enMainMenue::FindClient:
        system("cls");
        ShowFindClientScreen();
        GoBackToMaiMenue();
        break;
    case enMainMenue::Transaction:
        system("cls");
        ShowTransactionsScreen();
        break;
    case enMainMenue::Exit:
        system("cls");
        EndProgram();
        GoBackToMaiMenue();
        break;
    }
}
vector <stClient> UpdateClientDeposit(string FileName, vector<stClient> &vClient) {
    fstream MyFile;
    stClient Client;
    MyFile.open(FileName, ios::in);
    string DataLine;
    if (MyFile.is_open()) {
                DataLine = ConvertRecordToLine(Client);
                MyFile << DataLine << endl;
    }
    MyFile.close();
    return vClient;
}
short OperationType(short transaction) {
    char arr[2] = { '+','-' };
    return transaction;
}
double AddMoneyAmmount(vector<stClient>& vClientRecords,string AccountNumber, stClient& Client,enTransAction transaction) {
    double Money = 0;
    short number = 0;
    PrintClientRecord(Client);
    cout << "\nEnter Amount of Money to Deposit/withdraw: ";
    cin >> Money;
    cout << transaction << endl;
    if (OperationType((transaction)) == (enTransAction::Deposit)) {
        Client.AccountBalance += Money;
    }
    else if (OperationType(transaction) == (enTransAction::Wthdraw)) {
        if (Client.AccountBalance >= Money) {
            Client.AccountBalance -= Money;
        }
        else {
            cout << Money << "is more than balance enter another money \n";
        }
    }
    fstream MyFile;
    MyFile.open(FileName, ios::out);
    if (MyFile.is_open()) {
        for (stClient& c : vClientRecords) {
            if (c.AccountNumber == Client.AccountNumber) {
                c.AccountBalance = Client.AccountBalance; // Update balance in vector
            }
            MyFile << ConvertRecordToLine(c) << endl;
        }
        MyFile.close();
    }
    return Client.AccountBalance;
}
void ShowDepositMenueScreen(enTransAction transaction) {
    cout << "_______________________________________\n";
    cout << "Deposit Menue Screen\n";
    cout << "_______________________________________\n";
    stClient Client;
    vector <stClient> vClientRecords = LoadClientDataFromFile(FileName);
    AddDepositOperation(vClientRecords,transaction);
    
}
void AddDepositOperation(vector<stClient>& vClient,enTransAction& transaction) {
    stClient Client;
    string AccountNumber;
    AccountNumber = ReadClientAccountNumber();
    if(FindClientByAccountNumber(AccountNumber,vClient,Client)){
        cout << "Account number[" << AccountNumber << "] is available\n";
        Client.AccountBalance=AddMoneyAmmount(vClient, AccountNumber,Client,transaction);
        SaveClientToFile(FileName, vClient);
    }
    else {
        cout << "Account number[" << AccountNumber << "] is not available\n";
    }  
}
void GoBackToTransActionMenue() {
    cout << "\n\n Press Any Key to go back to TranactionsMenue...";
    system("pause>0");
    ShowTransactionsScreen();
}
void ChoosingTransAction(enTransAction Transaction) {
    vector <stClient> vClientRecords;
    stClient Client;
    string AccountNumber;
    switch (Transaction) {
    case enTransAction::Deposit:
        system("cls");
        ShowDepositMenueScreen(enTransAction::Deposit);
        GoBackToTransActionMenue();
        break;
    case enTransAction::Wthdraw:
        system("cls");
        ShowDepositMenueScreen(enTransAction::Wthdraw);
        GoBackToTransActionMenue();
        break;
    case enTransAction::TotalBalances:
        system("cls");
        ShowTotalBalancesClientsInformation(vClientRecords);
        GoBackToTransActionMenue();
        break;
    case enTransAction::MainMenue:
        system("cls");
        GoBackToMaiMenue();
        break;
    }  
}
short EnterTransActionNumber() {
    short number;
    cout << "Choose what do you want to do?[" << enTransAction::Deposit << " to " << enTransAction::MainMenue << "]\n";
    do {
        cin >> number;
    } while (number < 1 && number>4);
    return (number);
}

void ShowTransactionsScreen() {
    system("cls");
    vector<stClient>vClient = LoadClientDataFromFile(FileName);
    cout << "============================================\n";
    cout << left << "Show transactions screen\n";
    cout << "============================================\n";
    cout << left << "[" << enTransAction::Deposit << "] Deposits." << endl;
    cout << left << "[" << enTransAction::Wthdraw << "] Wthdraw." << endl;
    cout << left << "[" << enTransAction::TotalBalances << "] Total Balances." << endl;
    cout << left << "[" << enTransAction::MainMenue << "] Main Menue." << endl;
    cout << "============================================\n";
    ChoosingTransAction(enTransAction (EnterTransActionNumber()));

}


void MainMenueScreen() {
    system("cls");
    cout << "============================================\n";
    cout << left << "Main Menue Screen"  << endl;
    cout << "============================================\n";
    cout << left << "[" << enMainMenue::ShowClientLists << "] Show Client List."  << endl;
    cout << left << "[" << enMainMenue::NewClient << "] Add New Client."  << endl;
    cout << left << "[" << enMainMenue::DeleteClient << "] Delete Client."  << endl;
    cout << left << "[" << enMainMenue::UpdateClientInfo << "] Update Client Information."  << endl;
    cout << left << "[" << enMainMenue::FindClient << "] Find Client." << endl;
    cout << left << "[" << enMainMenue::Transaction << "] Transactions." << endl;
    cout << left << "[" << enMainMenue::Exit << "] Exit."  << endl;
    cout << "============================================\n";
    ChoosingOperation((enMainMenue)EnterOperationNumber());
}
int main()
{   
    MainMenueScreen(); 
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
