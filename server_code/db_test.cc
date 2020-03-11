#include "db.hpp"

//使用这个程序测试刚才封装的 MySQL 操作是否正确

void TestBlogTable()
{
    MYSQL* mysql = blog_system::MySQLInit();
    blog_system::BlogTable blog_table(mysql);
    Json::Value blog;
    blog["title"] = "我的第一篇博客"；
    blog["content"] = "我要拿30万年薪"；
    blog["tag_id"] = 1;
    blog["create_time"] = "2019/09/09";
    bool ret = blog_table.Insert(blog);
    printf("insert: %d\n", ret);

    blog_system::MySQLRelease(mysql);
}

int main()
{
    TestBlogTable();
    return 0;
}