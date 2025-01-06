// A client for sending messages to a server

#include <array>
#include <string>
#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

void async_read(tcp::socket &socket)
{
    auto buffer = std::make_shared<boost::asio::streambuf>();
    boost::asio::async_read_until(socket, *buffer, '\n',
        [buffer, &socket](const boost::system::error_code &error, std::size_t bytes_transferred) {
            if (!error)
            {
                std::istream is(buffer.get());
                std::string message;
                std::getline(is, message);

                std::cout << "Received: " << message << std::endl;

                async_read(socket);
            }
            else
            {
                std::cerr << "Read error: " << error.message() << std::endl;
            }
        });
}

int main()
{
    try
    {
        boost::asio::io_context io_context;
        tcp::resolver resolver(io_context);

        auto endpoints = resolver.resolve("localhost", "8000");

        tcp::socket socket(io_context);
        boost::asio::connect(socket, endpoints);

        boost::asio::post(io_context, [&socket]()
                          { async_read(socket); });

         std::thread io_thread([&io_context]() { io_context.run(); });

         while (true)
        {
            std::string line;
            std::cout << "Enter message: ";
            if (!std::getline(std::cin, line))
                break;

            if (line == "exit")
            {
                socket.close();
                break;
            }

            boost::asio::post(io_context, [&, line]()
            {
   
                boost::asio::async_write(socket, boost::asio::buffer(line),
                    [](const boost::system::error_code &ec, std::size_t bytes)
                    {
                        if (!ec)
                            std::cout << "Sent " << bytes << " bytes.\n";
                        else
                            std::cerr << "Write error: " << ec.message() << std::endl;
                    });
            });
        }

        io_thread.join();
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}