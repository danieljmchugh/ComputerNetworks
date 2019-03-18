#include <iostream>
#include <vector>
#include <string>

class Client {
public:
    // CONSTRUCTOR
    Client(std::string name, int handle, bool loggedIn) 
        : name(name), socketHandle(handle), login(loggedIn)
    {}

    // GETTERS
    inline std::string getName() {
        return this->name;
    }
    inline int getHandle() {
        return this->socketHandle;
    }

    // SETTERS
    inline void setName(std::string name) {
        this->name = name;
    }
    inline void setHandle(int socketHandle) {
        this->socketHandle = socketHandle;
    }


    // isLoggedIn returns login truth value
    inline bool isLoggedIn() {
        return this->login;
    }

    // isLoggedIn with a parameter sets login value
    inline void setLoggedIn(bool value) {
        this->login = value;
    }

private:
    int socketHandle;
    std::string name;
    bool login;
};

class Clients {
public:
    // ADD CLIENT TO DATABASE
    inline void add(Client CurrentClient) {
        ClientDb.push_back(CurrentClient);
    }

    // REMOVE CLIENT FROM DATABASE
    // return 0 - success, return 1 - client not found
    inline int remove(std::string username) {
        for (int i = ClientDb.size(); i-- >0;) {
            if (ClientDb.at(i).getName() == username) {
                ClientDb.at(i).setLoggedIn(false);
                break;
            }
        }
        return 0;
    }

    // RESPONSE TO A WHO\n MESSAGE
    inline std::string who() {
        std::string message = "WHO-OK ";

        for (int i = 0; i < ClientDb.size() - 1; ++i) {
            if (ClientDb.at(i).isLoggedIn())
                message += ClientDb.at(i).getName() + ", ";
        }
        if (ClientDb.at(ClientDb.size() - 1).isLoggedIn())
            message += ClientDb.at(ClientDb.size() - 1).getName() + '\n';
        else {
            message = message.substr(0, message.length() - 2);
            message += '\n';
        }

        return message;
    }

    // PRINT EVERYONE
    inline void print() {
        std::cout << "handle\tname\tisLoggedIn\n";
        for (int i = 0; i < ClientDb.size(); ++i) {
            std::cout << ClientDb.at(i).getHandle() << "\t" << ClientDb.at(i).getName() << "\t" << ClientDb.at(i).isLoggedIn() << std::endl;
        }
    }

    // FIND USER HANDLE BY OTHER USER'S MSG
    // return handle if found, -1 if not
    inline int find(std::string buffer) {
        std::string username = "";
        int handle = 0;
        int pos = 0;

        for (int i = 0; i < buffer.length(); ++i) {
            if (buffer.at(i) == ' ') {
                pos = i + 1;
                break;
            }
        }

        for (int i = pos; i < buffer.length(); ++i) {
            if (buffer.at(i) == ' ')
                break;
            else
                username += buffer.at(i);
        }


        for (int i = 0; i < ClientDb.size(); ++i) {
            if (ClientDb.at(i).getName() == username)
                return ClientDb.at(i).getHandle();
        }

        return -1;
    }

    // EXTRACT MESSAGE FROM BUFFER
    inline std::string extractMessage(std::string buffer) {
        std::string message = "";
        int pos = 0;
        int counter = 0;

        for (int i = 0; i < buffer.length(); ++i) {
            if (buffer.at(i) == ' ') {
                counter++;
                pos = i + 1;

                if (counter == 2)
                    break;
            }
        }

        for (int i = pos; i < buffer.length(); ++i) {
            message += buffer.at(i);
        }

        return message;
    }

    // EXTRACT USERNAME FROM BUFFER
    inline std::string extractUsername(std::string buffer) {
        std::string username = "";
        int pos = 0;
        for (int i = 0; i < buffer.length(); ++i) {
            if (buffer.at(i) == ' ') {
                pos = i + 1;
                break;
            }
        }
        for (int i = pos; i < buffer.length(); ++i) {
            if (buffer.at(i) == ' ')
                break;
            else
                username += buffer.at(i);
        }

        return username;
    }

    // CHECK IF USERNAME IS TAKEN
    inline bool checkIfTaken(std::string username) {
        for (int i = 0; i < ClientDb.size(); ++i) {
            if ((username == ClientDb.at(i).getName()) && (ClientDb.at(i).isLoggedIn() == 1)) {
                return 1;
            }
        }

        return 0;
    }

private:
    std::vector<Client> ClientDb;
};