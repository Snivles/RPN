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
{
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
{
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
{
  double trash;
  bool cheak;
  while (!isEmpty(st)){
    Pop(st,&trash,&cheak);
  }
}




bool Readning(char *stroki , char *futurerpn,int buffer, int *Tab)
{ Tab['+'] = 1;
  Tab['-'] = 1;
  Tab['*'] = 2;
  Tab['/'] = 2;
  Tab['('] = 0;
  Tab['='] = -1;
  if (stroki == NULL){return false;}
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
  char  name;
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
    if(cheak == false)
    {
      *res = value;
      return true;
    }
    *res=(int)Tab[(unsigned char)value];
    return true;
}


bool CreateRPN(char *stroka,char *poliz,int *Tab)
{
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
  for (int i = 0; stroka[i] != '\0'; i++)
  {
    char element = stroka[i];
    if (element != ' ')
    {
      if ((element >= 'a' && element <= 'z') || (element >= 'A' && element <= 'Z') || (element >= '0' && element <= '9'))
      {
      if(((element >= 'a' && element <= 'z') || (element >= 'A' && element <= 'Z'))
        && first == -10&& ravno==false){
          first = (int)element;}

      if((element >= '0' && element <= '9')){
        digit = true;}
      poliz[lenrpn] = element;
      lenrpn++;
      prevskob = false;
      }
      else if (element == '(')
        {
          Push(&st,element,&skip);
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
        bool unznak = false;
        if (element == '='){
            if (znak || digit){return false;}
            prevskob=true;
            ravno = true;}
        if (prevskob && (element == '+' || element == '-')){
            unznak = true;
            prevskob = false;}
        if (unznak){
            poliz[lenrpn] = '0';
            lenrpn++;}
        double currentpr = Tab[(unsigned char)element];
        double Toppr;
        bool cont = true;
        while (!isEmpty(&st) && ShowTop(&st, &Toppr,&skip) && cont)
        {
          if (Tab[(unsigned char)Toppr]>= currentpr)
            {
              Pop(&st,&Toppr,&skip);
              poliz[lenrpn] = (char)Toppr;
              lenrpn++;
            }
          else{cont =false;}
        }
        Push(&st,element,&skip);
        znak = true;
        if (element != '='){
        prevskob = false;}
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
  Stek st;
  st.Top = NULL;
  st.size = 0;
  bool yrav = false;
  bool znak = true;
  for (int i = 0; rpn[i] != '\0'; i++)
  {
    char element = rpn[i];
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

int main()
{
  int Tab[256]={0};
  int buff = 2000;
  char url[1000] = "/Users/fliruden/vuz/RPN/file.txt";
  char exit[1000];
  char rpn[1000];
  double answer;
  Readning(url,exit,buff,Tab);
  // printf("%s",exit);
  // printf("\n");
  if (!(CreateRPN(exit,rpn,Tab))){
    printf("ERROR");
    return 0;}

  printf("%s",rpn);
  printf("\n");

  if(!(SolveRPN(rpn,&answer,Tab))){
    printf("ERROR");
    return 0;}

  if(Tab[0] != -10){
  printf("%c=%2.f",(char)Tab[0], answer);}
  else{
    printf("asnwer=%2.f",answer);
}
  // for(int j = 0;j <= 255; j++){
  //   if (Tab[j] != 0){printf("%d",Tab[j]);}}
  // printf("%s",exit);
  return 0;
}
//y=-(3+4)+2
//
//y=-(-(3+4)-a) a=150
//
//char url[1000] = "/Users/fliruden/vuz/RPN/fil.txt";

//5+4) ERROR
//(4+5 ERROR
//(4+5+) ERROR
//*(4+5) ERROR
//=(4+5) ERROR


//y=(4+a) a = 150 верно, y=154
//y=(4++a) ERROR
//y=(-4+a) a = 150 верно, y=146
//(-4+a) a = 150 верно, answer=146
//-4+a a = 150 верно, answer=146
//-4++a a = 150 ERROR
//y=-a a = 10 a=-10
