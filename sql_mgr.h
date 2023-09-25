/*

* 2023-09-18
* sql_mgr.h   
*
* This version of sql_mgr can add and view elements from tables
* but does not deal with retrieval of elements, rows or columns. 
* 
*/



#ifndef SQL_H
#define SQL_H

#define DEF_SQL_PORT (short)3306
//#define DEF_SQL_IP_ADDR {127,0,0,1 }
//#define ARRAY_SIZE 5
//#define DEF_SQL_IP_ADDR (int[4]){127,0,0,1}



//#include "draw.h"



#ifdef __unix__
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <mysql_driver.h>
#endif

#include <iostream>
#include <fstream>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <cstring>

#include <sstream>
#include <vector>
#include <array>



class sql_mgr {

    protected:
#ifdef __unix__
    sql::mysql::MySQL_Driver *driver;
    sql::Connection *con;
    sql::ConnectOptionsMap connection_properties;
    sql::Statement *stmt;
    sql::ResultSet  *res;
#endif

    bool connected;

std::string db_inuse_name;

int DEF_SQL_IP_ADDR[4] = {127, 0, 0, 1 };



//std::string
int ip_addr[4];
short port_num;



static std::string get_type_str(int index);

static int get_pub_ipv4();

void get_ipv4_arr(int ip_as_int, int arr[4]);


void get_existing_tables();




public:

    

    struct p_key {
        std::string col_name; //, tbl_name;
        void *query_val;
    };

    struct query_entry {
        int col_index;
        //string col_name;
        void *val;
        struct query_entry *next;
    };

    

   

    static std::string get_today_date_str();
    

    void connect();
    void use_db(std::string db_name), create_db(std::string db_name);
    

    class InvalidPlatform : public std::exception {
        std::string message;
        public:
        InvalidPlatform(std::string msg) : message(msg) {}
        std::string what();
    };

    class ConnectionFailure : public std::exception {
        std::string message;
        public:
        ConnectionFailure(std::string msg) : message(msg) {}
        std::string what();
    };

    class UnknownDB : public std::exception {
        std::string message;
        public:
        UnknownDB(std::string msg) : message(msg) {}
        std::string what();
    };

    /*
    class UnknownTable : public std::exception {
        std::string message;
        public:
        UnknownTable(std::string msg) : message(msg) {}
        std::string what();
    };
    */

    
    

    

    class SourceFileFailure : public std::exception {
        std::string message;
        public:
        SourceFileFailure(std::string msg) : message(msg) {}
        std::string what();
    };

    enum SQL_TABLE_TYPES {
            STT_INT = 0, STT_CHAR, STT_VARCHAR, STT_DATE //, ...
        };
    

    class sql_table {
        
        
#ifdef __unix__
        sql::mysql::MySQL_Driver *driver;
        sql::Connection *con;
        sql::ConnectOptionsMap *connection_properties;
        sql::Statement *stmt;
        sql::ResultSet  *res;
#endif

        std::string name;
        //struct row_info {
            struct col_info {
                std::string name;
                int type;
                void *val;
                bool is_nullable;
                bool in_pkey;
            };
            //std::vector<col_info> cols;
            std::vector<col_info> cols;
            void **col_ptr_arr; //[];

            
        //};
        //std::vector<row_info> rows;
        void set_col_ptrs();

        public:

        //auto convert(std::string col_name);
        inline auto convert(int col_index);

        struct col {
        std::string name; //, tbl_name;
        int index;
        void *val;
        struct col *next;
    };

    struct row {
        struct col *col_head;
        int index;
        struct row *next;
    };

         struct row *get_row(int index);
        struct col *get_col(int index);
        

        std::string get_update_str();

        int get_table_type_index(std::string tt);

#ifdef __unix__
        void set_sql_ptrs(sql::mysql::MySQL_Driver *d, sql::Connection *con,
                          sql::ConnectOptionsMap *cp, sql::Statement *stmt,
                          sql::ResultSet  *res);
#endif
        
        int n_cols() const;
        //int n_rows();
        
        sql_table(std::string name);

        //int size() { return this->cols.size(); } // TODO: why don't these work?

        //int empty() { return this->cols.empty(); }

        void add_column(std::string name, int type, bool is_nullable, bool in_pkey);

        

        void load(std::string col_name, void *val);
        void push(); 

        void unload(int row, std::string col_name);
        void * * pull();

        std::string get_name() { return this->name; }
    
        friend std::ostream& operator<< (std::ostream& out, const sql_table& data);

        std::string get_col_name(int index);
        int get_col_type(int index);
        bool get_col_isnullable(int index);
        bool get_col_in_pkey(int index);
        int get_col_index(std::string col_name);

        //void push_row();
        

        //void * pull_row();


    };

    //static inline int n_rows(sql_table *t);

    struct query_entry * retrieve_entry(std::string tbl_name, std::vector <p_key> *query);

    struct col *get_col_head(std::string tbl_name, std::vector <p_key> *query);
    
    //void update_table(sql_table *t);

    sql_table * create_table(std::string tbl_name);

    void add_table(sql_table *t);

    int n_tables() { return (int)this->tables.size(); }

    std::string get_db_inuse_name() const { return this->db_inuse_name; } 
    
    sql_mgr();
    ~sql_mgr();

    void display_table_query(sql_table *t);

    void source(std::string file_name);

    bool is_connected() { return this->connected; }
    
    bool contains_table(std::string tbl_name);
    sql_table *get_table(std::string tbl_name);
    friend std::ostream& operator<< (std::ostream& out, const sql_mgr& data);

    void clear_table(sql_table *t);

    std::vector<std::string> get_table_names();

    private:
    std::vector<sql_table> tables;

};

std::ostream& operator<<(std::ostream& output, const sql_mgr::sql_table& data);
std::ostream& operator<<(std::ostream& output, const sql_mgr& data);







#endif
