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




bool Readning(char *stroki , char *futurerpn,int buffer)
{
  if (stroki == NULL){return false;}
  FILE*in =fopen(stroki,"r");
  if (in== NULL){return false;}
  int count = 0;
  bool readingfs = true; // флаг чтобы считать 1 строку
  bool isequal = false;// проверка что есть знак
  bool lastDigit = false;
  bool lastZnak = false;
  unsigned char el = 0;
  char equal;
  while(count <= buffer && readingfs && fscanf(in,"%c",&el))
  {
  if (el == '\n')
    {
    readingfs = false;
    }

  else
    {
    if(isequal && el == '='){return false;}
    if (fscanf(in,"%c",&equal) ==1 && equal == '=' && count == 2) // то есть , 2 знак железно =
        {
          futurerpn[count] = equal;
          count++;
          isequal=true;
          lastZnak= true;
        }
    else if ((el >= 'a' && el <= 'z') || (el >= 'A' && el <= 'Z') || (el >= '0' && el <= '9'))
        {
          futurerpn[count] = el;
          count++;
          lastDigit = true;
        }
    else if ( (el >= '(' && el <= '+') || el =='-' || el=='/' || el == '=')
        {
          if (lastZnak ==true && (el =='/' ||el == '*') ){}
          futurerpn[count] = el;
          count++;
          lastZnak = true;
        }
    }

  }


return true;}







int main()
{
  Stek st;
  st.size = 0;
  st.Top = NULL;
  Push(&st, 1);
  Push(&st, 2);
  Push(&st, 3);
  Push(&st, 4);
  Push(&st, 5);
  Push(&st, 6);
    //     Node*pi = NULL, *pj = NULL;

    // //Вывод значения стека не экран без изменения струкутры стека(т.е. ничего не удаляем, просто выводим на экран)
    // //с помощю цикла выполняем навигацию по элементам стека.
    //     for (pi = st.Top; pi; pi = pi->next)
    //     {
    //             printf("%d,", pi->inf);
    //     }
  return 0;
}
