#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <memory.h>
#include <dirent.h>
#include <fstream>
#include <utmpx.h>
#include <ctime>
#include <cstdlib>
using namespace std;

int Socket(int domain, int type, int protocol)
{
  int result = socket(domain, type, protocol);
  if (result == -1)
  {
    cout << "Socket not create:" << endl;
  }
  return result;
}

bool Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
  bool boolConnect;
  int result = connect(sockfd, addr, addrlen);
  if (result == -1)
  {
    cout << "Not connected" << endl;
    boolConnect = false;
  }
  else
  {
    cout << "Connected!" << endl;
    boolConnect = true;
  }
  return boolConnect;
}
void Inet_pton(int af, const char *src, void *dst)
{
  int result = inet_pton(af, src, dst);
  if (result == 0)
  {
    cout << "inet_pton failed, src does not contain a character " << endl;
    exit(EXIT_FAILURE);
  }
  if (result == -1)
  {
    cout << "inet_pton failed " << endl;
    exit(EXIT_FAILURE);
  }
}

int main()
{
  char buf[1024];
  string heliko[] = {"libreoffice", "gnumeric", "firefox", "gnome-terminal-server", "gnome-system-monitor", "nm-connection-editor", "gnome-control-center", "seahorse", "mainwindow.py"};
  bool flag = false;
  bool sent = false;
  int MeaningObjects[size(heliko)] = {};
  int nread, sendmes;
  struct sockaddr_in adr = {0};
  Inet_pton(AF_INET, "192.168.133.5", &adr.sin_addr);
  adr.sin_port = htons(2432);
  adr.sin_family = AF_INET; // Семейство адрессов IPv4

  int ConnectSOCKET = Socket(AF_INET, SOCK_STREAM, 0);
  while (!flag)
  {
    cout << "Waiting for connect:" << endl;
    flag = Connect(ConnectSOCKET, (sockaddr *)&adr, sizeof(adr));
    sleep(3);
  }
  // Проверка когда зашел client
  struct utmpx *utmp_entry = nullptr;
  while (!sent)
  {
    while((utmp_entry = getutxent()))
    {
    if ((utmp_entry->ut_type == USER_PROCESS))
    {
      memset(buf, 0, sizeof(buf));
      tm time_info;
      time_t login_time = utmp_entry->ut_tv.tv_sec;
      localtime_r(&login_time, &time_info);
      strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &time_info);
      if (strlen(buf) > 0)
      {
        sendmes = send(ConnectSOCKET, buf, sizeof(buf), 0);
        if (sendmes != -1)
        {
          cout << buf << endl;
          sent = true;
        }
      }
    }
    }
  }
  endutxent();
  while (ConnectSOCKET)
  {
    DIR *proc_dir = opendir("/proc");
    if (proc_dir == nullptr)
    {
      cerr << "Failed to open /proc" << endl;
      return 1;
    }
    dirent *entry;
    while ((entry = readdir(proc_dir)))
    {

      // Проверяем, что имя директории в /proc является числом ( PID процесса)
      string pid_str = entry->d_name;
      if (pid_str.find_first_not_of("0123456789") != string::npos)
      {
        continue;
      }

      // Открываем файл /proc/PID/cmdline, который содержит командную строку запуска процесса
      string cmd_path = "/proc/" + pid_str + "/cmdline";
      ifstream cmd_file(cmd_path);
      if (!cmd_file)
      {
        continue;
      }

      // Читаем командную строку и проверяем, содержит ли она искомую подстроку
      string cmd_line;
      getline(cmd_file, cmd_line);
      for (int i = 0; i < size(heliko); i++)
      {
        if (cmd_line.find(heliko[i]) != string::npos)
        {
          // Выводим PID и командную строку процесса
          cout << "Process " << pid_str << ": " << cmd_line << endl;
          MeaningObjects[i] = 1;
        }
        else if (MeaningObjects[i] != 1)
        {
          MeaningObjects[i] = 0;
        }
      }
    }

    for (int i = 0; i < size(heliko); i++)
    {
      string str = to_string(MeaningObjects[i]);
      char const *n_char = str.c_str();
      sendmes = send(ConnectSOCKET, n_char, strlen(n_char), 0); // использовать strlen(n_char) вместо buf.size()
      if (sendmes == -1)
      {
        cout << "Message to server not delivered" << endl;
        close(ConnectSOCKET);
      }
      sleep(1);
    }

    sleep(30);
    closedir(proc_dir);
    fill(begin(MeaningObjects), end(MeaningObjects), 0);
  }
  close(ConnectSOCKET);

  return 0;
}