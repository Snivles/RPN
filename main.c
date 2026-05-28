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
  free(st);
}


int Tab[256]={0};


void Priority()
{
  Tab['+'] = 1;
  Tab['-'] = 1;
  Tab['*'] = 2;
  Tab['/'] = 2;
  Tab['('] = 0;
  Tab['='] = 9;
}

bool Readning(char *stroki , char *futurerpn,int buffer)
{
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


bool CreateRPN(char *stroka,char *poliz)
{
  Stek st;
  st.Top = NULL;
  st.size = 0;
  int lenrpn = 0;
  bool prevskob = false;
  for (int i = 0; stroka[i] != '\0'; i++)
  {
    char element = stroka[i];
    if (element != ' ')
    {
      if ((element >= 'a' && element <= 'z') || (element >= 'A' && element <= 'Z') || (element >= '0' && element <= '9'))
      {
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
      else if (element == '+' || element == '-' || element == '*' || element == '/')
      {
        bool unznak = false;
        if (prevskob && (element == '+' || element == '-')){unznak = true;}
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
        prevskob = false;
      }
    }
  }
  int simvol;
  while (Pop(&st, &simvol))
  {
      if ((char)simvol == '('){return false;}
      poliz[lenrpn] = (char)simvol;
      lenrpn++;
  }
  poliz[lenrpn] = '\0';
  return true;
}

int SolveRPN(char *rpn)
{
  Stek st;
  st.Top = NULL;
  st.size = 0;
  for (int i = 0; rpn[i] != '\0'; i++)
  {
    char element = rpn[i];
    if ((element >= 'a' && element <= 'z') || (element >= 'A' && element <= 'Z') || (element >= '0' && element <= '9'))
    {
      Push(&st,element);
    }
    else{
    int res = 0;
    int num1;
    int num2;
    if (!Pop(&st,&num1))
      {
      Clear(&st);
      return false;
      }
    if (!Pop(&st,&num2))
      {
      Clear(&st);
      return false;
      }
    if (element == '+'){res = num2 +num1;}
    else if (element == '-'){res = num2 -num1;}
    else if (element == '*'){res = num2 *num1;}
    else if (element == '/'){
      if (num1 == 0){return false;}
      res = num2/num1;
      }
    Push(&st,res);}
  int res;
  if (!isEmpty(st)){
    Pop(&st,&res);
    if (!isEmpty(st)){return false;}
    return res;
}
  return false;
}
  }


int main()
{

  int buff = 2000;
  char url[1000] = "/Users/fliruden/vuz/RPN/file.txt";
  char exit[1000];
  char rpn[1000];
  Priority();
  Readning(url,exit,buff);
  if (!(CreateRPN(exit,rpn))){printf("ERROR"); return 0;}
  printf("%s",exit);
  printf("\n");
  printf("%s",rpn);
  // for(int j = 0;j <= 255; j++){
  //   if (Tab[j] != 0){printf("%d",Tab[j]);}}
  // printf("%s",exit);
  return 0;
}
