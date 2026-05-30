#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


struct nodeList
{
        int inf;
        struct nodeList *next;
};


struct StekList
{
        struct nodeList *Top;
        int size;
};

typedef struct nodeList Node;
typedef struct StekList Stek;


bool Push(Stek*st, int value)
{
Node *el = (Node*)malloc(sizeof(Node));

        if (el) {
            el->inf = value;
                el->next = NULL;
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



bool ShowTop( Stek st, int *value)
{
        bool answer = false;
        if (value) {
                if (st.Top)
                {
                        *value = st.Top->inf;
                        answer = true;
                }
        }
        return answer;
}

bool isEmpty(Stek  st)
{
        if (st.Top == NULL)
                return true;
        return false;
}

bool Pop(Stek *st, int *value)
{
  if (isEmpty(*st)==true){return false;}
  Node *currenttop = st->Top;
  *value = currenttop->inf;
  st->Top= currenttop->next;
  free(currenttop);
  st->size--;
  return true;

}

void Clear(Stek *st)
{
  int trash;
  while (!isEmpty(*st)){
    Pop(st,&trash);
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
  bool readingfs = true; // флаг чтобы считать 1 строку
  unsigned char el = 0;
  while(count <= buffer && readingfs && fscanf(in,"%c",&el)==1)
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
            Tab[name] = valu;
        }
    }
  fclose(in);
return true;}


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
  for (int i = 0; stroka[i] != '\0'; i++)
  {
    char element = stroka[i];
    if (element != ' ')
    {
      if ((element >= 'a' && element <= 'z') || (element >= 'A' && element <= 'Z') || (element >= '0' && element <= '9'))
      {
      if(((element >= 'a' && element <= 'z') || (element >= 'A' && element <= 'Z')) && first == -10&& ravno==false){first = (int)element;}
      poliz[lenrpn] = element;
      lenrpn++;
      prevskob = false;
      }
      else if (element == '(')
        {
          Push(&st,element);
          prevskob = true;
        }
      else if (element == ')')
        {
          if (isEmpty(st)){return false;}
          int simvol;
          while (Pop(&st,&simvol) && (char)simvol != '(')
            {
              poliz[lenrpn] = (char)simvol;
              lenrpn++;
            }
          if (isEmpty(st) && (char)simvol != '('){return false;}
          prevskob=false;
        }
      else if (element == '+' || element == '-' || element == '*' || element == '/' || element == '=')
      {
        bool unznak = false;
        //if(znak == true){return false;}
        if (element == '='){prevskob=true; ravno = true;}
        if (prevskob && (element == '+' || element == '-')){unznak = true; prevskob = false;}
        if (unznak){poliz[lenrpn] = '0'; lenrpn++;}
        int currentpr = Tab[element];
        int Toppr;
        bool cont = true;
        while (!isEmpty(st) && ShowTop(st, &Toppr) && cont)
        {
          if (Tab[(char)Toppr]>= currentpr)
            {
              Pop(&st,&Toppr);
              poliz[lenrpn] = (char)Toppr;
              lenrpn++;
            }
          else{cont =false;}
        }
        Push(&st,element);
        znak = true;
        if (element != '='){
        prevskob = false;}
      }
    }
  }
  int simvol;
  if (ravno == false){first = -10;}
  Tab[0] = first;
  while (Pop(&st, &simvol))
  {
      if ((char)simvol == '('){return false;}
      poliz[lenrpn] = (char)simvol;
      lenrpn++;
  }
  poliz[lenrpn] = '\0';
  return true;
}

bool SolveRPN(char *rpn, int *res,int *Tab)
{
  Stek st;
  st.Top = NULL;
  st.size = 0;
  bool chislo = false;
  bool yrav = false;
  bool znak = true;
  for (int i = 0; rpn[i] != '\0'; i++)
  {
    char element = rpn[i];
    if ((element >= '0' && element <= '9'))
    {
      Push(&st,element-'0');
      znak = false;
      //chislo = true;
    }
    else if ((element >= 'a' && element <= 'z') || (element >= 'A' && element <= 'Z')){
        Push(&st,Tab[element]);
        znak = false;
        }
    else if (element == '=') {
    yrav = true;
    int num1;
    int num2;
    if (!Pop(&st, &num1)) {return false;}
    if (!Pop(&st, &num2)) {return false;}
      *res = num1;
      Push(&st, *res);
      znak = false;
      //chislo = false;
    }
    else{
    if(znak == true){Clear(&st); return false;}
    int num1;
    int num2;
    if (!Pop(&st,&num1))
      {
      return false;
      }
    if (!Pop(&st,&num2))
      {
      return false;
      }
    if (element == '+'){*res = num2 +num1;}
    else if (element == '-'){*res = num2 -num1;}
    else if (element == '*'){*res = num2 *num1;}
    else if (element == '/'){
      if (num1 == 0){return false;}
      *res = num2/num1;
      }
    Push(&st,*res);
    znak = false;
    //chislo = false;
    }}

  if (isEmpty(st)) return false;
  Pop(&st,res); // достаем ответ
  if (yrav == true&& !isEmpty(st))
    { int trash;
      Pop(&st,&trash);

    }
  return isEmpty(st);
  }

int main()
{
  int Tab[256]={0};
  int buff = 2000;
  char url[1000] = "/Users/fliruden/vuz/RPN/file.txt";
  char exit[1000];
  char rpn[1000];
  int answer;
  Readning(url,exit,buff,Tab);
  if (!(CreateRPN(exit,rpn,Tab))){printf("ERROR"); return 0;}

  printf("%s",rpn);
  printf("\n");
  if(!(SolveRPN(rpn,&answer,Tab))){printf("ERROR"); return 0;}
  if(Tab[0] != -10){
  printf("%c=%d",(char)Tab[0], answer);}
  else{
    printf("asnwer=%d",answer);
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
