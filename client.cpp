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
            std::getline(std::cin, buf);
            if (buf == "exit") break;

            
            boost::system::error_code error;
 boost::asio::async_write(socket, boost::asio::buffer(buf),
        [&socket](const boost::system::error_code& error, std::size_t bytes_transferred){
                     if (error)
            {
                std::cerr << "Write error: " << error.message() << std::endl;
            }
            else
            {
                std::cout << "Sent " << bytes_transferred << " bytes." << std::endl;
            }
                });

            std::array<char, 128> rbuf;
            size_t len = socket.read_some(boost::asio::buffer(rbuf), error);
            std::cout.write(rbuf.data(), len);

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