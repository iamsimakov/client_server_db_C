Вначале был установлен mysql-server
sudo apt-get install mysql-server
Затем из исходников был скомпилирован mysql++ (http://tangentsoft.net/mysql++/releases/mysql++-3.2.2.tar.gz)
c помощью команд
tar -xvzf  mysql++-3.2.2.tar.gz
cd mysql++-3.2.2
./configure --with-mysql-lib=/usr/lib/i386-linux-gnu
make install

сервер компилировался командой
g++ -I/usr/include/mysql -I/usr/include/mysql++ db.cpp -o db -lmysqlpp -lmysqlclient
клиент компилировался командой
g++ clientv2.cpp -o clientv2.h

Сервер запускается командой
./dbserverv2.h {port}
Клиент запускается
./clientv2.h {hostname} {port} {get|set} {key}

Сервер настроен следующим образом:
пользователь root
пароль root
база данных mydb
таблица mytable

Основные источники
http://www.linuxhowtos.org/C_C++/socket.htm
http://programmersforum.ru/showthread.php?t=59220
http://www.jjask.com/475403/setting-up-mysql-in-linux
