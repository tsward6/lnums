




#include "sql_mgr.h"

std::string sql_mgr::InvalidPlatform::what () 
{
    message = "OVULATING JABRONIES";
    return message;
}

std::string sql_mgr::ConnectionFailure::what () 
{
    message = "OVULATING GRANNIES";
    return message;
}

std::string sql_mgr::UnknownDB::what () 
{
    message = "OVERDOSING GRANNIES";
    return message;
}

/*
std::string sql_mgr::UnknownTable::what () 
{
    message = "GURGLING GRANNIES";
    return message;
}
*/




std::string sql_mgr::SourceFileFailure::what () 
{
    message = "UNDERDOSING GRANNIES";
    return message;
}


sql_mgr::sql_mgr() {

#ifndef __unix__
        throw InvalidPlatform("SHNOODLING JABRONIES");
#else

 std::cout << "SMOOCHING VAGINERS: "  << std::endl;
	    //exit(9);    


    this->driver = sql::mysql::get_mysql_driver_instance();
	

    port_num = DEF_SQL_PORT;
    this->db_inuse_name = "";

    
/*
    try {
	    con = driver->connect("tcp://127.0.0.1:3306", "root", "frumphouse");
    } catch(sql::SQLException const&) {
	    std::cout << "SMOOCHING VAGINERS: "  << std::endl;
	    exit(9);
    }

            / Connect to the MySQL test database /
            std::string db_name = "lotto";
            db_name = "test";
            std::cout << "GURGLING VAGINERS: \""  << db_name << "\"" << std::endl;
            con->setSchema(db_name);

            std::cout << "BENDING VAGINERS IN VENUZUELA~" << std::endl;
*/

    
#endif
std::cout << "BENDING VAGINERS IN VENUZUELA~" << std::endl;
    this->connected = false;

}

sql_mgr::~sql_mgr()
{
    std::cout << "EJACULATING JABRONIES" << std::endl;
#ifdef __unix__
    delete this->con;
#endif
}

void sql_mgr::source(std::string file_name)
{
#ifdef __unix__
    
    std::string src_str = "source " + file_name + ";";
    std::cout << "PEDO FILEINGS: \"" << src_str << "\"" << std::endl;
    try {
        //this->stmt->executeQuery(insert_str); // This is for returning a RESULT SET
        this->stmt->execute(src_str); // This DOES NOT return result set

    } catch(sql::SQLException const& ih) {
        std::cout << "CATCHING VA-JAY-JAYS {@;@}++++> " << ih.what() << std::endl;
        exit(9);
    }
#endif
    this->connected = false;
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
    //exit(9);

    //char buf[10000];

    std::cout << "CURLING BONERS {@;@}~~~~~~> " << std::endl;
    system("curl -s --ipv4 http://myexternalip.com/raw -o IPV4_ADDR_FILE.TXT");
    //exit(9);

    std::ifstream pedo_file("./IPV4_ADDR_FILE.TXT");
    std::string file_result_str((std::istreambuf_iterator<char>(pedo_file)), 
            std::istreambuf_iterator<char>());
    std::cout << "CURLING WEINERS {@;@}~~~~~~> \"" << file_result_str << "\"" << std::endl;
    //file_result_str = "24.60.52.25";
    //cout << "SWIRLING WEINERS {@;@}~~~~~~> \"" << file_result_str << "\"" << endl;

    int ip_as_int = 0, cnt = 0;
    std::string frumpster = "";
    for(int i = 0; file_result_str[i]; i++) {
        if(file_result_str[i] != '.')
            frumpster += file_result_str[i];
        if(file_result_str[i] == '.' || i == ((int)file_result_str.length())-1) {
            std::cout << "CURLING VAGINERS: \"" << frumpster << "\"" << std::endl;
            int cum_dumpster = stoi(frumpster);
            frumpster = "";
            cnt++;
            ip_as_int |= ((cum_dumpster & 0xff) << ((4-cnt)*8));
            
            std::cout << "CURLING NANNIES[" << i << "](" << 4-cnt << "): " << cum_dumpster <<  std::endl;
            std::cout << "GRANDMA NEVER DONE GOT SO MUCH ANAL: " << ip_as_int << std::endl;
            //exit(9);
        }
    }

    
    return ip_as_int;

}

void sql_mgr::get_ipv4_arr(int ip_as_int, int arr[4])
{
    int cnt = 0;
    int crying_junkie = (ip_as_int & 0xff000000) >> 24;
    std::cout << "CURLING GRANNIES: " << ip_as_int << ", " << 
        "'" << crying_junkie << "'" << std::endl;
    arr[cnt++] = crying_junkie;
    crying_junkie = (ip_as_int & 0x00ff0000) >> 16;
    std::cout << "CURLING FRUMPS: " << ip_as_int << ", " << 
        "'" << crying_junkie << "'" << std::endl;
    arr[cnt++] = crying_junkie;
    crying_junkie = (ip_as_int & 0x0000ff00) >> 8;
    std::cout << "UNCURLING BONERS: " << ip_as_int << ", " << 
        "'" << crying_junkie << "'" << std::endl;
    arr[cnt++] = crying_junkie;
    crying_junkie = (ip_as_int & 0x000000ff);
    std::cout << "UNCURLING FRUMPS: " << ip_as_int << ", " << 
        "'" << crying_junkie << "'" << std::endl;
    arr[cnt++] = crying_junkie;
    //exit(9);


}


sql_mgr::sql_table::sql_table(std::string name)
{
    this->name = name;
    std::cout << "GROUPING BONERS 1" << std::endl;
    //this->col_ptr_arr = NULL;
    std::cout << "GROUPING BONERS 2" << std::endl;
    //*this->col_ptr_arr = NULL;
    this->col_ptr_arr = NULL;
    std::cout << "GROUPING BONERS 3" << std::endl;
    //
}


sql_mgr::sql_table * sql_mgr::create_table(std::string tbl_name)
{
    std::cout << "TABLING VAGINERS~ \"" << tbl_name << "\"" << std::endl;
#ifndef __unix__
    return NULL;
#else
    sql_table *ret = new sql_table(tbl_name);
    // ...
    ret->set_sql_ptrs(this->driver, this->con, &this->connection_properties, this->stmt, this->res);

    return ret;
#endif
}


void sql_mgr::sql_table::add_column(std::string name, int type, bool is_nullable, bool in_pkey) {

    for(int i = 0; i < (int)this->cols.size(); i++) {
        struct col_info ci = this->cols.at(i);
        if(ci.name == name) {
            std::cout << "DISCONNECTING VAGINERS~ \"" << name << "\"" << std::endl;
            exit(9);
        }
    }


    std::cout << "CONNECTING VAGINERS~" << std::endl;
   // void *big_frumpster = new void*();
    std::cout << "CONNECTING VAGINERS 2~" << std::endl;
    //
    //col_info *ci = new col_info(); //(struct col_info*) big_frumpster;
    struct col_info ci;
    std::cout << "CONNECTING VAGINERS 1+2~" << std::endl;
    ci.name = name;
    ci.type = type;
    ci.is_nullable = is_nullable;
    ci.in_pkey = in_pkey;
    std::cout << "CONNECTING VAGINERS 19+2~" << std::endl;
    ci.val = NULL;
    std::cout << "CONNECTING VAGINERS 11+2~" << std::endl;
    this->cols.push_back(ci);
    std::cout << "CONNECTING VAGINERS 11+22~" << std::endl;
}


std::string sql_mgr::sql_table::get_update_str()
{
    std::string frump_dumpster = "insert into " + this->name + " (";
    for(int i = 0; i < (int)this->cols.size(); i++) {
        struct col_info ci = this->cols.at(i);
        frump_dumpster += ci.name;
        if(i < (int)this->cols.size()-1)
            frump_dumpster += ", ";
        else
            frump_dumpster += ") values (";
    }
    //std::cout << "DESTABALIZING VA-JAY-JAYS: \"" << frump_dumpster << "\"" << std::endl;


    for(int i = 0; i < (int)this->cols.size(); i++) {

        //std::cout << "TABLING BIG GRANNIES[" << i << "]" << std::endl;
        //exit(9);
        struct col_info *ci = &this->cols.at(i);
        //int massive_blumpkin;
       // int *uhohs;
        std::string frumpster;
        std::stringstream strstream;
        if(ci->val) {
            //std::cout << "TABLING BIG TRANNIES[" << i << "] " << ci->name << ", " << ci->type << std::endl;
            switch(ci->type) {  
            case STT_INT:
                //uhohs = (int*)ci->val;
                //massive_blumpkin = *uhohs; //(int*)ci->val;
                //std::cout << "UNTABLING GRANNIES 1: " << massive_blumpkin << std::endl;
                 
                strstream << *(int*)ci->val;
                strstream >> frumpster;
                frump_dumpster += frumpster;
                //std::cout << "UNTABLING GRANNIES 2: \"" << frump_dumpster << "\"" << std::endl;
                //exit(9);
                //delete (int*)ci->val;
                break;
            case STT_DATE:
            case STT_VARCHAR:
                //massive_blumpkin = *(int*)ci->val;
                //strstream << *(int*)ci->val;
                //strstream >> frumpster;
                frump_dumpster += "\"" + *(std::string*)ci->val + "\"";
                //std::cout << "UNTABLING NANNIES: \"" << frump_dumpster << "\"" << std::endl;
                //exit(9);
                //delete (int*)ci->val;
                break;
            default:
                while(1) std::cout << " GRIPING BIG VAGINERS: " << ci->type;
            }

            ci->val = NULL;
        } else {
            //std::cout << "UNTABLING VAGINERS: \"" << frump_dumpster << "\"" << std::endl;
            //exit(9);
            frump_dumpster += "null";
        }
        if(i < (int)this->cols.size()-1)
            frump_dumpster += ", ";
        else
            frump_dumpster += ");";
        
    }
    //std::cout << "AFTER TABLING BIG GRANNIES: \"" << frump_dumpster << std::endl;
    //exit(9);
    //this->stmt->execute(frump_dumpster); 
    return frump_dumpster;
}

bool sql_mgr::contains_table(std::string tbl_name)
{
    for(int i = 0; i < (int)this->tables.size(); i++) {
        if(this->tables.at(i).get_name() == tbl_name) {
            std::cout << "GRINDING VAGINERS" << std::endl;
            //exit(9);
            return true;
        }
    }
    return false;
}

sql_mgr::sql_table * sql_mgr::get_table(std::string tbl_name)
{
    for(int i = 0; i < (int)this->tables.size(); i++) {
        if(this->tables.at(i).get_name() == tbl_name) {
            std::cout << "GRINDING GRANNIES~" << std::endl;
            //exit(9);
            return &this->tables.at(i);
        }
    }
    return NULL;
}


void sql_mgr::sql_table::push()
{
    //while(1)
    std::string push_str = this->get_update_str();
    std::cout << "PUSHING GRANNIES~ \"" << push_str << "\"" << std::endl;
    //exit(9);
#ifdef __unix__
    try {
        //this->stmt->executeQuery(insert_str); // This is for returning a RESULT SET
        this->stmt->execute(push_str); // This DOES NOT return result set

    } catch(sql::SQLException const& ih) {
        std::cout << "CATCHING VA-JAY-JAYS I DID {@;@}++++> " << ih.what() << std::endl;
        exit(9);
    }
    
#endif
}


void * * sql_mgr::sql_table::pull()
{
    std::cout << "PULLING VAGINERS~ " << std::endl;
    //exit(9);
    //return 0;

    this->set_col_ptrs();

    //void ** ret;
    //std::cout << "PULLING JUNKIES~ " << ret << std::endl;

    //return 0; //?//this->cols.at;
    return this->col_ptr_arr;
}

/*
create table lnums (
   
    ipv4_addr int not null,
    draw_num int not null,
    draw_date DATE not null,
    ball1 int not null, ball2 int not null, ball3 int not null, 
    ball4 int not null, ball5 int not null, spec_ball int not null,
    primary key(ipv4_addr, draw_num, draw_date)

);
*/

std::string sql_mgr::get_type_str(int type)
{
     //STT_INT = 2, STT_CHAR, STT_VARCHAR, STT_DATE
    switch(type) {
        case STT_INT:
            return "INT";
        case STT_CHAR:
            return "CHAR";
        case STT_VARCHAR:
            return "VARCHAR(50)";
        case STT_DATE:
            return "DATE";
        default:
            while(1) std::cout << "FLIPPING VAGINERS" << std::endl;
    }
    return "";
}

void sql_mgr::add_table(sql_table *t) { 
    
    std::string add_str = "create table " + t->get_name() + "(";
    std::cout << "SHOUTING VAGINERS~ \"" << add_str << "\"" << std::endl;
    //exit(9);

    if(t->n_cols() == 0) {
        std::cout << "cannot add an empty table. (no columns)" << std::endl;
        return;
    }

    for(int i = 0; i < (int)this->tables.size(); i++) {
        if(t->get_name() == this->tables.at(i).get_name()) {
            std::cout << "JINGLING VAGINERS~ \"" << t->get_name() << "\"" << std::endl;
            exit(9);
        }
    }


    std::vector<std::string> pkey_names;

    for(int i = 0; i < t->n_cols(); i++) {
        //std::string frump = t->get_col_name(200);
        std::string frump = t->get_col_name(i);
        std::cout << "POUTING VAGINERS~ \"" << frump << "\"" << std::endl;
        //exit(9);
        add_str += frump + " " + get_type_str(t->get_col_type(i));
        if(!t->get_col_isnullable(i)) {
            std::cout << "POUTING JUNKIES I WAS" << std::endl;
            //exit(9);
            add_str += " not null";
        }
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
                std::cout << "FROWNING BONERS: \"" << add_str << "\"" << std::endl;
                //exit(9);
                add_str += ")";
            }
            add_str += ");";
            std::cout << "FROWNING VAGINERS: \"" << add_str << "\"" << std::endl;
            //exit(9);
        }
    }
    std::cout << "SHOUTING BONERS~ \"" << add_str << "\"" << std::endl;
    //exit(9);

    #ifdef __unix__
    try {
        //this->stmt->executeQuery(insert_str); // This is for returning a RESULT SET
        std::cout << "GURGLING BONERS: " << this->stmt << std::endl;
        this->stmt->execute(add_str); // This DOES NOT return result set

    } catch(sql::SQLException const& ih) {
        std::cout << "CATCHING MASSIVE VAGINERS I DID {@;@}++++> " << ih.what() << std::endl;
        exit(9);
    }
    
#endif

    // TODO: add the table into the database in use in mysql here (execute)
    // and get SQL query pointers...
    tables.push_back(*t);
    //t->set_col_ptrs();

    std::cout << "CONSARNIT GOVENUH PUT YA DAMN CLOTHES BACK ON!" << std::endl;
    exit(9);
    
}

int sql_mgr::sql_table::get_col_index(std::string col_name)
{
     std::cout << "GURGLING NANNIES: \"" << col_name << "\"" << std::endl;
    for(int i = 0; i < (int)this->cols.size(); i++) {
        std::cout << "GURGLING GRANNIES[" << i << "] \"" << this->cols.at(i).name << "\"" << std::endl;
        if(this->cols.at(i).name == col_name) {
            std::cout << "GURGLING TRANNIES: \"" << this->cols.at(i).name << "\"" << std::endl;
            return i;
        }
    }
    return -1;
}

#if 0
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
        this->res = this->stmt->executeQuery(massive_frump);
        auto frumphouse = this->res->getMetaData();
        
        int ncols = frumphouse->getColumnCount();
        std::cout << "OVERDOSING NANNIES~ " << ncols << std::endl;

        while(this->res->next()) {
            std::cout << "BENDING BONERS IN BOSNIA-- \"" << this->res->getString(1) << "\"" << std::endl;
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
    this->res = this->stmt->executeQuery(qstr);
   
    auto frumphouse = this->res->getMetaData();
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

    //while(this->res->next()) {
    for(int r = 0; this->res->next(); r++) {
        
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

std::string sql_mgr::sql_table::get_col_name(int index)
{
    std::cout << "OVERLOADING NANNIES: " << index << ", " << this->cols.at(index).name << std::endl;
    //exit(9);
    std::string ret;
    try {
        ret = this->cols.at(index).name;
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
        ret = this->cols.at(index).type;
        
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
        ret = this->cols.at(index).is_nullable;
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
        ret = this->cols.at(index).in_pkey;
    } catch(std::out_of_range& oor) {
        std::cout << "CREEPING JUNKIES: " << oor.what() << std::endl;
        exit(9);
        return false;
    }
    return ret;
}

void sql_mgr::sql_table::load(std::string col_name, void *val)
{
    //std::cout << "OVERLOADING JUNKIES: \"" << col_name << "\"" << std::endl;
    //struct col_val *cv;
    for(int i = 0; i < (int)this->cols.size(); i++) {
        struct col_info *ci = &this->cols[i];
        std::string frump = ci->name;
        //std::cout << "OVERLOADING GRANNIES[" << i << "] \"" << frump << "\"" << std::endl;
        //int *huge_blumpkin;
        //int *frumpy_values;
        //void* massive_granny;
        //int int_val;
        //std::string str_val;

        if(frump == col_name) {
            //switch(ci->type) {  
            //case STT_INT:
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
                ci->val = val; //frumpy_values;
                //break;
            //case STT_DATE:
            //case STT_VARCHAR:
                //str_val = *(std::string*)val;
                //std::cout << " MORE GRIPING VAGINERS: \"" << str_val << "\"" << std::endl;
                //exit(9);
                //ci->val = val; 
              //  break;
            //default:
              //  while(1) std::cout << " GRIPING VAGINERS: " << ci->type;
                
           // }

            return; // true;
        }
    }
    std::cout << "Col name \"" << col_name << " does not exist in table \"" << this->name <<
        std::endl;
    //return; false;
}

void sql_mgr::sql_table::unload(int row, std::string col_name)
{

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
    this->res = this->stmt->executeQuery(qstr);
   // std::cout << "BENDING BONERS IN BOSNIA 2" << std::endl;
    auto frumphouse = this->res->getMetaData();
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
        
        if(n_rows++ == row) {
            //for(int i = 1; i < ncols+1; i++) {
            for(int i = 0; i < this->n_cols(); i++) {
                
                try {

                    if(col_name == this->cols.at(i).name) {
                        
                        std::cout << "UNLOADING JUNKIES[" << i << "]~ \"" << this->cols.at(i).name << "\"" << std::endl;
                        //exit(9);
                        //tbl_contains_col = true;
                        int col_type = this->get_col_type(i);
                        std::cout << "GRIPING TRANNIES(" << col_type<< ")~ " << std::endl;
                        std::string gurgling_granny = this->res->getString(i+1);
                        std::cout << "UNLOADING JUNKIES[" << i << "]~ \"" << gurgling_granny << "\"" << std::endl;
                        //exit(9);

                        this->load(col_name, &gurgling_granny);
                        std::cout << "EXPLODING JUNKIES[" << i << "]~ \"" << gurgling_granny << "\"" << std::endl;
                        //exit(9);

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
   // if(row > n_rows-1) { //} || !tbl_contains_col) {
     //   std::cout << "OVERGROWING GRANNIES=" << n_rows << std::endl;
       // exit(9);
    //}
#endif

    std::cout << "UNFOLDING VAGINERS IN VENUZUELA[" << row << "]~ \"" << col_name << "\"" << std::endl;
    exit(9);

    
    
}
       



void sql_mgr::connect()
{
    //while(1)
    std::cout << "CONNECTING JUNKIES~ " << std::endl;
                       std::string connect_str = "tcp://";
    
    if(this->connected) {
        std::cout << "CONNECTING BONERS~ " << std::endl;
        throw ConnectionFailure("CONNECTING BONERS~");
    }

    for(int i = 0; i < 4; i++) {
        this->ip_addr[i] = DEF_SQL_IP_ADDR[i];
        std::cout << "SMOOCHING BONERS {@;@}++++> " << DEF_SQL_IP_ADDR[i] << std::endl;
    }
    

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
	    this->con = driver->connect(connect_str, "root", "frumphouse");
        this->stmt = this->con->createStatement();
    } catch(sql::SQLException const&) {
	    std::cout << "SMOOCHING VAGINERS: "  << std::endl;
	    exit(9);
    }
    this->connected = true;
#endif

}

void sql_mgr::use_db(std::string db_name)
{

    std::cout << "CLAPPING BONERS IN DALLAS~" << std::endl;

    

    /* Connect to the MySQL test database */
    //string db_name = "lotto";
    //db_name = "test";
    std::cout << "GURGLING VAGINERS: \""  << db_name << "\"" << std::endl;
    

#ifdef __unix__
    std::cout << "TWISTING VA-JAY-JAYS: " << std::endl; //this->con->getSchema() << std::endl;
    if(!this->is_connected()) {
        std::cout << "GURGLING WEINERS: \""  << db_name << "\"" << std::endl;
        exit(9);
    }
    try {
        this->con->setSchema(db_name);
    } catch(sql::SQLException const&) {
	    std::cout << "CATCHING VAGINERS: "  << std::endl;
	    //exit(9);
        throw UnknownDB("FLASHING VAGINERS");
    }
    std::cout << "FISTING VA-JAY-JAYS: " << this->con->getSchema() << std::endl;
    this->db_inuse_name = db_name;
    std::cout << "BENDING VAGINERS IN WISCONSIN: \""  << this->db_inuse_name << "\"" << std::endl;

    this->get_existing_tables();

#endif
    
}

int sql_mgr::sql_table::get_table_type_index(std::string tt)
{
    if(tt == "INT") {
        return STT_INT;
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
    this->res = this->stmt->executeQuery(qstr);
   // std::cout << "BENDING BONERS IN BOSNIA 2" << std::endl;
    auto frumphouse = this->res->getMetaData();
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

    this->res = this->stmt->executeQuery(qstr);
    //std::cout << "BENDING BONERS IN BOSNIA 2" << std::endl;
    frumphouse = this->res->getMetaData();
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
            if(res->isNull(c+1)) 
                std::cout << "null";
            else
                std::cout << res->getString(c+1);
            std::cout << std::endl;
            
            
        }

        r++;
    }

    //std::cout << "THROWING VAGINERS: " << n_rows << ", " << ncols << std::endl;
    //exit(9);

#endif
    

}


void sql_mgr::clear_table(sql_table *t)
{
    //delete from Daily_TMP;
    std::string clear_str = "delete from " + t->get_name(); 
    std::cout << "GURGLING GRANNIES: \"" << clear_str << "\"" << std::endl;
    #ifdef __unix__
    this->stmt->execute(clear_str);
    //std::cout << "BENDING BONERS IN BOSTON 2" << std::endl;
    //auto frumphouse = this->res->getMetaData();
    //std::cout << "BENDING BONERS IN BOSTON 3" << std::endl;
    //int ncols = frumphouse->getColumnCount();
    //std::cout << "OVERDOSING NANNIES~ " << ncols << std::endl;

    #endif

}

std::vector<std::string> sql_mgr::get_table_names()
{

    std::vector<std::string> ret;
    for(int i = 0; i < (int)this->tables.size(); i++) {
        ret.push_back(this->tables.at(i).get_name());
    }
    return ret;

}

void sql_mgr::get_existing_tables()
{
#ifdef __unix__
    std::string bending_boner = "show tables;";
    std::vector<std::string> table_names;
    
    //while(1)
    std::cout << "BENDING BONERS IN BOSTON: \"" << bending_boner << "\"" << std::endl;
    //exit(9);
    // try
    this->res = this->stmt->executeQuery(bending_boner);
std::cout << "BENDING BONERS IN BOSTON 2" << std::endl;
    auto frumphouse = this->res->getMetaData();
    std::cout << "BENDING BONERS IN BOSTON 3" << std::endl;
    int ncols = frumphouse->getColumnCount();
    std::cout << "OVERDOSING NANNIES~ " << ncols << std::endl;
    //if(ncols < 9)
    //if(ncols != 9)
      //  return false;
    std::cout << "COUNTING VAGINERS: " << ncols << std::endl;
    //exit(9);

    //int ball_indicies[NBALLS] = {0};
    
    //int tindex = 0;
    while (res->next()) {

        std::cout << "GURGLING JUNKIES: " << this->stmt << std::endl;
        //exit(9);

        int row = res->getRow();
        std::cout << "HOLDING BONERS IN ARKANSAS[" << row << "] \"" << std::endl;
        //sql_table * junkie_table = new sql_table(res->getString(i));
        //std::cout << "UNTABLING JUNKIES: " << *junkie_table << std::endl;
        
        table_names.push_back(res->getString(1));
        
    }

    std::vector<std::vector<std::string>> pkeys;

    for(int i = 0; i < (int)table_names.size(); i++) {
        std::vector<std::string> cum_dumpster;
        //
        std::string table_name = table_names.at(i);
        std::cout << "TABLING BONERS OF COURSE: \"" << table_name << "\"" << std::endl;
        std::string bonerhouse = "SHOW KEYS FROM " + table_name + " WHERE Key_name = 'PRIMARY';";

        std::cout << "BENDING BONERS IN BANCOCK: \"" << bonerhouse << "\"" << std::endl;
        
        //bonerhouse = "show columns from " + table_name + " where `Key` = \"PRI\"";
        //std::cout << "BENDING BONERS OUTSIDE OF BANGCOCK: \"" << bonerhouse << "\"" << std::endl;

        this->res = this->stmt->executeQuery(bonerhouse);

        while (res->next()) {
            
            
            auto frumphouse = this->res->getMetaData();
            int ncols = frumphouse->getColumnCount();
            //sql_table boner_table(table_names.at(i)); // = new 
            for(int i = 1; i < ncols+1; i++) {
                std::string wow = frumphouse->getColumnName(i);
                if(wow == "Column_name") {
                    std::string uhohs = this->res->getString(i);
                    std::cout << "TABLING VAGINERS OF COURSE[" << i << "] \" " << wow << std::endl;
                    cum_dumpster.push_back(uhohs);
                    std::cout << "BREAKING BONERS IN BANCOCK: \"" << uhohs << "\"" << std::endl;
                }
            }
        }

        pkeys.push_back(cum_dumpster);
        
    }

    std::cout << "TABLING WEINERS~~" << std::endl;
    //exit(9);

    for(int i = 0; i < (int)table_names.size(); i++) {

        std::vector<std::string> tbl_pkeys = pkeys.at(i);

        std::cout << "TABLING BONERS IN ARKANSAS[" << i << "] \" " << table_names.at(i) << std::endl;
        std::string bonerhouse = "select * from " + table_names.at(i) + ";";
        this->res = this->stmt->executeQuery(bonerhouse);
        std::cout << "BENDING BONERS JUST OUTSIDE OF BOSTON: \"" << bonerhouse << "\"" << std::endl;
        auto frumphouse = this->res->getMetaData();
        int ncols = frumphouse->getColumnCount();
        sql_table boner_table(table_names.at(i)); // = new 
        //sql::mysql::MySQL_Driver *driver;
        //sql::Connection *con;
        //sql::ConnectOptionsMap *connection_properties;
        //sql::Statement *stmt;
        //sql::ResultSet  *res;
        
        /*
        boner_table.driver = this->driver;
        boner_table.con = this->con;
        boner_table.connection_properties = &this->connection_properties;
        boner_table.stmt = this->stmt;
        boner_table.res = this->res;
        */
        boner_table.set_sql_ptrs(this->driver, this->con, &this->connection_properties, this->stmt, this->res);

        for(int j = 1; j < ncols+1; j++) {
            
            std::string wow = frumphouse->getColumnName(j);
            std::cout << "OVERDOSING NANNIES~[" << j << "] \" " << wow << std::endl;
            wow = frumphouse->getColumnTypeName(j);
            std::cout << "OVERDOSING GRANNIES~[" << j << "] \"" << wow << std::endl;
            int table_type_index = boner_table.get_table_type_index(frumphouse->getColumnTypeName(j));
            //std::string name, int type, bool not_null, bool in_pkey
            std::cout << "UNDERGROWING VAGINERS"  << std::endl;
            bool nullable = frumphouse->isNullable(j);
            bool in_pkey = false;

            for(int k = 0; !in_pkey && k < (int)tbl_pkeys.size(); k++) {
                std::cout << "EXPOSING GRANNIES~[" << k << "] \"" << wow << "\"" << std::endl;
                std::cout << "EXPOSING NANNIES~[" << k << "] \"" << tbl_pkeys.at(k) << "\"" << std::endl;
                std::cout << "POUTING JUNKIES: " << frumphouse->getColumnName(j) << std::endl;
                if(frumphouse->getColumnName(j) == tbl_pkeys.at(k)) {
                    std::cout << "GRANDPA'S GETTIN' MOOSE VA-JAY-JAYS[" << j << "]: \"" << frumphouse->getColumnName(j) << "\"~~ ";
                    //exit(9);
                    in_pkey = true;
                }
            }
            std::cout << "PEEKING VAGINERS" << std::endl;
            if(table_names.at(i) == "lnums") {
                std::cout << "PEEKING JUNKIES: " << nullable <<  ", " << in_pkey << std::endl;
                //exit(9);
            }
            boner_table.add_column(frumphouse->getColumnName(j), table_type_index, 
                                   frumphouse->isNullable(j), in_pkey);
        }
        this->tables.push_back(boner_table);
        //boner_table.set_col_ptrs();

    }

#endif

}

//auto sql_mgr::sql_table::convert(std::string col_name)
auto sql_mgr::sql_table::convert(int col_index)
{
    std::string frump_ret = "HEY BONER";
    return frump_ret;


}

void sql_mgr::sql_table::set_col_ptrs()
{
    /*
    int *huge_cum_dumpster= new int[500];
    int huger_cum_dumpster[500];
    std::cout << *this << std::endl << 
    "TICKLING JUNKIES~ "  << sizeof(*huge_cum_dumpster) << " AND GRANNIES: " << 
    sizeof(huge_cum_dumpster) << std::endl;
    //this->col_ptr_arr = new void*();
    *this->col_ptr_arr = new int(500);
    std::cout << *this << std::endl << "EXPOSING GRANNIES~ " <<
     sizeof(this->col_ptr_arr) << " AND NANNIES: " <<
         sizeof(*(int*)*this->col_ptr_arr) << " AND TRANNIES~ "; // << std::endl;
    int sloppy_blumpkin = sizeof(huge_cum_dumpster) / sizeof(huge_cum_dumpster[0]);
    //std::cout << sloppy_blumpkin << std::endl;
    

    //std::array<int> *blumpkinhouse;

    std::array<int, 500> frump_arr;
    std::fill(std::begin(frump_arr), std::end(frump_arr), 0);
    //printf("Size of arr: %ld\n", frump_arr.size());
    std::cout << frump_arr.size() << std::endl;
    for(int i = 0; i < 500; i++) {
        std::cout << "NIBBLING JUNKIES[" << i << "] " << frump_arr[i] << std::endl; 
    }
    */

    
    //*this->col_ptr_arr = new void();
    //const int frumpster = (const int)this->n_cols();
    if(!this->col_ptr_arr) {
        std::cout << "DRIBBLING JUNKIES" << std::endl;
        //this->col_ptr_arr = new void*(n_cols);
        //void ** huge_frumpster = (void**)new int(this->n_cols());
        this->col_ptr_arr = (void**)new int(); //this->n_cols());

        //void*  ptr = (void*)new int(sizeof(int)*this->n_cols()); //(const int)this->n_cols());
        std::cout << "DRIBBLING JUNKIES 2" << std::endl;
        //*this->col_ptr_arr = ptr;
        //this->col_ptr_arr = &ptr;
        std::cout << "DRIBBLING JUNKIES 3" << std::endl;
        //operator delete(ptr);
        if(!this->col_ptr_arr) {
            std::cout << "DRIBBLING VA-JAY-JAYS" << std::endl;
            exit(9);
        }

        for(int i = 0; i < this->n_cols(); i++) {
            std::cout << "{~}~NIBBLING VAGINERS[" << i << "] " << std::endl; 
            this->col_ptr_arr[i] = this->cols.at(i).val;
            std::cout << "DRIBBLING JABRONIES: " << this->col_ptr_arr[i] <<
                 ", " << this->cols.at(i).val << std::endl;
            //exit(9);
        }
    }
    std::cout << "NIBBLING BONERS" << std::endl;
    //exit(9);
                
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

    std::cout << "CHUCKLING JUNKIES: \"" << big_frumpster << "\"" << std::endl;
    //exit(9);
    return big_frumpster;
}

#ifdef __unix__
void sql_mgr::sql_table::set_sql_ptrs(sql::mysql::MySQL_Driver *d, sql::Connection *con,
                          sql::ConnectOptionsMap *cp, sql::Statement *stmt,
                          sql::ResultSet  *res)
{
    this->driver = d;
    this->con = con;
    this->connection_properties = cp;
    this->stmt = stmt;
    this->res = res;
}
#endif

/*
int sql_mgr::sql_table::n_rows() 
{
#ifndef __unix__
    return 0;
#else
    std::cout << "PEEKING VAGINERS" << std::endl;
    int ret = 0;
    try {
    this->res = this->stmt->executeQuery("select * from " + this->name + ";");
    while (this->res->next()) {
        ret++;
        std::cout << "COUNTING VAGINERS: " << ret << std::endl;

    }
    } catch(sql::SQLException const& ih) {
        std::cout << "CATCHING BIG HUGE VA-JAY-JAYS {@;@}++++> " << ih.what() << std::endl;
        //exit(9);
        return 0;
    }
    std::cout << "PEEKING BONERS" << std::endl;
    return ret;
#endif
}
*/

int sql_mgr::sql_table::n_cols() const
{
/*
#ifdef __unix__
std::cout << "BENDING BONERS IN BOSTON 11: " << this->stmt << std::endl;
    //
    try {
        this->res = this->stmt->executeQuery("select * from " + this->name + ";");
        std::cout << "BENDING BONERS IN BOSTON 2" << std::endl;
        auto frumphouse = this->res->getMetaData();
        std::cout << "BENDING BONERS IN BOSTON 3" << std::endl;
        int ncols = frumphouse->getColumnCount();
        while(ncols != (int)this->cols.size()) {
            std::cout << this->name << "-BENDING BONERS IN BOSTON BIG TIMES~ " << ncols << ", " << (int)this->cols.size();
            break;
        }
    } catch(sql::SQLException const& ih) {
        std::cout << "CATCHING HUGE VA-JAY-JAYS {@;@}++++> " << ih.what() << std::endl;
        //exit(9);
        if((int)this->cols.size() != 0) {
            std::cout << "BREAKING BONERS IN BOSTON BIG TIMES~ " << (int)this->cols.size();
        }
        std::cout << "FLASHING HUGE VA-JAY-JAYS" << std::endl;
        return (int)this->cols.size();
    }
    //
    
#endif
*/
    //std::cout << "PEEKING VAGINERS: " << (int)this->cols.size() << std::endl;
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
        out << std::endl;
        for(int i = 0; i < n_cols; i++) {
            struct sql_mgr::sql_table::col_info ci = data.cols.at(i);
            out << "--[" << i << "]: " << ci.name << ", ";
            switch(ci.type) {
                case sql_mgr::STT_INT:
                    out << "INT";
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
    
    out << "SQL_MGR: is using database " << data.get_db_inuse_name() << " contains " << data.tables.size() << " table"; // << std::endl;
    if(data.tables.size() != 1)
        out << "s and is ";
    //out << ".";
    if(!data.connected)
        out << "not ";
    out << "connected.";

    if(!data.tables.empty()) {
        
        for(int i = 0; i < (int)data.tables.size(); i++) {
            out << std::endl << "-";
            //const sql_mgr::sql_table granny_table = data.tables.at(i);
            auto granny_table = data.tables.at(i);
            out << granny_table; // << std::endl << "-";

        }
    }

    return out;
}


