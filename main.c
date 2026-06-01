#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


struct nodeList
{
        double inf;
        struct nodeList *next;
        bool flag;
};


struct StekList
{
        struct nodeList *Top;
        int size;
};

typedef struct nodeList Node;
typedef struct StekList Stek;


bool Push(Stek*st, double value,bool cheak)
{
  if (st == NULL) {return false;}
Node *el = (Node*)malloc(sizeof(Node));

        if (el) {
            el->inf = value;
                el->next = NULL;
                el->flag = cheak;
                if (st->Top == NULL)
                {
                        st->Top = el;
                }
                else
                {
                        el->next = st->Top;
                        st->Top = el;
                }
                st->size++;
                return true;
        }
        return false;
}



bool ShowTop( Stek *st, double *value, bool* cheak)
{if (st == NULL || value == NULL || cheak == NULL) {return false;}
        bool answer = false;
        if (value) {
                if (st->Top)
                {
                        *value = st->Top->inf;
                        answer = true;
                        *cheak = st->Top->flag;

                }
        }
        return answer;
}

bool isEmpty(Stek  *st)
{
        if (st->Top == NULL)
                return true;
        return false;
}

bool Pop(Stek *st, double *value,bool*cheak)
{ if (st == NULL || value == NULL || cheak == NULL) {return false;}
  if (isEmpty(st)==true){return false;}
  Node *currenttop = st->Top;
  *value = currenttop->inf;
  st->Top= currenttop->next;
  *cheak = currenttop->flag;
  free(currenttop);
  currenttop= NULL;
  st->size--;
  return true;

}

void Clear(Stek *st)
{ if(st!= NULL){
  double trash;
  bool cheak;
  while (!isEmpty(st)){
    Pop(st,&trash,&cheak);}
  }
}




bool Readning(char *stroki , char *futurerpn,int buffer, int *Tab)
{ if (stroki == NULL || futurerpn== NULL || Tab== NULL || buffer <= 0){return false;}
  FILE*in =fopen(stroki,"r");
  if (in== NULL){return false;}
  int count = 0;
  bool readingfs = true;
  unsigned char el = 0;
  while(count < buffer && readingfs && fscanf(in,"%c",&el)==1)
  {
  if (el == '\n')
    {
    readingfs = false;
    }

  else{
      if (el != ' ')
        {
          futurerpn[count] = el;
          count++;
        }

       }
  }
  futurerpn[count] = '\0';
  unsigned char name;
  int valu;
    while (fscanf(in, " %c = %d", &name, &valu) == 2) {
        if ((name >= 'a' && name <= 'z') || (name >= 'A' && name <= 'Z')) {
            Tab[(unsigned char)name] = valu;
        }
    }
  fclose(in);
return true;}

bool take_value(double value, bool cheak,double *res,int *Tab)
{
  if(res == NULL || Tab== NULL){return false;}
  if(cheak == false)
  {
    *res = value;
    return true;
  }
  *res=(int)Tab[(unsigned char)value];
  return true;
}


bool CreateRPN(char *stroka,char *poliz,int *Tab,int buff)
{ if (stroka== NULL || poliz == NULL || Tab==NULL || buff<=0){return false;}
  Tab['('] = 0;
  Tab['='] = 9;
  Tab['+'] = 6;
  Tab['-'] = 6;
  Tab['*'] = 7;
  Tab['/'] = 7;
  Stek st;
  st.Top = NULL;
  st.size = 0;
  int first = -10;
  int lenrpn = 0;
  bool ravno = false;
  bool prevskob = true;
  bool znak = false;
  bool digit = false;
  bool skip;
  int sravprior;
  int stekprior;
  for (int i = 0; stroka[i] != '\0'; i++)
  {
    unsigned char element = stroka[i];
    if (element != ' ')
    {
      if ((element >= 'a' && element <= 'z') || (element >= 'A' && element <= 'Z') || (element >= '0' && element <= '9'))
      {
      if(((element >= 'a' && element <= 'z') || (element >= 'A' && element <= 'Z'))
        && first == -10&& ravno==false){
          first = (int)element;
          digit=false;}

      if((element >= '0' && element <= '9')){
        digit = true;}

      if(lenrpn >= buff-1){
          Clear(&st);
          return false;}
      poliz[lenrpn] = element;
      lenrpn++;
      prevskob = false;



      }

      else if (element == '(' || element == ')' || element == '+' || element == '-'
      || element == '*' || element == '/' || element == '='){


        if (element == '(')
        {
          Push(&st,element,false);
          prevskob = true;
        }

      else if (element == ')')
        {
          if (isEmpty(&st)){
            Clear(&st);
            return false;}

          double simvol;
          while (Pop(&st,&simvol,&skip) && (char)simvol != '(')
            {
              poliz[lenrpn] = (char)simvol;
              lenrpn++;
            }
          if (isEmpty(&st) && (char)simvol != '('){
            Clear(&st);
            return false;}
          prevskob=false;
        }
      else if (element == '+' || element == '-' || element == '*' || element == '/' || element == '=')
        {
        double Toppr;
        bool cont = true;
        bool unar= false;
        bool cheak = false;
        if (prevskob && (element == '+' || element == '-')){
            sravprior = 8;
            unar = true;}
        else if (element == '='){sravprior = 9;}
        else {sravprior = Tab[(unsigned char)element];}

        if (element == '='){
            if (znak || digit){
              return false;}
            prevskob=true;
            ravno = true;
        }
        if (unar){
            if(lenrpn >= buff-1){
                Clear(&st);
                return false;}
            poliz[lenrpn] = '0';
            lenrpn++;
            prevskob=false;}





        while (!isEmpty(&st) && ShowTop(&st, &Toppr,&cheak) && cont)
        { if (cheak == true){stekprior = 8;}
          else if (Toppr!='='){stekprior=Tab[(unsigned char)Toppr];}
          else{stekprior=2; prevskob=true;}

          if (stekprior>= sravprior)
            {
              Pop(&st,&Toppr,&skip);
              poliz[lenrpn] = (char)Toppr;
              lenrpn++;
            }
          else{cont =false;}
        }
        if(lenrpn >= buff-1){
          Clear(&st);
          return false;}
        Push(&st,element,unar);
        znak = true;
        digit=false;
        if (element != '='){
        prevskob = false;}


      }
      else{
        Clear(&st);
        return false;}



      }
    }
  }
  double simvol;
  if (ravno == false){first = -10;}
  Tab[0] = first;
  while (Pop(&st, &simvol,&skip))
  {
      if ((char)simvol == '('){
        return false;}
      poliz[lenrpn] = (char)simvol;
      lenrpn++;
  }
  poliz[lenrpn] = '\0';
  return true;
}

bool SolveRPN(char *rpn, double *res,int *Tab)
{
  if(rpn==NULL || res == NULL || Tab == NULL){return false;}
  Stek st;
  st.Top = NULL;
  st.size = 0;
  bool yrav = false;
  bool znak = true;
  for (int i = 0; rpn[i] != '\0'; i++)
  {
    unsigned char element = rpn[i];
    if ((element >= '0' && element <= '9'))
    {
      Push(&st,(double)element-'0',false);
      znak = false;
    }
    else if ((element >= 'a' && element <= 'z') || (element >= 'A' && element <= 'Z')){
        Push(&st,(double)element,true);
        znak = false;
        }
    else if (element == '=') {
      if (yrav){
      return false;}
    yrav = true;
    double num1;
    double num2;
    bool cheak1;
    bool cheak2;
    if (!Pop(&st, &num1,&cheak1))
        {return false;}
    if (!Pop(&st, &num2,&cheak2))
        {return false;}

    if (cheak2 && !cheak1)
      {
        take_value(num1,cheak1,&num1,Tab);
        Push(&st,num1,false);
        znak = false;
      }
    else{
      return false;
      }
    }

    else{
    if(znak == true){
      Clear(&st);
      return false;}
    double num1;
    double num2;
    bool cheak1;
    bool cheak2;
    if (!Pop(&st,&num1,&cheak1))
      {
      return false;
      }
    if (!Pop(&st,&num2,&cheak2))
      {
      return false;
      }
    take_value(num2,cheak2,&num2,Tab);
    take_value(num1,cheak1,&num1,Tab);
    if (element == '+'){Push(&st,num2+num1,false);}
    else if (element == '-'){Push(&st,num2-num1,false);}
    else if (element == '*'){Push(&st,num2*num1,false);}
    else if (element == '/'){
      if (num1 == 0){
        return false;}
      Push(&st,num2/num1,false);
    }
    znak = false;
    }}
  if (st.size!= 1){
    Clear(&st);
    return false;}
  if (isEmpty(&st)){
      Clear(&st);
      return false;}
  bool flag;
  Pop(&st,res,&flag);
  if (yrav == true&& !isEmpty(&st))
    { double trash;
      bool cheaki;
      Pop(&st,&trash,&cheaki);

    }
  return isEmpty(&st);
  }
// test4 - ERROR
// test5 - ERROR
// test6 - ERROR
// test8 - ERROR
//test10 - ERROR
//test12 - ERROR
//test14 - ERROR
//test15 - ERROR
//test16 - ERROR

// test13!! (комплексный)
int main()
{
  int Tab[256]={0}; 
  int buff = 2000;
  char url[1000] = "/Users/fliruden/vuz/RPN/test17.txt";
  char exit[2000];
  char rpn[2000];
  double answer;

  if (!Readning(url,exit,buff,Tab)){
    printf("ERROR READING");
    return 0;}
  int buffer2 = 2000;
  if (!(CreateRPN(exit,rpn,Tab,buffer2))){
    printf("ERROR");
    return 0;}
  printf("%s",rpn);
  printf("\n");

  if(!(SolveRPN(rpn,&answer,Tab))){
    printf("ERROR");
    return 0;}

  if(Tab[0] != -10){
  printf("%c=%.4f",(char)Tab[0], answer);}
  else{
    printf("asnwer=%.4f",answer);
}
  // for(int j = 0;j <= 255; j++){
  //   if (Tab[j] != 0){printf("%d",Tab[j]);}}
  // printf("%s",exit);
  return 0;
}
//char url[1000] = "/Users/fliruden/vuz/RPN/fil.txt";

//5+4) ERROR
//(4+5 ERROR
//(4+5+) ERROR
//*(4+5) ERROR
//=(4+5) ERROR


//y=(4+a) a = 150 верно, y=154
//y=(4++a) ERROR
//y=(-4+a) a = 150 верно, y=146
//y=-(4+a) a= 10 y=-14
// y= -(-1)+5 y=6
//(4*15) ERROR
//2a= a/b ERROR
//y= 4a+b ERROR
//4a+b ERROR
//40+b ERROR
//y= -(-(a-b)+(a+b))*((a-b)+(a+b)) a=2 b=4 , y=-3
//-(-(a-b)+(a+b))*((a-b)+(a+b)) a=2 b=4 , y=-3
//(4+a)=y , a=5 ERROR
// y= a / b a=100 b = 21 Верно y=4.7619
//(-4+a) a = 150 верно, answer=146
//-4+a a = 150 верно, answer=146
//-4++a a = 150 ERROR
//y=-a a = 10 a=-10
