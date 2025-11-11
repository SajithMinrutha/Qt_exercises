#include <QCoreApplication>
#include <QFile>
#include <QDataStream>
#include <QMap>
#include <QString>
#include <QTextStream>

class AddressBook {
private:
    QMap<QString, QString> contacts;
    QTextStream in;
    QTextStream out;

public:
    AddressBook() : in(stdin), out(stdout) {
        load_file();
        menu();
        save_file();
    }

    void load_file() {
        QFile file("addressbook.dat");
        if (file.open(QIODevice::ReadOnly)) {
            QDataStream inFile(&file);
            inFile.setVersion(QDataStream::Qt_6_0);

            int size;
            inFile >> size;
            for (int i = 0; i < size; ++i) {
                QString name, number;
                inFile >> name >> number;
                contacts.insert(name, number);
            }
            file.close();
        }
    }

    void save_file() {
        QFile file("addressbook.dat");
        if (file.open(QIODevice::WriteOnly)) {
            QDataStream outFile(&file);
            outFile.setVersion(QDataStream::Qt_6_0);

            outFile << contacts.size();
            for (auto it = contacts.begin(); it != contacts.end(); ++it)
                outFile << it.key() << it.value();

            file.close();
        }
    }

    void save_contact() {
        out << "\nEnter the name: " << Qt::flush;
        QString name = in.readLine().trimmed();

        out << "Enter the phone number: " << Qt::flush;
        QString number = in.readLine().trimmed();

        if (name.isEmpty() || number.isEmpty()) {
            out << " Invalid input.\n";
            return;
        }

        contacts.insert(name, number);
        out << "Contact added successfully!\n";
    }

    void remove_contact() {
        out << "\nEnter the name of the contact to remove: " << Qt::flush;
        QString name = in.readLine().trimmed();

        if (name.isEmpty()) {
            out << " Invalid input.\n";
            return;
        }

        if (contacts.remove(name) > 0)
            out << " Contact removed successfully!\n";
        else
            out << " Contact not found.\n";
    }

    void list_contacts() {
        if (contacts.isEmpty()) {
            out << "\n No contacts available.\n";
            return;
        }

        out << "\n Contact List:\n";
        out << "-----------------------------\n";
        for (auto it = contacts.begin(); it != contacts.end(); ++it)
            out << it.key() << ": " << it.value() << "\n";
        out << "-----------------------------\n";
    }

    void menu() {
        while (true) {
            out << "\n=============================\n";
            out << "    My Address Book Menu   \n";
            out << "=============================\n";
            out << "1. Add a contact\n";
            out << "2. Remove a contact\n";
            out << "3. List all contacts\n";
            out << "Type 'exit' to quit\n";
            out << "=============================\n";
            out << "Enter your choice: " << Qt::flush;

            QString choice = in.readLine().trimmed();

            if (choice == "1") save_contact();
            else if (choice == "2") remove_contact();
            else if (choice == "3") list_contacts();
            else if (choice.toLower() == "exit") {
                out << "\n Saving and exiting... Goodbye!\n";
                break;
            } else {
                out << " Invalid choice. Try again.\n";
            }
        }
    }
};

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);
    AddressBook addressBook;
    return 0;
}
