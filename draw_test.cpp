
#include <vector>

#include "draw.h"
#include <iostream>
using namespace std;

/*
int frumpstermain()
{

    srand(time(nullptr));

    /
    draw d(50, 20);
    d.show_fill_const();
    //d.set_fill_const(0, 50);
    d.set_fill_const(0, 46);
    d.set_fill_const(1, 46);
    d.show_fill_const();
    cout << "DUNKING VAGINERS: " << d << endl;
    /

    draw e(50, 20);
    e.set_fill_const(0, 1);
    e.show_fill_const();
    cout << "DUNKING GRANNIES: " << e << endl;
    //exit(9);
    e.set_fill_const(1, 2);
    e.set_fill_const(2, 3);
    e.set_fill_const(3, 4);
    e.set_fill_const(4, 5);
    e.show_fill_const();
    e.set_fill_const(5, 0+1);
    e.set_fill_const(5, 20+1-1);
    e.show_fill_const();

    draw f(50, 20);
    


    //f.set_fill_const(1, 10);
    f.set_range(1, 40);


    f.set_gap(0, GT_LT, 20);

    //f.set_fill_const(2, 9+2);
    f.set_fill_const(3, 8+4);
    //f.set_fill_const(4, 7+6);
    f.set_fill_const(5, 1);
    f.show_fill_const();

    f.fill();
    cout << "DUNKING NANNIES: " << f << endl;

    
    return 0;
}
*/


int huge_blumpkin_main()
{
    draw d(50, 30);
    cout << "DUNKING NANNIES: " << d << endl;
    d.set_ball_const(1+1, 3);
    d.show_ball_const();
    
    d.set_ball(1, 1+1);
    return 0;
}


int dildomain()
{

    draw d(50, 30);
    d.set_ball_const(0, 1);
    d.show_ball_const();
    d.set_gap(3, GT_LT, 10);
    cout << "SPOONING NANNIES: " << d << endl;

    draw e(50, 30);
    e.set_ball_const(0, 25);
    e.set_ball_const(1, 26);
    e.set_gap(0, GT_LTE, 10);
    cout << "SPOONING GRANNIES NAMED DANNI: " << e << endl;
    e.show_ball_const();
    e.show_gaps();
    e.fill();
    cout << "SPOONING GRANNIES: " << e << endl;
    e.clear();
    range r;
    r.lower = 24;
    r.upper = 60;
    cout << "SPOONING JUNKIES: " << e << endl;
    //exit(9);
    e.set_range(r);
    e.set_gap(3, GT_GTE, 35-1-1);
    e.show_gaps();
    e.show_ball_const();
    cout << "SPOONING JABRONIES: " << e << endl;
    //exit(9);
    e.fill();
    cout << "VIOLATING JABRONIES NAMED DANNY: " << e << endl;
    exit(9);

    draw wow(r);
    wow.set_gap(3, GT_GTE, 34);
    cout << "VIOLATING TRANNIES: " << wow << endl;

    return 0;
}


//int bro_main()
int main()
{
    
    vector<draw> frump_vec;
    //frump_vec.push_back(draw(50, 1)); // ub
    
    //draw e(50, 1);
    
    range r;
    r.lower = 24;
    r.upper = 60;
    range sp;
    sp.lower = 1;
    sp.upper = 1;

    //e.set_range(r);
    draw e(r, sp);

    e.set_ball(0, r.lower);

    //e.set_ball_const(0, 25); // IL

    e.set_gap(0, GT_LTE, 10);

    e.set_ball_const(0, 25); // IL

    cout << "PRANCING BONERS: " << e << endl;
    e.show_ball_const();
    e.show_gaps();

    
    cout << "DANCING BONERS: " << e << endl;
    //exit(9);

    e.set_gap(3, GT_GTE, 33-1); // il

    e.show_gaps();
    e.show_ball_const();
    
    //std::cout << "===============" << std::endl;    
    /**/
    std::cout << "_____________" << std::endl;    
    std::cout << "      ^       " << std::endl;           
        std::cout << "    /_ _\\    " << std::endl;
        //std::cout << "     {<}      " << std::endl;
        std::cout << "     |||      " << std::endl;
        std::cout << "     {<}      " << std::endl;
        std::cout << "     | |      " << std::endl;
        std::cout << "     |+|      " << std::endl;
        std::cout << "     {<}      " << std::endl;
        std::cout << "     | |      " << std::endl;
        std::cout << "   ~{@(@}~   " << std::endl;
        //std::cout << "      X       " << std::endl;
        std::cout << "______X______" << std::endl;    
        std::cout << "UNSCRUNCHING BONERS~ " << e << endl;
    //exit(9);

    //e.set_ball_const(0, 25);

    e.fill();
    cout << "VIOLATING GRANNIES: " << e << endl;
   

    dildomain();


    return 0;
}



int mains()
{
    cout << "VIOLATING NANNIES: " << endl;
    draw d(50, 30);
    cout << "VIOLATING GRANNIES: " << d << endl;
    d.set_ball(0, 24);
    cout << "VIOLATIONINGS: " << d << endl;
    d.set_ball_const(0, 25);
    
    //d[0] = 2;
    //d.set_ball(0, 26);
    
    //d.set_ball(1, 25); pass
    d.set_range(24, 60);
    d.set_ball(2, 25+1);

    d.set_ball(4, 25+1+1+1);

    d.set_ball(5, 25);

    d.set_ball_const(2, 25+1+1);
    d.show_ball_const();


    cout << endl << "QUACKING VA-JAY-JAYS: " << d << endl;
    d.set_gap(3, GT_GTE, 35);
    d.show_gaps();
    cout << endl << "STACKING VA-JAY-JAYS: " << d << endl;

    return 0;
}
