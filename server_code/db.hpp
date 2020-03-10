// 创建一些相关的类来封装数据库操作

#include <cstdio>
#include <cstdlib>
#include <mysql/mysql.h>
#include <jsoncpp/json/json.h>

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

    //创建一个类，用于操作博客表的类
    class BlogTable
    {
        public:
            BlogTable(MYSQL* connect_fd)
            {
                // 通过这个构造函数获取到一个数据库的操作句柄
            }
            
            // 以下操作相关参数都统一使用 JSON 的方式
            // Json::Value jsoncpp 中最核心的类
            // Json::Value 就表示一个具体的 json 对象
            //形如
            // {
            //     title:"博客标题",
            //     content:"博客正文",
            //     create:"创建时间",
            //     tag_id:"标签id"
            // }
            // 最大的好处是方便扩展
            bool Insert(const Json::Value& blog)
            {
                return true;
            }

            // blogs 作为一个输出型参数
            bool SelectAll(Json::Value* blogs, const std::string& tag_id)
            {
                return true;
            }

            //blog 同样是输出型参数，根据当前的 blog_id 在数据库中找到具体的博客内通通过blog参数返回给调用者
            bool SelectOne(int32_t blog_id, Json::Value* blog)
            {

            }

            bool Update()
            {

            }

            bool Delete()
            {

            }

        private:
    };
    
} // end blog_system