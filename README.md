
# Trivia Game

## Introduction
**Magshimim 2nd year final project by Uri Tabic and Roy Lamberg Liany**.
Trivia Game is an online multiplayer trivia game that allows users to play trivia games from different computers. The game includes server and client components, enabling players to connect over a network.</br>This project has been copied from gitlab, so there is no Branches and commit history.

## Table of Contents
- [Installation](#installation)
- [Usage](#usage)
- [Features](#features)
- [Dependencies](#dependencies)
- [Configuration](#configuration)
- [Troubleshooting](#troubleshooting)
- [Contributors](#contributors)
- [License](#license)

## Installation
To install the Trivia Game, follow these steps:
1. Clone the repository:
   ```sh
   git clone https://github.com/UriTabic/Trivia-Game.git
2. Navigate to run folder
   ```sh
   cd Trivia-Game\Run
3. Run the server:
	   Server\Trivia server.exe file.
4. Run clients:
		 Client\Trivia client.exe file.


## Usage
1. **Server**: Start the server by executing the server script.
2. **Client**: Start the client(s) by executing the client script.
3. **Configuration**: You can change the IP address and port the client uses to connect to the server in the configuration files.

**Default Configuration**:
- IP: Loopback (127.0.0.1)
- Port: 8175


## Features
-   **User Authentication**: Register or log in to the game.
-   **Room Management**: Create or join a room to play games.
-  **Real Time Games**: Play and see results of you and your competitors in real time.
-   **Statistics**: View personal and game statistics.
-  **Data Base**: A SQLite db that stores users, scores and questions.

## Dependencies

-   **Server**:
    -   C++
    -   Winsock2
-   **Client**:
    -   XML
    -   C#
 
## Configuration

You can configure the client's IP address and port by editing the configuration files located in the `config.txt` directory of the client. and server

## Troubleshooting

-   **Connection Issues**: Ensure the server is running and the IP address and port in the client's configuration file match the server's settings.
-   **Authentication Problems**: Verify that you have registered before attempting to log in.

## Contributors

-   [Roy Lamberg Liany](https://github.com/R0yli) - co worker

## License
- You are welcome to copy and use whatever you want from this project 😀
