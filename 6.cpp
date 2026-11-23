#include <iostream>
#include <string>
#include <limits>
#include <stdexcept>
#include <cctype>

using namespace std;

const int MAX_ITEMS = 100;

class LibraryItem {
private:
    string title;
    string author;
    string dueDate;

public:
    LibraryItem(string itemTitle = "", string itemAuthor = "", string itemDueDate = "Not Checked Out")
        : title(itemTitle), author(itemAuthor), dueDate(itemDueDate) {}

    virtual ~LibraryItem() {}

    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    string getDueDate() const { return dueDate; }

    void setTitle(string newTitle) {
        if (newTitle.empty()) {
            throw invalid_argument("Title cannot be empty.");
        }
        title = newTitle;
    }

    void setAuthor(string newAuthor) {
        if (newAuthor.empty()) {
            throw invalid_argument("Author cannot be empty.");
        }
        author = newAuthor;
    }

    void setDueDate(string newDueDate) {
        dueDate = newDueDate;
    }

    virtual void checkOut() = 0;
    virtual void returnItem() = 0;
    virtual void displayDetails() const = 0;
};

class Book : public LibraryItem {
private:
    string isbn;
    int quantity;

public:
    Book(string title, string author, string bookIsbn, int bookQuantity)
        : LibraryItem(title, author, "Not Checked Out") {
        setIsbn(bookIsbn);
        setQuantity(bookQuantity);
    }

    void setIsbn(string newIsbn) {
        if (newIsbn.empty()) {
            throw invalid_argument("Invalid ISBN format: ISBN cannot be empty.");
        }
        int digitCount = 0;
        for (char ch : newIsbn) {
            if (isdigit(static_cast<unsigned char>(ch))) {
                digitCount++;
            } else if (ch != '-') {
                throw invalid_argument("Invalid ISBN format: only digits and hyphens are allowed.");
            }
        }
        if (digitCount != 10 && digitCount != 13) {
            throw invalid_argument("Invalid ISBN format: ISBN must contain 10 or 13 digits.");
        }
        isbn = newIsbn;
    }

    string getIsbn() const { return isbn; }

    void setQuantity(int newQuantity) {
        if (newQuantity < 0) {
            throw invalid_argument("Book quantity cannot be negative.");
        }
        quantity = newQuantity;
    }

    int getQuantity() const { return quantity; }

    void checkOut() override {
        if (quantity <= 0) {
            throw runtime_error("Book is currently out of stock.");
        }
        quantity--;
        setDueDate("2026-12-04");
        cout << "Book '" << getTitle() << "' checked out successfully. Remaining copies: " << quantity << endl;
    }

    void returnItem() override {
        quantity++;
        setDueDate("Not Checked Out");
        cout << "Book '" << getTitle() << "' returned successfully. Current copies: " << quantity << endl;
    }

    void displayDetails() const override {
        cout << "[Book] Title: " << getTitle()
             << " | Author: " << getAuthor()
             << " | ISBN: " << isbn
             << " | Quantity: " << quantity
             << " | Due Date: " << getDueDate() << endl;
    }
};

class DVD : public LibraryItem {
private:
    int duration; 
    bool isCheckedOut;

public:
    DVD(string title, string directorOrAuthor, int dvdDuration)
        : LibraryItem(title, directorOrAuthor, "Not Checked Out"), isCheckedOut(false) {
        setDuration(dvdDuration);
    }

    void setDuration(int newDuration) {
        if (newDuration <= 0) {
            throw invalid_argument("DVD duration must be a positive number of minutes.");
        }
        duration = newDuration;
    }

    int getDuration() const { return duration; }

    void checkOut() override {
        if (isCheckedOut) {
            throw runtime_error("DVD is already checked out.");
        }
        isCheckedOut = true;
        setDueDate("2026-11-27");
        cout << "DVD '" << getTitle() << "' checked out successfully." << endl;
    }

    void returnItem() override {
        if (!isCheckedOut) {
            throw runtime_error("DVD was not checked out.");
        }
        isCheckedOut = false;
        setDueDate("Not Checked Out");
        cout << "DVD '" << getTitle() << "' returned successfully." << endl;
    }

    void displayDetails() const override {
        cout << "[DVD] Title: " << getTitle()
             << " | Creator: " << getAuthor()
             << " | Duration: " << duration << " mins"
             << " | Status: " << (isCheckedOut ? "Checked Out" : "Available")
             << " | Due Date: " << getDueDate() << endl;
    }
};

class Magazine : public LibraryItem {
private:
    int issueNumber;
    bool isCheckedOut;

public:
    Magazine(string title, string publisherOrAuthor, int issueNum)
        : LibraryItem(title, publisherOrAuthor, "Not Checked Out"), isCheckedOut(false) {
        setIssueNumber(issueNum);
    }

    void setIssueNumber(int newIssue) {
        if (newIssue <= 0) {
            throw invalid_argument("Issue number must be positive.");
        }
        issueNumber = newIssue;
    }

    int getIssueNumber() const { return issueNumber; }

    void checkOut() override {
        if (isCheckedOut) {
            throw runtime_error("Magazine is already checked out.");
        }
        isCheckedOut = true;
        setDueDate("2026-11-25");
        cout << "Magazine '" << getTitle() << "' (Issue #" << issueNumber << ") checked out." << endl;
    }

    void returnItem() override {
        if (!isCheckedOut) {
            throw runtime_error("Magazine was not checked out.");
        }
        isCheckedOut = false;
        setDueDate("Not Checked Out");
        cout << "Magazine '" << getTitle() << "' returned successfully." << endl;
    }

    void displayDetails() const override {
        cout << "[Magazine] Title: " << getTitle()
             << " | Author/Publisher: " << getAuthor()
             << " | Issue #: " << issueNumber
             << " | Status: " << (isCheckedOut ? "Checked Out" : "Available")
             << " | Due Date: " << getDueDate() << endl;
    }
};

int readInt(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid numeric input. Please try again.\n";
    }
}

string readLine(const string& prompt) {
    string input;
    cout << prompt;
    getline(cin, input);
    return input;
}

void displayAllItems(LibraryItem* items[], int count) {
    if (count == 0) {
        cout << "\nNo items in the catalog.\n";
        return;
    }
    cout << "\n--- Library Catalog (" << count << " items) ---\n";
    for (int i = 0; i < count; ++i) {
        cout << (i + 1) << ". ";
        items[i]->displayDetails();
    }
}

void searchItems(LibraryItem* items[], int count) {
    if (count == 0) {
        cout << "\nCatalog is empty.\n";
        return;
    }
    string query = readLine("\nEnter title or author to search: ");
    bool found = false;
    for (int i = 0; i < count; ++i) {
        if (items[i]->getTitle().find(query) != string::npos ||
            items[i]->getAuthor().find(query) != string::npos) {
            cout << (i + 1) << ". ";
            items[i]->displayDetails();
            found = true;
        }
    }
    if (!found) {
        cout << "No matching items found for '" << query << "'.\n";
    }
}

void deleteItem(LibraryItem* items[], int& count) {
    if (count == 0) {
        cout << "\nNo items to remove.\n";
        return;
    }
    displayAllItems(items, count);
    int index = readInt("Enter item number to remove: ") - 1;
    if (index < 0 || index >= count) {
        throw out_of_range("Selected item number is out of range.");
    }

    delete items[index];
    for (int i = index; i < count - 1; ++i) {
        items[i] = items[i + 1];
    }
    items[count - 1] = nullptr;
    count--;
    cout << "Item removed and memory released.\n";
}

int main() {
    LibraryItem* libraryItems[MAX_ITEMS] = {nullptr};
    int itemCount = 0;

    try {
        libraryItems[itemCount++] = new Book("Clean Code", "Robert C. Martin", "978-0132350884", 3);
        libraryItems[itemCount++] = new DVD("Inception", "Christopher Nolan", 148);
        libraryItems[itemCount++] = new Magazine("National Geographic", "NatGeo Society", 204);
    } catch (const exception& e) {
        cout << "Initialization error: " << e.what() << endl;
    }

    int choice = 0;
    do {
        cout << "   Library Management System  \n";
        cout << "1. Add Book\n";
        cout << "2. Add DVD\n";
        cout << "3. Add Magazine\n";
        cout << "4. Display All Items\n";
        cout << "5. Search Item\n";
        cout << "6. Check Out Item\n";
        cout << "7. Return Item\n";
        cout << "8. Delete Item\n";
        cout << "9. Exit\n";

        choice = readInt("Enter your choice: ");

        try {
            switch (choice) {
                case 1: {
                    if (itemCount >= MAX_ITEMS) {
                        throw overflow_error("Catalog is full (MAX_ITEMS reached).");
                    }
                    string title = readLine("Enter Book Title: ");
                    string author = readLine("Enter Author: ");
                    string isbn = readLine("Enter ISBN (10 or 13 digits): ");
                    int qty = readInt("Enter Quantity: ");

                    libraryItems[itemCount] = new Book(title, author, isbn, qty);
                    itemCount++;
                    cout << "Book added successfully.\n";
                    break;
                }
                case 2: {
                    if (itemCount >= MAX_ITEMS) {
                        throw overflow_error("Catalog is full (MAX_ITEMS reached).");
                    }
                    string title = readLine("Enter DVD Title: ");
                    string director = readLine("Enter Director/Author: ");
                    int duration = readInt("Enter Duration (minutes): ");

                    libraryItems[itemCount] = new DVD(title, director, duration);
                    itemCount++;
                    cout << "DVD added successfully.\n";
                    break;
                }
                case 3: {
                    if (itemCount >= MAX_ITEMS) {
                        throw overflow_error("Catalog is full (MAX_ITEMS reached).");
                    }
                    string title = readLine("Enter Magazine Title: ");
                    string author = readLine("Enter Author/Publisher: ");
                    int issue = readInt("Enter Issue Number: ");

                    libraryItems[itemCount] = new Magazine(title, author, issue);
                    itemCount++;
                    cout << "Magazine added successfully.\n";
                    break;
                }
                case 4:
                    displayAllItems(libraryItems, itemCount);
                    break;
                case 5:
                    searchItems(libraryItems, itemCount);
                    break;
                case 6: {
                    displayAllItems(libraryItems, itemCount);
                    if (itemCount > 0) {
                        int idx = readInt("Enter item number to check out: ") - 1;
                        if (idx < 0 || idx >= itemCount) {
                            throw out_of_range("Invalid item selection.");
                        }
                        libraryItems[idx]->checkOut();
                    }
                    break;
                }
                case 7: {
                    displayAllItems(libraryItems, itemCount);
                    if (itemCount > 0) {
                        int idx = readInt("Enter item number to return: ") - 1;
                        if (idx < 0 || idx >= itemCount) {
                            throw out_of_range("Invalid item selection.");
                        }
                        libraryItems[idx]->returnItem();
                    }
                    break;
                }
                case 8:
                    deleteItem(libraryItems, itemCount);
                    break;
                case 9:
                    cout << "Exiting Library Management System...\n";
                    break;
                default:
                    cout << "Invalid choice. Please select between 1 and 9.\n";
                    break;
            }
        } catch (const invalid_argument& e) {
            cout << "[Input Error] " << e.what() << endl;
        } catch (const exception& e) {
            cout << "[Error] " << e.what() << endl;
        }

    } while (choice != 9);

    for (int i = 0; i < itemCount; ++i) {
        delete libraryItems[i];
        libraryItems[i] = nullptr;
    }

    return 0;
}