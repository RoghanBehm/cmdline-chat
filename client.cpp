// A client for sending messages to a server

#include <array>
#include <string>
#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

int main()
{
    try
    {
        boost::asio::io_context io_context;
        tcp::resolver resolver(io_context);

        tcp::resolver::results_type endpoints =
            resolver.resolve("localhost", "8000");

        tcp::socket socket(io_context);
        boost::asio::connect(socket, endpoints);

        for (;;)
        {

            std::cout << "Enter message:";
            std::string buf;
            if (buf == "exit")
                break;

            std::getline(std::cin, buf);
            boost::system::error_code error;
            boost::asio::write(socket, boost::asio::buffer(buf), error);

            if (error == boost::asio::error::eof)
                break;
            else if (error)
                throw boost::system::system_error(error);
        }
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}