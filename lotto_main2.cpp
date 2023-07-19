

#include <iostream>
using namespace std;

#include <string>
#include <thread>
#include <unistd.h>
#include <chrono>
#include <cstdio>
#include <future>
#include <istream>
#include <ostream>
#include <iterator>
#include <sstream>
#include <vector>

#include "draw.h"

#include "draw_pt.h"

#include "match.h"
#include "ticket.h"



#define MAX_N_CMD_THREADS 100
#define MAX_N_MATCHES MAX_N_CMD_THREADS

#define LOT_TYP_NONE  0
#define LOT_TYP_PB    1
#define LOT_TYP_MM    2
#define MM_MAX 70
#define MM_SPEC_MAX 25
#define PB_MAX 69
#define PB_SPEC_MAX 26

int DRAW_TYPE; // = DTYPE_RAND;

enum DTYPES {
    DTYPE_RAND, DTYPE_POT
};


thread cmd_threads[MAX_N_CMD_THREADS]; 
match matches[MAX_N_CMD_THREADS];

mutex cmd_mux, match_mux; 
int thr_done_cmd[MAX_N_CMD_THREADS] = {0}; 

int lottery_day, first_five_max, spec_max;

struct thread_info_cmd {
    int id;
    vector <string> tokens;
    
};




static char get_usr_in()
{
  return getchar();
}

void usage()
{
    cout << "--lnums program usage: " << endl;
    cout << "\tdraw [NDRAWS]" << endl;
    cout << "\tmatch <B1> [B2] [B3] [B4] [B5] [SP] (BN > BN-1 for N>1)" << endl;
    cout << "\tuse <random|pot>" << endl;
}


void match_command(struct thread_info_cmd ti)
{

    int id_cmd = ti.id;
    auto tokens = ti.tokens;
    auto it = tokens.begin();
    
    draw d(first_five_max, spec_max);

    int ball_index = 0;

    if(tokens.size() < 2 || tokens.size() > 7) {
        usage();
        return; 
    }

    for(it++; it != tokens.end(); ++it) {
        string tmp_str = *it;
        int ball_val;
        try {
            ball_val = stoi(tmp_str);
        } catch(...) {
            usage();
            return; 
        }
        d[ball_index++] = ball_val;
    }
  
    match m;

    match_mux.lock();
    m = matches[id_cmd];
    m.set_draw(&d);
    match_mux.unlock();

    long n_tries = m.run();

    cout << "(n_tries=" << n_tries << ") " << m << endl;
    cmd_mux.lock();
    thr_done_cmd[id_cmd]++;
    cmd_mux.unlock();

}


void draw_command(struct thread_info_cmd ti)
{
    int id = ti.id;
    ticket t;
    auto tokens = ti.tokens;
    int n_draws = 1;
    if(tokens.size() == 2) 
        n_draws = stoi(tokens.at(1));   
    
    if(DRAW_TYPE == DTYPE_RAND) 
        cout << "(using RANDOM type draw...)" << endl;
    else
        cout << "(using POT type draw...)" << endl;
    
    for(int i = 0; i < n_draws; i++) {
        draw d;
        if(DRAW_TYPE == DTYPE_RAND) 
            d = draw(first_five_max, spec_max);
        else 
            d = draw_pt(first_five_max, spec_max);
        d.fill();
        t.add_draw(d);
    }
    cout << t << endl;

    cmd_mux.lock();
    thr_done_cmd[id]++;
    cmd_mux.unlock();
}


void process_input(string usr_input, int *thr_index)
{
    struct thread_info_cmd ti;
    bool async = *thr_index != -1;
    stringstream strstr(usr_input);
    istream_iterator<string> it(strstr);
    istream_iterator<string> end;
    vector<string> results(it, end);
    ostream_iterator<string> oit(cout); // send the vector to stdout.
    copy(results.begin(), results.end(), oit);
    if(results.size() > 0) {
        /* Make all the input lowercase. */
        for(auto it = results.begin(); it != results.end(); ++it) {
            for(int i = 0; (*it)[i]; i++) 
                (*it)[i] = tolower((*it)[i]);
        }
        string tmp_str = *results.begin();
        ti.id = *thr_index;
        ti.tokens = results;
        if(tmp_str == "draw") {
            bool do_draw = true;
            if(results.size() > 2) {
                usage();
                do_draw = false;
            } else if(results.size() == 2) {
                string n_draws_str = results.at(1);
                try {
                    int n_draws = stoi(n_draws_str);
                    if(n_draws == 0) {
                        usage();
                        do_draw = false;
                    }
                } catch(...) {
                    usage();
                    do_draw = false;
                }
            }
            if(do_draw) {
                if(async)
                    cmd_threads[(*thr_index)++] = thread(draw_command, ti);
                else
                    draw_command(ti);
            }
        } else if(tmp_str == "match") {
            bool do_match = true;
            if(results.size() < 2 || results.size() > 7) {
                usage();
                do_match = false;
            } 
            int ball_vals[6] = {0};
            for(int i = 1; do_match && i < results.size(); i++) {
                try {
                    int ball_val = stoi(results.at(i));
                    if(ball_val == 0) {
                        usage();
                        do_match = false;
                    } else
                        ball_vals[i-1] = ball_val;
                } catch(...) {
                    usage();
                    do_match = false;
                }
            }
            if(do_match) {
                match m(ball_vals);
                if(!m.matchable()) 
                    usage();
                else if(async) 
                    cmd_threads[(*thr_index)++] = thread(match_command, ti); 
                else 
                    match_command(ti);
            }
        } else if(tmp_str == "use") {
            if(results.size() != 2) 
                usage();
            else if(results.at(1) != "random" && results.at(1) != "pot") 
                usage();
            else if(results.at(1) == "random")
                DRAW_TYPE = DTYPE_RAND;
            else
                DRAW_TYPE = DTYPE_POT;
        }
        else {
            cout << "\"" << tmp_str << "\"" << " is an invalid command." << endl;
            usage();
        }
    }

}

int main(int argc, char *argv[])
{
    bool use_async_input = false;
    if(argc > 1) {
        for(int i = 1; i < argc; i++) {
            string arg = (string)argv[i];
            if(arg == "-a") 
                use_async_input = 1;
            else {
                cout << "\"" << arg << "\" is an invalid program argument." << endl;
                return 1;
            }
        }
    }

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

    if(strcmp("Tue", day)==0 || strcmp("Fri", day)==0) {
        lottery_day = LOT_TYP_MM;
        first_five_max = MM_MAX;
        spec_max = MM_SPEC_MAX;   
    } else if(strcmp("Mon", day)==0 || strcmp("Wed", day)==0 || strcmp("Sat", day)==0) {
        lottery_day = LOT_TYP_PB;
        first_five_max = PB_MAX;
        spec_max = PB_SPEC_MAX;
    } else {
        lottery_day = LOT_TYP_NONE;
        if(strcmp("Mon", day)==0) 
            printf("--(next lottery day Tue (tomorrow), MEGA MILLIONS)\n");
        else if(strcmp("Thu", day)==0) {
            printf("--(next lottery day Fri (tomorrow), MEGA MILLIONS)\n");
        } else if(strcmp("Sun", day)==0) {
            printf("--(next lottery day Tues, MEGA MILLIONS)\n");
        }

        // Test with PB range
        lottery_day = LOT_TYP_PB;
        first_five_max = PB_MAX;
        spec_max = PB_SPEC_MAX;

    }
    
    int thr_index = 0;
    char usr_input[1000] = "";
    int ui_index = 0;
    const char *exit_cmd = "exit";
    char ch = '!';

    memset(cmd_threads, 0, sizeof(cmd_threads));

    DRAW_TYPE = DTYPE_RAND;

    const string prompt = "LNUMS> ";

    cout << prompt; 
    srand(time(nullptr));

    if(!use_async_input) {
        string line;
        while(getline(cin,line)) {
            int neg_one = -1;
            process_input(line, &neg_one);
            cout << prompt; 
        }

    } else {

        /* We need an asynchronous stdin framework so we can check for thread
           completion while accepting user input. */
        chrono::seconds timeout(0);
        auto future = async(launch::async, get_usr_in);

        while(1) {
        
            if(future.wait_for(timeout) == future_status::ready) {
                ch = future.get();
                future = async(launch::async, get_usr_in);
                //cout.clear(); ??
                //cin.clear(); ??
                if(ch == '\n') {
                    process_input(usr_input, &thr_index);
                    memset(usr_input, 0, sizeof(usr_input));
                    ui_index = 0;
                    //cout.clear(); ??
                    //cin.clear(); ??
                    //cout << prompt;
                } else 
                    usr_input[ui_index++] = ch;  
                //cout << prompt;      
            }

            for(int i = 0; i < MAX_N_CMD_THREADS; i++) {
                cmd_mux.lock();
                if(thr_done_cmd[i]) {
                    cmd_threads[i].join();
                    thr_done_cmd[i] = 0;
                }
                cmd_mux.unlock();
            }
        }
    }
    cout << "Exiting C++ lotto program, good bye!" << endl;
    return 0;
}


