//BIO 08, q3 solution. (c) 2014 William Eustace
//MIT licensed - see repository
//breadth first search algorithm
//http://www.olympiad.org.uk/papers/2008/bio/bio08-exam.pdf
#include <iostream>
#include <time.h>
#include <unordered_map>
#include <queue>
//Shirt operations. See map at start of each function for details.
int op1(int pos){
    //1234 567 -> 2341 567
    int maintain = (int) pos % 1000;
    pos = pos / 1000;
    int maintain2 = pos % 1000;
    int removed_shirt = pos / 1000;
    pos = maintain2 * 10000;
    pos += removed_shirt * 1000;
    pos += maintain;
    return pos;
}

int op2(int pos){
    //123 4567 -> 123 7456
    int maintain = (int) pos / 10000;
    pos = pos % 10000;
    int maintain2 = pos /10;
    int removed_shirt = pos % 10;
    pos = maintain2;
    pos += removed_shirt * 1000;
    pos += maintain * 10000;
    return pos;
}

int op3(int pos){
    //123 4 567 -> 412 3 567
    int removed_shirt = (pos/1000)%10;
    int maintain2 = pos / 10000;
    pos = (pos % 1000) + (maintain2 * 1000) + (removed_shirt * 1000000);//reassemble
    return pos;
}

int op4(int pos){
    //123 4 567 -> 123 5 674
    int removed_shirt = (pos/1000)%10;
    int maintain2 = pos / 10000;
    
    pos = ((pos % 1000)*10) + (maintain2 * 10000) + removed_shirt;//reassemble
    return pos;
}

typedef int (*shirtFnPointer)(int);//will have an array of pointers. Calls each one rapidly with
//a for loop, rather than having to use bulky operations.
int main(int argc, const char * argv[]) {
    shirtFnPointer operations[4] = {NULL};
    operations[0] = &op1;
    operations[1] = &op2;
    operations[2] = &op3;
    operations[3] = &op4;//Populate the array of operations.
    int shirts = 0;
    const int target = 1234567;
    std::unordered_map<int,int> created;
    //key is the shirt order, value is the number of steps to get there.
    
    std::queue<int> queued_shirts;//FIFO queue
    std::cin >> shirts;//read in the washing line. No exception handling because not required by question.
    //If you plan to use this program to sort a real world washing line, I suggest implementing exception handling and input checking!
    if(shirts==target){
        std::cout << "0" << std::endl;return 0;//don't miss anything obvious
    }
    
    long timer = clock();//begin the timer - just for interest here, but BIO execution times limited to 2s.
    //shirt processing (like at a laundry) code here.
    queued_shirts.push(shirts);//start at the entered line
    std::unordered_map<int,bool> visited;//Where we've been
    int steps = 0;//how many steps it has taken us to get here
    int current_line;//what the washing line looks like at the moment
    while(true){
        current_line = queued_shirts.front();//get the next one in the (FIFO) queue
        if(current_line == target)
            break;
        queued_shirts.pop();//remove it from the queue
        if(!visited[current_line]){//provided we haven't already visited this position...
            visited[current_line]=true;
            steps = created[current_line] + 1;
            //number of steps to current position
            for(int i = 0;i<4;i++){
                int temp = operations[i](current_line);//try op1
                int ref_steps = created[temp];
                if(ref_steps != 0){//if this is 0, it means we haven't assigned a value to it (=created it) yet
                    //has been created already - add it to the queue and
                    if(steps < ref_steps) created[temp]=steps;//if have done it in fewer steps
                    queued_shirts.push(temp);//queue
                }
                else{
                    queued_shirts.push(temp);//if we've never created it, we can't have queued it
                    created[temp] = steps;
                }
            }
        }
    }
    
    timer = clock() - timer;//stop the clock
    std::cout << current_line << std::endl;//Output the final line, to show what it looks like (always 1234567)
    std::cout << created[current_line] << " steps" << std::endl;//number of steps taken
    double time_spent = timer / (double)CLOCKS_PER_SEC;//time spent in seconds
    std::cout << "Took " << time_spent << " seconds at " << CLOCKS_PER_SEC << " clocks per second." << std::endl;//print
    //done!
    return 0;
}