


#include "lnums_v1_0.h"

#define N_WHITES 5
#define LOT_TYP_NONE  0
#define LOT_TYP_PB    1
#define LOT_TYP_MM    2
#define MM_MAX 70
#define MM_SPEC_MAX 25
#define PB_MAX 69
#define PB_SPEC_MAX 26


#define SUCCESS 0
#define FAILURE 1
#define EXIT (SUCCESS+2)
#define IS_SPACE(c) ( (c == ' ') || (c == '\t') )

#define SHOW_POT do {  \
 pthread_mutex_lock(&pot_mux); \
 for(int i = 0; i < first_five_max; i++) \
    printf("~JUGGLING BONERS[%d] ~ %d ~\n", i, pot[i]); \
 pthread_mutex_unlock(&pot_mux); \
} while(0)

#define SHOW_SPEC_POT do {  \
 pthread_mutex_lock(&pot_spec_mux); \
 for(int i = 0; i < spec_max; i++) \
    printf("\t~JUGGLING JUNKIES[%d] ~ %d ~\n", i, spec_pot[i]); \
 pthread_mutex_unlock(&pot_spec_mux); \
} while(0)

enum PROGRAM_MODE {
    PMODE_NORMAL, PMODE_DEBUG
};
int prog_mode; // = PMODE_NORMAL;

enum FRUMPHOUSE {
    GAP_TYPE_LT, GAP_TYPE_EQ, GAP_TYPE_GT
};

struct gap {
    const int space; // 1-4 (0-3)
    int val; // max=65 (66) {1,66,67,68,69}, {1,67,68,69,70}
    int less_than; // greater than gap by default
} gaps[] = {
    {0,0,0}, {1,0,0}, {2,0,0}, {3,0,0}
};

//int intervals[4] = {0};
int pot[MM_MAX] = {0}, spec_pot[PB_SPEC_MAX] = {0};

pthread_mutex_t pot_mux, pot_spec_mux, jumbling_mux; //, frump_mux; 

struct thread_info {
    int thread_id;
};

int JUMBLING = 0;
int huge_gunt = 0;
#define SHOWING_BONERS 3

int run_program();


#define GAP_SET(g) (g->val)
#define GAP_NOTSET(g) !GAP_SET(g)
#define GAPS_SET (gaps[0].val || gaps[1].val || gaps[2].val || gaps[3].val)
#define GAPS_NOTSET !GAPS_SET

//#define INTERVALS_SET (intervals[0] || intervals[1] || intervals[2] || intervals[3])
//#define INTERVALS_NOTSET !INTERVALS_SET

#define N_GAPS (sizeof(gaps)/sizeof(gaps[0]))

//int consecs[N_GAPS] = {0};
//#define CONSECS_SET (consecs[0] || consecs[1] || consecs[2] || consecs[3])

int lottery_day, first_five_max, spec_max; 

typedef struct {
    int draw_date[3];
    int lnums[100][6];
    int n_draws;
} ticket;

typedef struct {
    //struct t_entry {
    ticket ticket_table[100];
        //struct t_entry *prev, *next;
    //} entries[1000];
    int n_entries;
} booklet;

ticket daily_ticket; 

int (*fill_routine)(ticket *t);


int start = 0, end = 0;
int range[2], range_sp[2];
#define RANGE_LOWER range[0]
#define RANGE_UPPER range[1]
#define RANGE_LOWER_SP range_sp[0]
#define RANGE_UPPER_SP range_sp[1]
#define PRINT_RANGE printf("Current Draw Range: {%d, %d}\n", RANGE_LOWER, RANGE_UPPER)
#define PRINT_RANGE_SP printf("         (Special): {%d, %d} \n", RANGE_LOWER_SP, RANGE_UPPER_SP)
#define PRINT_RANGES {PRINT_RANGE;} {PRINT_RANGE_SP;}

#define RESTRICTIONS_SET (GAPS_SET || CONSECS_SET || INTERVALS_SET ||  (range[0] != 1 || range[1] != first_five_max || start || end) )


int get_rand_num(int min, int max) 
{
    return (rand() % (max - min + 1)) + min;
}


int get_rand_lot_num(int min, int max) 
{
    //printf("GRANDMA WAS UNFOLDING BONERS DOWNTOWN~ %d %d\n", min, max);
    return get_rand_num(min, max);
}



void present_draw(int draw, ticket *t);


void array_in_order(int arr[], int arr_size)
{

    //for(int i = 0; i < arr_size; i++) {
        //printf("GURGLING weiners[%d] %d\n", i, arr[i]);
    //}

    int tmp_vals[first_five_max];
    memset(tmp_vals, 0, sizeof(int)*first_five_max);
    for(int i = 0; i < arr_size; i++) {
        if(arr[i] == 0) {
            //printf("GURGLING VAGINERS\n");
            //exit(9);
            //return;
            //break;
        }
        tmp_vals[arr[i]]++;
    }
    int huge_frump_values[N_WHITES];
    memset(huge_frump_values, 0, N_WHITES);
    int huge_frump_index = 0;
    for(int i = 0; i < first_five_max; i++) {
        //printf("SQUISHING BONERS[%d] %d\n", i, tmp_vals[i]);
        //if(tmp_vals[i]) 
        for(int j = 0; j < tmp_vals[i]; j++)
            huge_frump_values[huge_frump_index++] = i;
    }
    for(int i = 0; i < N_WHITES; i++) 
        arr[i] = 0;
    for(int i = 0; i < N_WHITES; i++) 
        arr[i] = huge_frump_values[i];
}


void draw_in_order(int arr[])
{
    int tmp_vals[first_five_max];
    memset(tmp_vals, 0, sizeof(int)*first_five_max);
    for(int i = 0; i < N_WHITES; i++) {
        if(arr[i] == 0) {
            printf("GURGLING BONERS\n");
            exit(9);
            //return;
            //break;
        }
        tmp_vals[arr[i]-1]++;
    }
    int huge_frump_values[N_WHITES];
    memset(huge_frump_values, 0, N_WHITES);
    int huge_frump_index = 0;
    for(int i = 0; i < first_five_max; i++) {
        if(tmp_vals[i]) 
            huge_frump_values[huge_frump_index++] = i+1;
    }
    for(int i = 0; i < N_WHITES; i++) 
        arr[i] = 0;
    for(int i = 0; i < N_WHITES; i++) 
        arr[i] = huge_frump_values[i];
}

int invalid_restrictions() {

#if 0
    for(int i = 0; i < 4; i++) {

        //--gaps not yet implemented
        if(gaps[i].val) {
            printf("Gaps not yet implemented...\n");
            return 1;
        }
        //----------------

        if(gaps[i].val && (intervals[i] || consecs[i]) ) {
            printf("~~~WHINING BONERS WON[%d]~~~\n", i);
            return 1;
        } else if(intervals[i] && consecs[i]) {
            printf("~~~WHINING BONERS WON+WON~~~\n");
            return 1;
        }
    }
    
    /* try run with lowest possible numbers to make sure the draw
       is possible. */

    int balls[5] = {0}, cur_val;

    if(start) 
        balls[0] = cur_val = start;
     else 
        balls[0] = cur_val = RANGE_LOWER;

    for(int i = 1; i < 5; i++) {
        if(consecs[i-1])
            balls[i] = cur_val = (balls[i-1] + 1);
        else if(intervals[i-1]) 
            balls[i] = cur_val = (balls[i-1] + intervals[i-1]);
        else {
            if(end && i == 4) {
                if(cur_val >= end) 
                    return 1;
            }
            balls[i] = ++cur_val; // account for the ball
        }
        if(cur_val > RANGE_UPPER) 
            return 1;
    }
#endif
    return 0;
}



void * jumble_pot(void *tinfo)
{

    struct thread_info *ti = (struct thread_info*)tinfo;
    const int tid = ti->thread_id;
    const int creeping_junkie = 10000;
    int cunt = 0;
    int rand_index1, rand_index2;
    int still_jumbling; 
    
    pthread_mutex_lock(&jumbling_mux);
    still_jumbling = JUMBLING;
    pthread_mutex_unlock(&jumbling_mux);

    while(still_jumbling) {
        usleep(creeping_junkie);
        BEG_SWAP:
        //printf("FINGER BLASTING GRANNIES~ {%d;%d}~=~+~=~=>\n", cunt, huge_gunt);
        //SHOW_POT;
        rand_index1 = get_rand_num(0, first_five_max-1);
        rand_index2 = get_rand_num(0, first_five_max-1);
        while(rand_index1 == rand_index2) 
            rand_index2 = get_rand_num(0, first_five_max-1);
        pthread_mutex_lock(&pot_mux);
        if(!pot[rand_index1] || !pot[rand_index2]) {
            pthread_mutex_unlock(&pot_mux);
            goto BEG_SWAP;
        }
        int tmp = pot[rand_index1];
        pot[rand_index1] = pot[rand_index2];
        pot[rand_index2] = tmp;
        pthread_mutex_unlock(&pot_mux);
        pthread_mutex_lock(&jumbling_mux);
        still_jumbling = JUMBLING;
        huge_gunt++;
        pthread_mutex_unlock(&jumbling_mux);
        
        //printf("BONE SMUGGLING JUNKIES ~~~~ ");

//#ifdef GROWING_BONERS
        int n_threads = 100; //sizeof(pot_threads)/sizeof(pot_threads[0]);
        int blumpkinhouse = get_rand_num(0, n_threads-1);
        if(0 && (++cunt % 5000) == 0) {
            //if(tid == blumpkinhouse) {
                printf("~JUMBLING VAGINERS{%d}: %d %d <%d>\n", tid, cunt, huge_gunt, n_threads);
                pthread_mutex_lock(&pot_mux);
                for(int i = 0; i < first_five_max; i++) {
                    printf("{@;@}=~=~=-> UNFOLDING VAGINERS [%d] {%d} %d %d\n", i, tid, pot[i], cunt);
                }
                pthread_mutex_unlock(&pot_mux);
            //}
        }
//#endif
        //SHOW_POT;
    }

    return 0;

}

void * jumble_spec_pot(void *tinfo)
{

    struct thread_info *ti = (struct thread_info*)tinfo;
    const int tid = ti->thread_id;
    const int creeping_junkie = 10000;
    int cunt = 0;
    int rand_index1, rand_index2;
    int still_jumbling; 
    
    pthread_mutex_lock(&jumbling_mux);
    still_jumbling = JUMBLING;
    pthread_mutex_unlock(&jumbling_mux);

    while(still_jumbling) {
        usleep(creeping_junkie);
        BEG_SWAP:

        //printf("FINGER BLASTING GRANNIES~ {%d;%d}~=~+~=~=>\n", cunt, huge_gunt);
        //SHOW_SPEC_POT;

        rand_index1 = get_rand_num(0, spec_max-1);
        rand_index2 = get_rand_num(0, spec_max-1);
        while(rand_index1 == rand_index2) 
            rand_index2 = get_rand_num(0, spec_max-1);
        pthread_mutex_lock(&pot_spec_mux);
        if(!spec_pot[rand_index1] || !spec_pot[rand_index2]) {
            pthread_mutex_unlock(&pot_spec_mux);
            goto BEG_SWAP;
        }
        int tmp = spec_pot[rand_index1];
        spec_pot[rand_index1] = spec_pot[rand_index2];
        spec_pot[rand_index2] = tmp;
        pthread_mutex_unlock(&pot_spec_mux);
        pthread_mutex_lock(&jumbling_mux);
        still_jumbling = JUMBLING;
        huge_gunt++;
        pthread_mutex_unlock(&jumbling_mux);
        

#ifdef GROWING_BONERS
        int n_threads = 100; //sizeof(pot_threads)/sizeof(pot_threads[0]);
        int blumpkinhouse = get_rand_num(0, n_threads-1);
        if((++cunt % 1000) == 0) {
            if(tid == blumpkinhouse) {
                printf("~JUMBLING VAGINERS{%d}: %d %d <%d>\n", tid, cunt, huge_gunt, n_threads);
                pthread_mutex_lock(&pot_spec_mux);
                for(int i = 0; i < spec_max; i++) {
                    printf("{@;@}=~=~=-> UNFOLDING VAGINERS [%d] {%d} %d %d\n", i, tid, spec_pot[i], cunt);
                }
                pthread_mutex_unlock(&pot_spec_mux);
            }
        }
#endif
        //SHOW_POT;
    }

    return 0;

}

void * frump$junkie_pot(void *tinfo)
{

    struct thread_info *ti = (struct thread_info*)tinfo;
    int tid = ti->thread_id;

    //for(int i = 0; i < first_five_max; i++)
      //  printf("~~FISTING JUNKIES IN DOWNTOWN CHICAGO[%d] %d\n", i, pot[i]); 
    
    //int *pot = ((struct thread_info*)tinfo)->pot;
    int cunt = 0; 
    //printf("~~~JUMBLING BONERS~~~ %d\n", tid);
    //exit(9);
    int creeping_junkie = 100000;

    creeping_junkie = 10000;

    //plock
    int still_jumbling; // = JUMBLING;
    //punlock
    //int *boner_pot;
    
    
    
    pthread_mutex_lock(&jumbling_mux);

    //boner_pot = pot;
    still_jumbling = JUMBLING;

    pthread_mutex_unlock(&jumbling_mux);

    // let the pot jumble for a bit
    //usleep(5000);
    

    while(still_jumbling) {
        //printf("~~~JUMBLING JUNKIES~~~ ");
        //fflush(stdout);
        //sleep(1);
        
        //printf("~~~JUMBLING JUNKIES~~~ %d %d\n", creeping_junkie, cunt);
        usleep(creeping_junkie);

        //int pot_created;
        //pthread_mutex_lock(&mux1);
        //pot_created = pot != 0;
        //printf("~~~SPLOOGING JUNKIES~~~ %d\n", pot_created);
        //pthread_mutex_unlock(&mux1);
        

        //if(pot_created) {
            int rand_index1, rand_index2;
            //printf("~~~STUMBLING JUNKIES~~~\n");
            
            BEG_SWAP:
            rand_index1 = get_rand_num(0, first_five_max-1);
            rand_index2 = get_rand_num(0, first_five_max-1);
            int massive_gunt = 0;
            while(rand_index1 == rand_index2) {
                //printf("GURGLING GRANNIES IN GEORGIA %d %d %d %d\n", rand_index1, rand_index2, cunt, ++massive_gunt);
                rand_index2 = get_rand_num(0, first_five_max-1);
            }

            pthread_mutex_lock(&pot_mux);
            /*
            if((cunt % 100) == 0)
                printf("SWAPPING BONERS[%d/%d](%d) {%d %d} %d %d\n", 
                    rand_index1, rand_index2, tid, huge_gunt, cunt, pot[rand_index1], pot[rand_index2]);
            */
            if(!pot[rand_index1] || !pot[rand_index2]) {
                //printf("GURGLING VAGINERS\n");
                //for(int i = 0; i < first_five_max; i++) {
                    //printf("GURGLING VAGINERS I WAS[%d] %d %d\n", i, pot[i], cunt);
                //}
                pthread_mutex_unlock(&pot_mux);
                goto BEG_SWAP;
            }

            int tmp = pot[rand_index1];
            //int tmp2 = pot[rand_index2];
            pot[rand_index1] = pot[rand_index2];
            pot[rand_index2] = tmp;

            /*
            if((cunt % 100) == 0) {
                printf("SWAPPING VAGINERS: (%d) %d %d %d %d\n", tid, huge_gunt, cunt, pot[rand_index1], pot[rand_index2]);
                for(int i = 0; i < first_five_max; i++) {
                    //printf("GURGLING GRANNIES IN GOFFSTOWN[%d] %d %d\n", i, pot[i], cunt);
                }
            }
            */
            pthread_mutex_unlock(&pot_mux);
 
        //} else {

            //pthread_mutex_lock(&mux1);
            //boner_pot = pot;
            //pthread_mutex_unlock(&mux1);
            //printf("SLAPPING BONERS\n");

        //}

        pthread_mutex_lock(&jumbling_mux);

        //boner_pot = pot;
        still_jumbling = JUMBLING;
        //pot_created = pot != 0;
        huge_gunt++;

        pthread_mutex_unlock(&jumbling_mux);


        cunt++;
        
        if((cunt % 10000) == 0) {
            printf("~COUNTING VAGINERS I DID{%d}: %d %d\n", tid, cunt, huge_gunt);
            if(tid == 2) {
                pthread_mutex_lock(&pot_mux);
                for(int i = 0; i < first_five_max; i++) {
                    printf("{@;@}=~=~=-> UNFOLDING BONERS IN DOWNTOWN MANHATTAN[%d] {%d} %d %d\n", i, tid, pot[i], cunt);
                }
                pthread_mutex_unlock(&pot_mux);
            }
        }

        //usleep(5000);
        //sleep(1);
        //usleep(10000);
    }

    //while(1)
    //printf("~~~UNLOCKING VA-JAY-JAYS~~~\n");

    return 0;
}

int fill_lottery_nums_rand_v3(ticket *t)
{
    /*
    if(invalid_restrictions()) {
        printf("~~~invalid_restrictions~~~\n");
        return FAILURE;
    }
    */

    int use_rand_index = 0;
    int ball_index, ball_val;
    
    use_rand_index = 1;
    int colossol_gunt = 0;

    for(int i = 0; i < 5; i++) {
        do {
            if(use_rand_index) 
                ball_index = get_rand_lot_num(0, first_five_max-(i+1));
            else 
                ball_index = i;
            pthread_mutex_lock(&pot_mux);
            ball_val = pot[ball_index];
            pthread_mutex_unlock(&pot_mux);
            //printf("PUNCHING BONERS %d %d\n", ball_val, colossol_gunt++);
        } while(!ball_val || (ball_val < RANGE_LOWER || ball_val > RANGE_UPPER));

        pthread_mutex_lock(&pot_mux);
        pot[ball_index] = 0;
        pthread_mutex_unlock(&pot_mux);

        t->lnums[t->n_draws][i] = ball_val;
    }

    if(use_rand_index) {
        ball_index = get_rand_lot_num(0, spec_max-1);
    } else {
        ball_index = 0;
    }

    pthread_mutex_lock(&pot_spec_mux);
    ball_val = spec_pot[ball_index];
    pthread_mutex_unlock(&pot_spec_mux);

    t->lnums[t->n_draws][5] = ball_val; 
    draw_in_order(t->lnums[t->n_draws]);

    for(int i = 0; 0 && i < N_GAPS; i++) {
      
        //printf("GAPING JUNKIES BIG TIME[%d]\n", i);
        struct gap *g = &gaps[i];
        if(GAP_SET(g)) {
            printf("GAPING VAGINERS BIG TIME[%d]\n", i);
            exit(9);
        }
    }


    t->n_draws++;

    return SUCCESS;
}



void show_weights(int ball_num, int * weights)
{
    printf("SHOWING BONERS[%d]\n", ball_num);
    for(int j = 0; j < 100; j++) {
        printf("[");
        if(j < 10)
            printf(" "); 
        printf("%d] ", j);
        if(weights[j] < 10)
            printf(" ");
        printf("%d-", weights[j]); // = ?;
        if(((j+1)%10) ==0)
            printf("\n");
    }
}

void fill_weights2(int cur_val, int ball_num, int **weightsy, ticket *t)
{
    int *weights = *weightsy;
    //printf("VALUING JUNKIES[%d]\n", cur_val);
    memset(weights, 0, sizeof(int)*sizeof(weights)/sizeof(weights[0]));
    
    int cur_weight = cur_val + 1;
    for(int i = 0; i < 100; i++) {
        //if(((i+1)%10)==0)
            
        if(i < 9) {
            //cur_weight++;
            weights[i] = cur_weight++;
            //printf("FINGER BLASTING GRANNIES[%d]\n", i);
        } else if(i < 20) {
            weights[i] = cur_weight++;
        }  else if(i < 40) {
            if(i == 20) {
                cur_weight = weights[i-10];
                //printf("FINGER BLASTING JUNKIES[%d] %d\n", i, cur_weight);
            }
            weights[i] = cur_weight++;
        } else if(i < 65) {
            if(i == 40) {
                cur_weight = weights[i-10];
                //printf("FINGER BLASTING TRANNIES[%d] %d\n", i, cur_weight);
            }
            weights[i] = cur_weight++;
        } else if(i < 70) {
            cur_weight += 5;
            weights[i] = cur_weight;
        //} else if(i < 80) {
        } else {
            if(i == 70) {
                cur_weight = weights[29]; //cur_val + 1;
            }
            weights[i] = cur_weight++;
        }   
            

    }

    //show_weights(ball_num, weights);


    for(int i = 0; i < 100; i++) {
        int cur_weight = weights[i];
        if(end) {
            if(cur_weight > end-(4-ball_num)) {
                //printf("~BENDING VAGINERS[%d] %d %d\n", ball_num, cur_weight, end-(4-ball_num));
                weights[i] = end -(4-ball_num);
            }
        } else if(cur_weight >= (RANGE_UPPER-(4-ball_num))) {
            //printf("~BENDING WEINERS[%d] %d %d\n", ball_num, cur_weight, end-(4-ball_num));   
            weights[i] = (RANGE_UPPER-(4-ball_num));
        }
        //printf("~BENDING WEINERS[%d] %d %d\n", ball_num, cur_weight, RANGE_UPPER-(4-ball_num));
    }

    //show_weights(ball_num, weights);
    //printf("~OVULATING GRANNIES~\n");
    //exit(9);

}

void fill_weights1(int cur_val, int i, int **weightsy, ticket *t)
{
    int *weights = *weightsy;
    memset(weights, 0, sizeof(int)*sizeof(weights)/sizeof(weights[0]));

    int cur_weight = cur_val + 1;
    for(int k = 0; k < 100; k++) {
        printf("FINGER BLASTING JUNKIES[%ld]\n", sizeof(int)*sizeof(weights)/sizeof(weights[0]));
                int j = k;
        if(k < 9) {
            cur_weight++;
                    if(end && cur_weight > end)
                        cur_weight = end -(4-i);
                    else if(cur_weight >= (RANGE_UPPER-(4-i)))
                        cur_weight = (RANGE_UPPER-(4-i));
                    
                    
                    weights[j] = cur_weight;
                } else if(k < 20) {
                    weights[j] = cur_weight;
                } else if(k == 20) {
                    cur_weight += 5;
                    
                    if(end && cur_weight > end)
                        cur_weight = end -(4-i);
                    else if(cur_weight >= (RANGE_UPPER-(4-i)))
                        cur_weight = (RANGE_UPPER-(4-i));
                    weights[j] = cur_weight;
                } else if(k < 40) {
                    weights[j] = cur_weight;
                //} else if(k == 30) {
                    //cur_weight = cur_val + 10;
                    //weights[j] = cur_weight;
                //} else if(k < 40) {
                  //  weights[j] = cur_weight;
                } else if(k == 40) {
                    cur_weight += 10; //5;
                    if(end && cur_weight > end)
                        cur_weight = end -(4-i);
                    else if(cur_weight >= (RANGE_UPPER-(4-i)))
                        cur_weight = (RANGE_UPPER-(4-i));
                    
                    weights[j] = cur_weight;
                } else if(k < 50) {
                    weights[j] = cur_weight;
                } else if(k == 50) {
                    cur_weight += 10; //5;
                    if(end && cur_weight > end)
                        cur_weight = end -(4-i);
                    else if(cur_weight >= (RANGE_UPPER-(4-i)))
                        cur_weight = (RANGE_UPPER-(4-i));
                    
                    weights[j] = cur_weight;
                } else if(k < 60) {
                    weights[j] = cur_weight;
                } else if(k < 70) {
                    cur_weight += 10; //5;
                    if(end && cur_weight > end)
                        cur_weight = end -(4-i);
                    else if(cur_weight >= (RANGE_UPPER-(4-i)))
                        cur_weight = (RANGE_UPPER-(4-i));
                    
                    weights[j] = cur_weight; // % (RANGE_UPPER-(5-i));
                } else if(k == 70) {
                    cur_weight = cur_val + 20;
                    if(end && cur_weight > end)
                        cur_weight = end -(4-i);
                    else if(cur_weight >= (RANGE_UPPER-(4-i)))
                        cur_weight = (RANGE_UPPER-(4-i));
                    
                    weights[j] = cur_weight;
                } else if(k < 80) {
                    weights[j] = cur_weight;
                } else if(k == 80) {
                    cur_weight -= 10;
                    if(cur_weight < t->lnums[t->n_draws][i-1]) {
                        while(1)
                        printf("SMOOCHING BONERS[%d] %d %d\n", i, cur_weight, t->lnums[t->n_draws][i-1]);
                        exit(9);
                        cur_weight = t->lnums[t->n_draws][i-1]+1;
                    }
                    weights[j] = cur_weight;
                } else if(k < 90) {
                    weights[j] = cur_weight;
                } else if(k == 90) {
                    cur_weight += 10; //5;
                    if(end && cur_weight > end)
                        cur_weight = end -(4-i);
                    else if(cur_weight >= (RANGE_UPPER-(4-i)))
                        cur_weight = (RANGE_UPPER-(4-i));
                    
                    weights[j] = cur_weight;
                } else {
                    weights[j] = cur_weight;
                }

            }

            /*
            
            */

            printf("FOLDING BONERS IN ARKANSAS: %d\n",i);
            show_weights(i, weights);
    
}

//int fill_past_winnings(int lot_type, booklet *b, int n_entries);


int fill_lottery_nums_rand_v2(ticket *t)
{
    
//#ifdef DEBUG_MODE
    ticket past_winning_pb;
    memset(&past_winning_pb, 0, sizeof(past_winning_pb));
    //past_winning_pb.lnums[past_winning_pb.n_draws][0] = ?;

    booklet b;

    //if(fill_past_winnings(LOT_TYP_PB, &b, 10)) {
      //  printf("GURGLING JABRONIES %d %d\n", LOT_TYP_PB, 10);
    
    //}

    //TODO: be sure to check for consecs, gaps here too...
    if(invalid_restrictions()) {
        printf("~~~WHINING BONERS BIG TIMES~~~\n");
        exit(9);
        return FAILURE;
    }

    int cur_val = RANGE_LOWER;
    int weights[100] = { 0 }; 

    //int bending_boner = sizeof(weights)/sizeof(weights[0]); //((5-i) * 15); // + 24;
    
    
    for(int i = 0; i < 5; i++) {

        if(i == 0 && start) 
            t->lnums[t->n_draws][i] = cur_val = start;
        else if(0 && (i == 4 && end)) {
            if(t->lnums[t->n_draws][3] >= end) {
                printf("\n\n@~@WHINING VAGINERS[%d] %d %d %d %d\n", i, 
                        t->lnums[t->n_draws][0], t->lnums[t->n_draws][1],
                        t->lnums[t->n_draws][2], t->lnums[t->n_draws][3]);
                i = -1;
                memset(t->lnums[t->n_draws], 0, sizeof(t->lnums[t->n_draws]));
                memset(weights, 0, sizeof(weights));
                cur_val = 0; 
                continue;
            } else 
                t->lnums[t->n_draws][i] = end;
            printf("~--~ BENDING VAGINERS[%d] %d %d %d\n", i, cur_val, t->lnums[t->n_draws][3], end);
            
        } else {

           int *frumpy_cum_dumpster = weights;
           
           //fill_weights1(cur_val, i, &frumpy_cum_dumpster, t);
           //fill_weights2(cur_val, i, (int **)&weights[0], t);
           fill_weights2(cur_val, i, &frumpy_cum_dumpster, t);

            
            
            

            //printf("GRANDPA'S GETTIN' GOOSE BONERS: %d\n",i);
            //exit(9);

            //continue;

            //while(1) {
            int rand_weight_index = get_rand_num(0, sizeof(weights)/sizeof(weights[0])-1);
            //printf("~!@~!OVERDOSING NANNIES[%d] %d\n", i, rand_weight_index);
            int rand_upper_range = weights[rand_weight_index];
            //printf("~!@~!OVERDOSING GRANNIES[%d] %d %d\n", i, rand_upper_range,cur_val);
                //if(cur_val + 1 -1 <= rand_upper_range) {
            if(cur_val + 1 -1 > rand_upper_range) {
                printf("~!@~!@OVERDOSING TRANNIES[%d] %d %d\n", i, cur_val + 1 , rand_upper_range);
                exit(9);
            }   
            //int frumpy_values = cur_val;
            //if(i > 0)
              //  frumpy_values++;    
            //printf("RE-ARRANGING BLUMPKINS BIG TIMES[%d] %d %d\n", i,cur_val + 1 , rand_upper_range);
            cur_val = get_rand_lot_num(cur_val+1, rand_upper_range);
            //printf("BENDING VAGINERS FROM HERE TO TIMBUCKTOO[%d] %d\n",i, cur_val);
            t->lnums[t->n_draws][i] = cur_val;
        }

   

    }
    t->lnums[t->n_draws][5] = get_rand_lot_num(1, spec_max);
    //present_cur_draw();

    //for(int i = 0; i < 6; i++) 
      //  daily_ticket.lnums[daily_ticket.n_draws][i] = lnums[i];
    
    
    if(t->lnums[t->n_draws][0] >= 20) {
                //printf("   ____^______\n");
                //for(int i = 0; i < 50; i++)
                //printf(">~-(GRANDMA'S GETTIN' A BIG BROWN WHALE TONIGHT);>\n");
            } 
    
    t->n_draws++;
    //printf("")
    

    return SUCCESS;
}

/*
            int start_at = (cur_val + 10) % (RANGE_UPPER-(5-i));

            start_at = (i + 1) * 10; // * 2;
            int stop_at = start_at; // + 5;
            int cur_weight = start_at;
            int cnt = 0;
            for(int j = 0; j < 100; j++) {

                //printf("BENDING VA-JAY-JAYS[%d/%d] %d %d\n",i,j, cur_val, stop_at);
                if(j == stop_at) {
                    printf("GURGLING GRANNIES[%d/%d] %d %d %d %d\n",i,j, cur_weight, cur_val, start_at, stop_at);
                    //exit(9);
                    //stop_at += (6-i) * 2;
                    stop_at += start_at; //5;
                    cur_weight = start_at; // % (RANGE_UPPER-(5-i));;
                    if(cur_weight >= (RANGE_UPPER-(5-i)))
                        cur_weight = RANGE_UPPER-(5-i);
                }
                /
                if(i == 4)
                    weights[j] = RANGE_UPPER-(5-i);
                else {
                    int frump_dumpster = (((cur_val+1) + j) % (RANGE_UPPER-(5-i))) + 1;
                    printf("BENDING VA-JAY-JAYS[%d/%d] %d\n",i,j, frump_dumpster);
                    //exit(9);
                    weights[j] = frump_dumpster;
                }
                /
               printf("BENDING VA-JAY-JAYS[%d/%d] %d %d\n",i,j, start_at, cur_weight);
               
               //weights[j] = cur_weight % (RANGE_UPPER-(5-i));;
               if(cur_weight >= (RANGE_UPPER-(5-i)))
                cur_weight = RANGE_UPPER-(5-i);

                weights[j] = cur_weight;
               //else
                //cur_weight = 


               cnt++;
               if(cnt > 5) 
                  cur_weight++;
                if(cnt == 10)
                    cnt = 0;
                //printf("COUNTING VA-JAY-JAYS: %d\n", cnt);
               
            }
            printf("BENDING VAGINERS TOO PLUS WON[%d] %d %d\n",i, cur_val, start_at);
            
            

            for(int j = 0; j < 100; j++) {
            printf("[");
            if(j < 10)
                printf(" ");
            printf("%d] %d-", j, weights[j]); // = ?;
            if(((j+1)%10) ==0)
                printf("\n");
        }
        printf("\nBALL BASHING TRANNIES[%d]~\n", i);
        //exit(9);

        if(i > 3) {
                printf("CREEPING JUNKIES\n");
                //exit(9);
            }

            int rand_weight_index = get_rand_lot_num(0, sizeof(weights)/sizeof(weights[0]));
            printf("OVERDOSING NANNIES[%d] %d\n", i, rand_weight_index);
            int rand_upper_range = weights[rand_weight_index];
            printf("OVERDOSING TRANNIES[%d] %d\n", i, rand_upper_range);
            cur_val = get_rand_lot_num(cur_val+1, rand_upper_range);
            printf("OVERDOSING GRANNIES[%d] %d\n", i, cur_val);

            printf("BENDING VAGINERS FROM HERE TO TIMBUCKTOO[%d] %d\n",i, cur_val);
            //exit(9);
            balls[i] = cur_val;

        }
    }

    for(int i = 0; i < 5; i++)
        lnums[i] = balls[i];
    
    present_cur_draw();
    return 1;
}
*/


/*


            //cur_val = i + 1;
            int stop_at = 10, cnt = cur_val, frumpster=1;
            for(int j = 0; j < 100; j++) {
                //int huge_nanny = (5-i);
                if(j == stop_at) {
                    stop_at += 10*(++frumpster);
                    //incr *= (i+1);
                    //printf("GRABBING VAGINERS[%d/%d] %d\n", i,j, stop_at);
                    cnt = cur_val;
                } 
                weights[j] = (cnt++) % (RANGE_UPPER-(5-i));
                
                printf("COUNTING VAGINERS[%d/%d]: %d\n", i,j,cnt);
                if(weights[j] < cur_val) {
                    printf("POUTING VAGINERS[%d/%d] %d %d\n",i, j, weights[j], cur_val);
                    exit(9);
                }
            }
            //printf("NABBING VAGINERS\n");
            //exit(9);
            //cur_val = get_rand_lot_num();
            int rand_weight_index = get_rand_lot_num(0, sizeof(weights)/sizeof(weights[0]));
            printf("OVERDOSING NANNIES[%d] %d\n", i, rand_weight_index);
            int rand_upper_range = weights[rand_weight_index];
            printf("OVERDOSING TRANNIES[%d] %d\n", i, rand_upper_range);
            cur_val = get_rand_lot_num(cur_val+1, rand_upper_range);
            printf("OVERDOSING GRANNIES[%d] %d\n", i, cur_val);
        }

        for(int j = 0; j < 100; j++) {
            printf("[");
            if(j < 10)
                printf(" ");
            printf("%d] %d-", j, weights[j]); // = ?;
            if(((j+1)%10) ==0)
                printf("\n");
        }
        printf("\nBALL BASHING TRANNIES[%d]~\n", i);
        exit(9);


    }


    return !SUCCESS;
}
*/


/*

            bending_boner = ((5-i) * 15) + 24;
            printf("BENDING VAGINERS IN VENEZUALA[%d] %d\n",i,bending_boner);
            int j;
            for(j = 0; j < bending_boner; j++) {
                int frumpy = (cur_val + (j % (i * bending_boner%10))); // % 65;
                //printf("BENDING BONERS IN BRUSSELS[%d] %d %d\n",i,bending_boner, frumpy);
                //exit(9);
                frumpy %= RANGE_UPPER-(5-i);
                //printf("BENDING WEINERS IN WALES[%d/%d] %d %d\n",i,j,bending_boner, frumpy);
                //exit(9);
                weights[j] = frumpy;

                int frumphouse = ((j / 10) + 1) * (i+1);
                printf("BENDING BONERS IN BRUSSELS %d\n", frumphouse);

                int bonerhouse = cur_val + frumphouse;
                printf("BENDING BONERS IN BETHLEHEM %d\n", bonerhouse);

                

            }

            for( ; j < 100; j++) {
                //weights[i] = 12;
            }


        }

        for(int j = 0; j < 100; j++) {
            printf("[");
            if(j < 10)
                printf(" ");
            printf("%d] %d-", j, weights[j]); // = ?;
            if(((j+1)%10) ==0)
                printf("\n");
        }
        printf("\nBALL BASHING TRANNIES~\n");
        exit(9);

    }

    

    return FAILURE;

}
*/

/*
            bending_boner = ((5-i) * 15); // + 24;
            printf("BENDING BONERS IN IDAHO[%d] %d\n",i,bending_boner);

            int total = 0, frumphouse = 1;

            //bending_boner = 100;
            for(int j = 0; j < bending_boner; j++) {
                //int frumpster = (j % 5) + 1 + j;
                total += (j % 5) + 1; //frumpster;

                //int upper_range = j;
                weights[j] = total;
                
                printf("RE-ARRANGING BLUMPKINS[%d/%d] %d %d\n", i,j, frumphouse, total);
                if(total >= ((frumphouse++)%(bending_boner-j))) {
                    weights[j] = total;
                    total = 0;
                } else 
                    weights[j] = total;
                //if(frumphouse )

                //if(total >= RANGE_UPPER-(5-i)) {
                printf("YODELLING JUNKIES[%d/%d] %d %d\n", i,j, frumphouse, total);

                    //total =  (j % 5) + 1;
                //}
                
                //weights[j] = total;
                
                
            }
            //for(int q = sizeof(weights)/sizeof(weights[0]); q > sizeof(weights)/sizeof(weights[0])-3; q--) {
                weights[sizeof(weights)/sizeof(weights[0])-1] = RANGE_UPPER-(5-i);
                weights[sizeof(weights)/sizeof(weights[0])-11] = RANGE_UPPER-(5-i)-5;
                weights[sizeof(weights)/sizeof(weights[0])-22] = RANGE_UPPER-(5-i)-10;
                weights[sizeof(weights)/sizeof(weights[0])-32] = RANGE_UPPER-(5-i)-20;
                printf("GAPING JABRONIES: %ld\n", sizeof(weights)/sizeof(weights[0])-3);
            //if(!cur_val) {
                printf("\n\n\nCATCHING BONERS SOMEWHERE OUTSIDE OF TALLAHASSEE[%d]: %d\n", 9, cur_val);
                //exit(9);
            //}
            if(i == 0) {
                for(int j = 0; j < 100; j++) {
        printf("[");
        if(j < 10)
            printf(" ");
        printf("%d] ", j);
        if(weights[j] < 10)
            printf(" ");
        printf("%d-", weights[j]); // = ?;
        if(((j+1)%10) ==0)
            printf("\n");
    }
    printf("\nBALL BASHING TRANNIES~\n");
                printf("FOLDING BONERS SOMEWHERE OUTSIDE OF TALLAHASSEE: %d\n", cur_val);
                //exit(9);
            }
        

       

    printf("\n~=AFTER BALL BASHING NANNIES~\n");
            //BALL_BASH:
            int frumpness = get_rand_lot_num(0, ( sizeof(weights)/sizeof(weights[0]))-1 );
            printf("CLICKING VAGINERS: %d %d %d\n", frumpness, cur_val, weights[frumpness]);
            cur_val = get_rand_lot_num(cur_val+1, weights[frumpness]);
            printf("~~~WHINING VAGINERS BIG TIMES %d~~~\n", cur_val);
            balls[i] = cur_val;

    } // else
    }
    //cur_val = get_rand_lot_num(cur_val+1, weights[frumpness]);
    //balls[4] = cur_val;
    

   for(int i = 0; i < 5; i++)
        lnums[i] = balls[i];
    lnums[5] = 69;

    present_cur_draw();

    return FAILURE;
}
*/
    
    
/*
    // RANGE_UPPER-(4-i), RANGE_LOWER+};
    //weights[9] = 
    for(int j = 0; j < 100; j++) {
        printf("[");
        if(j < 10)
            printf(" ");
        printf("%d] %d-", j, weights[j]); // = ?;
        if(((j+1)%10) ==0)
            printf("\n");
    }
    printf("\nBALL BASHING TRANNIES~\n");
    //exit(9);
    
    for(int i = 0; i < 5; i++) {
        if(i == 0 && start) {
            balls[i] = cur_val = start;
            printf("BENDING VAGINERS\n");
            exit(9);
        } else if(i == 4 && end) {
            printf("BENDING VAGINERS TOO\n");
            exit(9);
        } else {

            if(!cur_val) {
                printf("CATCHING BONERS SOMEWHERE OUTSIDE OF TALLAHASSEE\n");
                exit(9);
            }
            int bending_boner = ((5-i) * 15) + 24;
            printf("BENDING BONERS[%d] %d\n",i,bending_boner);
            
            //exit(9);
            int frump = cur_val+1;
            if(frump > RANGE_UPPER-(5-i))
                frump = RANGE_UPPER-(5-i);
            //printf("GRIPING PEENIES: %d\n", frump);
            //exit(9);
            
            int frump_cur_val = cur_val + 1;
            for(int j = 0; j < bending_boner; j++) {
                frump = cur_val+1 + j;
                //printf("GRIPING PEENIES AGAIN: %d\n", frump);
                
                if(frump > 20 - (i * 5)) {
                    //printf("GRIPING WEENIES\n");
                    //exit(9);
                    frump = frump_cur_val;
                    //if(frump > RANGE_UPPER-(5-i))
                      //  frump = RANGE_UPPER-(5-i);
                }
                weights[j] = frump;
            }

            for(int j = bending_boner; j < 100; j++) {
                weights[j] = RANGE_UPPER-(5-i);
            }
            //memcpy(weights, 100, sizeof(int));
            //memset(weights, 1, 100*sizeof(weights[0]));
            //weights[sizeof(weights)/sizeof(weights[0])-1] = RANGE_UPPER-(5-i);
            //break;
            printf("\n~=BALL BASHING NANNIES[%d]~\n", i);
            for(int j = 0; j < 100; j++) {
                printf("[");
                if(j < 10)
                    printf(" ");
                printf("%d] %d-", j, weights[j]); // = ?;
                if(((j+1)%10) ==0)
                    printf("\n");
            }
            printf("\n~=AFTER BALL BASHING NANNIES~\n");

            int frumpness = get_rand_lot_num(0, ( sizeof(weights)/sizeof(weights[0]))-1 );
            printf("CLICKING VAGINERS: %d %d %d\n", frumpness, cur_val, weights[frumpness]);
            cur_val = get_rand_lot_num(cur_val+1, weights[frumpness]);
            printf("~~~WHINING VAGINERS BIG TIMES %d~~~\n", cur_val);
            balls[i] = cur_val;
        }
    }

    

    
            
            
    printf("\nBALL BASHING GRANNIES~\n");
    //exit(9);

   for(int i = 0; i < 5; i++)
        lnums[i] = balls[i];
    lnums[5] = 69;

    present_cur_draw();

    return FAILURE;
}
*/

            /*
            int frump_dumpster = 0;
            for(int j = 0; j < 10; j++) {
                int frump = cur_val + j;
                printf("SMILING BONERS: %d\n", frump);
                if(i > 1) {
                    printf("SMILING WEINERS: %d\n", frump);
                    //exit(9);
                }
                frump_dumpster += frump;
                if(frump_dumpster > RANGE_UPPER-(5-i)) {
                    int huge_frump_dumpster = frump_dumpster % (RANGE_UPPER-(5-i)); 
                    printf("BENDING WEINERS[%d/%d] %d %d %d\n", i,j, frump_dumpster, huge_frump_dumpster, RANGE_UPPER-(5-i));
                    //exit(9);
                    frump_dumpster = frump = RANGE_UPPER-(5-i);
                    //exit(9);
                    for(int k = j; k < 10; k++) {
                        printf("SWINGIN' DILDOS[%d]\n", k);
                        weights[k] = frump;
                    }

                    for(int q = 0; q < 10; q++) {
                        printf("SWINGIN' BONERS[%d] %d\n", q, weights[q]);
                    }

                    printf("SMILING VAGINERS: %d\n", frump);
                    //exit(9);

                    break;
                }
                weights[j] = frump;
                printf("BENDING VA-JAY-JAYS[%d] %d %d\n", i, frump, frump_dumpster);
                
            }
            weights[9] = RANGE_UPPER-(5-i);
            int frumpness = get_rand_lot_num(0, (sizeof(weights)/sizeof(weights[0]))-1);
            printf("CLICKING VAGINERS: %d %d %d\n", frumpness, cur_val, weights[frumpness]);
            cur_val = get_rand_lot_num(cur_val+1, weights[frumpness]);
            printf("~~~WHINING VAGINERS BIG TIMES %d~~~\n", cur_val);
            balls[i] = cur_val; // = cur_val;
        }
    }
    for(int i = 0; i < 5; i++)
        lnums[i] = balls[i];
    lnums[5] = 69;

    present_cur_draw();

    return FAILURE;
}
*/

/* Think of it in 'sections'... */
#if 0
int fill_lottery_nums_rand_vwow()
{

    //TODO: be sure to check for consecs, gaps here too...
    if(invalid_restrictions()) {
        printf("~~~WHINING BONERS BIG TIMES~~~\n");
        exit(9);
        return FAILURE;
    }


    //int ball1 = get_rand_lot_num(RANGE_LOWER, RANGE_UPPER-4);
    //printf("~~~free ballin' NANNIES BIG TIME %d~~~\n", ball1);
    //exit(9);
    //int cur_val;

    int balls[5] = {0}, cur_val;

    int cunt = 0;

    int frumpsters[] = { //RANGE_LOWER+1, RANGE_LOWER+2, RANGE_LOWER+3,
                         RANGE_LOWER+20, RANGE_LOWER+21, RANGE_LOWER+22, 
                         RANGE_UPPER-4};

    //int draw_complete = 0;
    int i; // = 0;
    //do {

        int frumpster = frumpsters[get_rand_lot_num(0, (sizeof(frumpsters)/sizeof(frumpsters[0])-1))];
        printf("GRANDPA'S GETTIN' ANAL BIG TIME: %d\n", frumpster);
        
        START_DRAW:
        cunt++;
        cur_val = 0;
        memset(balls, 0, sizeof(balls));

        if(start) {
            balls[0] = cur_val = start;
        } else {
            //balls[0] = cur_val = get_rand_lot_num(RANGE_LOWER, RANGE_UPPER-4);
            balls[0] = cur_val = get_rand_lot_num(RANGE_LOWER, frumpster);
            printf("DROP KICKING BONERS 1 %d\n", balls[0]);
            //exit(9);
        }
        int huge_gunt = 0, prev_val;
        for(i = 1; i < 5; i++) {

            //REDO_BALL:
            huge_gunt++;
            prev_val = cur_val;
            if(consecs[i-1])
                balls[i] = cur_val = (balls[i-1] + 1);
            else if(intervals[i-1]) 
                balls[i] = cur_val = (balls[i-1] + intervals[i-1]);
            else {
                printf("GRABBING BONERS[%d] %d\n", i, cur_val);
                if(i == 4 && end) {
                    /*
                    if(balls[i-1] >= end) {
                        printf("DROOPING BOOBIES[%d] %d %d~~~\n", i, balls[i-1], end);
                        //exit(9);
                        goto START_DRAW;
                    } else {
                        printf("GRABBING DILDOS[%d]\n", i);
                        //exit(9);
                        balls[i] = end;
                    }
                    */
                    balls[i] = end;
                } else {
                    if(i > 3) {
                        if(cur_val < 45) {
                            printf("{@;@}=====> CRYING BONERS\n\n\n");
                            //cur_val = 60;
                            int big_frumpsters[] = { //cur_val+10, cur_val+11, cur_val+12,
                                cur_val, 45, 45,45,45,45, //cur_val+15, 
                                };
                                //balls[i] = cur_val = get_rand_lot_num(RANGE_LOWER, RANGE_UPPER-(i+1));
                        
                            int huge_frumpster = big_frumpsters[get_rand_lot_num(0, (sizeof(big_frumpsters)/sizeof(big_frumpsters[0])-1))];
                            balls[i] = cur_val = get_rand_lot_num(huge_frumpster, RANGE_UPPER);
        
                        }else 
                            balls[i] = cur_val = get_rand_lot_num(cur_val+1, RANGE_UPPER);
                        
                    }
                    //balls[i] = ++cur_val; // account for the ball
                    //int frumpster = get_rand_lot_num(cur_val+1, RANGE_UPPER);
                    else if(RANGE_UPPER - cur_val >=40) {
                        int big_frumpsters[] = { //cur_val+10, cur_val+11, cur_val+12,
                         cur_val+40, cur_val+40, cur_val+40, //cur_val+15, 
                         RANGE_UPPER-(4-i)};
                        //balls[i] = cur_val = get_rand_lot_num(RANGE_LOWER, RANGE_UPPER-(i+1));
                        
                        int huge_frumpster = big_frumpsters[get_rand_lot_num(0, (sizeof(big_frumpsters)/sizeof(big_frumpsters[0])-1))];
        
                        balls[i] = cur_val = get_rand_lot_num(RANGE_LOWER, huge_frumpster);
                    }
                    else if(RANGE_UPPER - cur_val >=30) {
                        int big_frumpsters[] = { //cur_val+10, cur_val+11, cur_val+12,
                         cur_val+30, cur_val+30, cur_val+30, //cur_val+15, 
                         RANGE_UPPER-(4-i)};
                        //balls[i] = cur_val = get_rand_lot_num(RANGE_LOWER, RANGE_UPPER-(i+1));
                        
                        int huge_frumpster = big_frumpsters[get_rand_lot_num(0, (sizeof(big_frumpsters)/sizeof(big_frumpsters[0])-1))];
        
                        balls[i] = cur_val = get_rand_lot_num(RANGE_LOWER, huge_frumpster);
                    } else if(RANGE_UPPER - cur_val >=20) {
                        int big_frumpsters[] = { //cur_val+10, cur_val+11, cur_val+12,
                         cur_val+20, cur_val+20, cur_val+20, //cur_val+15, 
                         RANGE_UPPER-(4-i)};
                        //balls[i] = cur_val = get_rand_lot_num(RANGE_LOWER, RANGE_UPPER-(i+1));
                        
                        int huge_frumpster = big_frumpsters[get_rand_lot_num(0, (sizeof(big_frumpsters)/sizeof(big_frumpsters[0])-1))];
        
                        balls[i] = cur_val = get_rand_lot_num(RANGE_LOWER, huge_frumpster);
                    } else if(RANGE_UPPER - cur_val >=10) {
                        int big_frumpsters[] = { //cur_val+10, cur_val+11, cur_val+12,
                         cur_val+10, cur_val+10, cur_val+10, //cur_val+15, 
                         RANGE_UPPER-(4-i)};
                        //balls[i] = cur_val = get_rand_lot_num(RANGE_LOWER, RANGE_UPPER-(i+1));
                        
                        int huge_frumpster = big_frumpsters[get_rand_lot_num(0, (sizeof(big_frumpsters)/sizeof(big_frumpsters[0])-1))];
        
                        balls[i] = cur_val = get_rand_lot_num(RANGE_LOWER, huge_frumpster);
                    } else {
                        
                        printf("FROWNING BONERS\n");
                        //exit(9);
                        balls[i] = cur_val = get_rand_lot_num(cur_val+1, RANGE_UPPER-(4-i));
                    }
                    //balls[i] = cur_val = get_rand_lot_num(cur_val+1, RANGE_UPPER);
                    

                    //balls[i] = cur_val = get_rand_lot_num(cur_val+1, RANGE_UPPER-(i) );
                    printf("DROP KICKING VAGINERS 2[%d]: %d %d %d %d\n", i, cur_val, balls[i], RANGE_UPPER, RANGE_UPPER-((5-1)-i));
                    //exit(9);
                }
            }

            for(int j = 0; j < i; j++) {
                printf("~~~free ballin' NANNIES BIG TIME[%d] %d %d~~~\n", j, i, balls[j]);
            }
    
            printf("~~~free ballin' GRANNIES 2 %d~~~\n", cur_val);
            if(cur_val > RANGE_UPPER) {
                printf("DROOPING WEENESES 1 %d %d %d~~~\n", cunt, cur_val, RANGE_UPPER);
                //exit(9);
                //return 1;
                //continue;
                
                goto START_DRAW;
                //balls[i] = 0;
                //cur_val = prev_val;
                //i--;
                //goto REDO_BALL;
            } else if(balls[i-1] == balls[i]) {
                printf("DROOPING WEENESES 2~~~[%d] %d %d~~~\n", i, balls[i-1], balls[i]);
                //exit(9);
                //continue;
                goto START_DRAW;
            }

            //if(i == 4) {
              //  printf("BREAKING VAGINERS\n");
                //break;
            //}
            printf("BREAKING VAGINERS: %d\n", huge_gunt);
        }

    //} while(i < 4); // while(cur_val > RANGE_UPPER);
        
   

    for(int i = 0; i < 5; i++) {
        printf("~~~free ballin' TRANNIES BIG TIME[%d] %d~~~\n", i, balls[i]);
        lnums[i] = balls[i];
    }

    lnums[5] = get_rand_lot_num(1, spec_max);

    //present_cur_draw();
    printf("~~~GAPING GRANNIES BIG TIME %d (%d)~~~\n", cur_val, cunt);
    //exit(9);

    return SUCCESS;

}
#endif

//int frump_spec_nums[30] = {0};
//int frump_cnt = 0;

int fill_lottery_nums_rand_v1(ticket *t)
{

    if(GAPS_SET) { //} || CONSECS_SET || INTERVALS_SET) {
        printf("Version 1 does not support GAPS...\n");
        printf("\t(no draw)\n");
        return FAILURE;
    }

    //printf("GAPING NANNIES: %d\n", t->n_draws);

    int rand_int;
    for(int i = 0; i < 6; i++) {
        if(i < 5) {
            GET_NEXT_RAND_INT:
            rand_int = get_rand_lot_num(RANGE_LOWER, RANGE_UPPER);

            for(int j = 0; j < i; j++) {
                //printf("CORNPOP WAS UNFOLDING BONERS{%d;%d} %d %d\n", i, j, t->lnums[t->n_draws][i], rand_int);
                if(t->lnums[t->n_draws][j] == rand_int) {
                    
                    /*while(1) {
                    printf("GRANDPA'S GETTIN' ");
                    if(spec_max == MM_SPEC_MAX)
                        printf("G");
                    else
                        printf("M");
                    printf("OOSE BONERS TONIGHT!!!\n"); 
                    printf("\t[%d] (%d==%d)\n", j, t->lnums[t->n_draws][j], rand_int);
                    }*/ 
                    goto GET_NEXT_RAND_INT;
                }
            }
            t->lnums[t->n_draws][i] = rand_int; 
        } else {
            
            /*
            int frump_dumpster;
            //time_t tm = time(0);
            //srand((unsigned) time(&tm));
            BLOWING_CHODES:
            frump_dumpster = get_rand_lot_num(1, spec_max);
            //printf("FLAPPING VAGINERS %d\n", frump_dumpster);
            if(frump_cnt < spec_max) {
                if(frump_spec_nums[frump_dumpster]++) {
                    printf("GURGLING BONERS BIG TIMES\n");
                    //exit(9);
                    goto BLOWING_CHODES;
                }
                frump_cnt++;
            }
            */
            
            t->lnums[t->n_draws][i] = get_rand_lot_num(1, spec_max);
            //printf("SLAPPING VAGINERS %d\n", t->lnums[t->n_draws][i]);
        }
    }
 
    draw_in_order(  t->lnums[t->n_draws++]   );

    //for(int i = 0; i < 5; i++) {
        //printf("DONE FOLDING BONERS[%d]~\n", i);
      //  if(!t->lnums[t->n_draws][i]) {
        //    printf("CORNPOP WAS DONE FOLDING BONERS\n");
          //  exit(9);
        //}
    //}  

    //t->n_draws++;
    
    
    //print_ticket
    return SUCCESS;
    
   
}

void show_ticket(ticket *t)
{
    printf("-#_#-#_----__ TICKET __----_#-#_#-\n");
    printf("  contains %d draw", t->n_draws);
    if(t->n_draws > 1)
        printf("s");
    printf(".\n");
    if(t == &daily_ticket)
        printf("  (is the DAILY ticket)\n");
    else
        printf("  (is CURRENT ticket)\n");
    if(t->n_draws == 0)
        printf("   Ticket contains no draws.\n");
    for(int i = 0; i < t->n_draws; i++) 
        present_draw(i, t);
    printf("-#_#-#_----~-~-~-~-~-~-----_#-#_#-\n");

}

void show_daily_ticket()
{
    show_ticket(&daily_ticket);
}

void present_draw(int draw, ticket *t)
{

    printf("\n-Draw %d-+-+-+\n", draw+1);
    /*
    for(int i = 0; i < 5; i++) {
        printf("Ball [%d]...  ", i+1);
        //usleep(5000000);
        printf("%d\n", lnums[i]);
    }
    */
    if(t->n_draws == 0) {
        printf("No ticket has been drawn.\n");
        return;
    }

    //draw--;

    //for(int i = 0; i < 6; i++) {
    //    printf("INTERRUPTING BLUMPKINS I WAS[%d] %d\n", i, daily_ticket.lnums[draw][i]);
    //}
    //exit(9);
    
    //usleep(5000000);
    if(t->lnums[draw][0] >= 60) {
        printf("   ____^______\n");
        printf(">~(GOLDEN WHALE):>\n");
    } else if(t->lnums[draw][0] >= 50) {
        printf("   ____^______\n");
        printf(">~(YE OLD WHITE WHALE!):>\n");
    } else if(t->lnums[draw][0] >= 40) {
        printf("   ____^______\n");
        printf(">~(GRAY WHALE):>\n");
    } else if(t->lnums[draw][0] >= 30) {
        printf("   ____^______\n");
        printf(">~(BLACK WHALE):>\n");
    } else if(t->lnums[draw][0] >= 20) {
        printf("   ____^______\n");
        printf(">~(GRANDMA'S GETTIN' A BIG OL' BROWN WHALE);>\n");
    } else if(t->lnums[draw][0] >= 10) {
        printf("   ____^______\n");
        printf(">~(GREEN WHALE):>\n");
    } 
    printf("Special Ball: %d\n", t->lnums[draw][5]);
    printf("---[");
    if(t->lnums[draw][0] < 10)
        printf(" ");
    printf("%d]-[", t->lnums[draw][0]);
    if(t->lnums[draw][1] < 10)
        printf(" ");
    printf("%d]-[", t->lnums[draw][1]);
    if(t->lnums[draw][2] < 10)
        printf(" ");
    printf("%d]-[", t->lnums[draw][2]);
    if(t->lnums[draw][3] < 10)
        printf(" ");
    printf("%d]-[", t->lnums[draw][3]);
    if(t->lnums[draw][4] < 10)
        printf(" ");
    printf("%d]=:={", t->lnums[draw][4]);
    if(t->lnums[draw][5] < 10)
        printf(" ");
    printf("%d}---\n", t->lnums[draw][5]);
    //printf("%d]-[%d]-[%d]-[%d]=:={%d}---\n", 
    //    lnums[0], lnums[1], lnums[2], lnums[3], lnums[4], lnums[5]);
    printf("===================================================\n");
}


void display_daily_lotto_info();

    void print_gap(struct gap *g);
    void print_gaps();

int ATOI(char *str) {
    for(int i = 1; i < strlen(str); i++) {
        if(!isdigit(str[i])) {
            printf("BOUNCING BONERS~\n");
            return 0;
        }
    }
    return atoi(str); 
}

void display_draw_info()
{
    const char *div = "~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-\n";
    printf("%s", div);
    PRINT_RANGES;
    if(start)
        printf("Start=%d", start);
    if(end) {
        if(start)
            printf(", ");
        printf("end=%d", end);
    }
    if(start || end)
        printf("\n");
    printf("%s", div);
}


int parse_args(int argc, char *argv[], int *num_draws)
{
     
    printf("ARRANGING BONERS: %d %d %d\n", RANGE_LOWER, RANGE_UPPER, argc);

    for(int i = 1; i < argc; i++) {
        char *arg = argv[i];
        printf("RE-ARRANGING BONERS[%d] \"%s\"\n", i, arg);

        if(i == 1 && ATOI(argv[1])) {
            printf("YODELLING VA-JAY-JAYS: \n");
            //exit(9);
            *num_draws = atoi(argv[1]);
            //break;
            continue;
        }
        
        if(arg[0] != '-') {
            if(strcmp(arg, "info") == 0 || strcmp(arg, "INFO") == 0) {
                printf("RE-ARRANGING VA-JAY-JAYS\n");
                display_daily_lotto_info(lottery_day);
                return EXIT; //SUCCESS+2; // 
            } else {
                printf("DUMPING BONERS 1~~\n");
                return FAILURE;
            }
        }
        arg++;
            //printf("AFTER RE-ARRANGING BONERS[%d] \"%s\"\n", i, arg);
        printf("{@;@}=~=~=-> UNFOLDING BONERS IN LOWER MANHATTAN[%d]: \"%s\"\n", i, arg);
        int tmp;
        switch(arg[0]) {
                case 'R':
                case 'r':
                    if(strlen(&arg[0]) > 1) {
                        printf("GRIPING BONERS 1\n");
                        exit(1);
                    }
                    //printf("CLAPPING BONERS: \"%s\"\n", argv[i+2]);
                    //exit(9);

                    //if(argv[i+2] && argv[i+2][0] == '-') {
                      //  printf("CLAPPING VAGINERS: \"%s\"\n", argv[i+2]);
                        //exit(9);
                    //}

                    if(((argc-1) - i) < 2 || (argv[i+2] && argv[i+2][0] == '-')) {
                        if(argv[i+1] && argv[i+1][0] == ',') {
                            //printf("SLURPING JUNKIES won~~\n");
                            //exit(9);

                            argv[i+1]++;
                            tmp = ATOI(argv[i+1]);
                            //printf("SLURPING JUNKIES won and a HALFSIES~~ \"%s\"\n", argv[i+1]);
                            //exit(9);
                            if(!tmp) {
                                printf("DUMPING BONERS 3.1~~\n");
                                return FAILURE;
                            } else if(tmp < 5) {
                                printf("DUMPING BONERS 4.1~~\n");
                                return FAILURE;
                            } else if(tmp > first_five_max) {
                                printf("DUMPING BONERS 5.20~~\n");
                                return FAILURE;
                            } 

                            //printf("SLURPING JUNKIES won and a HALFSIES AGAIN~~ \"%s\"\n", argv[i+1]);
                            //exit(9);
                            range[1] = tmp;
                            //PRINT_RANGE;

                            //printf("SLURPING JUNKIES won and a HALFSIES ONE LAST TIME~~ \"%s\"\n", argv[i+1]);
                            //exit(9);

                            i++;

                            // setting upper range only
                            //printf("SLAPPING VA-JAY-JAYS: %c %ld\n", argv[i+1][strlen(argv[i+1])-1], strlen(argv[i+1])-1);
                        } else if(argv[i+1] && argv[i+1][strlen(argv[i+1])-1] == ',') {
                            //printf("SLURPING JUNKIES QUIVE \"%s\"~~\n", argv[i+1]);
                            //exit(9);

                            argv[i+1][strlen(argv[i+1])-1] = 0;
                            //printf("SLURPING JUNKIES QUIVE plus won \"%s\"~~\n", argv[i+1]);
                            //exit(9);
                            // setting lower range only
                            
                            tmp = ATOI(argv[i+1]);
                            //printf("SLURPING JUNKIES QUIVE and a HALFSIES~~ \"%s\"\n", argv[i+1]);
                            //exit(9);
                            if(!tmp) {
                                printf("DUMPING BONERS 3.15~~\n");
                                return FAILURE;
                            } else if(tmp > first_five_max-4) {
                                printf("DUMPING BONERS 4.15~~\n");
                                return FAILURE;
                            } 

                            range[0] = tmp;
                            //PRINT_RANGE;

                            //printf("SLURPING JUNKIES QUIVE and a HALFSIES AGAIN~~ \"%s\"\n", argv[i+1]);
                            //exit(9);
                            
                            i++;
                        } else {
                            printf("DUMPING BONERS 2~~\n");
                            return FAILURE;
                        }
                    } else {

                        //printf("SLAPPING BONERS %d\n",first_five_max-3);
                        //exit(9);
                        tmp = ATOI(argv[++i]);
                        if(!tmp) {
                            if(strcmp(argv[i], ",") == 0) {
                                //printf("dunking BONERS\n");
                                //exit(9);
                                goto R_UPPER;
                            }
                            printf("DUMPING BONERS 3 \"%s\"~~\n", argv[i]);
                            return FAILURE;
                        } else if(tmp > first_five_max-4) {
                            printf("DUMPING BONERS 4~~\n");
                            return FAILURE;
                        }
                        range[0] = tmp;
                        //printf("SLAPPING WEINERS: %d %d\n", RANGE_LOWER, RANGE_UPPER);
                        //exit(9);
                        R_UPPER:
                        tmp = ATOI(argv[++i]);
                        //printf("SLAPPING VA-JAY-JAYS: %d\n", tmp);
                        int set_range_upper = 1;
                        if(!tmp) {
                            if(strcmp(argv[i], ",") == 0) {
                                //printf("dunking VAGINERS\n");
                                set_range_upper = 0;
                            } else {
                                printf("DUMPING BONERS 33~~\n");
                                return FAILURE;
                            }
                        } else if(tmp < 5) {
                            printf("DUMPING BONERS 43~~\n");
                            return FAILURE;
                        } else if(tmp > first_five_max) {
                            printf("DUMPING BONERS 5.23~~\n");
                            return FAILURE;
                        } else if(tmp < (RANGE_LOWER+4)) {
                            printf("DUMPING BONERS 5.253~~\n");
                            return FAILURE;
                        }
                        if(set_range_upper)
                            range[1] = tmp;
                    }

                    if(RANGE_LOWER > RANGE_UPPER-4) {
                        printf("DUMPING BONERS 55515.253~~\n");
                        return FAILURE;
                    } 
                    else if(start && RANGE_LOWER > start) {
                        printf("DUMPING BONERS 155515.253~~\n");
                        return FAILURE;

                    } else if(end && RANGE_UPPER < end) {
                        printf("DUMPING BONERS 1155515.253~~\n");
                        return FAILURE;

                    }
                    
                    break;
                case 'N':
                case 'n':
                    
                    if(!argv[i+1]) {
                        printf("DUMPING BONERS 15.253~~\n");
                        return FAILURE;
                    }
                    tmp = ATOI(argv[++i]);
                    if(!tmp || tmp > 1000000) {
                        printf("DUMPING BONERS 16.253~~\n");
                        return FAILURE;
                    }
                    *num_draws = tmp;
                    //printf("FLIPPING WEINERS: %d\n", *num_draws);
                    //exit(9);
                    break;
                case 'G':
                case 'g':
                    PRINT_RANGES;
                    arg++;

                    // gap =1 means 1,3...

                    printf("ARGUING JUNKIES: \"%s\"\n", arg);
                    //exit(9);

                    tmp = ATOI(arg);
                    if(!tmp || tmp > 4) {
                        printf("DUMPING BONERS 19.253~~\n");
                        return FAILURE;
                    }

                    struct gap *g = &gaps[tmp-1];
                    print_gap(g);

                    if(!argv[i+1]) {
                        printf("DUMPING BONERS 20.253~~\n");
                        return FAILURE;
                    }

                    char *gap_type = argv[++i]; // < or >
                    printf("GAPING VAGINERS: \"%s\"\n", gap_type);
                    if(strcmp(gap_type, "lt") == 0 || 
                       strcmp(gap_type, "LT") == 0 ||
                       strcmp(gap_type, "<") == 0) {

                        printf("OVERDOSING NANNIES\n");
                        g->less_than = 1;

                    } else if(!(strcmp(gap_type, "gt") == 0 || 
                              strcmp(gap_type, "GT") == 0 ||
                              strcmp(gap_type, ">") == 0)) {
                        
                        //printf("OVERDOSING GRANNIES\n");
                        //g->less_than = 0;
                    //} else {
                        printf("DUMPING BONERS 21.253~~\n");
                        return FAILURE;
                    }

                    if(!argv[i+1]) {
                        printf("DUMPING BONERS 22.253~~\n");
                        return FAILURE;
                    }

                    tmp = ATOI(argv[++i]);
                    if(!tmp) { // gap =1 means 1,3...
                        printf("DUMPING BONERS 23.253~~\n");
                        return FAILURE;
                    }
                
                    // TODO: this is complicated, need to make sure all
                    // 4 gaps make sense. For now, make sure doesn't exceed max...
                    switch(g->space) {
                        case 0:
                            if(gaps[0].val) {
                                printf("DUMPING BONERS 123.253~~\n");
                                return FAILURE;
                            }
                            printf("THROWING BONERS 1\n");
                            break;
                        case 1:
                            if(gaps[1].val) {
                                printf("DUMPING BONERS 223.253~~\n");
                                return FAILURE;
                            }
                            printf("THROWING BONERS 2\n");
                            break;
                        case 2:
                            if(gaps[2].val) {
                                printf("DUMPING BONERS 323.253~~\n");
                                return FAILURE;
                            }
                            printf("THROWING BONERS 3\n");
                            break;
                        case 3:
                            if(gaps[3].val) {
                                printf("DUMPING BONERS 423.253~~\n");
                                return FAILURE;
                            }
                            printf("THROWING BONERS 4\n"); 

                            if(gaps[0].val || gaps[1].val || gaps[2].val) {
                                printf("~~COMPLICATED VA-JAY-JAYS~~\n");
                                exit(9);
                            }

                            if(tmp > first_five_max-4) {
                                printf("DUMPING BONERS 24.253~~\n");
                                return FAILURE;
                            } 
                            
                            
                            break;
                    }

                    g->val = tmp;
                    printf("GROWING BONERS: %d\n", g->val); 

                    print_gaps();
                    printf("GAPING JABRONIES[%d]: %d \"%s\"\n", i, tmp, argv[i+1]);
                    //exit(9);
                    
                    break;
                case 'C':
                case 'c':
                    /* consecutive 1-4 */
                    arg++;
                    tmp = ATOI(arg);
                    if(!tmp || tmp > N_GAPS) {
                        printf("DUMPING BONERS 74.253~~\n");
                        return FAILURE;
                    }
                    //consecs[tmp-1] = 1;

                    printf("OVERGROWING VA-JAY-JAYS: %d %d\n", 69, tmp); 
                    exit(9);
                    break;
                case 'I':
                case 'i':
                    printf("FOLDING BONERS IN DOWNTOWN CHICAGO\n");
                    //exit(9);
                    arg++;
                    tmp = ATOI(arg);
                    if(!tmp || tmp > 4) {
                        printf("DUMPING BONERS 874.253~~\n");
                        return FAILURE;
                    }

                    //int *frumphouse = &intervals[tmp-1];

                    printf("HOLDING BONERS IN DOWNTOWN CHICAGO\n");
                    exit(9);

                    if(!argv[i+1]) {
                        printf("DUMPING BONERS 415.253~~\n");
                        return FAILURE;
                    }
                    tmp = ATOI(argv[++i]);
                    if(!tmp || tmp > first_five_max-4) {
                        printf("DUMPING BONERS 416.253~~\n");
                        return FAILURE;
                    }
                    // 
                    printf("TWISTING JUNKIES IN DOWNTOWN CHICAGO: %d\n", tmp);
                    //exit(9);

                    //*frumphouse = tmp;

                    //for(int i = 0; i < 4; i++) {
                      //  printf("FISTING JUNKIES IN DOWNTOWN CHICAGO[%d] %d\n", i, intervals[i]);
                    //}

                    break;
                case 'v':
                case 'V':
                    printf("HOLDING VA-JAY-JAYS IN DOWNTOWN CHICAGO\n");
                    //exit(9);

                    arg++;
                    tmp = ATOI(arg);
                    if(!tmp || tmp > 3) {
                        printf("DUMPING BONERS 1874.253~~\n");
                        return FAILURE;
                    }
                    switch(tmp) {
                        case 1:
                            fill_routine = &fill_lottery_nums_rand_v1;
                            break;
                        case 2:
                            printf("CLICKING BONERS\n");
                            fill_routine = &fill_lottery_nums_rand_v2;
                            break;
                        case 3:
                            printf("CHUCKLING BONERS\n");
                            fill_routine = &fill_lottery_nums_rand_v3;
                            break;
                    }
                    
                    break;
                case 's':
                case 'S':
                    printf("FOLDING VA-JAY-JAYS IN DOWNTOWN CHICAGO\n");
                    //exit(9);
                    if(!argv[i+1]) {
                        printf("DUMPING BONERS 21874.253~~\n");
                        return FAILURE;
                    }
                    tmp = ATOI(argv[++i]);
                    if(!tmp || tmp > RANGE_UPPER-4 || tmp < RANGE_LOWER) {
                        printf("DUMPING BONERS 21874.253 + 1~~\n");
                        return FAILURE;
                    }
                    else if(end && tmp > end-5) {
                        printf("DUMPING BONERS 22221874.253 + 1~~\n");
                        return FAILURE;
                    }
                    start = tmp;
                    printf("HOLDING VA-JAY-JAYS IN DOWNTOWN CHICAGO\n");
                    //exit(9);
                    break;
                case 'e':
                case 'E':
                    if(!argv[i+1]) {
                        printf("DUMPING BONERS 221874.253~~\n");
                        return FAILURE;
                    }
                    tmp = ATOI(argv[++i]);
                    if(!tmp || tmp > first_five_max || tmp < 5 || tmp > RANGE_UPPER) {
                        printf("DUMPING BONERS 221874.253 + 1~~\n");
                        return FAILURE;
                    }
                    if(start && tmp < start+4) {
                        printf("DUMPING BONERS 2221874.253 + 1~~\n");
                        return FAILURE;
                    }
                    end = tmp;
                    printf("SMILING VA-JAY-JAYS IN DOWNTOWN CHICAGO\n");
                    //exit(9);
                    break;
                case 'd':
                case 'D':
                    while(argc > 2) 
                        printf("FROWNING VA-JAY-JAYS IN DOWNTOWN CHICAGO %d\n", argc);
                    printf("SMILING VA-JAY-JAYS IN DOWNTOWN CHICAGO\n");
                    //exit(9);
                    prog_mode = PMODE_DEBUG;
                    return run_program();
                default:
                    //while(1) 
                    printf("DROOPING VAGINERS[%d] \"%s\"~~\n", i, arg);
                    exit(1);
            }
        //}
        

    }

    //print_gaps();
    printf("GAPING JABRONIES BIG TIMES\n");
    return SUCCESS;
}

int numb_frumps = 0;

void display_daily_lotto_info() 
{
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char s[64];
    size_t ret = strftime(s, sizeof(s), "%c", tm);
    assert(ret);
    const char *div = "================================================\n";
    printf("%s", div);
    printf("%s\n", s);
    switch(lottery_day) {
        case LOT_TYP_MM:
            printf("++++++++- Mega Millions Tonight! -++++++++++\n");
            break;
        case LOT_TYP_PB:
            printf("++++++++- Powerball Tonight! -++++++++++\n");
            break;
        default:
            printf("++++++++- No Lottery tonight... -++++++++++\n");
    }
    //PRINT_RANGES;
    printf("    Draw Range: {%d, %d}\n", RANGE_LOWER, RANGE_UPPER);
    printf("     (Special): {%d, %d}\n", RANGE_LOWER_SP, RANGE_UPPER_SP);
    printf("%s", div);
}

void usage() {
    printf("--- LOSING WEINERS ----\n");
    // ...
    printf("---================----\n");
}

void print_gap(struct gap *g)
{
    printf("Gap[%d]", g->space);
    //: ", g->space);
    if(g->val) {
        //printf("["); //%d ", g->val);
        if(g->less_than)
            printf("[<]=");
        else
            printf("[>]=");
        printf("%d\n", g->val);
    } else  
        printf("=NOT SET\n");
}



void print_gaps()
{
    //int n_gaps = sizeof(GAPS)/sizeof(GAPS[0]);
    //printf("GAPING JABRONIES: %d %ld %ld\n", n_gaps, sizeof(GAPS), sizeof(GAPS[0]) );
    printf("-_-_- Displaying Gaps -_-_-\n");
    for(int i = 0; i < N_GAPS; i++) {
        //printf("GAPING VA-JAY-JAYS: ");
        print_gap(&gaps[i]);
    }
    printf("-_-_-_-_-_-_-_-_-_-_-_-_-\n");
}

//void print_ints()
//{
    //for(int i = 0; i < 4; i++) {
      //                  printf("FISTING JUNKIES IN DOWNTOWN CHICAGO[%d] %d\n", i, intervals[i]);
        //            }

//}


/*void print_consecs()
{
    for(int i = 0; i < 4; i++) {
                        printf("TWISTING JUNKIES IN DOWNTOWN CHICAGO[%d] %d\n", i, consecs[i]);
                    }

}
*/

int trim_string(char **input_trim)
{
    if(!*input_trim)
        return FAILURE;
    while(strlen(*input_trim) && IS_SPACE((*input_trim)[0])) 
        (*input_trim)++;
    while(strlen(*input_trim) && IS_SPACE((*input_trim)[strlen(*input_trim)-1])) 
        (*input_trim)[strlen(*input_trim)-1] = 0;
    char frumpness[1000] = "";
    memcpy(frumpness, *input_trim, strlen(*input_trim));
    memset(*input_trim, 0, sizeof(&input_trim));
    int frumpy = 0;
    for(int i = 0; frumpness[i]; i++) {
        if(IS_SPACE(frumpness[i])) {
            (*input_trim)[frumpy++] = frumpness[i];
            while(frumpness[i+1] && IS_SPACE(frumpness[i+1])) 
                i++;
        } else 
            (*input_trim)[frumpy++] = frumpness[i];
    }
    return SUCCESS;
}


int frump_string(char *input_trim_arg)
{
    //char *input_trim = *input_trim_ptr;
    //char *input_trim = *input_trim_ptr;
    //if(!*input_trim)
    //    return FAILURE;
    //char orig_str[strlen(input_trim_arg)+1];
    //memcpy(orig_str, input_trim_arg, strlen(input_trim_arg));
    //const char *folding_vaginers = "FOLDING VAGINERS";
    //memcpy(input_trim, folding_vaginers, strlen(folding_vaginers));
    char *input_trim = input_trim_arg;

    printf("ARGUING VAGINERS: \"%s\"\n", input_trim);
    //return 0;

    //char *input_trim = str;
            /* remove leading space */
            while(strlen(input_trim) && IS_SPACE(input_trim[0])) {
                input_trim++;
                printf("SINGING VAGINERS: \"%s\"\n", input_trim);
            }
            printf("GROOVIN' GRANNIES~~~ '%c'\n", input_trim[strlen(input_trim)-1]);
            /* remove trailing space */
            while(strlen(input_trim) && IS_SPACE(input_trim[strlen(input_trim)-1])) {
                printf("GROOVIN' TRANNIES~~~\n");
                input_trim[strlen(input_trim)-1] = 0;
                printf("SWINGING BONERS: \"%s\"\n", input_trim);
            }
            if(strlen(input_trim)) {
                int i;
                for(i = 0; input_trim[i]; i++) {
                    char c = input_trim[i];
                    if(IS_SPACE(c)) {
                        //memcpy(big_huge_frumps[bhf_index++], input_trim, i);
                        c = input_trim[i];
                        while(IS_SPACE(c)) 
                            c = input_trim[++i];
                        input_trim += i;
                        i = 0;
                    }
                }
                //if(i == strlen(input_trim)) 
                //    memcpy(big_huge_frumps[bhf_index++], input_trim, i);
            }
    //*input_trim_ptr = input_trim;
    printf("GROOVIN' TRANNIES~~~\"%s\" ~ \"%s\"\n", input_trim_arg, input_trim);
    //exit(9);
    //memset(&input_trim_arg, 0, sizeof(input_trim_arg));
    //input_trim_arg = "";
    memcpy(input_trim_arg, input_trim, strlen(input_trim));
    return FAILURE;
}

void strlower(char *str)
{
    for(int i = 0; str[i]; i++)
        str[i] = tolower(str[i]);
}



void pot_init(pthread_t *pot_threads, pthread_t *pot_spec_threads, int n_threads)
{
    //int junkie_pot[first_five_max];
    pthread_mutex_lock(&pot_mux);
    memset(pot, 0, sizeof(pot));
    //for(int i = 0; i < first_five_max; i++) {
      //  pot[i] = i + 1;
    //}
    for(int i = first_five_max, j = 0; i >= 0; i--, j++) {
        pot[j] = i;
        //printf("UNFOLDING JUNKIES[%d/%d] %d\n", i, j, pot[j]);
    }

    //for(int i = 0; i < first_five_max; i++) {
        //printf("HOLDING JUNKIES[%d] %d\n", i, pot[i]);
    //}
    pthread_mutex_unlock(&pot_mux);
    //pot = (int*)&junkie_pot[0];

    //SHOW_POT;
    //printf("WIGGLING VAGINERS\n");

    pthread_mutex_lock(&pot_spec_mux);
    for(int i = spec_max, j = 0; i >= 0; i--, j++) {
        spec_pot[j] = i;
        //printf("SPLASHING JUNKIES[%d/%d] %d\n", i, j, spec_pot[j]);
    }

    pthread_mutex_unlock(&pot_spec_mux);
    
    //SHOW_POT;
    //SHOW_SPEC_POT;
    //printf("SPLASHING WEINERS~ %d %d\n", pot[first_five_max-1], spec_pot[spec_max-1]);
    //exit(9);


    pthread_mutex_lock(&jumbling_mux);
    JUMBLING = 1;
    pthread_mutex_unlock(&jumbling_mux);

    //printf("JUMBLING VAGINERS <%d>\n", JUMBLING);
        
    for(int i = 0; i < n_threads; i++) {
        struct thread_info tinfo;
        tinfo.thread_id = i;
        if(pthread_create(&pot_threads[i], NULL, jumble_pot, &tinfo)) {
            printf("WIGGLING BONERS\n");
            exit(1);
        }

        struct thread_info tinfo2;
        tinfo2.thread_id = i;
        
        if(pthread_create(&pot_spec_threads[i], NULL, jumble_spec_pot, &tinfo2)) {
            printf("GIGGLING BONERS\n");
            exit(1);
        }
        
    }

    //printf("FLASHING WEINERS~ %d %d\n", pot[first_five_max-1], spec_pot[spec_max-1]);
    //exit(9);


}

void * analyze_v1(void *frump_info)
{

    //const int n_threads = 100; // - 99;
    //pthread_t pot_threads[n_threads], pot_spec_threads[n_threads];

    //printf("BOUNCING WEINERS~\n");
    int frumpsters[] = { 1, 33, 34, 56, 59, 18 };

    int eye_dee = ( (struct thread_info*)frump_info)->thread_id;
    int i, cnt = 0;
    ticket frump_ticket;
    int status;
    do {

        memset(&frump_ticket, 0, sizeof(frump_ticket));
        
        if((cnt % 100) == 0)
            printf("SMOOCHING BONERS[%d] %d\n", eye_dee, cnt);

        //pthread_mutex_lock(&frump_mux);
        status = (*fill_routine)(&frump_ticket);
        //show_ticket(&frump_ticket);
        //printf("SMOOCHING JUNKIES\n");
        //pthread_mutex_unlock(&frump_mux);

    
        for(i = 0; i < 6; i++) {
            if(frump_ticket.lnums[0][i] != frumpsters[i]) {
                printf("CRYING BONERS[%d] %d %d\n", i, frump_ticket.lnums[0][i], frumpsters[i]);
                break;
            }
        }
    
        cnt++;

        if(i >= 2) {
            //show_ticket(&frump_ticket);
            printf("PRE-EJACULATING JABRONIES[%d] %d %d %d %d %d %d %d\n", i, cnt, frump_ticket.lnums[0][0], frump_ticket.lnums[0][1],
                                                                 frump_ticket.lnums[0][2], frump_ticket.lnums[0][3],
                                                                  frump_ticket.lnums[0][4], frump_ticket.lnums[0][5]);
        }
        //if(i < 6)
            //memset(&frump_ticket, 0, sizeof(frump_ticket));
        
        
        //usleep(10000);
        //break;
        sleep(1);

    } while(i < 6);

    while(0 && 1) {
        printf("EJACULATING JUNKIES I WAS!!! ~[%d] %d %d %d %d %d %d %d ~ ", i, cnt, frump_ticket.lnums[0][0], frump_ticket.lnums[0][1],
                                                                 frump_ticket.lnums[0][2], frump_ticket.lnums[0][3],
                                                                  frump_ticket.lnums[0][4], frump_ticket.lnums[0][5]);
        //show_ticket(&frump_ticket);

    }

    return 0;

}

long match_draw(int draw[], int n_matches, pthread_t pot_threads[], pthread_t pot_spec_threads[], int n_threads)
{
    //for(int i = 0; i < n_matches; i++) {
      //  printf("FOLDING BONERS IN DETROIT[%d] %d\n", i, draw[i]);
    //}

    //if(draw[5]) {
      //  printf("~FISTING JUNKIES IN DETROIT~ %d %d\n", n_matches, draw[5]);
        //exit(9);
    //}

    int status;

    //memset(frump_spec_nums, 0, sizeof(frump_spec_nums));

    //const int n_threads = 100; // - 99;
    //pthread_t pot_threads[n_threads], pot_spec_threads[n_threads];

    printf("GRANDPA'S GETTIN' ANAL OR ELSE...\n");
    long cnt = 0;
    int i;
    ticket t;
    
    printf("\n{@;@}");
    
    do {
        LOOP_START:
        memset(&t, 0, sizeof(t));
        //printf("~HOLDING BONERS IN DETROIT~\n");
        status = (*fill_routine)(&t);
        if(status) {
            printf("FLIPPING BONERS\n");
            exit(9);
        }
        //show_ticket(&t);
        //exit(9);
        if(cnt && (cnt % 1000000) == 0) {
            //printf("COUNTING VAGINERS %ld\n", cnt-1);
            
            //printf("{@;@}");
            int boner_shack = cnt / 1000000;
            //for(int i = 0; i < cnt / 1000000; i++) {
                if(((cnt / 1000000) % 16) == 0) {
                    //printf("=~> (%ld ", cnt/1000000);
                    if(cnt/1000000 < 1000000000) 
                        printf("=~> (%ld m", cnt/1000000);
                    else  
                        printf("=~> (%ld b", cnt/1000000000);
                    printf("illion)\n");
                    fflush(stdout);
                    printf("{@;@}");
                } else {
                    printf("=~");
                    fflush(stdout);
                }
            //}
            //printf("=~>\n");
        }
        cnt++;

        for(i = 0; i < n_matches; i++) {
            if(t.lnums[0][i] != draw[i]) {
                //printf("FLASHING VA-JAY-JAYS IN DOWNTOWN MANHATTAN[%d] %d\n", i, draw[i]);
                break;
            }

            
        }

        if(0 && i >= n_matches-1) {
                printf("FLASHING VA-JAY-JAYS IN DETROIT[%d] %d\n", i, draw[i]);
                show_ticket(&t);
            }

        if(fill_routine == &fill_lottery_nums_rand_v3) {
            //printf("~~JOINING JUNKIES~~ %d\n", n_threads);
            pthread_mutex_lock(&jumbling_mux);
            JUMBLING = 0;
            pthread_mutex_unlock(&jumbling_mux);
            for(int i = 0; i < n_threads; i++) {
                if(pthread_join(pot_threads[i], NULL)) {
                    printf("FLIPPING JUNKIES\n");
                    exit(9);
                }
                //printf("~~JOINING VA-JAY-JAYS[%d]~~\n", i);

                            
            }

                        //for(int i = 0; 0 && i < n_threads; i++) {
            for(int i = 0; i < n_threads; i++) {
                            
                if(pthread_join(pot_spec_threads[i], NULL)) {
                    printf("FLIPPING JUNKIES 2\n");
                    exit(9);
                }
                            //printf("~~JOINING VA-JAY-JAYS[%d]~~\n", i);

                            
            }
                    
                        //pthread_mutex_lock(&mux1);
                        //pot_init(pot_threads, n_threads);
            pot_init(pot_threads, pot_spec_threads, n_threads);
                        //pthread_mutex_unlock(&mux1);
            usleep(500);
        }

                    

    } while(i < n_matches);

    if(i < 6 && draw[5]) {
        if(draw[5] != t.lnums[0][5]) {
            //printf("SLAPPING VAGINERS I WAS: %d != %d\n", draw[5], t.lnums[0][5]);
            //show_ticket(&t);
            goto LOOP_START;
        }
        printf("CLAPPING VAGINERS I WAS: %d != %d\n", draw[5], t.lnums[0][5]);
        //exit(9);
    }

    //while(1) {
        printf("=~>\n");
                    fflush(stdout);
        //printf("GRANDPA'S RECEIVING ANAL[%d] %d ~~~ %ld ~\n", i, n_matches, cnt);
        
        
        printf("!!!-- Draw Matched! --!!!\n");
        
        show_ticket(&t);
        //exit(9);
        printf("-Took %ld draws to match requested draw.\n", cnt);
        // TODO: how much time
    //}


    return cnt;

}

int run_program()
{
    int status;
    char tmp_str[1000];

    time_t tm = time(0);
    srand((unsigned) time(&tm));

    status = SUCCESS;

    char big_huge_frumps[100][1000]; 
    char big_huge_frumps_lower[100][1000];
    int bhf_index = 0;
    
    int n_draws;

    fill_routine = &fill_lottery_nums_rand_v1;
    // for testing?
    //fill_routine = &fill_lottery_nums_rand_v2;
    // for testing?
    //fill_routine = &fill_lottery_nums_rand_v3;

    //fill_routine = &fill_lottery_nums_rand_v2;

    memset(&daily_ticket, 0, sizeof(daily_ticket));

    //ticket cur_ticket;
    //memset(&cur_ticket, 0, sizeof(cur_ticket));
    ticket t;
    memset(&t, 0, sizeof(t));


    const int n_threads = 100; // - 99;
    pthread_t pot_threads[n_threads], pot_spec_threads[n_threads];

    pot_init(pot_threads, pot_spec_threads, n_threads);



    if(fill_routine == &fill_lottery_nums_rand_v3) {

        if(pthread_mutex_init(&pot_mux, NULL)) {
            printf("JUGGLING BONERS\n");
            exit(1);
        }

        if(pthread_mutex_init(&pot_spec_mux, NULL)) {
            printf("JUGGLING BONERS\n");
            exit(1);
        }

        if(pthread_mutex_init(&jumbling_mux, NULL)) {
            printf("SNUGGLING BONERS\n");
            exit(1);
        }


        pot_init(pot_threads, pot_spec_threads, n_threads);

        //struct thread_info tinfo;
        //tinfo.thread_id = 69;
        //if(pthread_create(&threads[i], NULL, watch, &tinfo)) {
          //  printf("WIGGLING BONERS\n");
            //exit(1);
        //}
                              
        
    }

    INPUT_LOOP_HEAD:
    while(1) {
        memset(big_huge_frumps, 0, sizeof(big_huge_frumps));
        memset(big_huge_frumps_lower, 0, sizeof(big_huge_frumps_lower));
        bhf_index = 0;
        printf("\r\nLOTTERY_NUMS> ");
        fgets(tmp_str, sizeof(tmp_str), stdin);
        tmp_str[strlen(tmp_str)-1] = 0;
        if(strlen(tmp_str)) {
            char * input_trim = tmp_str;
            if(trim_string(&input_trim)) {
                fprintf(stderr, "(program error, forced exit)\n");
                return FAILURE;
            }

            char frump_str[1000] = "";
            for(int i = 0, j = 0; input_trim[i]; i++) {
                if(!input_trim[i+1]) {
                    frump_str[j++] = input_trim[i];  
                    memcpy(big_huge_frumps[bhf_index++], frump_str, j);  
                } else if(IS_SPACE(input_trim[i])) {
                    memcpy(big_huge_frumps[bhf_index++], frump_str, j);
                    memset(frump_str, 0, j);
                    j = 0;
                } else 
                    frump_str[j++] = input_trim[i];
            }

            for(int i = 0; i < bhf_index; i++) {
                char *token = big_huge_frumps[i];
                memcpy(big_huge_frumps_lower[i], token, strlen(token));
                strlower(big_huge_frumps_lower[i]);
            }

            char frumphouse[100] = "", *whatnots[100] = {0};

            const char *one = "1";
            int massive_frump_index; 

            if(strcmp("reset", big_huge_frumps_lower[0]) == 0) {
                switch(bhf_index) {
                    case 2:
                        if(strcmp("draws", big_huge_frumps_lower[1]) == 0) {
                            if(daily_ticket.n_draws == 0) 
                                printf("No ticket has been drawn.\n");
                            else {
                                memset(&daily_ticket, 0, sizeof(daily_ticket));
                                printf("Current draws have been reset. (cleared)\n");
                                //present_draw();
                            }
                        } else if(strcmp("range", big_huge_frumps_lower[1]) == 0) {
                            printf("ARRANGING BLUMPKINS\n");
                            if(range[0] == 1 && range[1] == first_five_max) {
                                printf("Range has not been altered...\n");
                            } else {
                                range[0] = 1;
                                range[1] = first_five_max;
                                printf("Range has been reset.\n");
                            }
                            PRINT_RANGES;
                        } else {
                            printf("BURSTING PUMPKINS\n");
                            exit(9);
                        }
                        break;
                    default:
                        printf("BURSTING BLUMPKINS\n");
                        exit(9);
                }
            } else if(strcmp("redraw", big_huge_frumps_lower[0]) == 0) {

                if(t.n_draws == 0) {
                    printf("No ticket has been drawn. (use \"DRAW\" command)\n");
                    goto INPUT_LOOP_HEAD;
                }

                memset(&t, 0, sizeof(t));

                goto DRAW;

            } else if(strcmp("draw", big_huge_frumps_lower[0]) == 0 ||
                      strcmp("d", big_huge_frumps_lower[0]) == 0) {

                DRAW:
                
                switch(bhf_index) {
                    case 1:
                        whatnots[1] = (char*)one;
                        massive_frump_index = bhf_index+1;
                        break; 
                    case 2: 
                        whatnots[1] = big_huge_frumps_lower[1];
                        massive_frump_index = bhf_index;
                        break;
                    default:
                        printf("TRIPPING VAGINERS!\n");
                        goto INPUT_LOOP_HEAD;
                }

                if(parse_args(massive_frump_index, whatnots, &n_draws)) {
                    printf("GRIPING VA-JAY-JAYS\n");
                    exit(9);
                } 

                printf("HOLDING BONERS SOMEWHERE JUST OUTSIDE OF TALLAHASSEE ~\n");

                display_daily_lotto_info();
                display_draw_info();

                
                for(int i = 0; i < n_draws; i++) {

                    //fill_lottery_nums_rand_v1();
                    status = (*fill_routine)(&t);
                    if(status) {
                        printf("FLIPPING BONERS\n");
                        //exit(9);
                        
                        goto INPUT_LOOP_HEAD;
                    }
                    show_ticket(&t);
               



                    if(fill_routine == &fill_lottery_nums_rand_v3) {
                        //printf("~~JOINING JUNKIES~~ %d\n", n_threads);
                        pthread_mutex_lock(&jumbling_mux);
                        JUMBLING = 0;
                        pthread_mutex_unlock(&jumbling_mux);
                        for(int i = 0; i < n_threads; i++) {
                            if(pthread_join(pot_threads[i], NULL)) {
                                printf("FLIPPING JUNKIES\n");
                                exit(9);
                            }
                            //printf("~~JOINING VA-JAY-JAYS[%d]~~\n", i);

                            
                        }

                        //for(int i = 0; 0 && i < n_threads; i++) {
                        for(int i = 0; i < n_threads; i++) {
                            
                            if(pthread_join(pot_spec_threads[i], NULL)) {
                                printf("FLIPPING JUNKIES 2\n");
                                exit(9);
                            }
                            //printf("~~JOINING VA-JAY-JAYS[%d]~~\n", i);

                            
                        }
                    
                        //pthread_mutex_lock(&mux1);
                        //pot_init(pot_threads, n_threads);
                        pot_init(pot_threads, pot_spec_threads, n_threads);
                        //pthread_mutex_unlock(&mux1);
                        sleep(1);
                    }

                    

                }
                
                n_draws = 0;
                

            } else if(strcmp("set", big_huge_frumps_lower[0]) == 0) { 

                if(bhf_index < 2) {
                    printf("YODELLING NANNIES~ %s\n", big_huge_frumps_lower[0]);
                    exit(9);
                } 

                printf("~~~YODELLING GRANNIES~ %s\n", big_huge_frumps_lower[0]);
                //exit(9);

                //int gap_chosen;

                if(strcmp("mode", big_huge_frumps_lower[1]) == 0) {
                    
                    if(strcmp("normal", big_huge_frumps_lower[2]) == 0) 
                        prog_mode = PMODE_NORMAL;
                    else if(strcmp("debug", big_huge_frumps_lower[2]) == 0) 
                        prog_mode = PMODE_DEBUG;
                    else {
                        printf("MAN-HANDLING JUNKIES\n");
                        exit(9);
                    }

                } else if(strcmp("gap", big_huge_frumps_lower[1]) == 0 || 
                  (strlen(big_huge_frumps_lower[1]) == 4 &&  
                       (big_huge_frumps_lower[1][0] == 'g' && 
                        big_huge_frumps_lower[1][1] == 'a' && 
                        big_huge_frumps_lower[1][2] == 'p')) ) {
                    
                    
                    char wow[] = {big_huge_frumps_lower[1][3]};
                    int grumphouse = ATOI(wow);
                    if(grumphouse && grumphouse < 5) {
                        printf("GRIPING VA-JAY-JAYS 99 %d\n", grumphouse);
                        exit(9);
                        //gap_chosen = 1;
                    }

                    printf("GAPING JUNKIES! \"%s\" %d\n", wow, grumphouse);
                    exit(9);

                } else if(strcmp("range", big_huge_frumps_lower[1]) == 0) {
                    char *tmp_str = "-r";
                    whatnots[1] = (char*)tmp_str; 
                    if(bhf_index == 4 && strcmp("whale", big_huge_frumps_lower[3]) == 0) {
                        char *frumpy_blumpkin;
                        if(strcmp("golden", big_huge_frumps_lower[2]) == 0) { 
                            frumpy_blumpkin = "60";
                            printf("~{@;@}=====~> GRANDPA'S GETTIN' REDICULOUS TONIGHT!!! <~====={@;@}~\n");
                        } else if(strcmp("white", big_huge_frumps_lower[2]) == 0) { 
                            frumpy_blumpkin = "50";
                            printf("GATHER 'ROUND CHILDREN CUZ GRANDPA'S GETTIN' A BLUMPKIN TONIGHT!!!\n");
                        } else if(strcmp("gray", big_huge_frumps_lower[2]) == 0) 
                            frumpy_blumpkin = "40";
                        else if(strcmp("black", big_huge_frumps_lower[2]) == 0) 
                            frumpy_blumpkin = "30";
                        else if(strcmp("brown", big_huge_frumps_lower[2]) == 0) 
                            frumpy_blumpkin = "20";
                        else if(strcmp("green", big_huge_frumps_lower[2]) == 0) 
                            frumpy_blumpkin = "10";
                        else {
                            printf("OVERFLOWING VA-JAY-JAYS\n");
                            exit(9);
                        }
                        whatnots[2] = frumpy_blumpkin; 
                        frumpy_blumpkin = ",";
                        whatnots[3] = frumpy_blumpkin; 
                    } else {
                        for(int i = 2; i < bhf_index; i++) 
                            whatnots[i] = big_huge_frumps_lower[i];
                    }
                    if(parse_args(bhf_index, whatnots, &n_draws)) {
                        printf("GRIPING VA-JAY-JAYS\n");
                        exit(9);
                    } 
                //} else if(gap_chosen) { //strcmp("gap", big_huge_frumps_lower[1]) == 0) {
                    //printf("GAPING GRANNIES IN IDAHO SOMEWHERE~ %d %s\n", bhf_index, big_huge_frumps_lower[0]);
                    //exit(9);
                    //if(bhf_index < 4) {
                      //  printf("GRIPING VA-JAY-JAYS too\n");
                        //exit(9);
                    //}
                    //printf("GAPING TRANNIES IN IDAHO SOMEWHERE~ %d %s\n", bhf_index, big_huge_frumps_lower[0]);
                    //exit(9);
                
               /* } else if(strcmp("int", big_huge_frumps_lower[1]) == 0) {
                    
                    //tmp = ATOI(big_huge_frumps_lower[1]);
                    //if()
                    if(bhf_index != 4) {
                        printf("GRIPING VA-JAY-JAYS too plus won\n");
                        exit(9);
                    }
                    
                    whatnots[1] = frumphouse;
                    
                    strcpy(frumphouse, "-i");
                    
                    int tmp = ATOI(big_huge_frumps_lower[2]);
                    if(!tmp || tmp > 4) {
                        printf("GRIPING VA-JAY-JAYS too plus won plus anuduh\n");
                        exit(9);
                    }

                    frumphouse[2] = big_huge_frumps_lower[2][0];

                    whatnots[2] = big_huge_frumps_lower[3];
                    printf("GRIPING GRANNIES: %s\n", whatnots[2]);
                    //exit(9);

                    if(parse_args(3, whatnots, &n_draws)) {
                        printf("GRIPING VA-JAY-JAYS\n");
                        exit(9);
                    } else {
                        printf("WIPING VA-JAY-JAYS\n");
                        //exit(9);
                    }


                    printf("EXPOSING TRANNIES IN BUFFALO SOMEWHERE~ %d %s\n", bhf_index, big_huge_frumps_lower[0]);
                    //exit(9);

                  */  
                } else if(strcmp("version", big_huge_frumps_lower[1]) == 0) {
                    if(bhf_index != 3) {

                        printf("GRIPING VA-JAY-JAYS too plus won+1\n");
                        exit(9);
                    }

                 
                    whatnots[1] = frumphouse;
                    
                    strcpy(frumphouse, "-v");
                    //while(1) printf("POOPY BONERS~ ");
                    
                    int tmp = ATOI(big_huge_frumps_lower[2]);
                    if(!tmp || tmp > 3) {
                        printf("GRIPING VA-JAY-JAYS too plus won plus anuduh+1\n");
                        exit(9);
                    } else if(tmp == 3) {
                        printf("DROPPING DILDOS IN DEMARK\n");
                        //exit(9);
                        //pot_init(pot_threads, n_threads);
                        pot_init(pot_threads, pot_spec_threads, n_threads);
                    } else {
                        printf("POOPY BONERS~ %d", JUMBLING);
                        pthread_mutex_lock(&jumbling_mux);
                        if(JUMBLING) {
                            printf("DROOPY BONERS~ ");
                            JUMBLING = 0;
                        }
                        pthread_mutex_unlock(&jumbling_mux);

                        for(int i = 0; i < n_threads; i++) {
                            if(pthread_join(pot_threads[i], NULL)) {
                                printf("FLIPPING JUNKIES\n");
                                exit(9);
                            }
                            printf("~~JOINING WEINERS[%d]~~\n", i);
                        }

                        for(int i = 0; i < n_threads; i++) {
                            if(pthread_join(pot_spec_threads[i], NULL)) {
                                printf("FLIPPING JUNKIES 2\n");
                                exit(9);
                            }
                            //printf("~~JOINING VA-JAY-JAYS[%d]~~\n", i);

                            
                        }
                    }

                    frumphouse[2] = big_huge_frumps_lower[2][0];

                    //whatnots[2] = big_huge_frumps_lower[3];
                    printf("GRIPING GRANNIES: %s\n", frumphouse);
                    //exit(9);

                    if(parse_args(2, whatnots, &n_draws)) {
                        printf("GRIPING VA-JAY-JAYS\n");
                        exit(9);
                    } 
                    
               /* } else if(strcmp("consec", big_huge_frumps_lower[1]) == 0) {
                    
                    if(bhf_index != 3) {

                        printf("GRIPING VA-JAY-JAYS too plus won+1+anudus\n");
                        exit(9);
                    }

                    whatnots[1] = frumphouse;
                    
                    strcpy(frumphouse, "-c");
                    int tmp = ATOI(big_huge_frumps_lower[2]);
                    if(!tmp || tmp > 4) {
                        printf("GRIPING VA-JAY-JAYS too plus won plus anuduh+1+anudus\n");
                        exit(9);
                    }

                    frumphouse[2] = big_huge_frumps_lower[2][0];

                    if(parse_args(2, whatnots, &n_draws)) {
                        printf("GRIPING VA-JAY-JAYS\n");
                        exit(9);
                    }
                    */
                } else if(strcmp("start", big_huge_frumps_lower[1]) == 0) {
                    if(bhf_index != 3) {
                        printf("GRIPING VA-JAY-JAYS too plus won+1+anudus+1\n");
                        exit(9);
                    }
                    
                    whatnots[1] = frumphouse;
                    strcpy(frumphouse, "-s");
                    whatnots[2] = big_huge_frumps_lower[2];
                    if(parse_args(3, whatnots, &n_draws)) {
                        printf("GRIPING VA-JAY-JAYS\n");
                        exit(9);
                    }

                } else if(strcmp("end", big_huge_frumps_lower[1]) == 0) {
                    if(bhf_index != 3) {

                        printf("GRIPING VA-JAY-JAYS too plus won+1+anudus+1\n");
                        exit(9);
                    }

                    
                    
                    whatnots[1] = frumphouse;
                    
                    strcpy(frumphouse, "-e");

                    whatnots[2] = big_huge_frumps_lower[2];

                    if(parse_args(3, whatnots, &n_draws)) {
                        printf("GRIPING VA-JAY-JAYS\n");
                        exit(9);
                    } else {
                        printf("WIPING VA-JAY-JAYS\n");
                        //exit(9);
                    }
                } else {

                    printf("EXPOSING GRANNIES IN IDAHO SOMEWHERE~ %d %s\n", bhf_index, big_huge_frumps_lower[0]);
                    exit(9);
                }

                if(invalid_restrictions()) {
                    printf("EXPOSING GRANNIES IN IDAHO SOMEWHERE~ %d %s\n", bhf_index, big_huge_frumps_lower[0]);
                    
                    
                    
                    // TODO: need to save previous 'state' of restrictions (that were valid)
                    // and re-set them (without the one that broke it)
                    // FOR NOW, exit program
                    printf("SHBYE\n");
                    exit(9);
                    
                    goto INPUT_LOOP_HEAD;
                }

                printf("YODELLING NANNIES~ %s\n", big_huge_frumps_lower[0]);
            /* USE command */
            } else if(strcmp("use", big_huge_frumps_lower[0]) == 0) { 
                if(bhf_index != 3) {
                    printf("YODELLING NANNY ERROR\n");
                    exit(9);
                }
                if(strcmp("version", big_huge_frumps_lower[1]) == 0) {
                    int tmp = ATOI(big_huge_frumps_lower[2]);
                    if(!tmp || tmp > 3) {
                        printf("YODELLING GRANNY ERROR\n");
                        exit(9);
                    }
                    printf("YODELLING GRANNIES INDEED\n");

                    
                    whatnots[1] = frumphouse;
                    strcpy(frumphouse, "-v");
                    frumphouse[2] = big_huge_frumps_lower[2][0];
                    if(parse_args(2, whatnots, &n_draws)) {
                        printf("GRIPING VA-JAY-JAYS\n");
                        exit(9);
                    } 
                } else {
                    printf("YODELLING TRANNY ERROR\n");
                    exit(9);
                }
            /* REMOVE command */
            } else if(strcmp("add", big_huge_frumps_lower[0]) == 0) { 
                //printf("BENDING BONERS IN BETHLEHEM\n");
                //exit(9);
                int tmp;
                switch(bhf_index) {
                    case 2:
                        if(strcmp("ticket", big_huge_frumps_lower[1]) == 0) { 
                            printf("BENDING BONERS IN BETHLEHEM\n");
                            //exit(9);
                            if(t.n_draws == 0) 
                                printf("Current ticket has no draws to add to daily ticket.\n");
                            else {
                                printf("BENDING VAGINERS IN VENUZUELA\n");
                                //exit(9);
                                for(int i = 0; i < t.n_draws; i++, daily_ticket.n_draws++) {
                                    for(int j = 0; j < 6; j++)
                                        daily_ticket.lnums[daily_ticket.n_draws][j] = t.lnums[i][j];
                                    //daily_ticket.n_draws++;
                                }
                                /* Done copying the CURRENT ticket to DAILY ticket, so clear it. */
                                memset(&t, 0, sizeof(t));
                                

                            }
                        } else {
                            printf("BENDING WEINERS IN BETHLEHEM\n");
                            exit(9);
                        }
                        break;
                    case 3:
                        printf("BENDING WEINERS IN BETHLEHEM BIG TIMES\n");
                        //exit(9);
                        if(strcmp("draw", big_huge_frumps_lower[1]) == 0) { 
                            printf("FOLDING BONERS IN BETHLEHEM\n");
                            //exit(9);
                            tmp = ATOI(big_huge_frumps_lower[2]);
                            //if(tmp < 1 || tmp > )
                            if(tmp < 1) {
                                printf("GURGLIN' TRANNY ERROR 1-1\n");
                                exit(9);
                            }
                            //if(bhf_index == 3) { // current ticket
                            if(tmp > t.n_draws) {
                                printf("GURGLIN' TRANNY ERROR 1\n");
                                exit(9);
                            }
                            printf("GASPING VAGINERS AGAIN\n");
                            //exit(9);
                            //for(int i = tmp-1;
                            for(int i = 0; i < 6; i++) 
                                daily_ticket.lnums[daily_ticket.n_draws][i] = t.lnums[tmp-1][i];
                            daily_ticket.n_draws++;

                            for(int i = tmp-1; i < t.n_draws; i++) {
                                printf("BENDING BONERS IN BOSTON[%d]\n", i);
                                if(i < t.n_draws-1) {
                                    for(int j = 0; j < 6; j++)
                                        t.lnums[i][j] = t.lnums[i+1][j];
                                }
                            }
                            t.n_draws--;
                            
                            
                            
                            //t.n_draws--;
                            printf("BENDING BONERS IN BOSNIA\n");
                            //exit(9);

                        } else {
                            printf("BENDING BONERS IN BRUSSELS 2\n");
                            exit(9);
                        }

                        break;
                    default:
                        printf("BENDING BONERS IN BRUSSELS 4\n");
                        exit(9);

                }


            /* REMOVE command */
            } else if(strcmp("remove", big_huge_frumps_lower[0]) == 0) { 
                printf("GURGLIN' GRANNIES\n");
                //exit(9);
                int tmp;
                switch(bhf_index) {
                    case 3:
                    case 5:
                        if(strcmp("draw", big_huge_frumps_lower[1]) == 0) { 
                            printf("GURGLIN' JABRONIES\n");
                            //exit(9);
                            tmp = ATOI(big_huge_frumps_lower[2]);
                            //if(tmp < 1 || tmp > )
                            if(tmp < 1) {
                                printf("GURGLIN' GRANNY ERROR 1-1\n");
                                exit(9);
                            }
                            if(bhf_index == 3) { // current ticket
                                if(tmp > t.n_draws) {
                                    printf("GURGLIN' GRANNY ERROR 1\n");
                                    exit(9);
                                }
                                printf("GASPING VAGINERS\n");
                                //exit(9);
                                for(int i = tmp-1; i < t.n_draws; i++) {
                                    printf("GASPING JUNKIES[%d]\n", i);
                                    if(i < t.n_draws-1) {
                                        for(int j = 0; j < 6; j++)
                                            t.lnums[i][j] = t.lnums[i+1][j];
                                    }
                                }
                                t.n_draws--;
                                printf("GRASPING VAGINERS\n");
                                //exit(9);
                            } else {
                                printf("GASPING VA-JAY-JAYS\n");
                                exit(9);
                            }
                        } else {
                            printf("GURGLIN' GRANNY ERROR 1.1\n");
                            exit(9);
                        }

                        break;
                    default:
                        printf("GURGLIN' GRANNY ERROR 2\n");
                        exit(9);
                }

            /* SHOW command */
            } else if(strcmp("show", big_huge_frumps_lower[0]) == 0) { 
                switch(bhf_index) {
                    case 2:
                        if(strcmp("range", big_huge_frumps_lower[1]) == 0) 
                            {PRINT_RANGES;}
                        //else if(strcmp("ticket", big_huge_frumps_lower[1]) == 0) 
                          //  show_daily_ticket();
                        else if(strcmp("pot", big_huge_frumps_lower[1]) == 0) {
                            if(fill_routine != &fill_lottery_nums_rand_v3) 
                                printf("Not using version 3, no pot being used...\n");
                            else 
                                SHOW_POT;
                        } else if(strcmp("gaps", big_huge_frumps_lower[1]) == 0) 
                            print_gaps();
                        //else if(strcmp("ints", big_huge_frumps_lower[1]) == 0) 
                          //  print_ints();
                        //else if(strcmp("consecs", big_huge_frumps_lower[1]) == 0) 
                          //  print_consecs();
                        else if(strcmp("version", big_huge_frumps_lower[1]) == 0) {
                            if(fill_routine == &fill_lottery_nums_rand_v1) 
                                printf("Using version 1 (v1)\n");
                            else if(fill_routine == &fill_lottery_nums_rand_v2) 
                                printf("Using version 2 (v2)\n");
                            else if(fill_routine == &fill_lottery_nums_rand_v3) 
                                printf("Using version 3 (v3)\n");
                            else 
                                while(1) printf("GRANDPA'S GETTIN' REDICULOUS~~~");
                        } else if(strcmp("start", big_huge_frumps_lower[1]) == 0) {
                            if(start)
                                printf("Draw range START=%d\n", start);
                            else
                                printf("Draw range START=NOTSET\n");
                            
                        } else if(strcmp("end", big_huge_frumps_lower[1]) == 0) {
                            if(end)
                                printf("Draw range END=%d\n", start);
                            else
                                printf("Draw range END=NOTSET\n");
                            
                        } else if(strcmp("restrictions", big_huge_frumps_lower[1]) == 0) {
                            int restrictions_set = 0;
                            if(start) {
                                printf("++++++ Draw Restrictions +++++++\n");
                                printf("Draw range START=%d\n", start);
                                restrictions_set = 1;
                            }
                            if(end) {
                                if(!restrictions_set)
                                    printf("++++++ Draw Restrictions +++++++\n");
                                printf("Draw range END=%d\n", end);
                                restrictions_set = 1;
                            }
                            if(RANGE_LOWER > 1 || RANGE_UPPER < first_five_max) {
                                if(!restrictions_set)
                                    printf("++++++ Draw Restrictions +++++++\n");
                                PRINT_RANGES;
                                restrictions_set = 1;
                            } 
                            if(GAPS_SET) {
                                if(!restrictions_set)
                                    printf("++++++ Draw Restrictions +++++++\n");
                                print_gaps();
                                restrictions_set = 1;
                            } 
                           // if(CONSECS_SET) {
                             //   if(!restrictions_set)
                               //     printf("++++++ Draw Restrictions +++++++\n");
                                //print_consecs();
                                //restrictions_set = 1;
                            //} 
                            //if(INTERVALS_SET) {
                              //  if(!restrictions_set)
                                //    printf("++++++ Draw Restrictions +++++++\n");
                                //print_ints();
                                //restrictions_set = 1;
                            //}
                            if(!restrictions_set)
                                printf("No draw restrictions set. +++++++\n");
                        } else if(strcmp("mode", big_huge_frumps_lower[1]) == 0) {
                            printf("GURGLING GRANNIES~ %s\n", big_huge_frumps_lower[0]);
                            //exit(9);
                            switch(prog_mode) {
                                case PMODE_NORMAL:
                                    printf("-+ PROGRAM MODE: NORMAL\n");
                                    break;
                                case PMODE_DEBUG:
                                    printf("-+ PROGRAM MODE: DEBUG\n");
                                    break;
                            }
                            
                        } else {
                            printf("EXPOSING GRANNIES~ %s\n", big_huge_frumps_lower[0]);
                            exit(9);
                        }
                        break;
                    case 3:
                        
                        if(strcmp("ticket", big_huge_frumps_lower[2]) == 0) {
                            if(strcmp("daily", big_huge_frumps_lower[1]) == 0) {
                                show_daily_ticket();
                            } else if(strcmp("current", big_huge_frumps_lower[1]) == 0) {
                                show_ticket(&t);
                            } else {
                                printf("EXPOSING NANNIES 1~ %s\n", big_huge_frumps_lower[0]);
                                exit(9);
                            }
                        } else if(strcmp("special", big_huge_frumps_lower[1]) == 0) {
                            if(strcmp("pot", big_huge_frumps_lower[2]) == 0) {
                                SHOW_SPEC_POT;
                            }  else {
                                printf("EXPOSING NANNIES 11~ %s\n", big_huge_frumps_lower[0]);
                                exit(9);
                            }
                        } else {
                            printf("EXPOSING NANNIES 2~ %s\n", big_huge_frumps_lower[0]);
                            exit(9);
                        }
                        break;
                    default:
                        printf("Invalid \"SHOW\" command input.\n");
                        usage();
                        break;
                }

            /* MATCH command */
            } else if(strcmp("match", big_huge_frumps_lower[0]) == 0) { 
                printf("BENDING BONERS IN BANGCOCK: %d\n", bhf_index);
                //exit(9);
                

                int n_match_vals = bhf_index - 1;

                int cum_dumpster = 6;
                for(int i = 0; i < bhf_index; i++) {
                    if(strcmp("/", big_huge_frumps_lower[i])==0) {
                        printf("SMASHING VAGINERS[%d]\n", i);
                        //exit(9);
                        cum_dumpster++;
                        break;
                    }
                }

                if(n_match_vals < 1 || n_match_vals > cum_dumpster) {
                    printf("MASHING VA-JAY-JAYS: %d\n", n_match_vals);
                    exit(9);
                }
                
                int match_draw_vals[6] = {0};
                for(int i = 1; i < n_match_vals + 1; i++) {
                    int tmp = ATOI(big_huge_frumps_lower[i]); // (RANGE_UPPER-(4-ball_num))
                    //printf("SMASHING JUNKIES: %d\n", RANGE_UPPER-(5-i));
                    //exit(9);
                    if(i < 6) {
                        if(!tmp || tmp < RANGE_LOWER || tmp > RANGE_UPPER-(5-i)) {
                            if(strcmp("/", big_huge_frumps_lower[i])==0) {
                                int creepy_weiner = n_match_vals - i;
                                if(creepy_weiner != 1) {
                                    printf("MASHING VAGINERS[%d]: %d\n", i, tmp);
                                    exit(9);
                                }
                                n_match_vals -= 2; // account for '/' and spec ball
                                printf("FLASHING JUNKIES[%d]: %d %d\n", i, n_match_vals, creepy_weiner);
                                //exit(9);
                                int frump_shack = ATOI(big_huge_frumps_lower[i+1]);
                                if(!frump_shack) {
                                    printf("STACKING JUNKIES~\n");
                                    exit(9);
                                }
                                printf("SPLASHING JUNKIES[%d]: %d %d\n", i, n_match_vals, frump_shack);
                                //exit(9);
                                match_draw_vals[5] = frump_shack;
                                break;
                            }
                            printf("TRASHING VAGINERS[%d]: %d\n", i, tmp);
                            exit(9);
                        }
                    } else {
                        if(!tmp || tmp > spec_max) {
                            if(strcmp("/", big_huge_frumps_lower[i])==0) {
                                printf("TRASHING VAGINERS MORE: %d\n", tmp);
                                //exit(9);
                                //continue;
                                int frump = ATOI(big_huge_frumps_lower[i+1]);
                                if(!frump || frump > spec_max) {
                                    printf("TRASHING VAGINERS EVEN MORE: %s\n", big_huge_frumps_lower[i+1]);
                                    exit(9);
                                }
                                printf("CRASHING VAGINERS MORE: %d\n", frump);
                                match_draw_vals[5] = frump;
                                n_match_vals--;
                                break;
                            }
                            printf("MASHING VAGINERS MORE: %d\n", tmp);
                            exit(9);
                        }
                    }
                    if(i < 6) {
                        for(int j = 0; j < i-1; j++) {
                            //printf("SPLASHING WEINERS[%d/%d]\n", i, j);
                            if(match_draw_vals[j] == tmp) {
                                printf("FLASHING WEINERS[%d/%d]\n", i, j);
                                exit(9);
                            }
                        }
                    } //else {
                        //for(int j = 0; j < i-1; j++) {
                          //  printf("SPLASHING VA-JAY-JAYS[%d/%d]\n", i, j);
                            
                        //}
                    //}

                    match_draw_vals[i-1] = tmp;

                }

                
                //draw_in_order(match_draw_vals);
                //(condition) ? (variable = Expression2) : (variable = Expression3);
                //int n_to_order = n_match_vals > N_WHITES ? n_to_order = N_WHITES : (n_to_order = n_match_vals);
                
                    //printf("ORDERING VAGINERS~@~ %d\n", n_to_order);
                    //exit(9);
                //}

                array_in_order(match_draw_vals, n_match_vals > N_WHITES ? N_WHITES : n_match_vals);

                //for(int i = 0; i < n_match_vals; i++) {
                  //  printf("SMASHING BONERS: %d\n", match_draw_vals[i]);
                    //exit(9);
                //}
                //printf("SMASHING VAGINERS: %d\n", bhf_index);
                //exit(9);

                //printf("POOPY BONERS %d\n", n_match_vals);
                //exit(9);

                long n_draws_to_match = match_draw(match_draw_vals, n_match_vals,
                    pot_threads, pot_spec_threads, n_threads);

                printf("~~~SPLASHING VA-JAY-JAYS: {%ld}==>\n", n_draws_to_match);
                //exit(9);

            /* SUBMIT command */
            } else if(strcmp("submit", big_huge_frumps_lower[0]) == 0) { 
                printf("SQUISHING VAGINERS\n");
                if(daily_ticket.n_draws == 0) 
                    printf("Daily ticket contains no draws. (nothing to submit)\n");
                else {
                    
                    
                    GET_ANSWER:
                    printf("Are you sure you want to submit the daily ticket?");
                    printf(" (containing %d draws)\n", daily_ticket.n_draws);
                    show_daily_ticket();

                    char tmp_str[1000] = "";
                    fgets(tmp_str, sizeof(tmp_str), stdin);
                    tmp_str[strlen(tmp_str)-1] = 0;
                    char *input_trim = tmp_str;
                    if(trim_string(&input_trim)) {
                        printf("FISTING JUNKIES: \"%s\"\n", input_trim);
                        exit(9);
                    } 
                    
                    if(strcmp("y", input_trim) == 0 || strcmp("Y", input_trim) == 0 ||
                        strcmp("yes", input_trim) == 0 || strcmp("YES", input_trim) == 0) {
                        printf("Submitting daily ticket...(TODO)\n");
                        memset(&daily_ticket, 0, sizeof(daily_ticket));
                    } else if(!(strcmp("n", input_trim) == 0 || strcmp("N", input_trim) == 0 ||
                        strcmp("no", input_trim) == 0 || strcmp("NO", input_trim) == 0)) {
                        printf("Enter Y/ES or N/O (y/es or n/o)\n");
                        goto GET_ANSWER;
                    } else 
                        printf("Daily ticket NOT submitted.\n");
                }
            } else {
                if(bhf_index == 1 && 
                            (strcmp("exit", big_huge_frumps_lower[0]) == 0 ||
                    strcmp("quit", big_huge_frumps_lower[0]) == 0)) {
                    if(fill_routine == &fill_lottery_nums_rand_v3) {
                        pthread_mutex_lock(&jumbling_mux);
                        if(JUMBLING) {
                            //while(1) {
                            //printf("PILLOW BITING JUNKIES~~~");
                            //}
                            JUMBLING = 0;
                        }
                        pthread_mutex_unlock(&jumbling_mux);

                        for(int i = 0; i < n_threads; i++) {
                            if(pthread_join(pot_threads[i], NULL)) {
                                printf("FLIPPING BONERS\n");
                                exit(9);
                            }
                            //printf("~~JOINING BONERS[%d]~~\n", i);
                        }

                        for(int i = 0; i < n_threads; i++) {
                            if(pthread_join(pot_spec_threads[i], NULL)) {
                                printf("FLIPPING JUNKIES 2\n");
                                exit(9);
                            }
                            //printf("~~JOINING VA-JAY-JAYS[%d]~~\n", i);

                            
                        }
                    }

                    printf("Exiting program, good bye.\n");
                    return SUCCESS;
                }
                printf("\"%s\" is an invalid command.\n", big_huge_frumps[0]);
                usage();
            }
        }
        
    }
    
    return status;
}

int invalid_gaps()
{
    printf("SCRAPPLING JUNKIES\n");
    // TODO: be sure to check for consecs here too...

    int gap_total = 0;
    for(int i = 0; i < N_GAPS; i++, gap_total++) {
        struct gap *g = &gaps[i];
        print_gap(g);
        if(GAP_SET(g) && !g->less_than) {
            gap_total += g->val;
            printf("SCRAPPLING NANNIES: %d\n", gap_total);
        }
        
    }
    printf("SCRAPPLING GRANNIES: (%d > %d)\n", gap_total, first_five_max);
    return gap_total > first_five_max;
}



int main(int argc, char *argv[])
{

    char day[4] = {0};
    char s[64] = {0};
    size_t ret;
    int num_draws;  
    time_t t; 
    struct tm *tm; 
    int status;

    prog_mode = PMODE_NORMAL;
    
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

        printf("SPLASHING BONERS~~\n");
        lottery_day = LOT_TYP_PB;
        first_five_max = PB_MAX;
        spec_max = PB_SPEC_MAX;

    }

    range[0] = 1;
    range[1] = first_five_max;
    range_sp[0] = 1;
    range_sp[1] = spec_max;

    /* If not args supplied, run the program. */
    if(argc == 1) 
        return run_program();
    
    /* Otherwise run at least one draw or report error status. */
    num_draws = 1;

    //memset(consecs, 0, sizeof(int)*N_GAPS);

    fill_routine = &fill_lottery_nums_rand_v1;

    // for testing?
    fill_routine = &fill_lottery_nums_rand_v2;

    status = parse_args(argc, argv, &num_draws);

    
    switch(status) {
        case FAILURE:
            fprintf(stderr, "Invalid Program Arguments: status=%X\n", status);
            usage();
            return status;
        case EXIT:
            return SUCCESS;
     
    }

    const int n_threads = 100; 
    pthread_t pot_threads[n_threads], pot_spec_threads[n_threads];


    if(fill_routine == &fill_lottery_nums_rand_v3) {

        if(pthread_mutex_init(&pot_mux, NULL)) {
            perror("pthread_mutex_init FAILED (1)\n");
            exit(1);
        }

        if(pthread_mutex_init(&jumbling_mux, NULL)) {
            perror("pthread_mutex_init FAILED (2)\n");
            exit(1);
        }

        pot_init(pot_threads, pot_spec_threads, n_threads);
        
        sleep(1);
    }

    
    memset(&daily_ticket, 0, sizeof(daily_ticket));

    /* Show the ACTUAL (not user specified range) 5 number range. */
    int tmp_range_low = range[0], tmp_range_high = range[1];
    range[0] = 1;
    range[1] = first_five_max;
    display_daily_lotto_info();
    
    if(GAPS_SET)
        print_gaps();
    if(num_draws == 1)
        printf("-Running 1 draw.\n");
    else
        printf("-Running %d draws.\n", num_draws);
    printf("~ Good Luck! ~\n");
    printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n\n");

    /* Reset the range to user specified. */
    range[0] = tmp_range_low;
    range[1] = tmp_range_high;

    if(lottery_day != LOT_TYP_NONE) {
        srand((unsigned) time(&t));
    
        for(int i = 0; i < num_draws; i++) {
            
            if(i == 0) 
                display_draw_info();

            memset(&daily_ticket, 0, sizeof(daily_ticket));
            
            (*fill_routine)(&daily_ticket);

            printf("\n-Draw %d-+-+-+\n", i+1);
            present_draw(i, &daily_ticket);

            if(fill_routine == &fill_lottery_nums_rand_v3) {
                pthread_mutex_lock(&jumbling_mux);
                JUMBLING = 0;
                pthread_mutex_unlock(&jumbling_mux);
                /* join the pot threads */
                for(int i = 0; i < n_threads; i++) {
                    if(pthread_join(pot_threads[i], NULL)) {
                        perror("pthread_join FAILED (1)\n");
                        exit(9);
                    }
                }
                /* join the special pot threads */
                for(int i = 0; i < n_threads; i++) {
                    if(pthread_join(pot_spec_threads[i], NULL)) {
                        perror("pthread_join FAILED (2)\n");
                        exit(9);
                    }       
                }

                pot_init(pot_threads, pot_spec_threads, n_threads);
                /* */
                sleep(1);
            }
        }
        /* */
        sleep(1);
    }

    return 0;

}

