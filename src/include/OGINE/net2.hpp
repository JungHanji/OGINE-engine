#include <SFML/Network.hpp>
#include <OGINE/specialtools.hpp>

using namespace std;

sf::TcpSocket socket;
sf::Packet packet;
sf::IpAddress serverIp;
int port;

struct Server{
    sf::TcpListener listner;

    Server(){
        
    }

    void init(){
        listner.listen(port);

        if(listner.accept(socket) != sf::Socket::Done){
            cout << "[Server] Error! Cannot bind to this IP by this PORT: "<<port<<endl;
            exit(1);
        }
    }

    void send(string msg){
        if(msg != ""){
            packet.clear();
            packet << msg;
            socket.send(packet);
        }
    }

    string receive(){
        string msg_o;
        if(socket.receive(packet) == sf::Socket::Done){
            packet >> msg_o;
            return msg_o;
        }
    }
};

struct Client{
    Client(){
        
    }

    void init(){
        if(socket.connect(serverIp, port) != sf::Socket::Done){
            cout << "[Client] Error! Cannot connect to this IP by this PORT: "<<port<<endl;
            exit(1);
        }
    }

    void send(string msg){
        if(msg != ""){
            packet.clear();
            packet << msg;
            socket.send(packet);
        }
    }

    string receive(){
        string msg_o;
        if(socket.receive(packet) == sf::Socket::Done){
            packet >> msg_o;
            return msg_o;
        }
    }
};