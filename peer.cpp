#include "peer.h"

namespace p2psp
{

    peer::peer(QWidget *parent) : QWidget(parent), io_service_(),
        acceptor_(io_service_),
        source_socket_(io_service_),
        player_socket_(io_service_)
    {
        //TRACE("Console initialized");
    }

    void peer::ReceiveSourceEndpoint()
    {

        boost::array<char, 6> buffer;
        read(splitter_socket_, ::buffer(buffer));

        char *raw_data = buffer.data();

        in_addr ip_raw = *(in_addr *)(raw_data);
        source.addr = ip::address::from_string(inet_ntoa(ip_raw));
        source.port = ntohs(*(short *)(raw_data + 4));
        TRACE("source_endpoint = ("
              << source.addr.to_string()
              << ","
              << std::to_string(source.port)
              << ")");

    }

    ip::address peer::GetSourceAddr()
    {
        return source.addr;
    }

    PORT_TYPE peer::GetSourcePort()
    {


        return source.port;


    }

    void peer::SetGETMessage()
    {


        std::stringstream ss;
        ss << "GET /" << channel_ << " HTTP/1.1\r\n"
           << "\r\n";
        GET_message_ = ss.str();
        TRACE("GET_message = "
              << GET_message_);
        ss.str("");


    }

    void peer::ReceiveChannel()
    {


        unsigned short channel_size;
        {
            std::vector<char> message(2);
            read(splitter_socket_, boost::asio::buffer(message/*,2*/));
            channel_size = ntohs(*(short *)(message.data()));
        }
        TRACE("channel_size = "
              << channel_size);
        {
            std::vector<char> messagex(channel_size);
            boost::asio::read(splitter_socket_, boost::asio::buffer(messagex/*, channel_size*/));

            channel_ = std::string(messagex.data(), channel_size);
        }
        TRACE("channel = "
              << channel_);
        SetGETMessage();


    }

    std::string peer::GetChannel()
    {


        return channel_;


    }

    void peer::ReceiveHeaderSize()
    {


        boost::array<char, 2> buffer;
        read(splitter_socket_, ::buffer(buffer));

        header_size_ = ntohs(*(short *)(buffer.c_array()));

        TRACE("header_size (in bytes) = "
              << std::to_string(header_size_));

    }

    HEADER_SIZE_TYPE peer::GetHeaderSize()
    {


        return header_size_;


    }

    void peer::ConnectToTheSource() /*throw(boost::system::system_error) */
    {


        ip::tcp::endpoint source_ep(source.addr, source.port);
        system::error_code ec;
        source_socket_.connect(source_ep, ec);

        if (ec) {
            ERROR(ec.message());
            ERROR(source_socket_.local_endpoint().address().to_string()
                  << "\b: unable to connect to the source ("
                  << source.addr
                  << ", "
                  << to_string(source.port)
                  << ")");
            source_socket_.close();
            exit(-1);
        }
        TRACE("Connected to the source at ("
              << source.addr.to_string()
              << ","
              << std::to_string(source.port)
              << ") from "
              << source_socket_.local_endpoint().address().to_string());

    }

    void peer::RequestHeader()
    {
        source_socket_.send(asio::buffer(GET_message_));
    }

    void peer::RelayHeader()
    {


        boost::array<char, 128> buf;
        //boost::system::error_code error;
        for (int header_load_counter_ = 0; header_load_counter_ < GetHeaderSize();) {

            //size_t len = socket.read_some(boost::asio::buffer(buf), error);
            size_t len = source_socket_.read_some(boost::asio::buffer(buf));
            header_load_counter_ += len;

            if (len <= 0) break;

            player_socket_.send(boost::asio::buffer(buf, len));
            //boost::asio::write(socket, boost::asio::buffer(message), ignored_error);
        }


    }

    void peer::WaitForThePlayer()
    {

        ip::tcp::endpoint endpoint(ip::tcp::v4(), player_port_);

        acceptor_.open(endpoint.protocol());
        acceptor_.set_option(ip::tcp::acceptor::reuse_address(true));
        acceptor_.bind(endpoint);
        acceptor_.listen();
        std::cout
                << "Waiting for the player at ("
                << endpoint.address().to_string()
                << ","
                << std::to_string(endpoint.port())
                << ")"
                << std::endl;
        acceptor_.accept(player_socket_);
        TRACE("Player connected. Player is ("
              << player_socket_.remote_endpoint().address().to_string()
              << ","
              << std::to_string(player_socket_.remote_endpoint().port())
              << ")");

    }

    bool peer::PlayChunk(int chunk)
    {


        try {
            write(player_socket_, buffer(chunk_ptr[chunk % buffer_size_].data));
            return true;
        } catch (std::exception e) {
            std::cout
                    << "Player disconnected"
                    << std::endl;
            //std::cout << e.what() << std::endl;
            //player_alive_ = false;
            return false;
            //return true;
        }


    }

    void peer::SetPlayerPort(uint16_t player_port)
    {


        player_port_ = player_port;


    }

    uint16_t peer::GetPlayerPort()
    {


        return  player_port_;


    }

    uint16_t peer::GetDefaultPlayerPort()
    {


        return 9999;


    }

}
