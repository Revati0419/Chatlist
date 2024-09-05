#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_MESSAGES 100
#define MAX_MESSAGE_LENGTH 256


// Define the structure for a single message
typedef struct Message 
{
    char sender[50]; // Add sender's name
    char receiver[50]; 
    time_t time;

    char content[MAX_MESSAGE_LENGTH];
    int read;
    struct Message *next;
} Message;

// Define the structure for a user
typedef struct User 
{
    char name[50];
    int number;
    Message *chatHistory[MAX_MESSAGES];
    struct User *next;
} User;


// Global variable to store the head of the user linked list
User *head = NULL;


// Function to logout and return to the main menu
Message* createMessage(const char* sender, const char* receiver, const char* content) 
{
    Message* newMessage = (Message*)malloc(sizeof(Message));
    if (newMessage == NULL) 
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    // Copy the sender, receiver, and content into the new message
    strncpy(newMessage->sender, sender, sizeof(newMessage->sender));
    strncpy(newMessage->receiver, receiver, sizeof(newMessage->receiver));
    strncpy(newMessage->content, content, sizeof(newMessage->content));

    // Initialize the read status and next pointer
    newMessage->read = 0;
    newMessage->next = NULL;
    newMessage->time = time(NULL);

    return newMessage;
}



// Function to create a new user
User* createUser(char *name) 
{
    User *newUser = (User*)malloc(sizeof(User));
    if (newUser == NULL) 
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    strncpy(newUser->name, name, 49);

    // Count the number of users
    int userCount = 0;
    User *temp = head;
    while (temp != NULL) 
    {
        userCount++;
        temp = temp->next;
    }

    // Assign user number
    newUser->number = userCount + 1;

    newUser->next = NULL;
    for (int i = 0; i < MAX_MESSAGES; i++) 
    {
        newUser->chatHistory[i] = NULL;
    }
    return newUser;
}


void displayConversation(User *currentUser, User *receiver);
// Function to logout and return to the main menu


void logout(User **currentUser) 
{
    printf("Logging out.\n");
    *currentUser = NULL; // Set the current user to NULL to indicate logout
}


// Function to add a user to the linked list
void addUserToList(User *newUser) 
{
    // Check if the username already exists
    User *temp = head;
    while (temp != NULL) 
    {
        if (strcmp(temp->name, newUser->name) == 0) 
        {
            printf("Username already exists. Try with a different username.\n");
            free(newUser); // Free memory allocated for the new user
            return;
        }
        temp = temp->next;
    }

    // If the username doesn't exist, add the new user to the list
    if (head == NULL) 
    {
        head = newUser;
    } 
    
    else 
    {
        newUser->next = head;
        head = newUser;
    }
}
// Function to calculate the number of unread messages for a given user
int calculateUnreadMessages(User *user, const char *sender) 
{
    int unreadCount = 0;
    for (int i = 0; i < MAX_MESSAGES && user->chatHistory[i] != NULL; i++) 
    {
        // Check if the message is from the specified sender and unread
        if (strcmp(user->chatHistory[i]->sender, sender) == 0 && !user->chatHistory[i]->read) 
        {
            unreadCount++;
        }
    }
    return unreadCount;
}

int compareMessagesByTime(const void *a, const void *b) 
{
    const Message *messageA = *(const Message **)a;
    const Message *messageB = *(const Message **)b;

    if (messageA->time < messageB->time) 
    {
        return 1;
    } 
    else if (messageA->time > messageB->time) 
    {
        return -1;
    } 
    else 
    {
        return 0;
    }
}


void displayChatHistoryWithUnreadMessages(User *currentUser) 
{
    printf("\n********************************************\n");
    printf("*  Chat history with unread messages:\n");
    printf("********************************************\n");

    // Create a temporary array to store the sorted chat history
    Message *sortedChatHistory[MAX_MESSAGES];
    int sortedMessagesCount = 0;

    // Iterate through other users to find unread messages
    User *temp = head;
    while (temp != NULL) 
    {
        if (temp != currentUser) 
        {
            // Calculate unread messages from temp for currentUser
            int unreadFromTemp = calculateUnreadMessages(currentUser, temp->name);
            if (unreadFromTemp > 0) 
            {
                // Copy unread messages from temp's chat history to the sorted array
                for (int i = 0; i < MAX_MESSAGES && temp->chatHistory[i] != NULL; i++) 
                {
                    Message *message = temp->chatHistory[i];
                    if (!message->read && strcmp(message->receiver, currentUser->name) == 0) 
                    {
                        sortedChatHistory[sortedMessagesCount++] = message;
                        message->read = 1; // Mark message as read
                    }
                }
            }
        }
        temp = temp->next;
    }

    // Sort the sorted array based on message timestamps
    qsort(sortedChatHistory, sortedMessagesCount, sizeof(Message*), compareMessagesByTime);

    // Display the sorted chat history
    for (int i = 0; i < sortedMessagesCount; i++) 
    {
        printf("[%s] %s\n", sortedChatHistory[i]->sender, sortedChatHistory[i]->content);
    }

    if (sortedMessagesCount == 0) 
    {
        printf("No unread messages.\n");
    }

    printf("\n");
}




void displayAllUnreadMessages(User *currentUser) 
{
    printf("\n********************************************\n");
    printf("*       Unread Messages from Other Users     *\n");
    printf("********************************************\n");

    User *temp = head;

    while (temp != NULL) 
    {
        if (temp != currentUser) 
        {
            int unreadMessagesCount = 0;
            Message *unreadMessages[100]; // Array to store unread messages

            // Iterate through the chat history of currentUser
            for (int i = 0; i < 100 && currentUser->chatHistory[i] != NULL; i++) 
            {
                if (strcmp(currentUser->chatHistory[i]->sender, temp->name) == 0 && !currentUser->chatHistory[i]->read) 
                {
                    unreadMessages[unreadMessagesCount++] = currentUser->chatHistory[i];
                }
            }

            // Sort unread messages based on timestamp (recent messages at top)
            for (int i = 0; i < unreadMessagesCount - 1; i++) 
            {
                for (int j = i + 1; j < unreadMessagesCount; j++) 
                {
                    if (unreadMessages[i]->time < unreadMessages[j]->time) 
                    {
                        Message *tempMsg = unreadMessages[i];
                        unreadMessages[i] = unreadMessages[j];
                        unreadMessages[j] = tempMsg;
                    }
                }
            }

            // Display sorted unread messages
            for (int i = 0; i < unreadMessagesCount; i++) 
            {
                printf("* [%s] %s\n", temp->name, unreadMessages[i]->content);
            }

            if (unreadMessagesCount == 0) 
            {
                printf("* No unread messages from %s\n", temp->name);
            }

            printf("********************************************\n");
        }
        temp = temp->next;
    }
}



// Function to display all available users
void displayAllUsers() 
{
    printf("\n********************************************\n");
    printf("*              Available Users              *\n");
    printf("********************************************\n");
    User *current = head;
    while (current != NULL) 
    {
        printf("* %s\n", current->name);
        current = current->next;
    }
    printf("********************************************\n");
}

// Function to delete a user from the linked list
void deleteUser(char *name) 
{
    User *current = head;
    User *prev = NULL;

    // Find the user
    while (current != NULL && strcmp(current->name, name) != 0) 
    {
        prev = current;
        current = current->next;
    }

    // User not found
    if (current == NULL)
    {
        printf("User not found.\n");
        return;
    }

    // If the user to be deleted is the head
    if (prev == NULL)
    {
        head = head->next;
    } 

    else 
    {
        prev->next = current->next;
    }

    free(current);
    printf("User deleted successfully.\n");
}

// Function to display menu and get user's choice
int getMenuChoice() 
{
    int choice;
    printf("\n********************************************\n");
    printf("*                 Main Menu                  *\n");
    printf("********************************************\n");
    printf("* 1. View list of current users            *\n");
    printf("* 2. Add a user                            *\n");
    printf("* 3. Delete a user                         *\n");
    printf("* 4. Login to a user                       *\n");
    printf("* 5. Exit                                  *\n");
    printf("********************************************\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    return choice;
}


// Function to get user's choice after logging in
int getUserMenuChoice(User* currentUser) 
{
    int choice;
    printf("\n********************************************\n");
    printf("*               %s's Chat Menu                *\n",currentUser->name);
    printf("********************************************\n");
    printf("* 1. View Unread Messages                  *\n");
   
    printf("* 2. Search for a user to chat with        *\n"); // Modified option
    printf("* 3. Logout                                *\n");
    printf("********************************************\n");
    printf("Enter your choice: ");


    scanf("%d", &choice);
    return choice;
}

// Function to search for a user by their username
User* searchUser(const char* username) 
{
    User* temp = head;
    while (temp != NULL) 
    {
        if (strcmp(temp->name, username) == 0) 
        {
            return temp;
        }
        temp = temp->next;
    }
    return NULL; // User not found
}


void appendMessageToChatHistory(User *user, Message *message) \
{
    // Find an empty slot in the chat history
    int i;
    for (i = 0; i < MAX_MESSAGES; i++) 
    {
        if (user->chatHistory[i] == NULL) 
        {
            user->chatHistory[i] = message;
            break; // Message added, exit loop
        }
    }

    // If all slots are filled, shift messages to make room for the new message
    if (i == MAX_MESSAGES) 
    {
        for (i = MAX_MESSAGES - 1; i >= 1; i--) 
        {
            user->chatHistory[i] = user->chatHistory[i - 1];
        }
        user->chatHistory[0] = message;
    }
}

void sendMessage(User *sender, User *receiver, const char *messageContent) 
{
    // Create a new message
    Message *message = createMessage(sender->name, receiver->name, messageContent);
    if (message == NULL) 
    {
        printf("Failed to create message.\n");
        return;
    }

    // Add the message to the sender's chat history
    appendMessageToChatHistory(sender, message);

    // Add the message to the receiver's chat history
    appendMessageToChatHistory(receiver, message);

    // Mark the previous unread messages as read
    for (int i = MAX_MESSAGES - 1; i >= 0 && receiver->chatHistory[i] != NULL; i--) {

        if (!receiver->chatHistory[i]->read) {

            receiver->chatHistory[i]->read = 1;

            break;

        }

    }



    printf("Message sent successfully.\n");
}

// Function to start a chat with another user
void startChat(User *currentUser, User *receiver) 
{
    char message[MAX_MESSAGE_LENGTH];
    int chatChoice;

    do {
        printf("\n********************************************\n");
        printf("*            Chat with %s            *\n", receiver->name);
        printf("********************************************\n");
        printf("Enter your choice:\n");
        printf("* 1. Send message\n");
        printf("* 2. View conversation\n");
        printf("* 3. Back to user menu\n");
        printf("********************************************\n");
        printf("Enter your choice: ");
        scanf(" %d", &chatChoice); // Added space before %d

        switch (chatChoice) 
        {
            case 1:
                printf("Enter message: ");
                scanf(" %[^\n]", message); // Read the message
                sendMessage(currentUser, receiver, message);
                break;
            case 2:
                displayConversation(currentUser, receiver);
                break;
            case 3:
                printf("Exiting chat.\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (chatChoice != 3);
}


// Function to display conversation between two users
void displayConversation(User *currentUser, User *receiver) 
{
    printf("\n********************************************\n");
    printf("*        Conversation with %s        *\n", receiver->name);
    printf("********************************************\n");

    // Iterate through the chat history between the current user and the receiver
    for (int i = 0; i < MAX_MESSAGES && currentUser->chatHistory[i] != NULL; i++) 
    {
        if (strcmp(currentUser->chatHistory[i]->sender, receiver->name) == 0 ||
            strcmp(currentUser->chatHistory[i]->receiver, receiver->name) == 0) 
            {
            // Determine sender and receiver based on the message
            char *sender = (strcmp(currentUser->chatHistory[i]->sender, currentUser->name) == 0) ? "You" : currentUser->chatHistory[i]->sender;
            char *receiverName = (strcmp(currentUser->chatHistory[i]->receiver, currentUser->name) == 0) ? "You" : currentUser->chatHistory[i]->receiver;

            // Display message with sender, receiver, and content
            printf("[%s] %s\n", currentUser->chatHistory[i]->sender, currentUser->chatHistory[i]->content);

            // Add a check to display the conversation membership messages
            if (strcmp(currentUser->chatHistory[i]->sender, receiver->name) == 0 && strcmp(currentUser->chatHistory[i]->content, "joined the conversation") == 0) 
            {
                printf("User %s joined the conversation\n", receiver->name);
            }
        }
    }
}
time_t getMostRecentMessageTime(User *currentUser, User *otherUser);

// Function to display all available users except the current user, arranging based on recent messages
void displayAllUsersExceptCurrentUser(User *currentUser) 
{
    printf("\n********************************************\n");
    printf("*              Available Users              *\n");
    printf("********************************************\n");

    // Create an array to store all available users except the current user
    User *availableUsers[MAX_MESSAGES];
    int userCount = 0;

    // Get all available users except the current user
    User *current = head;
    while (current != NULL) 
    {
        if (current != currentUser) 
        {
            availableUsers[userCount++] = current;
        }
        current = current->next;
    }

    // Sort available users based on the most recent message time
    for (int i = 0; i < userCount - 1; i++) 
    {
        for (int j = i + 1; j < userCount; j++) 
        {
            // Find the most recent message time for each user
            time_t timeI = getMostRecentMessageTime(currentUser, availableUsers[i]);
            time_t timeJ = getMostRecentMessageTime(currentUser, availableUsers[j]);
            // Swap users if the message from user J is more recent than user I
            if (timeJ > timeI) 
            {
                User *temp = availableUsers[i];
                availableUsers[i] = availableUsers[j];
                availableUsers[j] = temp;
            }
        }
    }

    // Display users arranged based on recent messages
    for (int i = 0; i < userCount; i++) 
    {
        printf("* %s\n", availableUsers[i]->name);
    }
    
    printf("********************************************\n");
}

// Function to get the most recent message time between the current user and a specific user
time_t getMostRecentMessageTime(User *currentUser, User *otherUser) 
{
    time_t mostRecentTime = 0;

    // Iterate through the current user's chat history to find the most recent message time with the other user
    for (int i = 0; i < MAX_MESSAGES && currentUser->chatHistory[i] != NULL; i++) 
    {
        Message *message = currentUser->chatHistory[i];
        // Check if the message is either sent to or received from the other user
        if ((strcmp(currentUser->name, message->receiver) == 0 && strcmp(otherUser->name, message->sender) == 0) ||
            (strcmp(currentUser->name, message->sender) == 0 && strcmp(otherUser->name, message->receiver) == 0)) 
        {
            // Update the most recent message time if found
            if (message->time > mostRecentTime) 
            {
                mostRecentTime = message->time;
            }
        }
    }

    return mostRecentTime;
}

// Function to find the user who sent or received the most recent message to/from the current user
User* getMostRecentSender(User *currentUser) 
{
    User *mostRecentUser = NULL;
    time_t mostRecentTime = 0;

    // Iterate through the current user's chat history to find the most recent sender or receiver
    for (int i = 0; i < MAX_MESSAGES && currentUser->chatHistory[i] != NULL; i++) 
    {
        Message *message = currentUser->chatHistory[i];
        // Check if the message is either received by or sent by the current user
        if (strcmp(currentUser->name, message->receiver) == 0 || strcmp(currentUser->name, message->sender) == 0) 
        {
            // Check if the message is more recent than the previous most recent
            if (message->time > mostRecentTime) 
            {
                mostRecentTime = message->time;
                // Determine the user who recently messaged the current user
                if (strcmp(currentUser->name, message->receiver) == 0) 
                {
                    // If the current user received the message, the sender is the most recent user
                    mostRecentUser = searchUser(message->sender);
                } 
                else 
                {
                    // If the current user sent the message, the receiver is the most recent user
                    mostRecentUser = searchUser(message->receiver);
                }
            }
        }
    }

    return mostRecentUser;
}




int main() {
    int choice;

    do {
        choice = getMenuChoice();

        switch (choice) 
        {
            case 1:
                displayAllUsers();
                break;
            case 2:
                {
                    char name[50];
                    printf("Enter name of the user: ");
                    scanf("%s", name);
                    User *newUser = createUser(name);
                    addUserToList(newUser);
                    printf("User added successfully.\n");
                    break;
                }
            case 3:
                {
                    char name[50];
                    printf("Enter name of the user to delete: ");
                    scanf("%s", name);
                    deleteUser(name);
                    break;
                }
            case 4:
                {
                    char name[50];
                    printf("Enter your username to login: ");
                    scanf("%s", name);
                    User *currentUser = head;
                    while (currentUser != NULL) 
                    {
                        if (strcmp(currentUser->name, name) == 0) 
                        {
                            int userChoice;
                            do {
                                userChoice = getUserMenuChoice(currentUser);
                                switch (userChoice) 
                                {
                                    case 1:
                                        // displayAllConversations(currentUser);// Display chat history for the current user
                                        displayChatHistoryWithUnreadMessages(currentUser); // Display chat history for the current user

                                        break;
                                   
                                    // case 2:
                                    // {
                                    //     printf("\n********************************************\n");
                                    //     printf("*  Users with existing chat history: \n");
                                    //     printf("********************************************\n");

                                    //     User *temp = head;
                                    //     int found = 0;
                                        
                                    //     // Display users with whom the current user has existing chat history
                                    //     while (temp != NULL) 
                                    //     {
                                    //         if (temp != currentUser) 
                                    //         {
                                    //             // Calculate unread messages from the specific sender for the current user (temp)
                                    //             int unreadFromSender = calculateUnreadMessages(temp, currentUser->name);
                                    //             if (unreadFromSender > 0) 
                                    //             {
                                    //                 printf("* %s (Unread: %d)\n", temp->name, unreadFromSender);
                                    //                 found = 1;
                                    //             }
                                    //         }
                                    //         temp = temp->next;
                                    //     }

                                    //     if (!found) 
                                    //     {
                                    //         printf("No existing chats found.\n");
                                    //         break;
                                    //     }

                                    //     // Ask the user to select a user to open the chat
                                    //     char receiverName[50];
                                    //     printf("Enter the username of the user to open chat: ");
                                    //     scanf("%s", receiverName);

                                    //     User *receiver = head;
                                    //     while (receiver != NULL) 
                                    //     {
                                    //         if (strcmp(receiver->name, receiverName) == 0) 
                                    //         {
                                    //             // Display chat history with the selected user
                                    //             displayConversation(currentUser, receiver);

                                    //             // Mark unread messages as read
                                    //             for (int i = 0; i < MAX_MESSAGES && currentUser->chatHistory[i] != NULL; i++) 
                                    //             {
                                    //                 if (strcmp(currentUser->chatHistory[i]->sender, receiver->name) == 0) 
                                    //                 {
                                    //                     currentUser->chatHistory[i]->read = 1;
                                    //                 }
                                    //             }
                                    //             printf("All unread messages from %s marked as read.\n", receiver->name);
                                                
                                    //             // Enter into chat mode with the selected user
                                    //             int chatChoice;
                                    //             do {
                                    //                 printf("\n********************************************\n");
                                    //                 printf("*            Chat with %s            *\n", receiver->name);
                                    //                 printf("********************************************\n");
                                    //                 printf("* 1. Send message\n");
                                    //                 printf("* 2. View conversation\n");
                                    //                 printf("* 3. Back to user menu\n");
                                    //                 printf("********************************************\n");
                                    //                 printf("Enter your choice: ");
                                    //                 scanf("%d", &chatChoice);

                                    //                 while(getchar() != '\n');
                                                    
                                    //                 switch (chatChoice) 
                                    //                 {
                                    //                     case 1:
                                    //                     {
                                    //                         char messageContent[MAX_MESSAGE_LENGTH];
                                    //                         printf("Enter message: ");
                                    //                         scanf(" %[^\n]", messageContent);
                                    //                         User *receiver = head;
                                    //                         while (receiver != NULL) 
                                    //                         {
                                    //                             if (strcmp(receiver->name, receiverName) == 0) 
                                    //                             {
                                    //                                 sendMessage(currentUser, receiver, messageContent);
                                    //                                 break;
                                    //                             }
                                    //                             receiver = receiver->next;
                                    //                         }

                                    //                         if (receiver == NULL) 
                                    //                         {
                                    //                             printf("User not found.\n");
                                    //                         }

                                    //                         break;
                                                        
                                    //                     }

                                    //                     case 2:
                                    //                         // Display conversation with the selected user
                                    //                         displayConversation(currentUser, receiver);
                                    //                         break;
                                    //                     case 3:
                                    //                         printf("Exiting chat.\n");
                                    //                         break;
                                    //                     default:
                                    //                         printf("Invalid choice. Please try again.\n");
                                    //                 }
                                    //             } while (chatChoice != 3);

                                    //             break;
                                    //         }
                                    //         receiver = receiver->next;
                                    //     }

                                    //     if (receiver == NULL) 
                                    //     {
                                    //         printf("User not found.\n");
                                    //     }
                                    //     break;
                                    // }



                                    case 2:
                        
                                        {
                                            // Display all available users
                                            
                                            displayAllUsersExceptCurrentUser(currentUser);

                                            // Ask the current user to enter the username of the user to start a chat with
                                            char receiverName[50];
                                            printf("Enter the username of the user to start a new chat: ");
                                            scanf("%s", receiverName);
                                            User *receiver = head;
                                            while (receiver != NULL) {
                                                 if (strcmp(receiver->name, receiverName) == 0) {
                                            // Start a new chat with the selected user
                                                    startChat(currentUser, receiver);
                                                    break;
                                                }
                                                receiver = receiver->next;
                                            }
                                            if (receiver == NULL) {
                                                printf("User not found.\n");
                                            }
                                            break;
                                        }


                                    
                                   
                                    case 3:
                                        logout(&currentUser);
                                        printf("Returning to main menu.\n");
                                        
                                        break;
                                    default:
                                        printf("Invalid choice. Please try again.\n");
                                }
                            } while (userChoice != 3);
                            break;
                        }
                        currentUser = currentUser->next;
                    }
                    if (currentUser == NULL) 
                    {
                        printf("User not found.\n");
                    }
                    break;
                }
            case 5:
                printf("Exiting program.\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 5);

    // Free memory allocated for users and messages
    User *temp;
    while (head != NULL) 
    {
        temp = head;
        head = head->next;
        free(temp);
    }

    return 0;
}