#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Constants
#define MAX_BOOKS 100
#define MAX_MEMBERS 50

// Book structure
typedef struct {
    int book_id;
    char title[100];
    char author[100];
    int is_available;  // 1 if available, 0 if not
} Book;

// Member structure
typedef struct {
    int member_id;
    char name[100];
    char contact[100];
    int books_issued[MAX_BOOKS];  // array to store IDs of issued books
    int num_books_issued;         // count of issued books
} Member;

// Global variables
Book books[MAX_BOOKS];
int num_books = 0;

Member members[MAX_MEMBERS];
int num_members = 0;

// Function prototypes
void addBook();
void addMember();
void borrowBook();
void returnBook();
void displayBooks();
void displayMembers();
void saveData();
void loadData();
void generateReport();
void clearScreen();

// Main function
int main() {
    int choice;

    // Load data from file (if any)
    loadData();

    do {
        // Display menu
        printf("\n===== Library Management System Menu =====\n");
        printf("1. Add Book\n");
        printf("2. Add Member\n");
        printf("3. Borrow Book\n");
        printf("4. Return Book\n");
        printf("5. Display All Books\n");
        printf("6. Display All Members\n");
        printf("7. Generate Report\n");
        printf("8. Save Data\n");
        printf("9. Exit\n");
        printf("=========================================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearScreen();

        // Process user choice
        switch(choice) {
            case 1:
                addBook();
                break;
            case 2:
                addMember();
                break;
            case 3:
                borrowBook();
                break;
            case 4:
                returnBook();
                break;
            case 5:
                displayBooks();
                break;
            case 6:
                displayMembers();
                break;
            case 7:
                generateReport();
                break;
            case 8:
                saveData();
                break;
            case 9:
                printf("Exiting program...\n");
                break;
            default:
                printf("Invalid choice. Please enter a number between 1 and 9.\n");
        }
    } while(choice != 9);

    return 0;
}

// Function to add a new book to the library
void addBook() {
    if (num_books == MAX_BOOKS) {
        printf("Cannot add more books. Library is full.\n");
        return;
    }

    printf("Enter book title: ");
    getchar(); // Clear input buffer
    fgets(books[num_books].title, sizeof(books[num_books].title), stdin);
    books[num_books].title[strcspn(books[num_books].title, "\n")] = '\0'; // Remove newline character

    printf("Enter author name: ");
    fgets(books[num_books].author, sizeof(books[num_books].author), stdin);
    books[num_books].author[strcspn(books[num_books].author, "\n")] = '\0'; // Remove newline character

    books[num_books].book_id = num_books + 1; // Assign book ID
    books[num_books].is_available = 1; // Mark book as available

    num_books++; // Increment number of books
    printf("Book added successfully with ID %d.\n", books[num_books - 1].book_id);
}

// Function to add a new member to the library
void addMember() {
    if (num_members == MAX_MEMBERS) {
        printf("Cannot add more members. Member limit reached.\n");
        return;
    }

    printf("Enter member name: ");
    getchar(); // Clear input buffer
    fgets(members[num_members].name, sizeof(members[num_members].name), stdin);
    members[num_members].name[strcspn(members[num_members].name, "\n")] = '\0'; // Remove newline character

    printf("Enter contact details: ");
    fgets(members[num_members].contact, sizeof(members[num_members].contact), stdin);
    members[num_members].contact[strcspn(members[num_members].contact, "\n")] = '\0'; // Remove newline character

    members[num_members].member_id = num_members + 1; // Assign member ID
    members[num_members].num_books_issued = 0; // Initialize number of books issued to zero

    num_members++; // Increment number of members
    printf("Member added successfully with ID %d.\n", members[num_members - 1].member_id);
}

// Function to borrow a book from the library
void borrowBook() {
    int book_id, member_id;

    printf("Enter book ID to borrow: ");
    scanf("%d", &book_id);

    if (book_id < 1 || book_id > num_books) {
        printf("Invalid book ID.\n");
        return;
    }

    printf("Enter member ID: ");
    scanf("%d", &member_id);

    if (member_id < 1 || member_id > num_members) {
        printf("Invalid member ID.\n");
        return;
    }

    if (books[book_id - 1].is_available == 0) {
        printf("Book is not available for borrowing.\n");
        return;
    }

    // Add book to member's list of issued books
    members[member_id - 1].books_issued[members[member_id - 1].num_books_issued] = book_id;
    members[member_id - 1].num_books_issued++;

    // Mark book as unavailable
    books[book_id - 1].is_available = 0;

    printf("Book '%s' borrowed successfully by %s.\n", books[book_id - 1].title, members[member_id - 1].name);
}

// Function to return a borrowed book to the library
void returnBook() {
    int book_id, member_id, i, found = 0;

    printf("Enter book ID to return: ");
    scanf("%d", &book_id);

    if (book_id < 1 || book_id > num_books) {
        printf("Invalid book ID.\n");
        return;
    }

    printf("Enter member ID: ");
    scanf("%d", &member_id);

    if (member_id < 1 || member_id > num_members) {
        printf("Invalid member ID.\n");
        return;
    }

    // Check if member has borrowed the book
    for (i = 0; i < members[member_id - 1].num_books_issued; i++) {
        if (members[member_id - 1].books_issued[i] == book_id) {
            // Remove book from member's list of issued books
            members[member_id - 1].books_issued[i] = members[member_id - 1].books_issued[members[member_id - 1].num_books_issued - 1];
            members[member_id - 1].num_books_issued--;

            // Mark book as available
            books[book_id - 1].is_available = 1;

            printf("Book '%s' returned successfully by %s.\n", books[book_id - 1].title, members[member_id - 1].name);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Member %s has not borrowed book with ID %d.\n", members[member_id - 1].name, book_id);
    }
}

// Function to display all books in the library
void displayBooks() {
    int i;

    if (num_books == 0) {
        printf("No books in the library.\n");
        return;
    }

    printf("\nList of Books:\n");
    printf("--------------------------------------------------\n");
    printf("ID\tTitle\t\t\tAuthor\t\tAvailable\n");
    printf("--------------------------------------------------\n");

    for (i = 0; i < num_books; i++) {
        printf("%d\t%s\t\t%s\t\t%s\n", books[i].book_id, books[i].title, books[i].author, (books[i].is_available == 1) ? "Yes" : "No");
    }

    printf("--------------------------------------------------\n");
}

// Function to display all members of the library
void displayMembers() {
    int i, j;

    if (num_members == 0) {
        printf("No members in the library.\n");
        return;
    }

    printf("\nList of Members:\n");
    printf("--------------------------------------------------\n");
    printf("ID\tName\t\tContact\t\tBooks Issued\n");
    printf("--------------------------------------------------\n");

    for (i = 0; i < num_members; i++) {
        printf("%d\t%s\t\t%s\t\t", members[i].member_id, members[i].name, members[i].contact);

        if (members[i].num_books_issued == 0) {
            printf("None\n");
        } else {
            for (j = 0; j < members[i].num_books_issued; j++) {
                printf("%d ", members[i].books_issued[j]);
            }
            printf("\n");
        }
    }

    printf("--------------------------------------------------\n");
}

// Function to generate and display a report
void generateReport() {
    int i, total_books_issued = 0;
    float avg_books_per_member;

    if (num_members == 0 || num_books == 0) {
        printf("No books or members to generate report.\n");
        return;
    }

    // Calculate total books issued
    for (i = 0; i < num_members; i++) {
        total_books_issued += members[i].num_books_issued;
    }

    // Calculate average books per member
    avg_books_per_member = (float) total_books_issued / num_members;

    printf("\nLibrary Report:\n");
    printf("--------------------------------------------------\n");
    printf("Total Books: %d\n", num_books);
    printf("Total Members: %d\n", num_members);
    printf("Total Books Issued: %d\n", total_books_issued);
    printf("Average Books per Member: %.2f\n", avg_books_per_member);
    printf("--------------------------------------------------\n");
}

// Function to save library data to a file
void saveData() {
    FILE *fp;
    int i, j;

    fp = fopen("library_data.txt", "w");
    if (fp == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    // Save books data
    fprintf(fp, "%d\n", num_books);
    for (i = 0; i < num_books; i++) {
        fprintf(fp, "%d\n%s\n%s\n%d\n", books[i].book_id, books[i].title, books[i].author, books[i].is_available);
    }

    // Save members data
    fprintf(fp, "%d\n", num_members);
    for (i = 0; i < num_members; i++) {
        fprintf(fp, "%d\n%s\n%s\n%d\n", members[i].member_id, members[i].name, members[i].contact, members[i].num_books_issued);
        for (j = 0; j < members[i].num_books_issued; j++) {
            fprintf(fp, "%d ", members[i].books_issued[j]);
        }
        fprintf(fp, "\n");
    }

    fclose(fp);
    printf("Data saved successfully.\n");
}

// Function to load library data from a file
void loadData() {
    FILE *fp;
    int i, j, num_books_loaded, num_members_loaded;

    fp = fopen("library_data.txt", "r");
    if (fp == NULL) {
        printf("No previous data found.\n");
        return;
    }

    // Load books data
    fscanf(fp, "%d", &num_books_loaded);
    for (i = 0; i < num_books_loaded; i++) {
        fscanf(fp, "%d", &books[i].book_id);
        getchar(); // Clear newline character
        fgets(books[i].title, sizeof(books[i].title), fp);
        books[i].title[strcspn(books[i].title, "\n")] = '\0'; // Remove newline character
        fgets(books[i].author, sizeof(books[i].author), fp);
        books[i].author[strcspn(books[i].author, "\n")] = '\0'; // Remove newline character
        fscanf(fp, "%d", &books[i].is_available);
    }
    num_books = num_books_loaded;

    // Load members data
    fscanf(fp, "%d", &num_members_loaded);
    for (i = 0; i < num_members_loaded; i++) {
        fscanf(fp, "%d", &members[i].member_id);
        getchar(); // Clear newline character
        fgets(members[i].name, sizeof(members[i].name), fp);
        members[i].name[strcspn(members[i].name, "\n")] = '\0'; // Remove newline character
        fgets(members[i].contact, sizeof(members[i].contact), fp);
        members[i].contact[strcspn(members[i].contact, "\n")] = '\0'; // Remove newline character
        fscanf(fp, "%d", &members[i].num_books_issued);
        for (j = 0; j < members[i].num_books_issued; j++) {
            fscanf(fp, "%d", &members[i].books_issued[j]);
        }
    }
    num_members = num_members_loaded;

    fclose(fp);
    printf("Data loaded successfully.\n");
}

// Function to clear the screen (for better UI experience)
void clearScreen() {
    printf("\033[H\033[J");
}
