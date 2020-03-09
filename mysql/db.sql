create database blog_system;
use blog_system;

create table blog_table(
    blog_id int not null primary key auto_increment,
    title varchar(50),
    content text,
    tag_id int,
    create_time varchar(50),
);