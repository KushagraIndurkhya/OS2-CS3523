/*
File:logger.cpp
Utilities for logging and struct definition for thread argument
Author:Kushagra Indurkhya
*/
#include<string>
#include<algorithm>
#include<sys/time.h>

/*
Class Logger
Providing logging utillities

Members:

int hr,min,sec,ms : local time precise to milliseconds when object is created
epoch : is the number of milliseconds that have elapsed since January 1, 1970 (GMT)
int local_id : index of thread in thread_arr
string action : action which triggered the creation (request/entry/exit)

*/

class logger
{
    int hr,min,sec,ms;
    long long epoch;
    std::string action;

    std::string suffix(int i)
    {
        int units = i%10;
        i=i/10;
        int tens = i%10;
        if (units==1 && tens!=1) return "st";
        if (units==2 && tens!=1) return "nd";
        if (units==3 && tens!=1) return "rd";
        else return "th";


    }
    public:
        int local_id;
        //Constructor
        logger(int l_id,std::string act)
        {
            local_id=l_id;
            action = act;


            struct timeval time_now = {0};
            long time_mil;
            long a,b;
            //Getting current time
            gettimeofday(&time_now,NULL);
            //Converting to milliseconds
            time_mil=(time_now.tv_sec*1000 + time_now.tv_usec/1000);
            epoch=time_now.tv_sec*1000000 + time_now.tv_usec;


            a = (time_mil%(1000*60*60*24))/3600000; //converting to hrs
            b = (time_mil%(1000*60*60))/60000; //converting to mins

            //adjusting for GMT for IST +5:30
            hr=a+5+(b+30)/60;
            min=(b+30)%60;

            sec=(time_mil%(1000*60))/1000;
            ms=time_mil%(1000);
        }

        //Returns log string
        std::string fetch_log()
        {
            std::string time_str=std::to_string(hr)+":"+std::to_string(min)+":"+std::to_string(sec)+":"+std::to_string(ms);
            std::string res = std::to_string(local_id)+suffix(local_id)+" customer "+action+"at "+time_str+"\n";
            return res;
        }

        long get_epoch()
        {
            return epoch;
        }
};


/*Comparator for sort*/
bool compare_log_obj(logger l1,logger l2)
{

    /*if epoch of both is not equal return one with less epoch time (ie occured first)*/
    if(l1.get_epoch() != l2.get_epoch())
        return (l1.get_epoch()<l2.get_epoch());
    /*return one with smaller local_id if epoch is equal*/
    else 
    // if(l1.local_id != l2.local_id)
        return(l1.local_id<l2.local_id);
}