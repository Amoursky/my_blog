.PHONY:all
all:insert select

insert:insert.cc
	g++ $^ -o $@ -L/usr/lib64/mysql -lmysqlclient -ljsoncpp

select:select.cc
	g++ $^ -o $@ -L/usr/lib64/mysql -lmysqlclient -ljsoncpp

.PHONY:clean
clean:
	rm insert