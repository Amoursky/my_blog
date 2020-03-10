// 创建一些相关的类来封装数据库操作

#include <cstdio>
#include <cstdlib>
#include <mysql/mysql.h>

namespace blog_system
{
    static MySQL* MySQLInit()
    {
        // 初始化一个MySQL 句柄并建立链接
        // 1. 创建一个句柄
        MYSQL* connect_fd = mysql_init(NULL);
        // 2. 和数据库建立链接
        if (mysql_real_connect(connect_fd, "127.0.0.1", "root", "helloword", "blog_system", 3306, NULL, 0) == NULL)
        {
            printf("链接失败！ %s\n", mysql_error(connect_fd));
            return NULL; 
        }
        // 3. 设定字符编码
        mysql_set_character_set(connect_fd, "utf8");
        return connect_fd;
    }
 
    static void MySQLRelease(MYSQL* connect_fd)
    {
        // 释放句柄并断开链接
        mysql_close(connect_fd);
    }
    
} // end blog_system