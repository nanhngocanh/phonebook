#include "sqlite3.h"
#include <stdio.h>

int main(void)
{
  sqlite3 *db;
  char *err_msg = 0;
  int rc = sqlite3_open("test.db", &db);
  if (rc != SQLITE_OK)
  {
    fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return 1;
  }
  char *sql1 = "DROP TABLE IF EXISTS Phones;"
               "CREATE TABLE Phones(Phone TEXT UNIQUE, FullName TEXT, Name TEXT , Birthdate DATE);"
               "INSERT INTO Phones VALUES('0673738942' , 'Trần Văn Đạt' , 'Đạt' , '2001-03-24');"
               "INSERT INTO Phones VALUES('0646373241' , 'Nguyễn Thị Hà Giang' , 'Giang' , '2001-02-24');"
               "INSERT INTO Phones VALUES('0297758303' , 'Bùi Hồng Hạnh' , 'Hạnh' ,'2001-11-05');"
               "INSERT INTO Phones VALUES('0963301812' , 'Nguyễn Chí Hiếu','Hiếu','2001-07-13');"
               "INSERT INTO Phones VALUES('0207793343' , 'Lê Trần Thu Hương','Hương','2001-05-20');"
               "INSERT INTO Phones VALUES('0965350353' , 'Lê Thị Ngọc Linh' , 'Linh','2001-06-14');"
               "INSERT INTO Phones VALUES('0234538477' , 'Nguyễn Đức Nam','Nam','2001-12-22');"
               "INSERT INTO Phones VALUES('0993993345' , 'Chu Trần Tú','Tú','2001-06-05');"
               "INSERT INTO Phones VALUES('0234538346' , 'Cao Thị Đan Tâm','Tâm','2001-01-18');"
               "INSERT INTO Phones VALUES('0974838274' , 'Nguyễn Minh Anh','Anh','2001-07-28');"
               "INSERT INTO Phones VALUES('0954833212' , 'Nguyễn Hà Phương','Phương','2001-02-15');"
               "INSERT INTO Phones VALUES('0874838444' , 'Bùi Đức Huy','Huy','2001-04-30');"
               "INSERT INTO Phones VALUES('0154838233' , 'Trương Hiếu Hạnh','Hạnh','2001-01-27');"
               "INSERT INTO Phones VALUES('0334838215' , 'Trần Phạm Quốc Việt','Việt','2001-12-17');"
               "INSERT INTO Phones VALUES('0623485665' , 'Nguyễn Khắc Thắng','Thắng','2001-02-12');"
               "INSERT INTO Phones VALUES('0268385614' , 'Trần Anh Dũng','Dũng','2001-07-25');"
               "INSERT INTO Phones VALUES('0854838524' , 'Nguyễn Chí Hiếu','Hiếu','2001-08-23');"
               "INSERT INTO Phones VALUES('0664838562' , 'Phan Thị Trà Phương','Phương','2001-12-28');"
               "INSERT INTO Phones VALUES('0664838583' , 'Bùi Tuyết Mai','Mai','2001-05-31');"
               "INSERT INTO Phones VALUES('0974352271' , 'Nguyễn Thị Lệ Mỹ','Mỹ','2002-04-21');"
               "INSERT INTO Phones VALUES('0664838312' , 'Nguyễn Ngọc Hà','Hà','2001-03-25');"
               "INSERT INTO Phones VALUES('0664838553' , 'Nguyễn Tuấn Anh','Anh','2001-04-11');";
  rc = sqlite3_exec(db, sql1, 0, 0, &err_msg);
  if (rc != SQLITE_OK)
  {
    fprintf(stderr, "SQL error:%s\n", err_msg);
    sqlite3_free(err_msg);
    sqlite3_close(db);
    return 1;
  }
  sqlite3_close(db);

  return 0;
}
