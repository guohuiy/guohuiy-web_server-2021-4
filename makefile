CXX ?= g++

DEBUG ?= 1
ifeq ($(DEBUG), 1)
    CXXFLAGS += -g
else
    CXXFLAGS += -O2

endif

server: ./app/main.cpp  ./timer/lst_timer.cpp ./http/http_conn.cpp ./log/log.cpp ./sql/mysql/sql_connection_pool.cpp ./threadpool/threadpool.cpp ./config/config.cpp ./server/webserver.cpp
	$(CXX) -o test  $^ $(CXXFLAGS) -lpthread -L/usr/lib64/mysql -lmysqlclient

clean:
	rm  -r test
