

#include <iostream>
using namespace std;


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
  return std::getchar();
}



//bool MATCH_FOUND = false;
bool match_command(struct thread_info_cmd ti)
{

    int id_cmd = ti.id;
    vector <string>tokens = ti.tokens;
    vector<string>::iterator it = tokens.begin();
    
    draw d(first_five_max, spec_max);

    int ball_index = 0;

    if(tokens.size() < 2 || tokens.size() > 7) 
        return false; 

    for(it++; it != tokens.end(); ++it) {
        string tmp_str = *it;
        int ball_val;
        try {
            ball_val = stoi(tmp_str);
        } catch(...) {
            return false; 
        }
        d[ball_index++] = ball_val;
    }
  
    match m;

    match_mux.lock();
    m = matches[id_cmd];
    m.set_draw(&d);
    match_mux.unlock();

    if(!m.run()) 
        return false;

    cout << m << endl;
    cmd_mux.lock();
    thr_done_cmd[id_cmd]++;
    cmd_mux.unlock();
    return true;
}


bool draw_command(struct thread_info_cmd ti)
{
    int id = ti.id;
    vector <string>tokens = ti.tokens;
    if(tokens.size() > 0) {
        int n_draws = 1;
        vector<string>::iterator it = tokens.begin();
        for(it++; it != tokens.end(); ++it) {
            string token = *it;
            try {
                n_draws = stoi(token);
            } catch(...) {
                return false; 
            }
        
            if(n_draws == 0) {
                return false; 
            }
        }
        ticket t;
        for(int i = 0; i < n_draws; i++) {
            draw d;
            //d = draw_pt(first_five_max, spec_max);
            d = draw(first_five_max, spec_max);
            d.fill();
            t.add_draw(d);
        }
        cout << t << endl;
    }

    cmd_mux.lock();
    thr_done_cmd[id]++;
    cmd_mux.unlock();
    return true;
}

void usage()
{
    cout << "--lnums program usage: " << endl;
    // ..

}


int main()
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
    std::future<char> future;
    
    memset(cmd_threads, 0, sizeof(cmd_threads));

    /* We need an asynchronous stdin framework so we can check for thread
       completion while accepting user input. */
    std::chrono::seconds timeout(0);
    future = std::async(std::launch::async, get_usr_in);

    cout << "LNUMS> "; 
    srand(time(nullptr));
    
    while(1) {
        
        if(future.wait_for(timeout) == future_status::ready) {
            ch = future.get();
            future = async(launch::async, get_usr_in);
            //cout.clear(); ??
            //cin.clear(); ??
            if(ch == '\n') {
                
                stringstream strstr(usr_input);
                istream_iterator<string> it(strstr);
                istream_iterator<string> end;
                vector<string> results(it, end);
                ostream_iterator<string> oit(cout); // send the vector to stdout.
                copy(results.begin(), results.end(), oit);
                if(results.size() > 0) {
                    string tmp_str = *results.begin();
                    struct thread_info_cmd ti;
                    ti.id = thr_index;
                    ti.tokens = results;
                    if(tmp_str == exit_cmd) 
                        break;
                    if(tmp_str == "draw") 
                        cmd_threads[thr_index++] = thread(draw_command, ti);
                    else if(tmp_str == "match") 
                        cmd_threads[thr_index++] = thread(match_command, ti);
                    else {
                        cout << "\"" << tmp_str << "\"" << " is an invalid command." << endl;
                        memset(usr_input, 0, sizeof(usr_input));
                        ui_index = 0;
                        usage();
                        continue;
                    }
                    //results.clear();
                }
                memset(usr_input, 0, sizeof(usr_input));
                ui_index = 0;
                //cout.clear(); ??
                //cin.clear(); ??
                //cout << "LNUMS> "; 
            } else 
                usr_input[ui_index++] = ch;  
            //cout << "LNUMS> ";      
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
    cout << "Exiting C++ lotto program, good bye!" << endl;
    return 0;
}


