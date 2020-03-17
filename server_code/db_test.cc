#include "db.hpp"

//使用这个程序测试刚才封装的 MySQL 操作是否正确

void TestBlogTable()
{
    Json::StyledWriter writer;
    MYSQL* mysql = blog_system::MySQLInit();
    blog_system::BlogTable blog_table(mysql);
    bool ret = false;
    //测试插入
    Json::Value blog;
    // blog["title"] = "我的第一篇博客";
    // blog["content"] = "我要拿到年薪30万";
    // blog["tag_id"] = 1;
    // blog["create_time"] = "2019/09/09";
    // ret = blog_table.Insert(blog);
    // printf("insert: %d\n", ret);

    //测试查找全部博客
    // Json::Value blogs;
    // ret = blog_table.SelectAll(&blogs);
    // printf("select all %d\n", ret);
    // printf("blogs:%s\n", writer.write(blogs).c_str());

    //测试查找单个博客
    // ret = blog_table.SelectOne(4, &blog);
    // printf("select one %d\n", ret);
    // printf("blog: %s\n", writer.write(blog).c_str());

    //修改博客
    blog["blog_id"] = 5;
    blog["title"] = "第一篇 C 语言博客";
    blog["content"] = "1. 变量和类型\n什么是变量？创建一个‘变量’就相当于创建一块内存空间";
    blog["create_time"] = "200/10/10";
    ret = blog_table.Update(blog);
    printf("update %d\n", ret);

    // 删除博客
    // ret = blog_table.Delete(4);
    // printf("delete %d", ret);

    blog_system::MySQLRelease(mysql);
}

void TestTagTable()
{
    Json::StyledWriter writer;
    MYSQL* mysql = blog_system::MySQLInit();
    blog_system::TagTable tag_table(mysql);
    //测试插入
    // Json::Value tag;
    // tag["tag_name"] = "C语言";
    // bool ret = tag_table.Insert(tag);
    // printf("insert %d\n", ret);

    //测试查找
    // Json::Value tags;
    // bool ret = tag_table.SelectAll(&tags);
    // printf("select all %d\n",ret);
    // printf("tags:%s\n",writer.write(tags).c_str()); 

    //测试删除
    bool ret = tag_table.Delete(1);
    printf("delete %d\n", ret);

    blog_system::MySQLRelease(mysql);
}

int main()
{
    TestBlogTable();
    //TestTagTable();
    return 0;
}