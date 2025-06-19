#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <conio.h>
#endif

struct User
{
    int id;
    char name[50];
    char contact[20];
};

struct Request
{
    int id;
    int userId;
    char userComment[100];
    char address[100];
    int status; // 0 = Pending, 1 = In Progress, 2 = Rejected.
    char adminComment[100];
};

#define USER_FILE "users.dat"
#define REQ_FILE "requests.dat"

// Wait for user to press a key (press Enter on Unix).
void waitKey()
{
#ifdef _WIN32
    getch();
#else
    getchar();
#endif
}

// Count records in a binary file by size.
int countRecords(const char *filename, int recordSize)
{
    FILE *fp = fopen(filename, "rb");
    if (!fp)
        return 0;
    fseek(fp, 0, SEEK_END);
    long fileSize = ftell(fp);
    fclose(fp);
    return (int)(fileSize / recordSize);
}

// Print a message box with up to three lines.
void printBox(const char *line1, const char *line2, const char *line3)
{
    int len1 = line1 ? strlen(line1) : 0;
    int len2 = line2 ? strlen(line2) : 0;
    int len3 = line3 ? strlen(line3) : 0;
    int maxlen = len1;
    if (len2 > maxlen)
        maxlen = len2;
    if (len3 > maxlen)
        maxlen = len3;
    int innerWidth = maxlen + 2;
    int totalWidth = innerWidth + 2;
    for (int i = 0; i < totalWidth; i++)
        printf("*");
    printf("\n");
    if (line1)
    {
        printf("* %s", line1);
        for (int i = 0; i < innerWidth - 1 - len1; i++)
            printf(" ");
        printf("*\n");
    }
    if (line2)
    {
        printf("* %s", line2);
        for (int i = 0; i < innerWidth - 1 - len2; i++)
            printf(" ");
        printf("*\n");
    }
    if (line3)
    {
        printf("* %s", line3);
        for (int i = 0; i < innerWidth - 1 - len3; i++)
            printf(" ");
        printf("*\n");
    }
    for (int i = 0; i < totalWidth; i++)
        printf("*");
    printf("\n");
}

// Add a new user and write to the file.
void addNewUser()
{
    FILE *fp = fopen(USER_FILE, "ab");
    if (!fp)
    {
        printf("Error opening user file.\n");
        return;
    }
    struct User u;
    u.id = countRecords(USER_FILE, sizeof(struct User)) + 1;
    printf("\nEnter details of user (All fields are required)\n");
    printf("Enter user name: ");
    getchar(); // clear newline from previous input.
    fgets(u.name, sizeof(u.name), stdin);
    u.name[strcspn(u.name, "\n")] = 0;
    printf("Enter user contact: ");
    fgets(u.contact, sizeof(u.contact), stdin);
    u.contact[strcspn(u.contact, "\n")] = 0;
    fwrite(&u, sizeof(struct User), 1, fp);
    fclose(fp);
    char buf1[50], buf2[50];
    sprintf(buf1, "User added successfully");
    sprintf(buf2, "User Id: %d", u.id);
    printBox(buf1, buf2, "Press any key to continue");
    waitKey();
}

// View a user and any associated request status.
void viewUser()
{
    printf("\nEnter user id to view: ");
    int id;
    scanf("%d", &id);
    getchar(); // flush newline.
    FILE *fp = fopen(USER_FILE, "rb");
    if (!fp)
    {
        printf("No users found.\n");
        waitKey();
        return;
    }
    struct User u;
    int found = 0;
    while (fread(&u, sizeof(struct User), 1, fp) == 1)
    {
        if (u.id == id)
        {
            found = 1;
            break;
        }
    }
    fclose(fp);
    if (!found)
    {
        printf("User with id %d not found.\n", id);
        waitKey();
        return;
    }
    printf("\nUser Details\n");
    printf("User Id: %d\n", u.id);
    printf("User Name: %s\n", u.name);
    printf("User Contact: %s\n", u.contact);

    // Show associated request, if any.
    FILE *rp = fopen(REQ_FILE, "rb");
    if (rp)
    {
        struct Request r;
        int reqFound = 0;
        while (fread(&r, sizeof(struct Request), 1, rp) == 1)
        {
            if (r.userId == u.id)
            {
                if (!reqFound)
                {
                    printf("\nAssociated Request:\n");
                    reqFound = 1;
                }
                printf("Request Id: %d\n", r.id);
                const char *status = (r.status == 1) ? "In Progress" : (r.status == 2) ? "Rejected"
                                                                                       : "Pending";
                printf("Request Status: %s\n", status);
                printf("Comments: %s\n", (r.adminComment[0] ? r.adminComment : r.userComment));
                printf("Address: %s\n", r.address);
            }
        }
        fclose(rp);
        if (!reqFound)
        {
            printf("\nNo connection requests for this user.\n");
        }
    }
    printf("\nPress any key to return to main menu");
    waitKey();
}

// Add a new connection request.
void addRequest()
{
    FILE *fp = fopen(REQ_FILE, "ab");
    if (!fp)
    {
        printf("Error opening request file.\n");
        return;
    }
    struct Request r;
    r.id = countRecords(REQ_FILE, sizeof(struct Request)) + 1;
    printf("\nEnter details of Request made by user:\n");
    printf("Enter user id: ");
    scanf("%d", &r.userId);

    // Verify user exists.
    FILE *uf = fopen(USER_FILE, "rb");
    int userExists = 0;
    if (uf)
    {
        struct User u2;
        while (fread(&u2, sizeof(struct User), 1, uf) == 1)
        {
            if (u2.id == r.userId)
            {
                userExists = 1;
                break;
            }
        }
        fclose(uf);
    }
    if (!userExists)
    {
        printf("User id %d does not exist.\n", r.userId);
        fclose(fp);
        waitKey();
        return;
    }

    getchar(); // flush newline.
    printf("Enter any comment (optional): ");
    fgets(r.userComment, sizeof(r.userComment), stdin);
    r.userComment[strcspn(r.userComment, "\n")] = 0;
    printf("Enter address: ");
    fgets(r.address, sizeof(r.address), stdin);
    r.address[strcspn(r.address, "\n")] = 0;
    r.status = 0; // Pending.
    r.adminComment[0] = 0;
    fwrite(&r, sizeof(struct Request), 1, fp);
    fclose(fp);
    char buf1[50], buf2[50];
    sprintf(buf1, "Request added successfully");
    sprintf(buf2, "Request Id: %d", r.id);
    printBox(buf1, buf2, "Press any key to continue");
    waitKey();
}

// Edit an existing request status and comment.
void editRequest()
{
    printf("\nEnter request id: ");
    int id;
    scanf("%d", &id);
    getchar(); // flush newline.
    FILE *fp = fopen(REQ_FILE, "r+b");
    if (!fp)
    {
        printf("No requests found.\n");
        waitKey();
        return;
    }
    struct Request r;
    int found = 0;
    long pos;
    while ((pos = ftell(fp)) >= 0 && fread(&r, sizeof(struct Request), 1, fp) == 1)
    {
        if (r.id == id)
        {
            found = 1;
            break;
        }
    }
    if (!found)
    {
        fclose(fp);
        printf("Request with id %d not found.\n", id);
        waitKey();
        return;
    }
    // Display existing details.
    printf("\nRequest Details:\n");
    printf("Request Id: %d\n", r.id);
    FILE *uf = fopen(USER_FILE, "rb");
    if (uf)
    {
        struct User u2;
        while (fread(&u2, sizeof(struct User), 1, uf) == 1)
        {
            if (u2.id == r.userId)
            {
                printf("Requested By: %s (id: %d)\n", u2.name, u2.id);
                break;
            }
        }
        fclose(uf);
    }
    const char *statusStr = (r.status == 1) ? "In Progress" : (r.status == 2) ? "Rejected"
                                                                              : "Pending";
    printf("Request Status: %s\n", statusStr);
    printf("Address: %s\n", r.address);
    printf("Comments: %s\n", (r.adminComment[0] ? r.adminComment : r.userComment));

    // Prompt for new status/comment.
    printf("\n***************************\n");
    printf("Enter new details:\n");
    printf("Enter request status (1 -> In Progress, 2 -> Rejected): ");
    scanf("%d", &r.status);
    getchar(); // flush newline.
    printf("Enter any admin comments: ");
    fgets(r.adminComment, sizeof(r.adminComment), stdin);
    r.adminComment[strcspn(r.adminComment, "\n")] = 0;

    // Update the request in file.
    fseek(fp, pos, SEEK_SET);
    fwrite(&r, sizeof(struct Request), 1, fp);
    fclose(fp);
    printBox("Request updated successfully", "", "Press any key to continue");
    waitKey();
}

// View request details by ID.
void viewRequest()
{
    printf("\nEnter request id to view: ");
    int id;
    scanf("%d", &id);
    getchar(); // flush newline.
    FILE *fp = fopen(REQ_FILE, "rb");
    if (!fp)
    {
        printf("No requests found.\n");
        waitKey();
        return;
    }
    struct Request r;
    int found = 0;
    while (fread(&r, sizeof(struct Request), 1, fp) == 1)
    {
        if (r.id == id)
        {
            found = 1;
            break;
        }
    }
    fclose(fp);
    if (!found)
    {
        printf("Request with id %d not found.\n", id);
        waitKey();
        return;
    }
    printf("\nRequest Details\n");
    printf("Request Id: %d\n", r.id);
    FILE *uf = fopen(USER_FILE, "rb");
    if (uf)
    {
        struct User u2;
        while (fread(&u2, sizeof(struct User), 1, uf) == 1)
        {
            if (u2.id == r.userId)
            {
                printf("Requested By: %s (id: %d)\n", u2.name, u2.id);
                break;
            }
        }
        fclose(uf);
    }
    const char *statusStr = (r.status == 1) ? "In Progress" : (r.status == 2) ? "Rejected"
                                                                              : "Pending";
    printf("Request Status: %s\n", statusStr);
    printf("Address: %s\n", r.address);
    printf("Comments: %s\n", (r.adminComment[0] ? r.adminComment : r.userComment));
    printf("\nPress any key to return to main menu");
    waitKey();
}

int main()
{
    system("cls");
    // Initial welcome screen.
    system("clear||cls");
    printBox("Welcome to Electricity Board", "", "Press any key to continue");
    waitKey();

    // Main menu loop.
    while (1)
    {
        system("clear||cls");
        printf("+-----------------------------------------+\n");
        printf("|            Electricity Board            |\n");
        printf("+-----------------------------------------+\n");
        printf("| Select any option from below:           |\n");
        printf("| 1. Add new user.                        |\n");
        printf("| 2. View user.                           |\n");
        printf("| 3. Add new connection request.          |\n");
        printf("| 4. Edit existing request.               |\n");
        printf("| 5. View request.                        |\n");
        printf("| 6. Exit.                                |\n");
        printf("+-----------------------------------------+\n");
        int totalUsers = countRecords(USER_FILE, sizeof(struct User));
        int totalReq = countRecords(REQ_FILE, sizeof(struct Request));
        printf("Total Users: %d   Total Requests: %d\n", totalUsers, totalReq);
        printf("Enter your choice: ");
        int choice;
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            system("clear||cls");
            addNewUser();
            break;
        case 2:
            system("clear||cls");
            viewUser();
            break;
        case 3:
            system("clear||cls");
            addRequest();
            break;
        case 4:
            system("clear||cls");
            editRequest();
            break;
        case 5:
            system("clear||cls");
            viewRequest();
            break;
        case 6:
            printf("Exiting...\n");
            exit(0);
        default:
            printf("Invalid choice.\n");
            waitKey();
        }
    }
    return 0;
}