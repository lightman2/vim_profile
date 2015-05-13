#include <stdio.h>
/*int dow(int y, int m, int d)*/
/*{*/
/*    static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};*/
/*    y -= m < 3;*/
/*    return (y + y/4 - y/100 + y/400 + t[m-1] + d) % 7; */
/*}*/


/*--------------------------------------------------------------------------
  FUNC: 6/11/11 - Returns day of week for any given date
  PARAMS: year, month, date
  RETURNS: day of week (0-7 is Sun-Sat)
  NOTES: Sakamoto's Algorithm
 
http://en.wikipedia.org/wiki/Calculating_the_day_of_the_week#Sakamoto.27s_algorithm
 
    Altered to use char when possible to save microcontroller ram
--------------------------------------------------------------------------*/
char dow(int y, char m, char d)
   {
       static char t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
       y -= m < 3;
       return (y + y/4 - y/100 + y/400 + t[m-1] + d) % 7;
   }
 
/*--------------------------------------------------------------------------
  FUNC: 6/11/11 - Returns the date for Nth day of month. For instance,
    it will return the numeric date for the 2nd Sunday of April
  PARAMS: year, month, day of week, Nth occurence of that day in that month
  RETURNS: dat
  set DOW = 0 to be sunday that we search
  NthWeek = -1 means the last sunday
  NOTES: There is no error checking for invalid inputs.
--------------------------------------------------------------------------*/
char NthDate(int year, char month, char DOW, char NthWeek)
{
    char targetDate = 1;
    char firstDOW = dow(year,month,targetDate);
    while (firstDOW != DOW){
      firstDOW = (firstDOW+1)%7;
      targetDate++;
    }
    //Adjust for weeks
    if(NthWeek != -1)
        targetDate += (NthWeek-1)*7;
    else
    {
        if(month == 3)//dst start
        {
            while(targetDate + 7<=30)
                targetDate += 7;
        }
        else if(month == 10)//dst end
        {
            while(targetDate + 7<=31)
                targetDate += 7;
        }
        else//others we don't care
        {
            while(targetDate + 7<=31)
                targetDate += 7;
        }
    }
    return targetDate;
}
 
int main()
{
    int y,m,d;

char buff[50];
/*NthDate()*/
sprintf(buff,"%i",NthDate(2013,10,0,-1));
  printf("%s\n",buff);
/*    printf("getdata year\n");*/
/*    scanf ("%d", &y); */
/*    printf("getdata %d\n", y);*/
/*    scanf ("%d", &m); */
/*    scanf ("%d", &d); */
/*    printf("weekday: %d\n",dow(y,m,d));*/

}
