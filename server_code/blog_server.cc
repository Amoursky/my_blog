#include <signal.h>
#include "httplib.h"
#include "db.hpp"

MYSQL* mysql = NULL;

int main()
{
    using namespace httplib;
    using namespace blog_system;
    
    // 1.先和数据库建立链接
    mysql = blog_system::MySQLInit();
    signal(SIGINT, [](int signo){ blog_system::MySQLRelease(mysql); exit(0);});
    // 创建相关数据库处理对象
    BlogTable blog_table(mysql);
    TagTable tab_table(mysql);

    // 3. 创建服务器，并设置“路由”
    Server server;
    // 新增博客
    server.Post("/blog", [](const Request& req, Response& resp)
    {

    });

    // 查看所有的博客列表
    server.Get("/blog",[](const Request& req, Response& resp)
    {

    });

    //查看某个博客
    server.Get(R"(/blog/(\d+))",[](const Request& req, Response& resp)
    {

    });

    // 修改某个博客
    server.Put(R"(/blog/(\d+))",[](const Request& req, Response& resp)
    {

    });

    // 删除
    server.Delete(R"(/blog/(\d+))",[](const Request& req, Response& resp)
    {

    });

    // 新增标签
    server.Post("/tag",[](const Request& req, Response& resp)
    {

    });

    // 删除标签
    server.Delete(R"(/tag/(\d+))",[](const Request& req, Response& resp)
    {

    });    

    // 查看所有标签
    server.Get("/tag",[](const Request& req, Response& resp)
    {

    });    

    server.set_base_dir("./wwwroot");
    server.listen("0.0.0.0", 9093);

    return 0;
}