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


bool Push(Stek*st, double value,bool check)
{
  if (st == NULL) {return false;}
Node *el = (Node*)malloc(sizeof(Node));

        if (el) {
            el->inf = value;
                el->next = NULL;
                el->flag = check;
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



bool ShowTop( Stek *st, double *value, bool* check)
{if (st == NULL || value == NULL || check == NULL) {return false;}
        bool answer = false;
        if (value) {
                if (st->Top)
                {
                        *value = st->Top->inf;
                        answer = true;
                        *check = st->Top->flag;

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

bool Pop(Stek *st, double *value,bool*check)
{ if (st == NULL || value == NULL || check == NULL) {return false;}
  if (isEmpty(st)==true){return false;}
  Node *currenttop = st->Top;
  *value = currenttop->inf;
  st->Top= currenttop->next;
  *check = currenttop->flag;
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




bool ParsingFile(char *pathfile , char *futurerpn,int buffer, int *Tab)
{ if (pathfile == NULL || futurerpn== NULL || Tab== NULL || buffer <= 0){return false;}
  FILE*in =fopen(pathfile,"r");
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
  *res= Tab[(unsigned char)value];
  return true;
}



int getPriority(unsigned char c, int *Tab) {
    if (Tab == NULL){
        return -3;}
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')){
        return -1;}
    if (c >= '0' && c <= '9') {
        return -2;}
    if (c == '(' || c == ')' || c == '+' || c == '-' || c == '*' || c == '/' || c == '=') {
        return Tab[c];
    }
    return -3;
}



bool CreateRPN(char *stroka,char *poliz,int buff,int *yravnenie)
{ if (stroka== NULL || poliz == NULL || buff<=0){return false;}
  int Tab[127]={0};
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
  int priority_check;
  for (int i = 0; stroka[i] != '\0'; i++)
  {
    unsigned char element = stroka[i];
    if (element != ' ')
    {
      if(element >=128){
        Clear(&st);
        return false;
      }
      priority_check = getPriority(element, Tab);
      if (priority_check==-1 || priority_check==-2)
      {
      if((priority_check==-1) && first == -10&& ravno==false){
          first = (int)element;
          digit=false;}

      if(priority_check==-2){
        digit = true;}

      if(lenrpn >= buff-1){
          Clear(&st);
          return false;}
      poliz[lenrpn] = element;
      lenrpn++;
      prevskob = false;



      }

      else if (priority_check>=0){


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
      else if (priority_check>0)
        {
        double Toppr;
        bool cont = true;
        bool unar= false;
        bool cheak = false;
        if (prevskob && (priority_check==6)){
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
  *yravnenie = first;
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
      Clear(&st);
      return false;}
    yrav = true;
    double num1;
    double num2;
    bool check1;
    bool check2;
    if (!Pop(&st, &num1,&check1))
        {
        Clear(&st);
        return false;}
    if (!Pop(&st, &num2,&check2))
        {
        Clear(&st);
        return false;}

    if (check2 && !check1)
      {
        take_value(num1,check1,&num1,Tab);
        Push(&st,num1,false);
        znak = false;
      }
    else{
      Clear(&st);
      return false;
      }
    }

    else{
    if(znak == true){
      Clear(&st);
      return false;}
    double num1;
    double num2;
    bool check1;
    bool check2;
    if (!Pop(&st,&num1,&check1))
      {
      Clear(&st);
      return false;
      }
    if (!Pop(&st,&num2,&check2))
      {
      Clear(&st);
      return false;
      }
    take_value(num2,check2,&num2,Tab);
    take_value(num1,check1,&num1,Tab);
    if (element == '+'){Push(&st,num2+num1,false);}
    else if (element == '-'){Push(&st,num2-num1,false);}
    else if (element == '*'){Push(&st,num2*num1,false);}
    else if (element == '/'){
      if (num1 == 0){
        Clear(&st);
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
  char pathfile[1000] = "/Users/fliruden/vuz/RPN/test15.txt";
  char exit[2000];
  char rpn[2000];
  int code_yrav;
  double answer;

  if (!ParsingFile(pathfile,exit,buff,Tab)){
    printf("ERROR READING");
    return 0;}
  int buffer2 = 2000;
  if (!(CreateRPN(exit,rpn,buffer2,&code_yrav))){
    printf("ERROR");
    return 0;}
  printf("%s",rpn);
  printf("\n");

  if(!(SolveRPN(rpn,&answer,Tab))){
    printf("ERROR");
    return 0;}

  if(code_yrav != -10){
  printf("%c=%.4f",(char)code_yrav, answer);}
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
