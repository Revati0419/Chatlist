# Terminal-Based Communication Platform

## Project Overview
This project is a terminal-based communication platform that allows multiple users to interact and exchange messages. The platform provides a simple menu-driven interface for managing users, sending and receiving messages, viewing chat history, and tracking unread messages. The chat history is stored, and users are notified about unread messages. The project is implemented using a singly linked list data structure to manage messages and users efficiently.

## Features
- **User Management:**
  - Add new users.
  - Display the list of users.
  - Select users to send or receive messages.

- **Message Handling:**
  - Send messages to other users.
  - View received messages.
  - Track unread messages.

- **Chat History:**
  - View chat history for each user.
  - Mark messages as read.
  - Clear chat history if needed.

- **Unread Message Notification:**
  - Notifications for users with unread messages.

## Data Structure Used
The project uses a **Singly Linked List** to implement message storage and management. Each node in the linked list represents a message with the following structure:
- `Sender`: The user who sent the message.
- `Receiver`: The user who receives the message.
- `Message Content`: The actual text of the message.
- `Timestamp`: The time when the message was sent.
- `Read Status`: A flag to indicate whether the message has been read or not.

## How It Works
1. **User Management**: Users are managed using a list of user profiles. When a user is added, they are assigned a unique identifier, and their message history is tracked using the singly linked list.

2. **Message Handling**: When a message is sent from one user to another, a new node is created in the linked list containing the message details. The receiver can view and interact with their received messages.

3. **Chat History Access**: Users can access their full message history, which is stored in a linked list format. Each time a new message is received, it is appended to the list.

4. **Unread Messages**: Messages that haven't been read by the receiver are flagged as unread. When the receiver views the message, the read flag is updated.


