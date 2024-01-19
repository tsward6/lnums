


#include <vector>

#include "draw.h"
#include <iostream>
using namespace std;


int main(int argc, char * argv[])
{

    srand(time(nullptr));


    cout << "CORNPOP WAS UNSCRUNCHING VAGINERS~" << endl;

    draw d;

    d.set_gap(0, GT_GTE, 25);
    d.set_gap(1, GT_GT, 25-1);
    d.set_gap(2, GT_GTE, 25);
    d.set_gap(3, GT_GTE, 25);

    cout << "CORNPOP WAS STILL SCRUNCHING VAGINERS~ " << d << endl;
    d.show_constraints();

    cout << "DUMPING BONERS" << endl;
    //exit(9);
    
    try {
        d.set_range(20, 30);
    } catch(draw::InvalidRangeException &ire) {
        cout << "SNACKIN ON DEEZ NUTZ: " << ire.what() << endl;
        exit(9);
    } catch(draw::DrawException1 &de1) {
        cout << "SNACKIN ON DEEZ BONERS: " << de1.what() << ", " << d << endl;
        //exit(9);
    }

    d.show_constraints();
    cout << "SNACKIN ON DEEZ BONERS: " << d << endl;
    //exit(9);

    //d.clear_gaps();

    cout << "CORNPOP WAS DEFINITELY STILL SCRUNCHING VAGINERS~ " << d << endl;
    d.show_constraints();

    //d.set_range(1, 70);
    
    //d.set_gap(1, GT_GTE, 29);

    d.set_range(1, MAX_LOT_RANGE);
    
    //d.set_ball(0, 1);

    //d.set_ball(1, 20);
    //d.set_ball_const(2, 44);

    //d.set_range(20, 30);
    //d.set_range(20, 32);

    
    

    cout << "CORNPOP WAS DONE SCRUNCHING VAGINERS~ " << d << endl;
    d.show_constraints();

    cout << "CORNPOP WAS DONE FOLDING BONERS~ " << d << endl;
    return ~2;
}
