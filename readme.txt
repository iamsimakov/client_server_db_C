������� ��� ���������� mysql-server
sudo apt-get install mysql-server
����� �� ���������� ��� ������������� mysql++ (http://tangentsoft.net/mysql++/releases/mysql++-3.2.2.tar.gz)
c ������� ������
tar -xvzf  mysql++-3.2.2.tar.gz
cd mysql++-3.2.2
./configure --with-mysql-lib=/usr/lib/i386-linux-gnu
make install

������ �������������� ��������
g++ -I/usr/include/mysql -I/usr/include/mysql++ db.cpp -o db -lmysqlpp -lmysqlclient
������ �������������� ��������
g++ clientv2.cpp -o clientv2.h

������ ����������� ��������
./dbserverv2.h {port}
������ �����������
./clientv2.h {hostname} {port} {get|set} {key}

������ �������� ��������� �������:
������������ root
������ root
���� ������ mydb
������� mytable

�������� ���������
http://www.linuxhowtos.org/C_C++/socket.htm
http://programmersforum.ru/showthread.php?t=59220
http://www.jjask.com/475403/setting-up-mysql-in-linux
