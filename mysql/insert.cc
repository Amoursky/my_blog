// 通过这个程序使用 MySQL  API 实现数据的插入功能

#include <cstdio>
#include <cstdlib>
// 编译器磨人从 /usr/include 目录中查找头文件
#include <mysql/mysql.h>

int main()
{
    // 1.创建一个数据库的连接句柄
    MYSQL* connect_fd = mysql_init(NULL);
    // 2. 和数据库建立连接（和 TCP 的区分开，这是在应用层建立连接）
    //  连接过程需要指定一些必要的信息
    //  a) 连接句柄
    //  b) 服务器的 IP 地址
    //  c) 用户名
    //  d) 密码
    //  e) 数据库名(blog_system)
    //  f) 服务器的端口号
    //  g) unix_sock NULL
    //  h) client_flag 0
    if (mysql_real_connect(connect_fd, "127.0.0.1", "root", "helloword", "blog_system", 3306, NULL, 0) == NULL)
    {
        printf("链接失败！ %s\n", mysql_error(connect_fd));
        return 1; 
    }
    printf("链接成功！\n");

    // 断开链接
    mysql_close(connect_fd);
    return 0;
}