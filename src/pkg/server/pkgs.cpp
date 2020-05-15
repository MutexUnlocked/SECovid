#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <boost/asio.hpp>
#include <random>
#include <secovid/pkg.hpp>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <secovid/twilio.hpp>

using boost::asio::ip::tcp;
G1 generator;

PKG pkg;

auto process_number(std::string, std::string ,
 bool) -> bool;

auto randomnumber() -> std::string{
    std::string num;
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> dis(1, 6);
    
    for(int i = 0; i < 6; i++){
        num += dis(gen);
    }
    return num;
}


class session
  : public std::enable_shared_from_this<session>
{
public:
  session(tcp::socket socket)
    : socket_(std::move(socket))
  {
  }

  void start()
  {
    do_read();
  }

private:
  void do_read()
  {
    auto self(shared_from_this());
    socket_.async_read_some(boost::asio::buffer(data_, max_length),
        [this, self](boost::system::error_code ec, std::size_t length)
        {
          if (!ec)
          {
              std::string rez(data_);
              if(rez[0] == 'N'){
                  // SEND NUMBER TO PHONE NUMBER
                  this->num = randomnumber();
                  this->phone = rez.substr(1);
                  std::cout << "NUM 0006";
                  this->num = "0006";
                  process_number(num, this->phone,false);
                  sendback_ = "CHECK YOUR PHONE FOR A UNIQUE CODE";
              }else if(rez[0] == 'C'){
                  if(this->num == rez.substr(1)){
                      auto x = pkg.extract(&this->num[0]);
                      sendback_ = x.d.serializeToHexStr() + 
                      "CUTHERE" + x.q.serializeToHexStr();
                  }
              }
              bzero(data_, sizeof(data_));
            
            do_write(length);
          }
        });
  }

  void do_write(std::size_t length)
  {
    auto self(shared_from_this());
    boost::asio::async_write(socket_, boost::asio::buffer(sendback_, sendback_.length()),
        [this, self](boost::system::error_code ec, std::size_t /*length*/)
        {
          if (!ec)
          {
            do_read();
          }
        });
  }

  tcp::socket socket_;
  enum { max_length = 1024 };
  char data_[max_length];
  std::string sendback_;
  std::string num;
  std::string phone;
};

class server
{
public:
  server(boost::asio::io_context& io_context, short port)
    : acceptor_(io_context, tcp::endpoint(tcp::v4(), port))
  {
    do_accept();
  }

private:
  void do_accept()
  {
    acceptor_.async_accept(
        [this](boost::system::error_code ec, tcp::socket socket)
        {
          if (!ec)
          {
            std::make_shared<session>(std::move(socket))->start();
          }

          do_accept();
        });
  }

  tcp::acceptor acceptor_;
};

auto main(int argc, char* argv[]) -> int
{
  try
  {
    if (argc != 2)
    {
      std::cerr << "Usage: pkgs <port>\n";
      return 1;
    }

    boost::asio::io_context io_context;

    server s(io_context, std::atoi(argv[1]));

    io_context.run();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}

auto process_number(std::string message, std::string to_number,
 bool verbose = false) -> bool
{
    int cmd;
    std::string account_sid;
    std::string auth_token;
    std::string from_number;
    std::string picture_url;
    opterr = 0;

    account_sid = "ACc9e9d82e869795ed68d02d7b74b96608";
    auth_token = "2bfa1393497103c56121fbebee4ac386";
    from_number = "+12058903568";

    // Instantiate a twilio object and call send_message
    std::string response;
    auto twilio = std::make_shared<twilio::Twilio>(
        account_sid,
        auth_token);

    bool message_success = twilio->send_message(
        to_number,
        from_number,
        message,
        response,
        picture_url,
        verbose);

    // Report success or failure
    if (!message_success)
    {
        if (verbose)
        {
            std::cout << "Message send failed." << std::endl;
            if (!response.empty())
            {
                std::cout << "Response:" << std::endl
                          << response << std::endl;
            }
        }
        return false;
    }
    else if (verbose)
    {
        std::cout << "SMS sent successfully!" << std::endl;
        std::cout << "Response:" << std::endl
                  << response
                  << std::endl;
    }
    return true;
}