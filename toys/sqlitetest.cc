#include "sqlite_modern_cpp.h"

void doSqliteStuff() {
  sqlite::database db(":memory:");

  // executes the query and creates a 'user' table
  db << "create table if not exists user ("
        "   _id integer primary key autoincrement not null,"
        "   age int,"
        "   name text,"
        "   weight real"
        ");";

  // inserts a new user record.
  // binds the fields to '?' .
  // note that only types allowed for bindings are :
  //      int ,long, long long, float, double
  //      string , u16string
  // sqlite3 only supports utf8 and utf16 strings, you should use std::string
  // for utf8 and std::u16string for utf16. note that u"my text" is a utf16
  // string literal of type char16_t * .
  db << "insert into user (age,name,weight) values (?,?,?);" << 20 << u"bob"
     << 83.25;

  int age = 21;
  float weight = 68.5;
  std::string name = "jack";
  db << u"insert into user (age,name,weight) values (?,?,?);"  // utf16 query
                                                               // string
     << age << name << weight;

  std::cout << "The new record got assigned id " << db.last_insert_rowid()
            << std::endl;

  // slects from user table on a condition ( age > 18 ) and executes
  // the lambda for each row returned .
  db << "select age,name,weight from user where age > ? ;" << 18 >>
      [&](int age, std::string name, double weight) {
        std::cout << age << ' ' << name << ' ' << weight << std::endl;
      };

  // selects the count(*) from user table
  // note that you can extract a single culumn single row result only to :
  // int,long,long,float,double,string,u16string
  int count = 0;
  db << "select count(*) from user" >> count;
  std::cout << "cout : " << count << std::endl;

  // you can also extract multiple column rows
  db << "select age, name from user where _id=1;" >> std::tie(age, name);
  std::cout << "Age = " << age << ", name = " << name << std::endl;

  // this also works and the returned value will be automatically converted to
  // string
  std::string str_count;
  db << "select count(*) from user" >> str_count;
  std::cout << "scount : " << str_count << std::endl;
}

int main() {
  try {
    doSqliteStuff();
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
  return 0;
}