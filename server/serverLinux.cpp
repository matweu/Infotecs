#include <iostream>
#include <string>
#include <stdio.h>  //Для поддержки 2-ух const AF_INET и SOCK_STREAM
#include <stdlib.h> // В нем определена функция socket
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <memory.h>
#include <cstdlib>
#include <vector>
#include <fstream>
using namespace std;

int Socket(int domain, int type, int protocol)
{
    int result = socket(domain, type, protocol);
    if (result == -1)
    {
        cout << "Socket not create:" << result << endl;
        return 1;
    }
    return result;
}

void Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
    int result = bind(sockfd, addr, addrlen);
    if (result == -1)
    {
        cout << "ListenSocket not Bind :" << endl;
        exit(EXIT_FAILURE);
    }
}

void Listen(int sockfd, int backlog)
{
    int result = listen(sockfd, backlog);
    if (result == -1)
    {
        cout << "Listean failed:" << endl;
        exit(EXIT_FAILURE);
    }
}
int Accept(int sockfd, struct sockaddr *adrr, socklen_t *addrlen)
{
    int result = accept(sockfd, (sockaddr *)&adrr, addrlen);
    if (result == -1)
    {
        cout << "Accept if failed" << endl;
        return 1;
    }
    else
    {
        cout << "Client Connected!" << endl;
    }

    return result;
}
void Inet_pton(int af, const char *src, void *dst)
{
    int result = inet_pton(af, src, dst);
    if (result == 0)
    {
        cout << "inet_pton failed, src does not contain a character" << endl;
        exit(EXIT_FAILURE);
    }
    if (result == -1)
    {
        cout << "inet_ptoh failed" << endl;
        exit(EXIT_FAILURE);
    }
}

int main()
{
    int nread, value;
    int i = 0;
    int B[9] = {};
    vector<int> objects{};
    char buf[1024];
    ofstream out("/home/server/Main/output.txt", ios::app);
    string data_str;
    bool EXIT = false;
    int LISTENSOCKET = Socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in adr = {0};
    adr.sin_family = AF_INET; // Семейство адрессов IPv4
    adr.sin_port = htons(2432);
    Inet_pton(AF_INET, "192.168.133.5", &adr.sin_addr);
    Bind(LISTENSOCKET, (sockaddr *)&adr, sizeof(adr)); // Сокет,указатель на структуру sockaddr,размер структуры
    listen(LISTENSOCKET, SOMAXCONN);
    socklen_t addrlen = sizeof(adr);
    // accept возвращает файловый дескриптор(сокет),через который можно будет общаться с нашим клиентом
    int NEWSOCKET = Accept(LISTENSOCKET, (sockaddr *)&adr, &addrlen);
    do
    {
        memset(buf, 0, sizeof(buf));
        nread = read(NEWSOCKET, buf, sizeof(buf));
        if (nread > 0)
        {
            if (nread > 2)
            {
                if (out.is_open())
                {
                    out << buf << endl;
                }
                cout << buf << endl;
                continue;
            }

            if (stoi(buf) == 0 || stoi(buf) == 1)
            {
                value = stoi(buf);
                objects.push_back(value);
                B[i] = value;
                cout << "B[" << i << "]=" << B[i] << endl;
                i++;
                if (i == 9)
                {
                    i = 0;
                    for (int j = 0; j < objects.size(); j++)
                    {
                        data_str += to_string(objects[j]) + ",";
                    }
                    data_str.pop_back();
                    string command = "python3 /home/server/Main/prov.py " + data_str;

                    system(command.c_str());
                    objects.clear();
                    data_str.clear();
                }
            }
        }
        else if (nread == 0)
        {
            cout << "Connection closed..." << endl;
            close(NEWSOCKET);
            close(LISTENSOCKET);
            break;
        }
        else
        {
            cout << "Recive error(Error on Read):" << endl;
            close(NEWSOCKET);
            close(LISTENSOCKET);
            break;
        }

    } while (NEWSOCKET);
    out.close();
    return 0;
}