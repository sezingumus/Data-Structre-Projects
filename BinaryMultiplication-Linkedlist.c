#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

//150113841
//SezinGumus
//Project1
struct Digit
{
    int digit;
    struct Digit *next;
};
typedef struct Digit digitnode;
typedef digitnode *digitptr;
struct Digit_Result
{
   int digit;
   struct Digit_Res *next;
};

struct Number
{
      signed long long int number;
      struct Number *next;
      digitptr *firstdigit;
};

typedef struct Number numbernode;
typedef numbernode *numberptr;

struct Result
{     signed long long int number;
      digitptr *firstdigit;
      struct Result *next;
};
typedef struct Result resultnode;
typedef resultnode *resultptr;

numberptr node1 = NULL,node2 = NULL;

void final_result(signed long long int num);
signed long long int  convert_dec(signed long long int num);
void reverse_result(resultptr *sptr);
void transfer_result(resultptr sptr,int num);
void insert_number(numberptr *sptr,signed long long int num);
void add_digit(numberptr sptr,int num);
void list_result(resultptr sptr);
int isValid(signed long long int num);
void transfer(numberptr sptr,signed long long int num);
void keep_result(resultptr *sptr,signed long long int num);
void multiplication(numberptr num1ptr,numberptr num2ptr);
int main()
{
    signed long long int num1,num2,temp1,temp2;
    printf("enter a binary num:");
    scanf("%lli",&num1);
    printf("enter a binary num:");
    scanf("%lli",&num2);
    int status1 = isValid(num1);
    int status2 = isValid(num2);
   //Checking validity of these two numbers
  if(status1 + status2 == 2)
  {
    jump:
    insert_number(&node1,num1);
    insert_number(&node2,num2);
    transfer(node1,num1);
    transfer(node2,num2);
    multiplication(node1,node2);

  }
  else
  {

      while(status1 + status2 != 2)
      {
             printf("The number you enter is invalid. Please enter a binary number\n");
             printf("enter a binary number:");
             scanf("%lli",&num1);
             status1 = isValid(num1);
             printf("enter a binary number:");
             scanf("%lli",&num2);
             status2 = isValid(num2);
      }
      goto jump;
  }
    return 0;
}
//This  transfers result to the result linked list
void final_result(signed long long int num)
{
 resultptr d = NULL,dtemp = NULL;
 keep_result(&dtemp,num);
 while(num>0)
 {
     keep_result(&d,num%2);
     num /= 2;
 }
 printf("\n");
 list_result(d);

}
signed long long int convert_dec(signed long long int num)
 {
     signed long long int total = 0;
     int count = 0;
     while(num > 0)
     {
         total += num%10*powl(2,count);
         count++;
         num /= 10;
     }
     total += num * powl(2,count);
     return total;
 }
//This methods reverses linked list of result
void reverse_result(resultptr *sptr)
{
  resultptr p,q,r;
  p = q = r = *sptr;
  p = p->next->next;
  q = q->next;
  r->next = NULL;
  q->next = r;
  while(p != NULL)
  {
      r=q;
      q=p;
      p=p->next;
      q->next = r;
  }
  *sptr = q;
}
//This method multiplies two binary number.It contents three methods which are reverse result,convert dec,final result.
void multiplication(numberptr nptr1,numberptr nptr2)
{
    resultptr rnode = NULL;
    digitptr d1,d2;
    signed long long int total = 0,total2 = 0;
    d2 = nptr2->firstdigit;
    int count = 0;
    while(d2 != NULL)
    {
        total = nptr1->number * d2->digit * powl(10,count);
        count++;
        keep_result(&rnode,total);
        d2 = d2->next;
    }
   reverse_result(&rnode);

     printf("\n");
   while(rnode != NULL)
   {
       total2 += convert_dec(rnode->number);
       rnode = rnode->next;
   }
  final_result(total2);

}
//This method divides into numbers to digit
void transfer(numberptr sptr,signed long long int num)
              {
     if(num == 0)
        add_digit(sptr,0);
        while(num > 0)
    {
          add_digit(sptr,num%10);

        num /= 10;
    }
}
//This method checks the number you enter is binary or not
int isValid(signed long long int num)
{
    int status = 1;
    while(1)
    {
        if(num == 0)
            break;
        else
        {
            int temp = num%10;
            if(temp >1)
            {
                status = 0;
                break;
            }
            num /= 10;
        }
    }
    return status;
}
//This method keeps numbers you enter.
void insert_number(numberptr *sptr,signed long long int num)
{

    numberptr nptr,cptr,pptr;
    nptr = malloc(sizeof(numbernode)); //number node creation
     // digit node creation

    if(nptr != NULL)
    {
        nptr->number = num;
        nptr->firstdigit = NULL;
        nptr->next = NULL;
        pptr = NULL;
        cptr = *sptr;

         /*while(cptr != NULL)
      {
          pptr = cptr;
          cptr = cptr->next;
      }*/



        if(pptr == NULL) //first node
        {
            nptr->next = *sptr;
            *sptr = nptr;

        }
        else
        {
          pptr->next = *sptr;
          nptr->next = cptr;
        }

    }
    else
        printf("Error,no avaliable memory");

}
//This method keeps digit of number you enter
void add_digit(numberptr sptr,int num)
{

  digitptr nptr,pptr,cptr;
  nptr = malloc(sizeof(digitnode));
  if(nptr != NULL)
  {
      nptr->digit = num;
      nptr->next = NULL;
      pptr = NULL;
      cptr = sptr->firstdigit;
      while(cptr != NULL)
      {
          pptr = cptr;
          cptr = cptr->next;
      }
      if(pptr == NULL)
      {
          nptr->next = sptr->firstdigit;
          sptr->firstdigit = nptr;
      }
      else
      {
          pptr->next = nptr;
          nptr->next = cptr;
      }

  }
   else
    printf("Error");

}
//This method keeps result of multiplication
void keep_result(resultptr *sptr,signed long long int num)
{
    resultptr nptr,cptr,pptr;
     nptr = malloc(sizeof(resultnode));
    if(nptr != NULL)
    {
        nptr->number = num;
        nptr->firstdigit = NULL;
        nptr->next = NULL;
        pptr = NULL;
        cptr = *sptr;

        if(pptr == NULL) //first node
        {
            nptr->next = *sptr;
            *sptr = nptr;

        }
        else
        {
          pptr->next = *sptr;
          nptr->next = cptr;
        }
    }
    else
        printf("Error,no avaliable memory");

}
//This method shows result of multiplication
void list_result(resultptr sptr)
 {
     printf("Result:");
    while(sptr != NULL)
    {
        printf("%lli",sptr->number);
        sptr = sptr->next;
    }
 }
