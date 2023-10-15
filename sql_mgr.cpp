





#include "sql_mgr.h"

std::string sql_mgr::InvalidPlatform::what () 
{
    message = "Invalid platform. (need Linux)";
    return message;
}

std::string sql_mgr::ConnectionFailure::what () 
{
    message = "SQL Connection Failure";
    return message;
}

std::string sql_mgr::DB_Exception::what () 
{
    message = "DB Exception";
    return message;
}

std::string sql_mgr::UnknownDB::what () 
{
    message = "Unknown database";
    return message;
}

std::string sql_mgr::sql_table::NullEntry::what () 
{
    message = "Null Entry in table buffer.";
    return message;
}

std::string sql_mgr::sql_table::TypeMismatch::what () 
{
    message = "Type mismatch error";
    return message;
}


std::string sql_mgr::sql_table::ExecuteQueryException::what () 
{
    message = "SQL Execute Query Exception";
    return message;
}

sql_mgr::sql_mgr() 
{

#ifndef __unix__
    throw InvalidPlatform("SQL Manager class must be run with Linux.");
#else
    this->driver = sql::mysql::get_mysql_driver_instance();
    this->con = 0;
    this->stmt = 0;
    this->res = 0;
#endif

    this->port_num = 0; //DEF_SQL_PORT;
    this->db_inuse_name = "";
    this->connected = false;
    
    memset(this->ip_addr, 0, sizeof(this->ip_addr));
    
    //for(int i = 0; i < 4; i++) {
      //  this->ip_addr[i] = DEF_SQL_IP_ADDR[i];
        //std::cout << "SMOOCHING BONERS {@;@}++++> " << DEF_SQL_IP_ADDR[i] << std::endl;
    //}
    

}

sql_mgr::~sql_mgr()
{

    for(int i = 0; i < (int)this->tables.size(); i++) {
        if(!this->tables.at(i)->is_user_created()) 
            delete this->tables.at(i);
    }

#ifdef __unix__
    delete this->con; // will seg fault if you do these earlier...
    delete this->stmt;
#endif
    
}

sql_mgr::sql_table::~sql_table()
{
#ifdef __unix__
    if(this->con) {
        int nrows = this->n_rows();
        int ncols = this->n_cols();
        if(this->tbl_buffer) {
            for(int r = 0; r < nrows; r++) 
                delete [] this->tbl_buffer[r];
            delete [] this->tbl_buffer;
        } 
        
        for(int c = 0; c < ncols; c++) 
            delete this->cols.at(c);
    }
#endif
}


void sql_mgr::sql_table::execute(std::string str)
{
    std::cout << "BUMPING BONERS: \"" << str << "\"" << std::endl;

#ifdef __unix__

    while(!this->stmt) {
        std::cout << "{@;@}++++> BUMPING BONERS~ ";
    }

    try {       
        this->stmt->execute(str); 

    } catch(sql::SQLException const& ih) {
        std::cout << "CATCHING VA-JAY-JAYS {@;@}++++> WON" << ih.what() << std::endl;
        exit(9);
    }

     std::cout << "DUMPING BONERS: \"" << str << "\", " << this->tbl_buffer << std::endl;
#endif
}



void sql_mgr::sql_table::executeQuery(std::string qstr)
{

    if(!this->complete) 
        throw ExecuteQueryException("Table is incomplete (cannot execute queries)");

#ifdef __unix__

    if(!this->stmt) 
        throw ExecuteQueryException("SQL table connection error.");;

    int n_rows = this->n_rows();
    if(!this->tbl_buffer) {
        this->tbl_buffer = new std::string*[n_rows];
        for(int r = 0; r < n_rows; r++)
            this->tbl_buffer[r] = new std::string[this->n_cols()];
    }

    try {       
       this->res = this->stmt->executeQuery(qstr); 
    } catch(sql::SQLException const& ih) {
        throw ExecuteQueryException("stmt->executeQuery error.");
    }
    
    auto mdata = this->res->getMetaData();
    int ncols = mdata->getColumnCount();

    for(int r = 0; this->res->next(); r++) {
        for(int c = 0; c < ncols; c++) 
            this->tbl_buffer[r][this->get_col_index(mdata->getColumnName(c+1))] = 
                this->res->getString(c+1);
    }

    delete this->res;

#endif
}



void check_host_name(int hostname) { //This function returns host name for local computer
   if (hostname == -1) {
      perror("gethostname");
      exit(1);
   }
}
void check_host_entry(struct hostent * hostentry) { //find host info from host name
   if (hostentry == NULL){
      perror("gethostbyname");
      exit(1);
   }
}
void IP_formatter(char *IPbuffer) { //convert IP string to dotted decimal format
   if (NULL == IPbuffer) {
      perror("inet_ntoa");
      exit(1);
   }
}
    

int sql_mgr::get_pub_ipv4()
{

    char host[256];
    char *IP;
    struct hostent *host_entry;
    int hostname;
    hostname = gethostname(host, sizeof(host)); //find the host name
    check_host_name(hostname);
    host_entry = gethostbyname(host); //find host information
    check_host_entry(host_entry);
    IP = inet_ntoa(*((struct in_addr*) host_entry->h_addr_list[0])); //Convert into IP string
    printf("Current Host Name: %s\n", host);
    printf("Host IP: %s\n", IP);
    std::cout << "FOREHEAD DAVIS WAS BENDING BONERS {\"" << host << "\"\\\"" << IP << "\"}" << std::endl;

    system("curl -s --ipv4 http://myexternalip.com/raw -o IPV4_ADDR_FILE.TXT");

    std::ifstream pedo_file("./IPV4_ADDR_FILE.TXT");
    std::string file_result_str((std::istreambuf_iterator<char>(pedo_file)), 
            std::istreambuf_iterator<char>());
    int ip_as_int = 0, cnt = 0;
    std::string tmpstr = "";
    for(int i = 0; file_result_str[i]; i++) {
        if(file_result_str[i] != '.')
            tmpstr += file_result_str[i];
        if(file_result_str[i] == '.' || i == ((int)file_result_str.length())-1) {
            int tmpint = stoi(tmpstr);
            tmpstr = "";
            cnt++;
            ip_as_int |= ((tmpint & 0xff) << ((4-cnt)*8));
        }
    }

    return ip_as_int;

}

char sql_mgr::sql_table::get_char_val(int col_index)
{
    std::string thr_str;
    if(col_index < 0 || col_index > this->n_cols()) 
        throw NullEntry("col_index > this->n_cols()");
    else if(this->cols.at(col_index)->val.is_null) {
        thr_str = "table val[" + col_index;
        thr_str += "] is a NULL entry.";
        throw NullEntry(thr_str);
    }

    switch(this->cols.at(col_index)->type) {
    case STT_CHAR:
        return this->cols.at(col_index)->val.un_val.char_val;
    default:
        thr_str = "table val[" + col_index;
        thr_str += "] is a type mismatch. (not string type)";
        throw TypeMismatch(thr_str);
        
    }
}

long sql_mgr::sql_table::get_long_val(int col_index)
{
    std::string thr_str;
    if(col_index < 0 || col_index > this->n_cols()) 
        throw NullEntry("col_index > this->n_cols()");
    else if(this->cols.at(col_index)->val.is_null) {
        thr_str = "table val[" + col_index;
        thr_str += "] is a NULL entry.";
        throw NullEntry(thr_str);
    }

    switch(this->cols.at(col_index)->type) {
    case STT_LONG:
        return this->cols.at(col_index)->val.un_val.long_val;
    default:
        thr_str = "table val[" + col_index;
        thr_str += "] is a type mismatch. (not string type)";
        throw TypeMismatch(thr_str);
        
    }

} 


float sql_mgr::sql_table::get_float_val(int col_index)
{
    std::string thr_str;
    if(col_index < 0 || col_index > this->n_cols()) 
        throw NullEntry("col_index > this->n_cols()");
    else if(this->cols.at(col_index)->val.is_null) {
        thr_str = "table val[" + col_index;
        thr_str += "] is a NULL entry.";
        throw NullEntry(thr_str);
    }

    switch(this->cols.at(col_index)->type) {
    case STT_FLOAT:
        return this->cols.at(col_index)->val.un_val.float_val;
    default:
        thr_str = "table val[" + col_index;
        thr_str += "] is a type mismatch. (not string type)";
        throw TypeMismatch(thr_str);
        
    }

} 

double sql_mgr::sql_table::get_double_val(int col_index)
{
    std::string thr_str;
    if(col_index < 0 || col_index > this->n_cols()) 
        throw NullEntry("col_index > this->n_cols()");
    else if(this->cols.at(col_index)->val.is_null) {
        thr_str = "table val[" + col_index;
        thr_str += "] is a NULL entry.";
        throw NullEntry(thr_str);
    }

    switch(this->cols.at(col_index)->type) {
    case STT_DOUBLE:
        return this->cols.at(col_index)->val.un_val.double_val;
    default:
        thr_str = "table val[" + col_index;
        thr_str += "] is a type mismatch. (not string type)";
        throw TypeMismatch(thr_str);
        
    }

} 


std::string sql_mgr::sql_table::get_str_val(int col_index) 
{
    std::string thr_str;
    if(col_index < 0 || col_index > this->n_cols()) 
        throw NullEntry("col_index > this->n_cols()");
    else if(this->cols.at(col_index)->val.is_null) {
        thr_str = "table val[" + col_index;
        thr_str += "] is a NULL entry.";
        throw NullEntry(thr_str);
    }

    switch(this->cols.at(col_index)->type) {
    case STT_VARCHAR:
    case STT_DATE:
        return this->cols.at(col_index)->val.un_val.str;
    default:
        thr_str = "table val[" + col_index;
        thr_str += "] is a type mismatch. (not string type)";
        throw TypeMismatch(thr_str);
        
    }
}


int sql_mgr::sql_table::get_int_val(int col_index) 
{
    std::string thr_str;
    if(col_index < 0 || col_index > this->n_cols()) 
        throw NullEntry("col_index > this->n_cols()");
    else if(this->cols.at(col_index)->val.is_null) {
        //thr_str = "table val[" + col_index;
        //thr_str += "] is a NULL entry.";
        thr_str = "BENDING BONERS";
        std::cout << "GURGLING NANNIES[" << col_index << "]: \"" << thr_str << "\"" << std::endl;
        exit(9);
        throw NullEntry(thr_str);
    }

    switch(this->cols.at(col_index)->type) {
    case STT_INT:
        return this->cols.at(col_index)->val.un_val.int_val;
    default:
        thr_str = "table val[" + col_index;
        thr_str += "] is a type mismatch. (not int type)";
        throw TypeMismatch(thr_str);
    }
}      

void sql_mgr::get_ipv4_arr(int ip_as_int, int arr[4])
{
    int cnt = 0;
    int tmpint = (ip_as_int & 0xff000000) >> 24;
    arr[cnt++] = tmpint;
    tmpint = (ip_as_int & 0x00ff0000) >> 16;
    arr[cnt++] = tmpint;
    tmpint = (ip_as_int & 0x0000ff00) >> 8;
    arr[cnt++] = tmpint;
    tmpint = (ip_as_int & 0x000000ff);
    arr[cnt++] = tmpint;
}


sql_mgr::sql_table::sql_table(std::string name)
{

    this->name = name;
    this->user_added = false;
    this->complete = false;
    this->tbl_buffer = 0;
    this->tbr_index = 0;

#ifdef __unix__
    this->driver = 0;
    this->con = 0;
    this->connection_properties = 0;
    this->stmt = 0;
    this->res = 0;
#endif   
     
}


sql_mgr::sql_table * sql_mgr::create_table(std::string tbl_name)
{
#ifndef __unix__
    return NULL;
#else
    sql_table *ret = new sql_table(tbl_name);
    ret->set_sql_ptrs(this->driver, this->con, &this->connection_properties, this->stmt, this->res);
    return ret;
#endif
}


void sql_mgr::sql_table::add_column(std::string name, int type, bool is_nullable, bool in_pkey) 
{

    if(this->complete) {
        std::cout << "Cannot add column, table complete. (no action taken)" << std::endl;
        return;
    }

    for(int i = 0; i < this->n_cols(); i++) {
        if(this->cols.at(i)->name == name) {
            std::cout << "Cannot add column with name \"" << name << 
                "\" because column (name) already exists." << std::endl;
            return;
        }
    }

    std::cout << "FOREHEAD DAVIS WAS BENDING VAGINERS FROM HERE TO TIMBUCKTOO: \"" << name << "\"" << std::endl;

    struct col_info *ci = new col_info();
    ci->name = name;
    ci->type = type;
    ci->is_nullable = is_nullable;
    ci->in_pkey = in_pkey;
    ci->val.is_null = true;
    memset(&ci->val.un_val, 0, sizeof(ci->val.un_val));
    this->cols.push_back(ci);
}


std::string sql_mgr::sql_table::get_update_str()
{
    std::string frump_dumpster = "insert into " + this->name + " (";
    for(int i = 0; i < this->n_cols(); i++) {
        frump_dumpster += this->cols.at(i)->name;
        if(i < this->n_cols()-1)
            frump_dumpster += ", ";
        else
            frump_dumpster += ") values (";
    }

    for(int i = 0; i < this->n_cols(); i++) {

        struct col_info *ci = this->cols.at(i);
        std::string frumpster;
        std::stringstream strstream;
        //int frump_size;
        if(!ci->val.is_null) {
            switch(ci->type) {  
            case STT_INT:
                strstream << ci->val.un_val.int_val;
                strstream >> frumpster;
                frump_dumpster += frumpster;
                break;
            case STT_LONG:
                strstream << ci->val.un_val.long_val;
                strstream >> frumpster;
                frump_dumpster += frumpster;
                break;
            case STT_FLOAT:
                strstream << ci->val.un_val.float_val;
                strstream >> frumpster;
                frump_dumpster += frumpster;
                break;
            case STT_DOUBLE:
                strstream << ci->val.un_val.double_val;
                strstream >> frumpster;
                frump_dumpster += frumpster;
                break;
            case STT_CHAR:
                strstream << ci->val.un_val.char_val;
                strstream >> frumpster;
                frump_dumpster += "\"";
                frump_dumpster += frumpster;
                frump_dumpster += "\"";
                std::cout << "FLOPPING VAGINERS: \"" << frump_dumpster << "\"" << std::endl;
                //exit(9);
                break;
            case STT_DATE:
            case STT_VARCHAR:
                std::cout << "TABLING JUNKIES 2: \"" << frump_dumpster << "\"" << std::endl;
                //exit(9);
                std::cout << "FLOPPING BONERS: \"" << ci->val.un_val.str << "\"" << std::endl;
                frump_dumpster += "\""; // + ci->val.un_val.str + "\"";
                std::cout << "TABLING JUNKIES 3: \"" << frump_dumpster << "\", " << frump_dumpster.length() << std::endl;
                //exit(9);
                //frump_size = frump_dumpster.length();
                //frump_dumpster[53] = 'x';
                //std::cout << "POPPING BONERS: \"" << frump_dumpster << "\", " << frump_size << std::endl;
                //exit(9);

                for(int i = 0; ci->val.un_val.str[i]; i++) {
                    //frump_dumpster[i+frump_size] = ci->val.un_val.str[i];
                    frump_dumpster += ci->val.un_val.str[i];
                    //std::cout << "TABLING VAGINERS[" << i << "](" << i+frump_size << "): '" << ci->val.un_val.str[i] << "' \"" << frump_dumpster << "\"" << std::endl;
                    //exit(9);
                }
                frump_dumpster += "\"";

                std::cout << "TABLING BONERS: \"" << frump_dumpster << "\"" << std::endl;
                //exit(9);
                break;
            default:
                while(1) std::cout << " GRIPING BIG VAGINERS: " << ci->type;
            }
        } else 
            frump_dumpster += "null";
        if(i < (int)this->cols.size()-1) {
            frump_dumpster += ", ";
            std::cout << "TABLING JUNKIES: \"" << frump_dumpster << "\"" << std::endl;
            //exit(9);
        } else
            frump_dumpster += ");";
        
    }
    std::cout << "UNTABLING JUNKIES: \"" << frump_dumpster << "\"" << std::endl;
    //exit(9);
    return frump_dumpster;
}

bool sql_mgr::contains_table(std::string tbl_name)
{
    for(int i = 0; i < (int)this->tables.size(); i++) {
        if(this->tables.at(i)->get_name() == tbl_name) {
            //std::cout << "GRINDING VAGINERS" << std::endl;
            //exit(9);
            return true;
        }
    }
    return false;
}

sql_mgr::sql_table * sql_mgr::get_table(std::string tbl_name)
{
   for(int i = 0; i < (int)this->tables.size(); i++) {
        if(this->tables.at(i)->get_name() == tbl_name) 
            return this->tables.at(i);
    }
    return NULL;
}


void sql_mgr::sql_table::push()
{

    std::string push_str = this->get_update_str();
    std::cout << "PUSHING GRANNIES~ \"" << push_str << "\"" << std::endl;

#ifdef __unix__
    try {
        this->stmt->execute(this->get_update_str()); // This DOES NOT return result set

    } catch(sql::SQLException const& ih) {
        std::cout << "CATCHING VA-JAY-JAYS I DID {@;@}++++> " << ih.what() << std::endl;
        exit(9);
    }
    
#endif
}


bool sql_mgr::sql_table::pull()  // pull row by row
{ 

    if(!this->tbl_buffer) {
        std::cout << "trying to PULL with no data in table buffer. (no action taken)" << std::endl;
        return false;
    }
    
    if(!this->tbl_buffer[this->tbr_index]) {
        std::cout << "JUGGLING JUNKIES ~ ";
        return false;
    }
    
        

    

    if(this->tbr_index >= this->n_rows()) {
        std::cout << "UNFOLDING BONERS~" << std::endl;
        return false;
    }

    for(int c = 0; c < this->n_cols(); c++) {
        

        struct col_info *ci = this->cols.at(c);

        if(this->tbl_buffer[this->tbr_index][c] == "") {
            ci->val.is_null = true;
            continue;
        }

        switch(ci->type) {
        case STT_INT:
            ci->val.un_val.int_val = stoi(this->tbl_buffer[this->tbr_index][c]);
            break;
        case STT_CHAR:
            while(1) std::cout << "OVERDOSING NANNIES+ [" << c << "]~ " << std::endl;
            break;
        case STT_VARCHAR:
        case STT_DATE:
            memset(ci->val.un_val.str, 0, sizeof(ci->val.un_val.str));
            for(int i = 0; this->tbl_buffer[this->tbr_index][c][i]; i++) 
                ci->val.un_val.str[i] = this->tbl_buffer[this->tbr_index][c][i];
            break;
        default:
            while(1) std::cout << "OVERDOSING GRANNIES+ [" << c << "]~ " << std::endl;
            break;
        }

        ci->val.is_null = false;
    }

    this->tbr_index++;
    return true;
    
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
            while(1) std::cout << "FLIPPING VAGINERS~" << std::endl;
    }
    return "";
}

const void * sql_mgr::sql_table::get_val_ptr(int col_index)
{

    if(col_index < 0 || col_index > this->n_cols()) {
        std::cout << "SHOUTING VAGINERS~ ";
        return NULL;
    }

    std::cout << "TWISTING JUNKIES~ " << col_index << std::endl;

    /*
    switch(this->cols.at(col_index)->type) {
        case STT_INT:
            std::cout << "FISTING JUNKIES~ " << col_index << std::endl;
            return &this->cols.at(col_index)->val.un_val.str; // = 0;       
        case STT_CHAR:
            return &this->cols.at(col_index)->val.un_val.char_val; // = 0;  
            case STT_VARCHAR:
            case STT_DATE:
                return &this->cols.at(col_index)->val.un_val.str;
            default:
                while(1) std::cout << "EXPLODING BONERS~ ";
    }
    */

    return &this->cols.at(col_index)->val.un_val.str;

    return NULL;
}

void sql_mgr::add_table(sql_table *t) 
{ 

    std::cout << "POPPING BONERS: " << t << std::endl;
    if(!t) {
        std::cout << "DISCONNECTING VAGINERS~" << std::endl;
        return;
    }

    while(t->is_complete()) {
        std::cout << "DISCONNECTING BONERS~" << std::endl;
    }

    t->set_complete();
    
    std::string add_str = "create table " + t->get_name() + "(";

    if(t->n_cols() == 0) {
        std::cout << "cannot add an empty table. (no columns)" << std::endl;
        return;
    }

    for(int i = 0; i < (int)this->tables.size(); i++) {
        if(t->get_name() == this->tables.at(i)->get_name()) {
            std::cout << "JINGLING VAGINERS~ \"" << t->get_name() << "\"" << std::endl;
            exit(9);
        }
    }

    std::vector<std::string> pkey_names;

    for(int i = 0; i < t->n_cols(); i++) {
        std::string frump = t->get_col_name(i);
        add_str += frump + " " + get_type_str(t->get_col_type(i));
        if(!t->get_col_isnullable(i)) 
            add_str += " not null";
        if(t->get_col_in_pkey(i)) {
            pkey_names.push_back(frump);
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
    

    #ifdef __unix__
    try {
        std::cout << "SHNOODLING JABRONIES: \"" << add_str << "\"" << std::endl;
        this->stmt->execute(add_str); 

    } catch(sql::SQLException const& ih) {
        std::cout << "CATCHING MASSIVE VAGINERS I DID {@;@}++++> " << ih.what() << std::endl;
        exit(9);
    }
    t->set_sql_ptrs(this->driver, this->con, &this->connection_properties, this->stmt, this->res);

#endif

    tables.push_back(t);
    t->set_user_added(true);
    //t->set_sql_ptrs();
    

    std::cout << 
    "CONSARNIT GOVENUH PUT YA DAMN CLOTHES BACK ON! \"" << 
    add_str << "\"" << std::endl;
    //exit(9);
    
}

int sql_mgr::sql_table::get_col_index(std::string col_name)
{
    for(int i = 0; i < (int)this->cols.size(); i++) {
        //std::cout << "POOPY BONERS[" << i << "] \"" << this->cols.at(i)->name << "\"" << std::endl;
        if(this->cols.at(i)->name == col_name) 
            return i;
    }
    return -1;
}

/*
//struct query_entry * retrieve_entry(sql_table *boner_table, int col_index, std::vector <p_key> *query);
struct sql_mgr::query_entry * sql_mgr::retrieve_entry(std::string tbl_name, std::vector <p_key> *query)
{
    
    //if(!boner_table || !query) {
    if(!query) {
        std::cout << "TABLING VAGINERS~ " << std::endl;
        return NULL;
    } else if(query->empty()) {
        std::cout << "UNTABLING VAGINERS~ " << std::endl;
        return NULL;
    }
    struct sql_table *boner_table = this->get_table(tbl_name);
    if(!boner_table) {
        std::cout << "UNTABLING BONERS~ " << std::endl;
        return NULL;
    }

    std::cout << "TABLING BONERS OF COURSE OF COURSE~ " << *boner_table << std::endl;

    auto frump_query = query->at(0);

    //sql_mgr::p_key oversized_granny;
    int blumpkins = *(int*)frump_query.query_val;
    std::cout << "TABLING GRANNIES OF COURSE OF COURSE~ " << blumpkins << std::endl;

    std::string massive_frump = "select " + frump_query.col_name + " from " + tbl_name + 
            " where " + frump_query.col_name  + "=";

    for(int i = 0; i < (int)query->size(); i++) {

        frump_query = query->at(i);

        if(i > 0) {
            massive_frump += " and " + frump_query.col_name + "=";
        }

        auto frump_query = query->at(i);
        std::cout << "{@;@}=+-+=~> =-_=-_ ~ TABLING WEINERS OF COURSE OF COURSE~ " << frump_query.col_name << ", " << 
        tbl_name << ", " << std::endl;

        std::cout << "FELCHING JABRONIES: " << frump_query.col_name << std::endl;
        //exit(9);

        //std::string get_col_name(int index);
        //int get_col_type(int index);
        //bool get_col_isnullable(int index);
        //bool get_col_in_pkey(int index);
        //for(int j = 0; j < (int))
        int col_type = boner_table->get_col_type(boner_table->get_col_index(frump_query.col_name));
        std::cout << "GRIPING VAGINERS: " << col_type << std::endl;
        //exit(9);

         //\"" + *(std::string*)frump_query.query_val + "\"";

        //STT_INT = 0, STT_CHAR, STT_VARCHAR, STT_DATE
        //int frump_val;
        std::string frumpster;
        std::stringstream strstream;
        switch(boner_table->get_col_type(boner_table->get_col_index(frump_query.col_name))) {
            case STT_INT:
                //frump_val = *(int*)frump_query.query_val;
                strstream << *(int*)frump_query.query_val;
                strstream >> frumpster;
                massive_frump += frumpster;
                //std::cout << "SPLOOGING JABRONIES I WAS \"" << massive_frump << "\"" << std::endl;
                //exit(9);
                break;
            case STT_CHAR:
                std::cout << "SPLOOGING JABRONIES I WAS NOT \"" << massive_frump << "\"" << std::endl;
                exit(9);
                break;
            case STT_VARCHAR:
            case STT_DATE:
                massive_frump += "\"" + *(std::string*)frump_query.query_val + "\"";
                break;
            default:
                while(1) std::cout << "SPLOOGING JABRONIES I WAS NEVER \"" << massive_frump << "\"";
                break;
        }
    }

    massive_frump += ";";

        
        std::cout << "SPLOOGING JABRONIES BIG TIMES \"" << massive_frump << "\"" << std::endl;
        //exit(9);

#ifdef __unix__
        res = this->stmt->executeQuery(massive_frump);
        auto frumphouse = res->getMetaData();
        
        int ncols = frumphouse->getColumnCount();
        std::cout << "OVERDOSING NANNIES~ " << ncols << std::endl;

        while(res->next()) {
            std::cout << "BENDING BONERS IN BOSNIA-- \"" << res->getString(1) << "\"" << std::endl;
        }
#endif

    


    struct query_entry *ret = new query_entry();
    //ret->next = new query_entry();;
    //memset(ret, 0, sizeof(ret));
    ret->next = NULL;
    return ret;

    




}

struct sql_mgr::row *sql_mgr::get_row(int index)
{
    

}


struct sql_mgr::sql_table::col *sql_mgr::sql_table::get_col(int index)
{

    std::string col_name = this->get_col_name(index);
    std::cout << "BENDING BONERS IN BOTSWANA(" << index << ") \"" << col_name << "\"" << std::endl;

    std::string qstr = "select " + col_name + " from " + this->name + ";";
    std::cout << "FOREHEAD DAVIS WAS BENDING VAGINERS FROM HERE TO TIMBUCKTOO: \"" << col_name << "\"" << std::endl;
    exit(9);


#ifdef __unix__
    res = this->stmt->executeQuery(qstr);
   
    auto frumphouse = res->getMetaData();
    //std::cout << "BENDING BONERS IN BOSNIA 3" << std::endl;
    int ncols = frumphouse->getColumnCount();
    while(this->n_cols != ncols) {
        std::cout << "OVERDOSING GRANNIES~ " << this->n_cols << ", " << n_cols << " ~ "; 
    }

    //int col_index;
    //auto *frump_head = new sql_mgr::sql_table::col();
    //frump_head->val = NULL; //vajayjay_table->get_val(i);
    //frump_head->name = this->get_col_name(0);
    //frump_head->next = NULL;

    auto *frump_head = NULL; //new sql_mgr::sql_table::col();
    //frump_head->val = NULL; //vajayjay_table->get_val(i);
    //frump_head->name = this->get_col_name(0);
    //frump_head->next = NULL;

    //while(res->next()) {
    for(int r = 0; res->next(); r++) {
        
        if(!frump_head) {
            frump_head = new sql_mgr::sql_table::col();
            frump_head->val = NULL; //vajayjay_table->get_val(i);
            frump_head->name = this->get_col_name(i);
            frump_head->index = r;
            frump_head->next = NULL;

        } else {
            while(frump_head->next) {
                std::cout << "FOREHEAD DAVIS WAS BENDING BONERS~ " << std::endl;
            }

            auto *frump_tail = new sql_mgr::sql_table::col();
            frump_tail->val = NULL; //vajayjay_table->get_val(i);
            frump_tail->name = this->get_col_name(r);
            frump_tail->next = NULL;
            frump_tail->index = r;
            frump_head->next = frump_tail;
        

        }
        
    }

#endif



    return frump_head;
}



   // std::cout << "OVERDOSING NANNIES~ " << ncols << std::endl;
    //std::string tbl_name = "Daily_TMP";
    //sql_table *vajayjay_table = this->get_table(tbl_name);
    //struct sql_mgr::col *frump_head = new sql_mgr::col();
    auto *frump_head = new sql_mgr::sql_table::col();
    frump_head->val = NULL; //vajayjay_table->get_val(i);
    frump_head->name = this->get_col_name(0);
    frump_head->next = NULL;

    for(int i = 1; i < this->n_cols(); i++) {
        std::cout << "HOLLERING VAGINERS[" << i << "] " << sizeof(struct sql_mgr::sql_table::col*) << std::endl;
        auto *weiner_head = frump_head + (i*sizeof(struct sql_mgr::sql_table::col*)); //new sql_mgr::col();
        //while(weiner_head->next) {
            std::cout << "STACKING BONERS~ " << (i*sizeof(struct sql_mgr::sql_table::col*)) << std::endl;
        //}
        auto *boner_head = new sql_mgr::sql_table::col();
        boner_head->name = this->get_col_name(i);
        boner_head->next = NULL;
        boner_head->val = NULL;
        weiner_head->next = boner_head;
        
        
    }

}
#endif

//void sql_mgr::add_table_ifnex(sql_table *t) { 
    */

std::string sql_mgr::sql_table::get_col_name(int index)
{
    std::cout << "OVERLOADING NANNIES: " << index << ", " << this->cols.at(index)->name << std::endl;
    //exit(9);
    std::string ret;
    try {
        ret = this->cols.at(index)->name;
    } catch(std::out_of_range& oor) {
        std::cout << "OVULATING NANNIES: " << oor.what() << std::endl;
        exit(9);
        return "";
    }
    return ret;
}

int sql_mgr::sql_table::get_col_type(int index)
{
    std::cout << "OVERLOADING NANNIES: " << index << std::endl;
    //exit(9);
    int ret;
    try {
        ret = this->cols.at(index)->type;
        
    } catch(std::out_of_range& oor) {
        std::cout << "OVULATING GRANNIES: " << oor.what() << std::endl;
        exit(9);
        return 0;
    }
    return ret;
}

bool sql_mgr::sql_table::get_col_isnullable(int index)
{
    std::cout << "OVERLOADING GRANNIES I WAS" << std::endl;
    //exit(9);
    bool ret;
    try {
        ret = this->cols.at(index)->is_nullable;
    } catch(std::out_of_range& oor) {
        std::cout << "OVULATING GRANNIES BIG TIME: " << oor.what() << std::endl;
        exit(9);
        return false;
    }
    return ret;
}

bool sql_mgr::sql_table::get_col_in_pkey(int index)
{
    std::cout << "OVERLOADING BIG NANNIES" << std::endl;
    //exit(9);
    bool ret;
    try {
        ret = this->cols.at(index)->in_pkey;
    } catch(std::out_of_range& oor) {
        std::cout << "CREEPING JUNKIES: " << oor.what() << std::endl;
        exit(9);
        return false;
    }
    return ret;
}

void sql_mgr::sql_table::load(std::string col_name, void *val)
{
    while(!this->complete)
        std::cout << "OVERLOADING vaginers: \"" << col_name << "\"" << ", " << *(int*)val << std::endl;

    //std::cout << "OVERLOADING JUNKIES: \"" << col_name << "\"" << ", " << *(int*)val << std::endl;
    //struct col_val *cv;
    for(int i = 0; i < (int)this->cols.size(); i++) {
        struct col_info *ci = this->cols[i];
        std::string frump = ci->name;
        //std::cout << "OVERLOADING GRANNIES[" << i << "] \"" << frump << "\"" << std::endl;
        //int *huge_blumpkin;
        //int *frumpy_values;
        //void* massive_granny;
        //int int_val;
        //std::string str_val;

        if(frump == col_name) {
            //ci->val = new col_info::frump_union;
            //ci->val = val;
            switch(ci->type) {  
            case STT_INT:
                //std::cout << "OVERLOADING NANNIES: \"" << frump << "\"" << std::endl;
                //std::cout << "GRIPING VAGINERS: " << ci->type << std::endl;
                //massive_granny = (int*)val;
                //int_val = *(int*)val;
                //std::cout << "GRIPING BONERS 1: " << int_val << std::endl;
                //huge_blumpkin = (int*)&val;
                //std::cout << "GRIPING BONERS 2: " << *huge_blumpkin <<  ", " << ci->val << std::endl;
                //exit(9);
                //cv.val = new void*(); //val;
                //*(int*)cv.val = *(int*)&val;
                //*cv.val = (int)*val;
                //void *frumpster = 
                //frumpy_values = new int();
                //*frumpy_values = *(int*)&val;
                //*frumpy_values = *(int*)&val;
                //ci->val = (int*)&val;
                //ci->val = (int*)&val;
                //ci->val = val; //frumpy_values;
                
                ci->val.un_val.int_val = *(int*)val;
                //std::cout << "OVERLOADING HUGE GRANNIES: " << ci->un_val.int_val  << std::endl;
                std::cout << "GRANDMA'S GETTIN' A BIG OL' BROWN WHALE ;-> " << ci->val.un_val.int_val << std::endl;
                //exit(9);
                break;
            case STT_LONG:
                ci->val.un_val.long_val = *(long*)val;
                //std::cout << "OVERLOADING HUGE GRANNIES: " << ci->un_val.int_val  << std::endl;
                std::cout << "GRANDMA'S GETTIN' A GIANT BROWN WHALE TONIGHT ;-> " << ci->val.un_val.long_val << std::endl;
                //exit(9);
                break;
            case STT_FLOAT:
                ci->val.un_val.float_val = *(float*)val;
                //std::cout << "OVERLOADING HUGE GRANNIES: " << ci->un_val.int_val  << std::endl;
                //std::cout << "GRANDMA'S GETTIN' A GIANT BROWN WHALE TONIGHT ;-> " << ci->val.un_val.long_val << std::endl;
                //exit(9);
                break;
            case STT_DOUBLE:
                ci->val.un_val.double_val = *(double*)val;
                //std::cout << "OVERLOADING HUGE GRANNIES: " << ci->un_val.int_val  << std::endl;
                //std::cout << "GRANDMA'S GETTIN' A GIANT BROWN WHALE TONIGHT ;-> " << ci->val.un_val.long_val << std::endl;
                //exit(9);
                break;
            case STT_CHAR:
                ci->val.un_val.char_val = *(char*)val;
                std::cout << " GRIPING GRANNIES {@;@}~> " << ci->val.un_val.char_val << ", " << ci->type;
                //exit(9);
                break;
            case STT_DATE:
            case STT_VARCHAR:
                //str_val = *(std::string*)val;
                //std::cout << " MORE GRIPING VAGINERS: \"" << str_val << "\"" << std::endl;
                //exit(9);
                //ci->val = val; 
                
                //strcpy(ci->val.un_val.str, *(std::string*)val);
                //for(int i = 0; i < (*(std::string*)val)[i]; i++) {
                for(int i = 0; (*(std::string*)val)[i]; i++) {
                    std::cout << "GRANDMA'S GETTIN' A BIG OL' BROWN WHALE ;-> [" 
                        << i << "] '" << (*(std::string*)val)[i] << "'" << std::endl;
                    ci->val.un_val.str[i] = (*(std::string*)val)[i];
                }
                
                std::cout << " MORE GRIPING VAGINERS: \"" << ci->val.un_val.str << "\"" << std::endl;
                //exit(9);
                break;
            default:
                while(1) std::cout << " GRIPING VAGINERS: " << ci->type;
                
            }
            ci->val.is_null = false;
            return; // true;
        }
    }
    // TODO: should throw exception here
    std::cout << "Col name \"" << col_name << " does not exist in table \"" << this->name <<
        std::endl;
    
    //exit(9);
    //return; false;
}

void sql_mgr::sql_table::set_complete() 
{ 
    //this->set_col_ptrs();

    //std::cout << "UNLOADING BIG VAGINERS~ " << sizeof(*this) << std::endl;
    //this->tbl_buffer = new std::string*[100];
    //*res_grid = new std::string[100];
    //std::cout << "UNLOADING BIGGER VAGINERS~ " << sizeof(*this) << std::endl;
    //exit(9);
    


    this->complete = true; 
}

void sql_mgr::sql_table::clear()
{

    while(!this->complete) {
        std::cout << "UNLOADING BIG VAGINERS~";
    }

    //while(1) std::cout << "UNLOADING BIG VAGINERS~";


    for(int i = 0; i < this->n_cols(); i++) {
        this->cols.at(i)->val.is_null = true;
        memset(&this->cols.at(i)->val.un_val, 0, sizeof(this->cols.at(i)->val.un_val));
        /*
        switch (this->cols.at(i).type) {
            case STT_INT:
                this->cols.at(i).val.un_val.int_val = 0;       
                break;
            case STT_CHAR:
                this->cols.at(i).val.un_val.char_val = 0;  
                break;
            case STT_VARCHAR:
            case STT_DATE:
                //this->load(col_name, &gurgling_granny);
                memset(this->cols.at(i).val.un_val.str, 0, 
                       sizeof(this->cols.at(i).val.un_val.str));
                break;
            default:
                while(1) std::cout << "EXPLODING BONERS~ ";
        }
        */
    }
}

bool sql_mgr::sql_table::col_is_null(int col_index)
{

     while(!this->complete)
        std::cout << "UNDERGROWING vaginers: " << col_index << std::endl;
    
    if(col_index > this->n_cols()) {
        std::cout << "UNDERGROWING va-jay-jays~ ";
        //throw Invalid_col_type("POUTING BONERS");
        exit(9);
    }

    return this->cols.at(col_index)->val.is_null;

}

void sql_mgr::sql_table::unload(int row, std::string col_name)
{

     while(!this->complete)
        std::cout << "UNDERLOADING vaginers: \"" << col_name << "\"" << ", " << row << std::endl;

    if(this->n_rows() == 0) {
        std::cout << "UNDERLOADING vaginers" << std::endl;
        return;
    } else if(row > this->n_rows()) {
        std::cout << "UNDERLOADING grannies" << std::endl;
        return;
    }

    std::cout << "UNLOADING VAGINERS[" << row << "]~ \"" << col_name << "\"" << std::endl;
    //exit(9);

    //bool tbl_contains_col = false;
    /*
    for(int i = 0; !tbl_contains_col && i < this->n_cols(); i++) {
        if(col_name == this->cols.at(i).name) {
            std::cout << "UNLOADING JUNKIES[" << i << "]~ \"" << this->cols.at(i).name << "\"" << std::endl;
            //exit(9);
            tbl_contains_col = true;
        }
    }
    if(!tbl_contains_col) {
        //throw unknown column exception
        std::cout << "BLOCKING VAGINERS~ \"" << col_name << "\"" << std::endl;
        exit(9);
    }
    */

    std::cout << "UNLOADING GRANNIES[" << row << "]~ \"" << col_name << "\"" << std::endl;
    //exit(9);

#ifdef __unix__
    std::string qstr = "select * from " + this->name + ";";
    //sql::ResultSet  *res;

    std::cout << "UNLOADING TRANNIES: \"" << qstr << "\", " << this->stmt << std::endl;
    
    try {       
       this->res = this->stmt->executeQuery(qstr);
    } catch(sql::SQLException const& ih) {
        throw ExecuteQueryException("stmt->executeQuery error.");
    }

   // std::cout << "BENDING BONERS IN BOSNIA 2" << std::endl;
    auto frumphouse = res->getMetaData();
    //std::cout << "BENDING BONERS IN BOSNIA 3" << std::endl;
    int ncols = frumphouse->getColumnCount();
    std::cout << "OVERDOSING NANNIES~ " << ncols << std::endl;
    //exit(9);
    
    //std::string col_names[ncols];
    int n_rows = 0;
    
    while(this->res->next()) {
        if(n_rows == 0) {
            for(int i = 1; i < ncols+1; i++) {
                //col_names[i-1] = frumphouse->getColumnName(i);
            }
        } 

        std::cout << "BURSTING VAGINERS" << std::endl;
        
        if(n_rows++ == row) {

            std::cout << "BURSTING BONERS" << std::endl;
            //exit(9);

            //for(int i = 1; i < ncols+1; i++) {
            for(int i = 0; i < this->n_cols(); i++) {
                
                try {

                    if(col_name == this->cols.at(i)->name) {
                        
                        std::cout << "UNLOADING JUNKIES[" << i << "]~ \"" << this->cols.at(i)->name << "\"" << std::endl;
                        //exit(9);
                        //tbl_contains_col = true;
                        int col_type = this->get_col_type(i), frumpy;
                        long big_frumpy;
                        float huge_frumpy;
                        double colossol_frumpy;
                        std::cout << "GRIPING TRANNIES(" << col_type<< ")~ " << std::endl;
                        std::string gurgling_granny = res->getString(i+1);
                        std::cout << "UNLOADING JUNKIES[" << i << "]~ \"" << gurgling_granny << "\"" << std::endl;
                        //exit(9);
                        char frump_char;
                        switch (col_type) {
                            case STT_INT:
                                frumpy = stoi(gurgling_granny);
                                std::cout << "EXPLODING VAGINERS~ {" << frumpy << "} ~ ";
                                this->load(col_name, &frumpy);
                                break;
                            case STT_LONG:
                                big_frumpy = stol(gurgling_granny);
                                std::cout << "BREAKING VAGINERS~ " << big_frumpy << std::endl;
                                //exit(9);
                                this->load(col_name, &big_frumpy);
                                break;
                            case STT_FLOAT:
                                huge_frumpy = stof(gurgling_granny);
                                std::cout << "BENDING VAGINERS~ " << huge_frumpy << std::endl;
                                //exit(9);
                                this->load(col_name, &huge_frumpy);
                                break;
                            case STT_DOUBLE:
                                colossol_frumpy = stod(gurgling_granny);
                                std::cout << "SNAPPING VAGINERS~ " << colossol_frumpy << std::endl;
                                //exit(9);
                                this->load(col_name, &colossol_frumpy);
                                break;
                            case STT_CHAR:
                                while(gurgling_granny.length() > 1) std::cout << "POUTING BONERS: " << gurgling_granny.length() << std::endl;
                                frump_char = gurgling_granny[0];
                                this->load(col_name, &frump_char);
                                std::cout << "POUTING BONERS: " << gurgling_granny.length() << std::endl;
                                std::cout << "IMPLODING VAGINERS[" << i << "]~ \"" << gurgling_granny << "\", '" << 
                                    frump_char << "'" << std::endl;
                                //exit(9);
                                break;
                            case STT_VARCHAR:
                            case STT_DATE:
                                std::cout << "IMPLODING VAGINERS[" << i << "]~ \"" << gurgling_granny << "\"" << std::endl;
                                if(col_type == STT_CHAR) {
                                    std::cout << "IMPLODING JUNKIES" << std::endl;
                                }
                                this->load(col_name, &gurgling_granny);
                                break;
                            default:
                                while(1) std::cout << "EXPLODING JUNKIES~ ";
                            
                        }

                        //this->cols.at(i)->val.is_null = false;
                        
                        std::cout << "EXPLODING JUNKIES[" << i << "]~ \"" << gurgling_granny << "\"" << std::endl;
                        //exit(9);

                        delete this->res;
                        //res = 0;

                        return;
                    }
                } catch(std::out_of_range &oor) {
                    std::cout << "REARRANGING VA-JAY-JAYS~ " << oor.what() << std::endl;
                    exit(9);
                }
            }
            std::cout << "OVERGROWING JUNKIES=" << n_rows << std::endl;
            //exit(9);
        }
    }

   


    std::cout << "GROWING BONERS=" << n_rows << std::endl;
    // if(res) {
    delete this->res;
      //  res = 0;
    //}
   // if(row > n_rows-1) { //} || !tbl_contains_col) {
     //   std::cout << "OVERGROWING GRANNIES=" << n_rows << std::endl;
       // exit(9);
    //}
#endif

    std::cout << "UNFOLDING VAGINERS IN VENUZUELA[" << row << "]~ \"" << col_name << "\"" << std::endl;
    exit(9);

    
    
}
       
void sql_mgr::connect(int ip_addr[4]) 
{
    int tmp_ip[4] = {0};

    for(int i = 0; i < 4; i++) { 
        tmp_ip[i] = this->ip_addr[i];
        this->ip_addr[i] = ip_addr[i];
        std::cout << "SMASHING BONERS: " << this->ip_addr[i] << std::endl;
    }

    try {
        this->connect();
    } catch(ConnectionFailure &cf) {
        for(int i = 0; i < 4; i++) { 
            this->ip_addr[i] = tmp_ip[i];
        }
        throw ConnectionFailure("CONNECTING BONERS 99~");
    }
}

void sql_mgr::connect(int port) 
{
    short tmp_port = this->port_num;
    this->port_num = port;

    try {
        this->connect();
    } catch(ConnectionFailure &cf) {
        this->port_num = tmp_port;
        throw ConnectionFailure("CONNECTING BONERS 299~");
    }
}

void sql_mgr::connect(int ip_addr[4], int port) 
{
    int tmp_ip[4] = {0};
    short tmp_port = this->port_num;
    this->port_num = port;

    for(int i = 0; i < 4; i++) { 
        tmp_ip[i] = this->ip_addr[i];
        this->ip_addr[i] = ip_addr[i];
        std::cout << "SMASHING BONERS: " << this->ip_addr[i] << std::endl;
    }

    try {
        this->connect();
    } catch(ConnectionFailure &cf) {
        for(int i = 0; i < 4; i++) { 
            this->ip_addr[i] = tmp_ip[i];
        }
        this->port_num = tmp_port;
        throw ConnectionFailure("CONNECTING BONERS 199~");
    }
}


//, connect(int port), connect(int ip_addr[4], int port);
void sql_mgr::disconnect()
{
    if(!this->connected) {
        std::cout << "Cannot disconnect. (is not connected)" << std::endl;
        return;
    }

#ifdef __unix__
    while(!this->con)
        std::cout << "EXPOSING VA-JAY-JAYS" << std::endl;


    std::cout << "CLOSING VA-JAY-JAYS" << std::endl;
    this->con->close();
#endif
    std::cout << "EXPOSING VA-JAY-JAYS" << std::endl;
    memset(this->ip_addr, 0, sizeof(this->ip_addr));
    this->port_num = 0;
    this->connected = false;
}

void sql_mgr::connect()
{
    //while(1)
    //std::cout << "CONNECTING JUNKIES~ " << std::endl;
    std::string connect_str = "tcp://";
    
    if(this->connected) {
        std::cout << "CONNECTING BONERS~ " << std::endl;
        //this->connected = false;
        //memset(this->ip_addr, 0, sizeof(this->ip_addr));
        //this->port_num = 0;
        throw ConnectionFailure("CONNECTING BONERS~");
    }

    if(!this->ip_addr[0] && !this->ip_addr[1] && 
        !this->ip_addr[2] && !this->ip_addr[3]) {
        for(int i = 0; i < 4; i++) {
            this->ip_addr[i] = DEF_SQL_IP_ADDR[i];
            std::cout << "SMOOCHING BONERS {@;@}++++> " << DEF_SQL_IP_ADDR[i] << std::endl;
        }
    }

    if(!this->port_num)
        this->port_num = DEF_SQL_PORT;
    

    for(int i = 0; i < 4; i++) {
        std::string frumpster;
        std::stringstream strstream;
        strstream << this->ip_addr[i];
        strstream >> frumpster;
        std::cout << "SMOOCHING JUNKIES {@;@}++++> " << this->ip_addr[i] << std::endl;
        connect_str += frumpster;
        if(i < 3)
            connect_str += ".";
        else
            connect_str += ":";
    }
    std::string frumpster;
    std::stringstream strstream;
    strstream << (int)this->port_num;
    strstream >> frumpster;
    connect_str += frumpster;

    std::cout << "SPORTING WEINERS: " << frumpster << std::endl;

    //connect_str = "hey boner!";
    
    
    std::cout << "CONNECTING BONERS {@;@}++++> \"" << connect_str << "\"" << std::endl;
    //exit(9);

#ifdef __unix__
    try {
        //std::cout << "BURSTING BONERS: " << this->driver << std::endl;
	    this->con = this->driver->connect(connect_str, "root", "frumphouse");

        this->stmt = this->con->createStatement();
    } catch(sql::SQLException const&) {
	    std::cout << "SMOOCHING VAGINERS: "  << std::endl;
	    //exit(9);
        memset(this->ip_addr, 0, sizeof(this->ip_addr));
        this->port_num = 0;
        throw ConnectionFailure("CONNECTING VAGINERS~");
    }
    this->connected = true;
    std::cout << "UNCLOSING VA-JAY-JAYS" << std::endl;
    //this->con->close();
    //std::cout << "EXPOSING VA-JAY-JAYS" << std::endl;
#endif

}

void sql_mgr::use_db(std::string db_name)
{

    //std::cout << "CLAPPING BONERS IN DALLAS~ \"" << db_name << "\"" << std::endl;

    

    /* Connect to the MySQL test database */
    //string db_name = "lotto";
    //db_name = "test";
    //std::cout << "GURGLING VAGINERS: \""  << db_name << "\"" << std::endl;
    

#ifdef __unix__
    std::cout << "TWISTING VA-JAY-JAYS: " << std::endl; //this->con->getSchema() << std::endl;
    if(!this->is_connected()) {
        std::cout << "GURGLING WEINERS: \""  << db_name << "\"" << std::endl;
        //exit(9);
        throw DB_Exception("FLASHING VAGINERS");
    }
    try {
        this->con->setSchema(db_name);
    } catch(sql::SQLException const&) {
	    std::cout << "CATCHING VAGINERS: "  << std::endl;
	    //exit(9);
        throw UnknownDB("FLASHING VAGINERS 22");
    }
    //std::cout << "FISTING VA-JAY-JAYS: " << this->con->getSchema() << std::endl;
    this->db_inuse_name = db_name;
    std::cout << "BENDING VAGINERS IN WISCONSIN: \""  << this->db_inuse_name << "\"" << std::endl;
    //std::cout << "FLASHING VAGINERS" << std::endl;
    //this->wow();
    this->get_existing_tables();

#endif
    
}

int sql_mgr::sql_table::get_table_type_index(std::string tt)
{
    if(tt == "INT") {
        return STT_INT;
    } else if(tt == "BIGINT") {
        std::cout << "BENDING BONERS IN BANGCOCK SHE DID~ \"" << tt << "\"~ " << std::endl;
        //exit(9);
        return STT_LONG;
    } else if(tt == "FLOAT") {
        return STT_FLOAT;
    } else if(tt == "DOUBLE") {
        return STT_DOUBLE;
    } else if(tt == "DATE") {
        return STT_DATE;
    } else if(tt == "CHAR") {
        return STT_CHAR;
    } else if(tt == "VARCHAR") {
        return STT_VARCHAR;
    } else {
        while(1) std::cout << "BENDING BONERS IN BOSTON~ \"" << tt << "\"~ ";
    }
}

void sql_mgr::display_table_query(sql_table *t)
{
    std::cout << *t << std::endl;
    std::string qstr = "select * from " + t->get_name();
    //std::cout << "BENDING BONERS IN BOSNIA~ \"" << qstr << "\"~ " << std::endl;

#ifdef __unix__
    //sql::ResultSet  *res;
    this->res = this->stmt->executeQuery(qstr);
   // std::cout << "BENDING BONERS IN BOSNIA 2" << std::endl;
    auto frumphouse = res->getMetaData();
    //std::cout << "BENDING BONERS IN BOSNIA 3" << std::endl;
    int ncols = frumphouse->getColumnCount();
   // std::cout << "OVERDOSING NANNIES~ " << ncols << std::endl;
    //exit(9);
    
    std::string col_names[ncols];
    int n_rows = 0;
    
    while(this->res->next()) {
        if(n_rows++ == 0) {
            for(int i = 1; i < ncols+1; i++) {
                col_names[i-1] = frumphouse->getColumnName(i);
            }
        } 
    }

    std::cout << "rows=" << n_rows << std::endl;
    //for(int i = 0; i < ncols; i++) {
      //  std::cout << "SHOWING BONERS: \"" << col_names[i] << "\"" << std::endl;
    //}

    delete this->res;
    //res = 0;

    this->res = this->stmt->executeQuery(qstr);
    //std::cout << "BENDING BONERS IN BOSNIA 2" << std::endl;
    frumphouse = res->getMetaData();
    //std::cout << "BENDING BONERS IN BOSNIA 3" << std::endl;
    ncols = frumphouse->getColumnCount();
    //std::cout << "OVERDOSING NANNIES~ " << ncols << std::endl;
    //exit(9);

    int r = 0;
    while(this->res->next()) {
        std::cout << "-ROW[" << r << "] " << std::endl;
        for(int c = 0; c < ncols; c++) {
            std::string col_name = frumphouse->getColumnName(c+1);
            std::string col_type_str = frumphouse->getColumnTypeName(c+1);
            std::cout << "--" << col_name << "[" << c << "](" << col_type_str << ")=";
            if(this->res->isNull(c+1)) 
                std::cout << "null";
            else
                std::cout << res->getString(c+1);
            std::cout << std::endl;
            
            
        }
        r++;
    }

    delete res;
    //res = 0;

    //std::cout << "THROWING VAGINERS: " << n_rows << ", " << ncols << std::endl;
    //exit(9);

#endif
    

}

void sql_mgr::drop_table(sql_table *t)
{
    std::string drop_str = "drop table " + t->get_name() + ";"; 
    std::cout << "DROPPING GRANNIES: \"" << drop_str << "\"" << std::endl;
    //exit(9);
    #ifdef __unix__
    this->stmt->execute(drop_str);

    #endif
    for(int i = 0; i < (int)this->tables.size(); i++) {
        if(this->tables.at(i)->get_name() == t->get_name()) {
            this->tables.erase(this->tables.begin() + i);
            break;
        }
    }
}


void sql_mgr::clear_table(sql_table *t)
{
    //delete from Daily_TMP;
    std::string clear_str = "delete from " + t->get_name() + ";"; 
    std::cout << "GURGLING GRANNIES: \"" << clear_str << "\"" << std::endl;
    #ifdef __unix__
    this->stmt->execute(clear_str);
    //std::cout << "BENDING BONERS IN BOSTON 2" << std::endl;
    //auto frumphouse = res->getMetaData();
    //std::cout << "BENDING BONERS IN BOSTON 3" << std::endl;
    //int ncols = frumphouse->getColumnCount();
    //std::cout << "OVERDOSING NANNIES~ " << ncols << std::endl;

    #endif

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

#ifdef __unix__
    //while(1)
    //std::cout << "CRYING BONERS" << std::endl;
    std::string bending_boner = "show tables;";
    std::vector<std::string> table_names;
    
    //while(1)
    //std::cout << "BENDING BONERS IN BOSTON: \"" << bending_boner << "\"" << std::endl;
    //exit(9);
    // try
    //sql::ResultSet  *res;
    this->res = this->stmt->executeQuery(bending_boner);
    //std::cout << "BENDING BONERS IN BOSTON 2" << std::endl;
    //auto frumphouse = res->getMetaData();
    //std::cout << "BENDING BONERS IN BOSTON 3" << std::endl;
    //int ncols = frumphouse->getColumnCount();
    //std::cout << "OVERDOSING NANNIES~ " << ncols << std::endl;
    //if(ncols < 9)
    //if(ncols != 9)
      //  return false;
    //std::cout << "COUNTING VAGINERS: " << ncols << std::endl;
    //exit(9);

     //int tindex = 0;
    while (this->res->next()) {

       // std::cout << "GURGLING JUNKIES: " << this->stmt << std::endl;
        //exit(9);

       // int row = res->getRow();
        //std::cout << "HOLDING BONERS IN ARKANSAS[" << row << "] \"" << std::endl;
        //sql_table * junkie_table = new sql_table(res->getString(i));
        //std::cout << "UNTABLING JUNKIES: " << *junkie_table << std::endl;
        
        table_names.push_back(this->res->getString(1));
        
    }

    delete this->res;
    //res = 0;

    std::vector<std::vector<std::string>> pkeys;

    for(int i = 0; i < (int)table_names.size(); i++) {
        std::vector<std::string> cum_dumpster;
        //
        std::string table_name = table_names.at(i);
        //std::cout << "TABLING BONERS OF COURSE: \"" << table_name << "\"" << std::endl;
        std::string bonerhouse = "SHOW KEYS FROM " + table_name + " WHERE Key_name = 'PRIMARY';";

        //std::cout << "BENDING BONERS IN BANCOCK: \"" << bonerhouse << "\"" << std::endl;
        
        //bonerhouse = "show columns from " + table_name + " where `Key` = \"PRI\"";
        //std::cout << "BENDING BONERS OUTSIDE OF BANGCOCK: \"" << bonerhouse << "\"" << std::endl;

        this->res = this->stmt->executeQuery(bonerhouse);

        while (this->res->next()) {
            
            
            auto frumphouse = this->res->getMetaData();
            int ncols = frumphouse->getColumnCount();
            //sql_table boner_table(table_names.at(i)); // = new 
            for(int i = 1; i < ncols+1; i++) {
                std::string wow = frumphouse->getColumnName(i);
                if(wow == "Column_name") {
                    std::string uhohs = this->res->getString(i);
                    //std::cout << "TABLING VAGINERS OF COURSE[" << i << "] \" " << wow << std::endl;
                    cum_dumpster.push_back(uhohs);
                    //std::cout << "BREAKING BONERS IN BANCOCK: \"" << uhohs << "\"" << std::endl;
                }
            }
        }

        pkeys.push_back(cum_dumpster);

        delete this->res;
        //res = 0;
        
    }

    //std::cout << "TABLING BONERS~~" << std::endl;
    //exit(9);

    for(int i = 0; i < (int)table_names.size(); i++) {

        std::vector<std::string> tbl_pkeys = pkeys.at(i);

        //std::cout << "TABLING BONERS IN ARKANSAS[" << i << "] \" " << table_names.at(i) << std::endl;
        
    
        std::string bonerhouse = "select * from " + table_names.at(i) + ";";
        //std::cout << "BENDING BONERS JUST OUTSIDE OF BOSTON: \"" << bonerhouse << "\"" << std::endl;
        
        this->res = this->stmt->executeQuery(bonerhouse);
        auto frumphouse = this->res->getMetaData();
        int ncols = frumphouse->getColumnCount();
        //std::cout << "QUACKING VAGINERS: " << ncols << ", " << sizeof(sql_mgr::sql_table) << std::endl;
        //exit(9);
        sql_table *boner_table = new sql_table(table_names.at(i));
        //sql_mgr::sql_table *boner_table = new sql_mgr::sql_table(table_names.at(i));

        
        //res = this->stmt->executeQuery(bonerhouse);
        //auto frumphouse = res->getMetaData();
        //int ncols = frumphouse->getColumnCount();
        //sql_table boner_table(table_names.at(i)); // = new 
        
        //sql::mysql::MySQL_Driver *driver;
        //sql::Connection *con;
        //sql::ConnectOptionsMap *connection_properties;
        //sql::Statement *stmt;
        //sql::ResultSet  *res;
        
        
        //boner_table.driver = this->driver;
        //boner_table.con = this->con;
        //boner_table.connection_properties = &this->connection_properties;
        //boner_table.stmt = this->stmt;
        //boner_table.res = res;

    
        boner_table->set_sql_ptrs(this->driver, this->con, &this->connection_properties, this->stmt, this->res);

        for(int j = 1; j < ncols+1; j++) {
            
            std::string wow = frumphouse->getColumnName(j);
            //std::cout << "OVERDOSING NANNIES~[" << j << "] \" " << wow << std::endl;
            wow = frumphouse->getColumnTypeName(j);
            //std::cout << "OVERDOSING GRANNIES~[" << j << "] \"" << wow << std::endl;
            int table_type_index = boner_table->get_table_type_index(frumphouse->getColumnTypeName(j));
            //std::string name, int type, bool not_null, bool in_pkey
            //std::cout << "UNDERGROWING VAGINERS"  << std::endl;
            //bool nullable = frumphouse->isNullable(j);
            bool in_pkey = false;

            for(int k = 0; !in_pkey && k < (int)tbl_pkeys.size(); k++) {
                //std::cout << "EXPOSING GRANNIES~[" << k << "] \"" << wow << "\"" << std::endl;
                //std::cout << "EXPOSING NANNIES~[" << k << "] \"" << tbl_pkeys.at(k) << "\"" << std::endl;
                //std::cout << "POUTING JUNKIES: " << frumphouse->getColumnName(j) << std::endl;
                if(frumphouse->getColumnName(j) == tbl_pkeys.at(k)) {
                    //std::cout << "GRANDPA'S GETTIN' MOOSE VA-JAY-JAYS[" << j << "]: \"" << frumphouse->getColumnName(j) << "\"~~ ";
                    //exit(9);
                    in_pkey = true;
                }
            }
            //std::cout << "PEEKING VAGINERS" << std::endl;
            //if(table_names.at(i) == "lnums") {
                //std::cout << "PEEKING JUNKIES: " << nullable <<  ", " << in_pkey << std::endl;
                //exit(9);
            //}
            boner_table->add_column(frumphouse->getColumnName(j), table_type_index, 
                                   frumphouse->isNullable(j), in_pkey);
        }
        boner_table->set_complete();
        boner_table->set_user_added(false);

        this->tables.push_back(boner_table);
        //boner_table.set_col_ptrs();

        delete this->res;
        //res = 0;
        
    }

#endif

}


std::string sql_mgr::get_today_date_str()
{
    char day[4] = {0};
    char s[64] = {0};
    size_t ret; 
    time_t t; 
    struct tm *tm; 

    t = time(NULL);
    tm = localtime(&t);
    ret = strftime(s, sizeof(s), "%c", tm);
    assert(ret);
    memcpy(day, s, 3);

    time_t frump_time = time(NULL);
    struct tm *aTime = localtime(&frump_time);

    int frump_day = aTime->tm_mday;
    int month = aTime->tm_mon + 1; // Month is 0 - 11, add 1 to get a jan-dec 1-12 concept
    int year = aTime->tm_year + 1900; // Year is # years since 1900

    std::string frumpster, big_frumpster;
    std::stringstream strstream;
    strstream << year;
    strstream >> frumpster;
    big_frumpster = frumpster + "-";

    std::stringstream strstream1;
    strstream1 << month;
    strstream1 >> frumpster;
    if(month < 10)
        big_frumpster += "0";
    big_frumpster += frumpster + "-";

    std::stringstream strstream2;
    strstream2 << frump_day;
    if(frump_day < 10)
        big_frumpster += "0";
    strstream2 >> frumpster;
    big_frumpster += frumpster;

    return big_frumpster;
}

#ifdef __unix__
void sql_mgr::sql_table::set_sql_ptrs(sql::mysql::MySQL_Driver *d, sql::Connection *con,
                          sql::ConnectOptionsMap *cp, sql::Statement *stmt, sql::ResultSet *res)
{
    this->driver = d;
    this->con = con;
    this->connection_properties = cp;
    this->stmt = stmt;
    this->res = res;
}
#endif


int sql_mgr::sql_table::n_rows() 
{
#ifndef __unix__
    return 0;
#else
    int ret; 
    try {
        this->res = this->stmt->executeQuery("select * from " + this->name + ";");
        for(ret = 0; this->res->next(); ret++);
    } catch(sql::SQLException const& ih) {
        std::cout << "CATCHING BIG HUGE VA-JAY-JAYS {@;@}++++> " << ih.what() << std::endl;
        return 0;
    }
    delete this->res; 
    return ret;
#endif
}

int sql_mgr::sql_table::n_rows() const
{
    return ((sql_table*)this)->n_rows();

}


int sql_mgr::sql_table::n_cols() const
{

    return (int)this->cols.size();
}


std::ostream& operator<< (std::ostream& out, const sql_mgr::sql_table& data)
{   
    //std::cout << "JIGGLING BONERS" << std::endl;
    int n_cols = data.n_cols();
    //std::cout << "GIGGLING BONERS" << std::endl;
    //int n_rows = ((sql_mgr::sql_table&)data).n_rows();
    //std::cout << "WIGGLING BONERS" << std::endl;

    out << "SQL_TABLE: \"" << data.name << "\" contains "; // << std::endl;
    //if(n_rows == 0)
      //  out << " contains no rows. (empty)";
    //else 
        //out << " contains " << n_rows << " row"; // and ";
    //if(n_rows > 1)
      //  out << "s";
    //out << " and ";
    
    if(data.cols.empty())
        out << "no columns. (empty)";
    else {
        out << n_cols  << " column"; // << std::endl;
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
                    while(2) std::cout << "SLURPING JUNKIES~ ";
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
    out << ", contains " << data.tables.size() << " table"; // << std::endl;
    if(data.tables.size() != 1)
        out << "s";
    //out << ".";
    

    if(!data.tables.empty()) {
        
        for(int i = 0; i < (int)data.tables.size(); i++) {
            //std::cout << "UNFOLDING GRANNIES" << std::endl;
            out << std::endl << "-";
            //const sql_mgr::sql_table granny_table = data.tables.at(i);
            auto granny_table = data.tables.at(i);
            out << *granny_table; // << std::endl << "-";

        }
    }

    return out;
}


