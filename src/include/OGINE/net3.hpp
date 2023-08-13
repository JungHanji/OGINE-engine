#include <SFML/Network.hpp>
#include <OGINE/specialtools.hpp>

using namespace std;

sf::TcpSocket socket, socket2;
sf::Packet packet, packet2;
sf::IpAddress serverIp;
int port;

struct Server{
    sf::TcpListener listner;

    Server(){
        
    }

    void init(){
        listner.listen(port);

        if(listner.accept(socket) != sf::Socket::Done){
            cout << "[Server] Error! Cannot connect user to this IP by this PORT: "<<port<<endl;
            exit(1);
        }

        if(listner.accept(socket2) != sf::Socket::Done){
            cout << "[Server] Error! Cannot connect user to this IP by this PORT: "<<port<<endl;
            exit(1);
        }
    }

    void send(string msg, string msg2){
        if(msg != ""){
            packet.clear();
            packet << msg;
            socket.send(packet);

            packet2.clear();
            packet2 << msg2;
            socket2.send(packet2);
        }
    }

    vector<string> receive(){
        vector<string> msg_o;
        if(socket.receive(packet) == sf::Socket::Done){
            string msg;
            packet >> msg;
            msg_o.push_back(msg);
        }

        if(socket2.receive(packet2) == sf::Socket::Done){
            string msg;
            packet2 >> msg;
            msg_o.push_back(msg);
        }

        return {msg_o};
    }
};

struct Client{
    int mode;
    Client(int mode_){
        mode=mode_;
    }

    void init(){
        if(mode==0 && socket.connect(serverIp, port) != sf::Socket::Done){
            cout << "[Client] Error! Cannot connect to this IP by this PORT: "<<port<<endl;
            exit(1);
        } else if(mode==1 && socket2.connect(serverIp, port) != sf::Socket::Done){
            cout << "[Client] Error! Cannot connect to this IP by this PORT: "<<port<<endl;
            exit(1);
        }
    }

    void send(string msg){
        if(mode == 0 && msg != ""){
            packet.clear();
            packet << msg;
            socket.send(packet);
        } else if(mode == 1 && msg != ""){
            packet2.clear();
            packet2 << msg;
            socket2.send(packet2);
        }
    }

    string receive(){
        string msg_o;
        if(mode == 0 && socket.receive(packet) == sf::Socket::Done){
            packet >> msg_o;
            return msg_o;
        } else if(mode == 1 && socket2.receive(packet2) == sf::Socket::Done){
            packet2 >> msg_o;
            return msg_o;
        } 
    }
};