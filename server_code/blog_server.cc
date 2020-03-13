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
    TagTable tag_table(mysql);

    // 3. 创建服务器，并设置“路由”
    Server server;
    // 新增博客
    server.Post("/blog", [&blog_table](const Request& req, Response& resp)
    {
        printf("新增博客!\n");
        // 1.获取到请求中 body 并解析成 json
        Json::Reader reader;
        Json::FastWriter writer;
        Json::Value req_json;
        Json::Value resp_json;
        bool ret = reader.parse(req.body, req_json);
        if(!ret)
        {
            // 解析出错，给用户提示
            printf("解析请求失败！%s\n", req.body.c_str());
            // 构造一个响应对象，告诉客户端出错了
            resp_json["ok"] = false;
            resp_json["reason"] = "input data parse failed!";
            resp.status = 400;
            resp.set_content(writer.write(resp_json), "application/json");
            return;
        }

        // 2.对参数进行校验
        if (req_json["title"].empty()
            || req_json["content"].empty()
            || req_json["tag_id"].empty()
            || req_json["create_time"].empty())
        {
            // 解析出错，给用户提示
            printf("解析请求失败！%s\n", req.body.c_str());
            // 构造一个响应对象，告诉客户端出错了
            resp_json["ok"] = false;
            resp_json["reason"] = "input data format error!";
            resp.status = 400;
            resp.set_content(writer.write(resp_json), "application/json");
            return;
        }

        // 3.真正的调用 MySQL 借口来操作
        ret = blog_table.Insert(req_json);
        if (!ret)
        {
            printf("博客插入失败!\n");
            resp_json["ok"] = false;
            resp_json["reason"] = "blog insert error!";
            resp.status = 500;
            resp.set_content(writer.write(resp_json), "application/json");
            return;                        
        }

        // 4.构造一个正确的响应给客户端即可
        printf("博客插入成功!\n");
        resp_json["ok"] = true;
        resp.set_content(writer.write(resp_json), "application/json");
        return;
    });

    // 查看所有的博客列表
    server.Get("/blog",[&blog_table](const Request& req, Response& resp)
    {
        Json::FastWriter writer;
        printf("查看所有博客！\n");
        // 1.尝试获取 tag_id,如果 tag_id 这个参数不存在，返回空字符串
        const std::string& tag_id = req.get_param_value("tag_id");
        Json::Value resp_json;
        bool ret = blog_table.SelectAll(&resp_json, tag_id);
        if (!ret)
        {
            resp_json["ok"] = false;
            resp_json["reason"] = "select all failed";
            resp.status = 500;
            resp.set_content(writer.write(resp_json), "application/json");
            return;
        }
        resp.set_content(writer.write(resp_json), "application/json");
        return;
    });

    //查看某个博客
    server.Get(R"(/blog/(\d+))",[&blog_table](const Request& req, Response& resp)
    {
        Json::FastWriter writer;
        // 1. 解析获取到的 blog_id
        int32_t blog_id = std::stoi(req.matches[1].str());
        printf("查看 id 为 %d 的博客!\n",blog_id);
        // 2. 直接调用数据库操作
        Json::Value resp_json;
        bool ret = blog_table.SelectOne(blog_id, &resp_json);
        if (!ret)
        {
            resp_json["ok"] = false;
            resp_json["reason"] = "查看指定博客失败：" + std::to_string(blog_id);
            resp.status = 404;
            resp.set_content(writer.write(resp_json), "application/json");
            return;
        }
        //3. 执行正确的响应
        resp.set_content(writer.write(resp_json), "application/json");
        return;
    });

    // 修改某个博客
    server.Put(R"(/blog/(\d+))",[&blog_table](const Request& req, Response& resp)
    {
        // 1. 先获取到博客 id
        int32_t blog_id = std::stoi(req.matches[1].str());
        printf("修改 id 为 %d 的博客!\n", blog_id);
        //2. 获取到请求并解析结果
        Json::Reader reader;
        Json::FastWriter writer;
        Json::Value req_json;
        Json::Value resp_json;
        bool ret = reader.parse(req.body, req_json);
        if (!ret)
        {
            resp_json["ok"] = false;
            resp_json["reason"] = "updae blog parse request failde";
            resp.status = 400;
            resp.set_content(writer.write(resp_json), "application/json");
            return;
        }
        //3. 校验参数是否符合预期
        if (req_json["title"].empty()
            || req_json["content"].empty()
            || req_json["tag_id"].empty())
        {
            resp_json["ok"] = false;
            resp_json["reason"] = "update blog request format error";
            resp.status = 400;
            resp.set_content(writer.write(resp_json), "application/json");
            return;
        }
        // 4.调用数据库操作来完成更新博客操作
        req_json["blog_id"] = blog_id;//从 path 中得到的 id 设置进去
        ret = blog_table.Update(req_json);
        if(!ret)
        {
            resp_json["ok"] = false;
            resp_json["reason"] = "update blog database failed";
            resp.status = 500;
            resp.set_content(writer.write(resp_json), "application/json");
            return;
        }
        resp_json["ok"] = true;
        resp.set_content(writer.write(resp_json), "application/json");
        return;
    });

    // 删除
    server.Delete(R"(/blog/(\d+))",[&blog_table](const Request& req, Response& resp)
    {
        Json::FastWriter writer;
        Json::Value resp_json;
        //1.获取到 blog_id
        int32_t blog_id = std::stoi(req.matches[1].str());
        printf("删除 id 为 %d 的博客!\n", blog_id);
        //2. 调用数据库操作
        bool ret = blog_table.Delete(blog_id);
        if (!ret)
        {
            printf("执行删除失败!\n");
            resp_json["ok"] = false;
            resp_json["reason"] = "delete blog failed";
            resp.status = 500;
            resp.set_content(writer.write(resp_json), "application/json");
            return;
        }
        //3.构造一个正确的响应
        resp_json["ok"] = true;
        resp.set_content(writer.write(resp_json), "application/json");
        return;
    });

    //新增标签
    server.Post("/tag",[&tag_table](const Request& req, Response& resp)
    {
        Json::FastWriter writer;
        Json::Reader reader;
        Json::Value req_json;
        Json::Value resp_json;
        //1.解析请求
        bool ret = reader.parse(req.body, req_json);
        if (!ret)
        {
            printf("插入标签失败!1\n");
            resp_json["ok"] = false;
            resp_json["reason"] = "insert tag req parse failed";
            resp.status = 400;
            resp.set_content(writer.write(resp_json), "application/json");
            return;
        }
        // 2.对请求进行校验
        if (req_json["tag_name"].empty())
        {
            printf("插入标签失败!2\n");
            resp_json["ok"] = false;
            resp_json["reason"] = "insert tag req format error";
            resp.status = 400;
            resp.set_content(writer.write(resp_json), "application/json");
            return;
        }
        //3.调用数据库操作完成插入
        ret = tag_table.Insert(req_json);
        if (!ret)
        {
            printf("插入标签失败!3\n");
            resp_json["ok"] = false;
            resp_json["reason"] = "insert tag database failed";
            resp.status = 500;
            resp.set_content(writer.write(resp_json), "application/json");
            return;
        }
        //4.返回正确的结果
        resp_json["ok"] = true;
        resp.set_content(writer.write(resp_json), "application/json");
        return;
    });

    // 删除标签
    server.Delete(R"(/tag/(\d+))",[&tag_table](const Request& req, Response& resp)
    {
        Json::FastWriter writer;
        Json::Value resp_json;
        //1.解析出 tag_id
        int32_t tag_id = std::stoi(req.matches[1].str());
        printf("删除 id 为 %d 的标签\n", tag_id);
        //2.执行数据库操作
        bool ret = tag_table.Delete(tag_id);
        if (!ret)
        {
            printf("删除标签失败!\n");
            resp_json["ok"] = false;
            resp_json["reason"] = "delete tag database failed";
            resp.status = 500;
            resp.set_content(writer.write(resp_json), "application/json");
            return;           
        }
        //4.构造正确的结果
        resp_json["ok"] = true;
        resp.set_content(writer.write(resp_json), "application/json");
        return;
    });    

    // 查看所有标签
    server.Get("/tag",[&tag_table](const Request& req, Response& resp)
    {
        //1.不需要解析参数
        Json::FastWriter writer;
        Json::Value resp_json;
        bool ret = tag_table.SelectAll(&resp_json);
        if (!ret)
        {
            printf("获取标签失败!\n");
            resp_json["ok"] = false;
            resp_json["reason"] = "get tag database failed";
            resp.status = 500;
            resp.set_content(writer.write(resp_json), "application/json");
            return;    
        }
        //2.构造正确的结果
        resp.set_content(writer.write(resp_json), "application/json");
        return;
    });    

    server.set_base_dir("./wwwroot");
    server.listen("0.0.0.0", 9093);

    return 0;
}