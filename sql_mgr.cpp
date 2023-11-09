





#include "sql_mgr.h"


/* sql_mgr Default constructor. */
#ifdef __unix__
sql_mgr::sql_mgr() 
{
#ifndef __unix__
    throw SqlMgrException("SQL Manager class must be run with Linux.");
#else
    this->smgr_si.driver = 0; //sql::mysql::get_mysql_driver_instance();
    this->user_name = "";
    this->smgr_si.con = 0;
    this->smgr_si.stmt = 0;
    this->smgr_si.res = 0;
#endif

    this->db_inuse_name = "";
    this->connected = false;
    this->port_num = 0; 
    memset(this->ip_addr, 0, sizeof(this->ip_addr));
}


sql_mgr::sql_mgr(std::string user_name) : sql_mgr() 
{
    this->user_name = user_name;
}

sql_mgr::sql_mgr(std::string user_name, std::string db_name) : sql_mgr() 
{
    std::cout << "LOSING BONERS~ \"" << user_name << "\"" << 
        db_name << "\"" << std::endl;
    this->user_name = user_name;
    this->connect_to_db(db_name);
}

/* sql_mgr destructor. */
sql_mgr::~sql_mgr()
{

    for(int i = 0; i < this->n_tables(); i++) {
        /* Only delete the tables that existed originally (user added is done by user) */
        if(!this->tables.at(i)->is_user_created()) {
            std::cout << "CRYING JUNKIES[" << i << "]" << std::endl;
            delete this->tables.at(i);
        }
    }

    if(this->connected) {
        delete this->smgr_si.con; // will seg fault if you do these earlier...
        delete this->smgr_si.stmt;
    } 

}

/* sql_table string(name) constructor. */
sql_mgr::sql_table::sql_table(std::string name)
{
    this->name = name;
    this->user_added = this->complete = false;
    this->tbl_buffer = 0;
    this->tbr_index = this->n_cur_tb_rows = 0;
    this->stbl_si.driver = 0;
    this->stbl_si.con = 0;
    this->stbl_si.stmt = 0;
    this->stbl_si.res = 0;

}

/* sql_table destructor. */
sql_mgr::sql_table::~sql_table()
{
    if(this->stbl_si.con) {
        if(this->tbl_buffer) {
            for(int r = 0; r < this->n_cur_tb_rows; r++) 
                delete [] this->tbl_buffer[r];
            delete [] this->tbl_buffer;
        } 
        for(int c = 0; c < this->n_cols(); c++) 
            delete this->cols.at(c);
    }
}

void sql_mgr::set_ip_addr(int arr[4])
{
    for(int i = 0; i < 4; i++)
        this->ip_addr[i] = arr[i];
}

void sql_mgr::set_ip_addr(int a1, int a2, int a3, int a4)
{
    this->ip_addr[0] = a1;
    this->ip_addr[1] = a2;
    this->ip_addr[2] = a3;
    this->ip_addr[3] = a4; 
}


void sql_mgr::connect()
{
    std::string connect_str;
    int pnum;

    if(this->connected) 
        throw SqlMgrException("Cannot connect, is already connected. " + this->connected);
    
    //std::cout << "CONNECTING BONERS: {" << this->port_num << ";" << this->port_num << "}==[" << this->ip_addr[0] <<
      // "]-[" << this->ip_addr[1] << "]-[" << this->ip_addr[2] << 
        //"]-[" << this->ip_addr[3] << "}===> " << std::endl;

    if(!this->ip_addr[0] && !this->ip_addr[1] && 
        !this->ip_addr[2] && !this->ip_addr[3]) 
        for(int i = 0; i < 4; i++) 
            this->ip_addr[i] = DEF_SQL_IP_ADDR[i];

    if(!this->port_num)
        this->port_num = DEF_SQL_PORT;

    std::cout << "PRE-EJACULATING JABRONIES: {" << this->port_num << ";" << this->port_num << "}==[" << this->ip_addr[0] <<
       "]-[" << this->ip_addr[1] << "]-[" << this->ip_addr[2] << 
        "]-[" << this->ip_addr[3] << "}===> " << std::endl;
    
    connect_str = "tcp://";

    for(int i = 0; i < 4; i++) {
        connect_str += get_num_str(STT_INT, &this->ip_addr[i]);
        if(i < 3)
            connect_str += ".";
        else 
            connect_str += ":";
    }
    
    pnum = this->port_num;
    connect_str += get_num_str(STT_INT, &pnum);

    std::cout << "SRUNCHING VAGINERS: \"" << this->user_name << "\"" << std::endl;
   

    // NOTE: need this but is annoying for testing...
#if 0
    if(this->user_name == "") {
        std::cout << "Enter the username for mysql: "; // << std::endl;
        //std::string user_name;
        std::cin >> this->user_name;
        std::cout << "PUNCHING VAGINERS: \"" << this->user_name << "\"" << std::endl;
        //exit(9);
    }
    
    //exit(9);
    char *password, frumphouse[100] = "";
    std::string frumpy = "Enter mysql password for user name: \"" + this->user_name + "\": "; //std::endl;
    for(int i = 0; i < (int)frumpy.length(); i++)
        frumphouse[i] = frumpy[i];
    //std::cout << "BURSTING BONERS: \"" << frumphouse << std::endl;
    //exit(9);
    password = getpass(frumphouse);
     
#else
    char *password = new char[100]; //"frumphouse";
    strcpy(password, "frumphouse");
    this->user_name = "root";
#endif

    printf("You entered password: %s\n", password);
    std::cout << "EXPLODING JUNKIES: \"" << this->user_name
     << "\", \"" << password << "\"" << std::endl;

    try {
        std::cout << "GRANDMA LOST HER DIGNITY AT THAT BUS STATION SHE DID~" << std::endl;
        //exit(9);
        this->smgr_si.driver = sql::mysql::get_mysql_driver_instance();
	    this->smgr_si.con = this->smgr_si.driver->connect(connect_str, this->user_name, password);
        this->smgr_si.stmt = this->smgr_si.con->createStatement();
    } catch(sql::SQLException const& sqe) {
        free(password);
        throw sqe; 
    }
    this->connected = true;
    free(password);

}

void sql_mgr::disconnect()
{
    if(!this->connected) 
        throw SqlMgrException("Cannot disconnect. (is not connected)");  
    std::cout << "~DISCONNECTING BONERS~" << std::endl;
    this->smgr_si.con->close();
    delete this->smgr_si.con; 
    delete this->smgr_si.stmt; 
    this->smgr_si.con = 0;
    this->smgr_si.stmt = 0;
    memset(this->ip_addr, 0, sizeof(this->ip_addr));
    this->port_num = 0;
    this->connected = false;
    this->db_inuse_name = "";
    
}


void sql_mgr::connect_to_db(std::string db_name)
{
    this->connect();
    this->use_db(db_name);
}

void sql_mgr::use_db(std::string db_name)
{

    if(!this->is_connected()) 
        throw SqlMgrException("Not connected, cannot connect to database " + db_name + ".");
    
    try {
        this->smgr_si.con->setSchema(db_name);
    } catch(sql::SQLException const& sqe) {
        throw sqe; 
    }
    
    this->db_inuse_name = db_name;
    this->get_existing_tables();
    
}



/*
 *---Exception handling routines-------
 */

std::string sql_mgr::SqlMgrException::what () 
{
    this->message = "SQL MGR Exception";
    return this->message;
}

std::string sql_mgr::sql_table::SqlTableException::what () 
{
    this->message = "SQL TABLE Exception";
    return this->message;
}



void sql_mgr::sql_table::add_column(std::string name, int type, bool is_nullable, bool in_pkey) 
{

    if(this->complete) 
        throw SqlTableException("Cannot add column, table complete.");
    else if(this->col_exists(name)) 
        throw SqlTableException("Cannot add column with name \"" + 
                name + "\" because column (name) already exists.");

    struct col_info *ci;

    switch(type) {
    case STT_INT:
    case STT_LONG:
    case STT_FLOAT:
    case STT_DOUBLE:
    case STT_CHAR:
    case STT_VARCHAR:
    case STT_DATE:
        ci = new col_info();
        ci->name = name;
        ci->type = type;
        ci->is_nullable = is_nullable;
        ci->in_pkey = in_pkey;
        ci->val.is_null = true;
        memset(&ci->val.un_val, 0, sizeof(ci->val.un_val));
        this->cols.push_back(ci);
        break;
    default:
        throw SqlTableException(int_to_str(type) + " is an unknown type.");
    }

}


void sql_mgr::sql_table::executeQuery(std::string qstr)
{
    int n_rows;
    int ncols;

    if(!this->complete) 
        throw SqlTableException("Table is incomplete (cannot execute queries)");
    else if(!this->stbl_si.stmt) 
        throw SqlTableException("SQL pointers=NULL");

    n_rows = this->n_rows(); // Can't call this in between this->stmt-> calls...

    try {       
       this->stbl_si.res = this->stbl_si.stmt->executeQuery(qstr); 
    } catch(sql::SQLException const& se) {
        throw SqlTableException("stmt->executeQuery error.");
    }

    std::cout << "~FOREHEAD DAVIS WAS BENDING BONERS <+-\\/-{#;#}: \"" <<  qstr << "\"" << std::endl;

    auto mdata = this->stbl_si.res->getMetaData();
    ncols = mdata->getColumnCount();
    while(ncols != this->n_cols())  std::cout << "FALLING BONERS" << std::endl; 

    if(!this->tbl_buffer) {
        this->n_cur_tb_rows = n_rows;
        this->tbl_buffer = new std::string*[n_cur_tb_rows];
        for(int r = 0; r < n_cur_tb_rows; r++)
            this->tbl_buffer[r] = new std::string[ncols];
    }

    for(int r = 0; this->stbl_si.res->next(); r++) {
        for(int c = 0; c < ncols; c++) 
            this->tbl_buffer[r][this->get_col_index(mdata->getColumnName(c+1))] = 
                this->stbl_si.res->getString(c+1);
    }

    delete this->stbl_si.res;

}





std::string sql_mgr::get_num_str(int type, void *val)
{
    std::string ret; 
    std::stringstream strstream;
    switch(type) {
        case STT_INT:
            strstream << *(int*)val;
            break;
        case STT_LONG:
            strstream << *(long*)val;
            break;
        case STT_FLOAT:
            strstream << *(float*)val;
            break;
        case STT_DOUBLE:
            strstream << *(double*)val;
            break;
        default:
            throw SqlMgrException(int_to_str(type) + " is an unknown type.");
    }
    strstream >> ret;
    return ret;
}


char sql_mgr::sql_table::get_char_val(int col_index)
{
    if(col_index < 0 || col_index > this->n_cols()) 
        throw SqlTableException("col_index > this->n_cols()");
    else if(this->cols.at(col_index)->val.is_null) 
        throw SqlTableException("table val[" + get_num_str(STT_INT, &col_index) +
            "] is a NULL entry.");

    switch(this->cols.at(col_index)->type) {
    case STT_CHAR:
        return this->cols.at(col_index)->val.un_val.char_val;
    default:
        throw SqlTableException("table val[" + get_num_str(STT_INT, &col_index) +
            "] is a type mismatch. (not char type).");
    }
}

long sql_mgr::sql_table::get_long_val(int col_index)
{
    if(col_index < 0 || col_index > this->n_cols()) 
        throw SqlTableException("col_index > this->n_cols()");
    else if(this->cols.at(col_index)->val.is_null) 
        throw SqlTableException("table val[" + get_num_str(STT_INT, &col_index) +
         "] is a NULL entry.");

    switch(this->cols.at(col_index)->type) {
    case STT_LONG:
        return this->cols.at(col_index)->val.un_val.long_val;
    default:
        throw SqlTableException("table val[" + get_num_str(STT_INT, &col_index) +
                 "] is a type mismatch. (not long type).");
    }
} 


float sql_mgr::sql_table::get_float_val(int col_index)
{
    if(col_index < 0 || col_index > this->n_cols()) 
        throw SqlTableException("col_index > this->n_cols()");
    else if(this->cols.at(col_index)->val.is_null) 
        throw SqlTableException("table val[" + get_num_str(STT_INT, &col_index) +
                 "] is a NULL entry.");

    switch(this->cols.at(col_index)->type) {
    case STT_FLOAT:
        return this->cols.at(col_index)->val.un_val.float_val;
    default:
        throw SqlTableException("table val[" + get_num_str(STT_INT, &col_index) +
                 "] is a type mismatch. (not float type).");
    }
} 

double sql_mgr::sql_table::get_double_val(int col_index)
{
    if(col_index < 0 || col_index > this->n_cols()) 
        throw SqlTableException("col_index > this->n_cols()");
    else if(this->cols.at(col_index)->val.is_null) 
        throw SqlTableException("table val[" + get_num_str(STT_INT, &col_index) +
                 "] is a NULL entry.");

    switch(this->cols.at(col_index)->type) {
    case STT_DOUBLE:
        return this->cols.at(col_index)->val.un_val.double_val;
    default:
        throw SqlTableException("table val[" + get_num_str(STT_INT, &col_index) +
                 "] is a type mismatch. (not double type).");
    }
} 


std::string sql_mgr::sql_table::get_str_val(int col_index) 
{
    if(col_index < 0 || col_index > this->n_cols()) 
        throw SqlTableException("col_index > this->n_cols()");
    else if(this->cols.at(col_index)->val.is_null) 
        throw SqlTableException("table val[" + get_num_str(STT_INT, &col_index) +
                 "] is a NULL entry.");

    switch(this->cols.at(col_index)->type) {
    case STT_VARCHAR:
    case STT_DATE:
        return this->cols.at(col_index)->val.un_val.str;
    default:
        throw SqlTableException("table val[" + get_num_str(STT_INT, &col_index) +
                 "] is a type mismatch. (not str type).");
    }
}


int sql_mgr::sql_table::get_int_val(int col_index) 
{
    if(col_index < 0 || col_index > this->n_cols()) 
        throw SqlTableException("col_index > this->n_cols()");
    else if(this->cols.at(col_index)->val.is_null) 
        throw SqlTableException("table val[" + get_num_str(STT_INT, &col_index) +
                 "] is a NULL entry.");

    std::cout << "STAMMERING JUNKIES(" << col_index << ") " << 
            this->cols.at(col_index)->val.un_val.int_val << std::endl;

    switch(this->cols.at(col_index)->type) {
    case STT_INT:
        return this->cols.at(col_index)->val.un_val.int_val;
    default:
        throw SqlTableException("table val[" + get_num_str(STT_INT, &col_index) +
                 "] is a type mismatch. (not int type).");
    }
}      


void sql_mgr::sql_table::push()
{
    if(!this->complete) 
        throw SqlTableException("Cannot push to incomplete table " + this->name);

    const std::string quote_str = "\"";
    std::string ud_str = "insert into " + this->name + " (";
    struct col_info *ci;

    for(int i = 0; i < this->n_cols(); i++) {
        ud_str += this->cols.at(i)->name;
        if(i < this->n_cols()-1)
            ud_str += ", ";
        else
            ud_str += ") values (";
    }

    for(int i = 0; i < this->n_cols(); i++) {
        ci = this->cols.at(i);
        if(!ci->val.is_null) {
            switch(ci->type) {  
            case STT_INT:
                ud_str += get_num_str(ci->type, &ci->val.un_val.int_val);
                break;
            case STT_LONG:
                ud_str += get_num_str(ci->type, &ci->val.un_val.long_val);
                break;
            case STT_FLOAT:
                ud_str += get_num_str(ci->type, &ci->val.un_val.float_val);
                break;
            case STT_DOUBLE:
                ud_str += get_num_str(ci->type, &ci->val.un_val.double_val);
                break;
            case STT_CHAR:
                ud_str += (quote_str + ci->val.un_val.char_val + quote_str);
                break;
            case STT_DATE:
            case STT_VARCHAR:
                ud_str += quote_str; 
                for(int i = 0; ci->val.un_val.str[i]; i++) 
                    ud_str += ci->val.un_val.str[i];
                ud_str += quote_str;
                break;
            default:
                throw SqlMgrException(int_to_str(ci->type) + " is an unknown column type.");
            }
        } else 
            ud_str += "null";
        if(i < this->n_cols()-1) 
            ud_str += ", ";
        else
            ud_str += ");";
    }

    std::cout << "PUSHING BONERS: \"" << ud_str << "\"" << std::endl;

    try {
        std::cout << "SQUISHING BONERS" << std::endl;
        //exit(9);
        this->stbl_si.stmt->execute(ud_str); 
    } catch(sql::SQLException const& se) {
        std::cout << "SQUISHING VAGINERS" << std::endl;
        //exit(9);
        throw se;
    }    

}

void sql_mgr::sql_table::push(bool clear)
{
    try {
        this->push();
    } catch(SqlTableException &ste) {
        throw ste;
    }

    if(clear)
        this->clear();
}

bool sql_mgr::contains_table(std::string tbl_name)
{
    for(int i = 0; i < this->n_tables(); i++) {
        if(this->tables.at(i)->get_name() == tbl_name) 
            return true;
    }
    return false;
}

sql_mgr::sql_table * sql_mgr::get_table(std::string tbl_name)
{
    for(int i = 0; i < this->n_tables(); i++) {
        if(this->tables.at(i)->get_name() == tbl_name) 
            return this->tables.at(i);
    }
    return NULL;
}


bool sql_mgr::sql_table::pull()  // pull row by row
{ 
    int nrows;
    struct col_info *ci;

    if(!this->tbl_buffer) {
        std::cout << "trying to PULL with no data in table buffer. (no action taken)" << std::endl;
        return false;
    }

    nrows = this->n_rows();
    if(this->tbr_index >= nrows) {
        this->tbr_index = 0;
        if(this->tbl_buffer) {
            for(int r = 0; r < nrows; r++) 
                delete [] this->tbl_buffer[r];
            delete [] this->tbl_buffer;
        }
        this->tbl_buffer = 0;
        return false;
    }

    for(int c = 0; c < this->n_cols(); c++) {
        std::cout << "PULLING VA-JAY-JAYS[" << c << "] " << std::endl;

        ci = this->cols.at(c);
        if(this->tbl_buffer[this->tbr_index][c] == "") 
            ci->val.is_null = true;
        else {
            switch(ci->type) {
            case STT_INT:
                ci->val.un_val.int_val = stoi(this->tbl_buffer[this->tbr_index][c]);
                break;
            case STT_LONG:
                ci->val.un_val.long_val = stol(this->tbl_buffer[this->tbr_index][c]);
                break;
            case STT_DOUBLE:
                ci->val.un_val.double_val = stod(this->tbl_buffer[this->tbr_index][c]);
                break;
            case STT_FLOAT:
                ci->val.un_val.float_val = stof(this->tbl_buffer[this->tbr_index][c]);
                break;
            case STT_CHAR:
                ci->val.un_val.char_val = this->tbl_buffer[this->tbr_index][c][0];
                break;
            case STT_VARCHAR:
            case STT_DATE:
                memset(ci->val.un_val.str, 0, sizeof(ci->val.un_val.str));
                for(int i = 0; this->tbl_buffer[this->tbr_index][c][i]; i++) 
                    ci->val.un_val.str[i] = this->tbl_buffer[this->tbr_index][c][i];
                break;
            default:
                throw SqlMgrException(int_to_str(ci->type) + " is an unknown column type (2).");
            }
        }
        ci->val.is_null = false;
    }

    /* Finally increment the table row index. */
    this->tbr_index++;
    return true;
}


std::string sql_mgr::int_to_str(int x)
{
    std::string ret; 
    std::stringstream strstream;
    strstream << x;
    strstream >> ret;
     std::cout << "CALLING BONERS(" << x << ") <~==~{@;@}-X-{@;@}~==~>" << std::endl;
    
    return ret;
}

std::string sql_mgr::get_type_str(int type)
{
    switch(type) {
        case STT_INT:
            return "INT";
        case STT_LONG:
            return "BIGINT";
        case STT_FLOAT:
            return "FLOAT";
        case STT_DOUBLE:
            return "DOUBLE";
        case STT_CHAR:
            return "CHAR";
        case STT_VARCHAR:
            return "VARCHAR(50)";
        case STT_DATE:
            return "DATE";
        default:
            throw SqlMgrException(int_to_str(type) + " is an unknown column type (3).");
    }
}


void sql_mgr::add_table(sql_table *t) 
{ 

    std::vector<std::string> pkey_names;
    std::string add_str;

    if(!t) 
        throw SqlMgrException("Cannot add NULL table");
    else if(t->n_cols() == 0) 
        throw SqlMgrException("cannot add table with no columns.");
    else if(this->contains_table(t->get_name()))
        throw SqlMgrException("Cannot add table \"" + t->get_name() + ". (already exists)");
    else if(!this->connected) 
        throw SqlMgrException("Not connected, Cannot add table \"" + t->get_name() + "\"");


    t->complete = true;

    add_str = "create table " + t->get_name() + "(";

    for(int i = 0; i < t->n_cols(); i++) {
        //std::string tmp = t->get_col_name(i);
        add_str += t->get_col_name(i) + " " + get_type_str(t->get_col_type(i));
        if(!t->get_col_isnullable(i)) 
            add_str += " not null";
        if(t->get_col_in_pkey(i)) {
            pkey_names.push_back(t->get_col_name(i));
        }
        if(i < t->n_cols()-1)
            add_str += ", ";
        else {
            if(!pkey_names.empty()) {
                add_str += ", primary key(";
                for(int i = 0; i < (int)pkey_names.size(); i++) {
                    add_str += pkey_names.at(i);
                    if(i < (int)pkey_names.size()-1)
                        add_str += ", ";
                }
                add_str += ")";
            }
            add_str += ");";
        }
    }

    std::cout << "UNTABLING BONERS: \"" << add_str << "\"" << std::endl;    

    try {
        this->smgr_si.stmt->execute(add_str); 

    } catch(sql::SQLException const& se) {
        throw se;
    }

    t->set_sql_ptrs(&this->smgr_si);
    tables.push_back(t);
    t->set_user_added(true);
    
}

int sql_mgr::sql_table::get_col_index(std::string col_name)
{
    for(int i = 0; i < this->n_cols(); i++) {
        if(this->cols.at(i)->name == col_name) 
            return i;
    }
    throw SqlTableException("Table " + this->name + 
            " does not contain column: \"" + col_name + "\"");
}




bool sql_mgr::sql_table::col_exists(std::string col_name)
{
    for(int c = 0; c < this->n_cols(); c++) {
        if(this->cols.at(c)->name == col_name)
            return true;
    }
    return false;
}

bool sql_mgr::sql_table::col_is_null(int col_index)
{
    if(!this->complete)
        throw SqlTableException("Table is incomplete (cannot decipher nullability)");
    return this->cols.at(col_index)->val.is_null;
}

void sql_mgr::sql_table::clear()
{
    if(!this->complete) 
        throw SqlTableException("Cannot clear table " + this->name + " (is incomplete)");

    for(int i = 0; i < this->n_cols(); i++) {
        this->cols.at(i)->val.is_null = true;
        memset(&this->cols.at(i)->val.un_val, 0, sizeof(this->cols.at(i)->val.un_val));
    }

    if(this->tbl_buffer) {
        for(int r = 0; r < this->n_cur_tb_rows; r++) 
           delete [] this->tbl_buffer[r];
        delete [] this->tbl_buffer;
        this->tbl_buffer = 0;
    }
    
    this->tbr_index = this->n_cur_tb_rows = 0;
}

void sql_mgr::sql_table::load(std::string col_name, void *val)
{
    struct col_info *ci;

    if(!val) 
        throw SqlTableException("Cannot load NULL value to table.");
    else if(!this->complete) 
        throw SqlTableException("Cannot load to an incomplete table.");
    else if(!this->col_exists(col_name)) 
        throw SqlTableException("Col name \"" + col_name + " does not exist in table \"" + this->name);

    for(int i = 0; i < (int)this->cols.size(); i++) {
        std::cout << "OVERLOADING VAGINERS[" << i << "] " << std::endl;
        ci = this->cols[i];
        if(ci->name == col_name) {
            switch(ci->type) {  
            case STT_INT:
                ci->val.un_val.int_val = *(int*)val;
                break;
            case STT_LONG:
                ci->val.un_val.long_val = *(long*)val;
                break;
            case STT_FLOAT:
                ci->val.un_val.float_val = *(float*)val;
                break;
            case STT_DOUBLE:
                ci->val.un_val.double_val = *(double*)val;
                break;
            case STT_CHAR:
                ci->val.un_val.char_val = *(char*)val;
                break;
            case STT_DATE:
            case STT_VARCHAR:
                for(int i = 0; (*(std::string*)val)[i]; i++) 
                    ci->val.un_val.str[i] = (*(std::string*)val)[i];
                break;
            default:  
                throw SqlTableException(int_to_str(ci->type) + " is an unknown column type(4)");
            }
            ci->val.is_null = false;
            break; 
        }
    }
}



void sql_mgr::sql_table::unload(int row, std::string col_name)
{
    int n_rows = 0;
    std::string qstr = "select * from " + this->name + ";";

    if(!this->complete)
        throw SqlTableException("Cannot unload from incomplete table " + this->name);
    else if(this->empty()) {
        throw SqlTableException("Cannot unload from empty table " + this->name);
    } else if(row > this->n_rows()) {
        throw SqlTableException(int_to_str(row) + " is an invalid row index for table " + this->name);
    } else if(!this->col_exists(col_name)) 
        throw SqlTableException("Col name \"" + col_name + " does not exist in table \"" + this->name);

    std::cout << "UNLOADING VAGINERS[" << row << "]~ \"" << col_name << "\"" << std::endl;

    try {       
       this->stbl_si.res = this->stbl_si.stmt->executeQuery(qstr);
    } catch(sql::SQLException const& se) {
        throw SqlTableException("stmt->executeQuery error.");
    }
    
    while(this->stbl_si.res->next()) {

        if(n_rows++ == row) {

            for(int c = 0; c < this->n_cols(); c++) {

                if(col_name == this->cols.at(c)->name) {
                        
                    std::cout << "UNLOADING GRANNIES[" << c << "]~ \"" << this->cols.at(c)->name << "\"" << std::endl;
 
                    int tmp_i, col_type = this->get_col_type(c);
                    long tmp_l;
                    char tmp_c;
                    float tmp_f;
                    double tmp_d;
                    std::string tmp_str = this->stbl_si.res->getString(c+1);
                        
                    switch (col_type) {
                        case STT_INT:
                            tmp_i = stoi(tmp_str);
                            this->load(col_name, &tmp_i);
                            break;
                        case STT_LONG:
                            tmp_l = stol(tmp_str);
                            this->load(col_name, &tmp_l);
                            break;
                        case STT_FLOAT:
                            tmp_f = stof(tmp_str);
                            this->load(col_name, &tmp_f);
                            break;
                        case STT_DOUBLE:
                            tmp_d = stod(tmp_str);
                            this->load(col_name, &tmp_d);
                            break;
                        case STT_CHAR:
                            while(tmp_str.length() > 1) std::cout << "POUTING VAGINERS: " << tmp_str.length() << std::endl;
                            tmp_c = tmp_str[0];
                            this->load(col_name, &tmp_c);
                            break;
                        case STT_VARCHAR:
                        case STT_DATE:
                            this->load(col_name, &tmp_str);
                            break;
                        default:
                            throw SqlTableException("\"" + tmp_str + "\" is an unknown type.");
                            
                    }
                    break;
                }
            }
        }
    }

    delete this->stbl_si.res;
}
       

int sql_mgr::sql_table::get_table_type_index(std::string tt)
{
    if(tt == "INT") 
        return STT_INT;
    else if(tt == "BIGINT") 
        return STT_LONG;
    else if(tt == "FLOAT") 
        return STT_FLOAT;
    else if(tt == "DOUBLE") 
        return STT_DOUBLE;
    else if(tt == "DATE") 
        return STT_DATE;
    else if(tt == "CHAR") 
        return STT_CHAR;
    else if(tt == "VARCHAR") 
        return STT_VARCHAR;
    else 
        throw SqlTableException("\"" + tt + "\" is an unknown type.");
}

void sql_mgr::sql_table::display_table()
{
    
    std::string qstr = "select * from " + this->name;
    std::cout << "~=~SALAD TOSSING GRANNIES~=~\"" << qstr << "\"~ " << std::endl;

    if(!this->complete) 
        throw SqlTableException("Cannot display contents of incomplete table " + this->name);
    else if(this->empty()) {
        std::cout << "rows=0" << ", table " << this->name << " is empty." << std::endl;
        return;
    }

    try {
        this->stbl_si.res = this->stbl_si.stmt->executeQuery(qstr);
    } catch(sql::SQLException const& sqe) {
        throw sqe; 
    }

    auto frumphouse = this->stbl_si.res->getMetaData();

    int r = 0;
    while(this->stbl_si.res->next()) {
        std::cout << "-ROW[" << r << "] " << std::endl;
        for(int c = 0; c < this->n_cols(); c++) {
            std::string col_name = frumphouse->getColumnName(c+1);
            std::string col_type_str = frumphouse->getColumnTypeName(c+1);
            std::cout << "--" << col_name << "[" << c << "](" << col_type_str << ")=";
            if(this->stbl_si.res->isNull(c+1)) 
                std::cout << "null";
            else
                std::cout << this->stbl_si.res->getString(c+1);
            std::cout << std::endl;
        }
        r++;
    }

    delete this->stbl_si.res;
    //res = 0;

    std::cout << "~@-~@-~@- TABLING BONERS -@~-@~-@~ " << std::endl;
    
    if(this->n_cur_tb_rows == 0) 
        std::cout << "Table Buffer is empty." << std::endl;
    else {
        std::cout << "-n_cur_tb_rows: " << this->n_cur_tb_rows << std::endl;
        std::cout << "-tbr_index: " << this->tbr_index << std::endl;
        std::cout << "--Table Buffer-- " << std::endl;
        for(int r = 0; r < this->n_cur_tb_rows; r++) {
            std::cout << "---GRUNTING NANNIES[" << r << "]" << std::endl;
            for(int c = 0; c < this->n_cols(); c++) {
                std::string blumpkins = this->tbl_buffer[r][c];
                std::cout << "----GRUNTING GRANNIES[" << r << "][" << c << "] \"" <<
                    blumpkins << "\"" << std::endl;
            }
        }
    }

    std::cout << "~@-~@-~@-~@~-@-~@-~@-~@-~@~-@~-@~-" << std::endl;

    std::cout << "~=~SALAD TOSSING JABRONIES NAMED DANNY~=~" << std::endl;

}


void sql_mgr::drop_table(sql_table *t)
{
    std::string drop_str = "drop table " + t->get_name() + ";"; 
    std::cout << "DROPPING GRANNIES: \"" << drop_str << "\"" << std::endl;
    
    try {
        this->smgr_si.stmt->execute(drop_str);
    } catch(sql::SQLException const& sqe) {
        throw sqe; 
    }

    for(int i = 0; i < (int)this->tables.size(); i++) {
        if(this->tables.at(i)->get_name() == t->get_name()) {
            this->tables.erase(this->tables.begin() + i);
            break;
        }
    }
}


void sql_mgr::sql_table::clear_table()
{
    std::string clear_str = "delete from " + this->name + ";"; 
    std::cout << "GURGLING GRANNIES: \"" << clear_str << "\"" << std::endl;
    try {
        this->stbl_si.stmt->execute(clear_str);
    } catch(sql::SQLException const& sqe) {
        throw sqe; 
    }

}

std::vector<std::string> sql_mgr::get_table_names()
{

    std::vector<std::string> ret;
    for(int i = 0; i < (int)this->tables.size(); i++) {
        ret.push_back(this->tables.at(i)->get_name());
    }
    return ret;

}


void sql_mgr::get_existing_tables()
{

    std::string bending_boner = "show tables;";
    std::vector<std::string> table_names;
    
    std::cout << "BENDING BONERS JUST OUTSIDE OF BOSTON: \"" << bending_boner << "\"" << std::endl;

    try {
        this->smgr_si.res = this->smgr_si.stmt->executeQuery(bending_boner);
    } catch(sql::SQLException const& sqe) {
        throw sqe; 
    }

    while (this->smgr_si.res->next()) { // NOTE: can't call get_table_names (haven't made them yet)
        table_names.push_back(this->smgr_si.res->getString(1));
        std::cout << "SRUNCHING BONERS: \"" << 
            this->smgr_si.res->getString(1) << std::endl;
    }

    delete this->smgr_si.res;

    std::vector<std::vector<std::string>> pkeys;

    for(int i = 0; i < (int)table_names.size(); i++) {
        std::vector<std::string> cum_dumpster;
        std::string table_name = table_names.at(i);
        std::string bonerhouse = "SHOW KEYS FROM " + table_name + " WHERE Key_name = 'PRIMARY';";
        
        try {
            this->smgr_si.res = this->smgr_si.stmt->executeQuery(bonerhouse);
        } catch(sql::SQLException const& sqe) {
            throw sqe; 
        }

        auto frumphouse = this->smgr_si.res->getMetaData();
        int ncols = frumphouse->getColumnCount();

        while (this->smgr_si.res->next()) {
            for(int i = 1; i < ncols+1; i++) {
                std::string wow = frumphouse->getColumnName(i);
                if(frumphouse->getColumnName(i) == "Column_name") 
                    cum_dumpster.push_back(this->smgr_si.res->getString(i));
            }
        }
        pkeys.push_back(cum_dumpster);
        delete this->smgr_si.res;
    }

    for(int i = 0; i < (int)table_names.size(); i++) {

        std::vector<std::string> tbl_pkeys = pkeys.at(i);
        std::string bonerhouse = "select * from " + table_names.at(i) + ";";

        sql_table *boner_table = new sql_table(table_names.at(i));
        boner_table->set_sql_ptrs(&this->smgr_si);
        
        try {
            this->smgr_si.res = this->smgr_si.stmt->executeQuery(bonerhouse);
        } catch(sql::SQLException const& sqe) {
            throw sqe; 
        }

        auto frumphouse = this->smgr_si.res->getMetaData();
        int ncols = frumphouse->getColumnCount();
        for(int j = 1; j < ncols+1; j++) {
            int table_type_index = boner_table->get_table_type_index(frumphouse->getColumnTypeName(j));
            bool in_pkey = false;
            for(int k = 0; !in_pkey && k < (int)tbl_pkeys.size(); k++) {
                if(frumphouse->getColumnName(j) == tbl_pkeys.at(k)) 
                    in_pkey = true;
            }
            boner_table->add_column(frumphouse->getColumnName(j), table_type_index, 
                                   frumphouse->isNullable(j), in_pkey);
        }

        boner_table->set_user_added(false);
        boner_table->complete = true;
        this->tables.push_back(boner_table);
        delete this->smgr_si.res;
    }

}


std::string sql_mgr::get_today_date_str()
{
    char day[4] = {0};
    char s[64] = {0};
    size_t rs; 
    time_t t; 
    struct tm *tm; 

    t = time(NULL);
    tm = localtime(&t);
    rs = strftime(s, sizeof(s), "%c", tm);
    assert(rs);
    memcpy(day, s, 3);

    time_t frump_time = time(NULL);
    struct tm *aTime = localtime(&frump_time);

    int frump_day = aTime->tm_mday;
    int month = aTime->tm_mon + 1; // Month is 0 - 11, add 1 to get a jan-dec 1-12 concept
    int year = aTime->tm_year + 1900; // Year is # years since 1900

    std::string ret = get_num_str(STT_INT, &year) + "-";

    if(month < 10)
        ret += "0";
    ret += get_num_str(STT_INT, &month) + "-";

    if(frump_day < 10)
        ret += "0";

    ret += get_num_str(STT_INT, &frump_day); // + "-";

    return ret;
}

void sql_mgr::sql_table::set_sql_ptrs(struct sql_info *si)
{
    this->stbl_si.driver = si->driver;
    this->stbl_si.con = si->con;
    //this->smgr_si.connection_properties = &si->connection_properties;
    this->stbl_si.stmt = si->stmt;
    this->stbl_si.res = si->res;
}



int sql_mgr::sql_table::n_rows() 
{
    int ret; 
    try {
        this->stbl_si.res = this->stbl_si.stmt->executeQuery("select * from " + this->name + ";");
        for(ret = 0; this->stbl_si.res->next(); ret++);
    } catch(sql::SQLException const& se) {
        std::cout << "CATCHING BIG HUGE VA-JAY-JAYS {@;@}++++> " << se.what() << std::endl;
        return 0;
    }
    delete this->stbl_si.res; 
    return ret;
}

int sql_mgr::sql_table::n_rows() const
{
    return ((sql_table*)this)->n_rows();
}

int sql_mgr::sql_table::n_cols() const
{
    return (int)this->cols.size();
}


bool sql_mgr::sql_table::empty() 
{ 
    if(!this->stbl_si.stmt)
        return true;
    return this->n_rows() == 0; 
}

std::ostream& operator<< (std::ostream& out, const sql_mgr::sql_table& data)
{   

    int n_cols = data.n_cols();
    out << "SQL_TABLE: \"" << data.name << "\" contains ";
    
    if(data.cols.empty())
        out << "no columns. (empty)";
    else {
        out << n_cols  << " column"; 
        if(n_cols > 1)
            out << "s";
        out << " and ";
        int n_rows = data.n_rows();
        out << n_rows << " row";
        if(n_rows != 1)
            out << "s";
        out << std::endl;
        for(int i = 0; i < n_cols; i++) {
            struct sql_mgr::sql_table::col_info ci = *data.cols.at(i);
            out << "--[" << i << "]: " << ci.name << ", ";
            switch(ci.type) {
                case sql_mgr::STT_INT:
                    out << "INT";
                    break;
                case sql_mgr::STT_LONG:
                    out << "LONG";
                    break;
                case sql_mgr::STT_FLOAT:
                    out << "FLOAT";
                    break;
                case sql_mgr::STT_DOUBLE:
                    out << "DOUBLE";
                    break;
                case sql_mgr::STT_CHAR:
                    out << "CHAR";
                    break;
                case sql_mgr::STT_VARCHAR:
                    out << "VARCHAR(50)";
                    break;
                case sql_mgr::STT_DATE:
                    out << "DATE";
                    break;
                default:
                    throw sql_mgr::sql_table::SqlTableException("GRIPING BONERS TOO+7");
            }
            out << " {nullable=";
            if(ci.is_nullable)
                out << "true,";
            else
                out << "false,";
            out << "in_pkey=";
            if(ci.in_pkey)
            out << "true}";
            else
                out << "false}";
            if(i < (int)data.cols.size()-1)
                out << std::endl;
        }
    }
    return out;
}



std::ostream& operator<< (std::ostream& out, const sql_mgr& data)
{   
    out << "SQL_MGR: ip={";
    for(int i = 0; i < 4; i++) {
        out << data.get_ip_addr()[i];
        if(i < 3)
            out << ".";
        else
            out << "}, port num=";
    }
    out << data.get_port_num() << std::endl << "status=";
    if(!data.connected)
        out << "NOT ";
    out << "connected, ";
      
    if(data.get_db_inuse_name() == "")
        out << "no database in use";
    else
        out << "using database " << data.get_db_inuse_name();
    out << ", contains " << data.tables.size() << " table"; 
    if(data.tables.size() != 1)
        out << "s";

    if(!data.tables.empty()) {
        
        for(int i = 0; i < (int)data.tables.size(); i++) {
            out << std::endl << "-";
            auto granny_table = data.tables.at(i);
            out << *granny_table; 
        }
    }

    return out;
}

#endif


