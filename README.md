# cmdline-chat

This simple command-line chat application, consisting of a server and client, was created as a learning exercise to reinforce concepts from the official [Boost.Asio tutorials](https://www.boost.org/doc/libs/1_87_0/doc/html/boost_asio/tutorial.html).

    The server accepts connections from clients, listens for messages, and broadcasts each message to all connected clients, excluding the sender.
    The client connects to a server (hardcoded to localhost:8000) and asynchronously handles sending (via command-line input) and receiving messages.

