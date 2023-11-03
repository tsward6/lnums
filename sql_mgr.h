/*

* 2023-09-18
* sql_mgr.h   
*
* This version of sql_mgr ...
* 
*/



#ifndef SQL_H
#define SQL_H



#ifdef __unix__
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <mysql_driver.h>


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
#include <string>

#include <sstream>
#include <vector>
#include <array>

#define DEF_SQL_PORT (short)3306


const int DEF_SQL_IP_ADDR[4] = {127, 0, 0, 1 };


class sql_mgr 
{
    
    protected:

        struct sql_info {
            sql::mysql::MySQL_Driver *driver;
            sql::Connection *con;
            //sql::ConnectOptionsMap *connection_properties;
            sql::Statement *stmt;
            sql::ResultSet  *res;
        } smgr_si;

        bool connected;
        std::string db_inuse_name;
        int ip_addr[4];
        short port_num;
        std::string user_name;

        static std::string get_type_str(int index);
        static std::string int_to_str(int x);
        void get_existing_tables();


    public:

        static std::string get_today_date_str();
        void connect(), connect_to_db(std::string db_name);
        void disconnect();
        void use_db(std::string db_name); //, create_db(std::string db_name);

        class SqlMgrException : public std::exception {
            std::string message;
            public:
            SqlMgrException(std::string msg) : message(msg) {
                std::cout << "UNFOLDING VAGINERS: " << msg << " - ";
            }
            std::string what();
        };

        enum SQL_TABLE_TYPES {
            STT_INT = 0, STT_LONG, STT_FLOAT, STT_DOUBLE, STT_CHAR, STT_VARCHAR, STT_DATE //, ...
        };

        static std::string get_num_str(int type, void *val);
    

    class sql_table {

        friend class sql_mgr;

        struct sql_info stbl_si;
        
        bool complete;
        bool user_added;
        std::string name;

        struct col_info {
            std::string name;
            int type;
            bool is_nullable, in_pkey;

            struct cval {
                bool is_null;
                union tvals {
                    int int_val;
                    long long_val;
                    float float_val;
                    double double_val;
                    char str[1000];
                    //std::string str;
                    char char_val;
                } un_val;  
            } val;
        
        };

        std::vector<col_info*> cols;
        int tbr_index;
        std::string **tbl_buffer;
        int n_cur_tb_rows;   

        std::string get_update_str();

    public:

        

        void set_sql_ptrs(struct sql_info *si);
    
        bool is_user_created() { return this->user_added; }
        void set_user_added(bool user_added) { this->user_added = user_added; }

        void executeQuery(std::string qstr);

        int get_int_val(int col_index);
        std::string get_str_val(int col_index);
        char get_char_val(int col_index);
        long get_long_val(int col_index);
        float get_float_val(int col_index);
        double get_double_val(int col_index);

        class SqlTableException : public std::exception {
            std::string message;
            public:
            SqlTableException(std::string msg) : message(msg) {
                std::cout << "FOLDING BONERS: " << msg << " - ";
                //this->message = msg; ??
            }
            std::string what();
        };

        
        int get_table_type_index(std::string tt);
        int n_cols() const;
        int n_rows();
        int n_rows() const;

        void display_table();
        void clear_table();
        
        sql_table(std::string name);
        ~sql_table();

        void add_column(std::string name, int type, bool is_nullable, bool in_pkey);

        void load(std::string col_name, void *val);
        void push(), push(bool clear); 
        void unload(int row, std::string col_name);
        void clear();
        bool pull();
        
        bool col_is_null(int col_index);
        bool col_exists(std::string col_name);
        bool empty();
        int get_col_index(std::string col_name);      

        bool is_complete() { return this->complete; }
        std::string get_name() { return this->name; }               /* Can return any field address (is a union) */
        const void * get_val_ptr(int col_index) { return &this->cols.at(col_index)->val.un_val.str; }
        std::string get_col_name(int index) { return this->cols.at(index)->name; }
        int get_col_type(int index) { return this->cols.at(index)->type; }
        bool get_col_isnullable(int index) { return this->cols.at(index)->is_nullable; }
        bool get_col_in_pkey(int index) { return this->cols.at(index)->in_pkey; }

        friend std::ostream& operator<< (std::ostream& out, const sql_table& data);
    };

    sql_table * operator()(int i) const { return this->tables.at(i); }

    void add_table(sql_table *t);
    int n_tables() { return (int)this->tables.size(); }
    std::string get_db_inuse_name() const { return this->db_inuse_name; } 
    
    sql_mgr();
    sql_mgr(std::string user_name);
    sql_mgr(std::string user_name, std::string db_name);
    ~sql_mgr();

    
    bool is_connected() { return this->connected; }
    bool contains_table(std::string tbl_name);
    sql_table *get_table(std::string tbl_name);

    int *get_ip_addr() const { return &((sql_mgr*)this)->ip_addr[0]; };
    short get_port_num() const { return this->port_num; }

    
    void drop_table(sql_table *t);
    std::vector<std::string> get_table_names();

    void set_port_num(short port_num) { this->port_num = port_num; }
    void set_ip_addr(int a1, int a2, int a3, int a4), set_ip_addr(int arr[4]);

    private:
    std::vector<sql_table*> tables;

    friend std::ostream& operator<< (std::ostream& out, const sql_mgr& data);
};

std::ostream& operator<<(std::ostream& output, const sql_mgr::sql_table& data);
std::ostream& operator<<(std::ostream& output, const sql_mgr& data);






#endif
#endif
