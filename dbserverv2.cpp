//подключаем необходимые заголовочные файлы, пространства имен
#include <mysql++.h>
#include <iostream>

#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#include <sstream>

using namespace mysqlpp;
using namespace std;
//создаем экзмемпляры необходимых объектов
Connection conn; 
StoreQueryResult queryres;
string querystring; 

void dostuff(int); /* function prototype */

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{

     int sockfd, newsockfd, portno, pid;
     socklen_t clilen;
     struct sockaddr_in serv_addr, cli_addr;

     if (argc < 2) {
         cout << " ERROR, no port provided\n Uses " << argv[0] << " {port}\n";
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     
     while (1) {
         newsockfd = accept(sockfd, 
               (struct sockaddr *) &cli_addr, &clilen);
         if (newsockfd < 0) 
             error("ERROR on accept");
         pid = fork();
         if (pid < 0)
             error("ERROR on fork");
         if (pid == 0)  {
             close(sockfd);
             dostuff(newsockfd);
             exit(0);
         }
         else close(newsockfd);
     } /* end of while */
     close(sockfd);
     return 0; /* we never get here */

			
}

void dostuff (int sock)
{
   int n;
   char buffer[256];
   char buffer_key[256];
   char buffer_val[256];
   const char* buffer_res;
      
   bzero(buffer,256);
   n = read(sock,buffer,3);
   if (n < 0) error("ERROR reading from socket");
   bzero(buffer_key,256);
   n = read(sock,buffer_key,3);
   if (n < 0) error("ERROR reading from socket");
  
   ostringstream ostr1;
   
   	try
   	{
		conn.connect("mydb", "localhost", "root", "root"); //пробуем подключиться к базе
   	}
   	catch (ConnectionFailed err) //перехватываем возможное исключение типа ConnectionFailed
   	{
		cout << "Не удалось подключится к базе данных, причина: " << err.what() << endl;
   	}
   	if(conn.connected()) //проверяем, подключены ли мы к базе данных
	{
		
		ostr1.str("");
		ostr1 << "SELECT * FROM mytable WHERE id=" << buffer_key;			
		querystring = ostr1.str().c_str();
		queryres = conn.query(querystring.c_str()).store(); //выполняем запрос
		if(!queryres.empty()) //если что-то вернулось
		{
			ostr1.str("");
			ostr1<<"["<<queryres[0]["id"]<<",\""<< queryres[0]["line"]<< "\"]";
			buffer_res = ostr1.str().c_str();
		} else 
			buffer_res = "Запрос не вернул данных";   //иначе сообщаем что ничего не вернулось
	 	conn.disconnect(); //отключаемся от базы данных
	} else 
		buffer_res = "Подключение к базе данных потеряно..."; //иначе сообщаем что коннект отвалился
   
   if (strcmp(buffer,"get")==0) {	   
   	n = write(sock, buffer_res, 256);
	if (n < 0) error("ERROR writing to socket");
   }   //закончили обработку get
   else if ( strcmp(buffer,"set")==0 && !queryres.empty() ){
	ostr1.str("");
	ostr1<< "This record exists: " <<"["<<queryres[0]["id"]<<",\""<< queryres[0]["line"]<< "\"]";
	buffer_res = ostr1.str().c_str();	
	n = write(sock,buffer_res, 256);
   	if (n < 0) error("ERROR writing to socket");	
   }
   else if ( strcmp(buffer,"set")==0 && queryres.empty() ){
	bzero(buffer_val,256);
	n = read(sock,buffer_val,256);
	if (n < 0) error("ERROR reading from socket");
	
	try
   	{
		conn.connect("mydb", "localhost", "root", "root"); //пробуем подключиться к базе
   	}
   	catch (ConnectionFailed err) //перехватываем возможное исключение типа ConnectionFailed
   	{
		cout << "Не удалось подключится к базе данных, причина: " << err.what() << endl;
   	}
   	if(conn.connected()) //проверяем, подключены ли мы к базе данных
	{
		ostr1.str("");
		ostr1 << "insert mytable(id,line) values("<<buffer_key<<",'"<<buffer_val<<"')";			
		querystring = ostr1.str().c_str();
		querystring.erase(querystring.find('\n'),1);

		queryres = conn.query(querystring.c_str()).store(); //выполняем запрос
		
		ostr1.str(""); 
		ostr1 << "SELECT * FROM mytable WHERE id=" << buffer_key;			
		querystring = ostr1.str().c_str();
		queryres = conn.query(querystring.c_str()).store(); //выполняем запрос
		if(!queryres.empty()) //если что-то вернулось
		{
			ostr1.str("");
			ostr1<<"["<<queryres[0]["id"]<<",\""<< queryres[0]["line"]<< "\"]";
			buffer_res = ostr1.str().c_str();
		} else 
			buffer_res = "Запрос не вернул данных";   //иначе сообщаем что ничего не вернулось
	 	conn.disconnect(); //отключаемся от базы данных
	} else 
		buffer_res = "Подключение к базе данных потеряно..."; //иначе сообщаем что коннект отвалился
		
	n = write(sock,buffer_res, 256);
   	if (n < 0) error("ERROR writing to socket");
   }
}
