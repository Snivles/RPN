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

bool Readning(char *stroki , char *futurerpn,int buffer)
{
  if (stroki == NULL){return false;}
  FILE*in =fopen(stroki,"r");
  if (in== NULL){return false;}
  int count = 0;
  bool readingfs = true; // флаг чтобы считать 1 строку
  bool isequal = false;// проверка что есть знак
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
            Tab[(unsigned char)name] = valu;
        }
    }
  fclose(in);
return true;}


bool CreateRPN(char *stroka,char *poliz)
{
  for (int i = 0; stroka[i] != '\n'; i++)
  {
    char element = stroka[i];//берем элемент
  }

}




int main()
{

  int buff = 2000;
  char url[1000] = "/Users/fliruden/vuz/RPN/file.txt";
  char exit[1000];
  Readning(url,exit,buff);
  printf("%s",exit);
  //printf("%s",exit);
    //     Node*pi = NULL, *pj = NULL;

    // //Вывод значения стека не экран без изменения струкутры стека(т.е. ничего не удаляем, просто выводим на экран)
    // //с помощю цикла выполняем навигацию по элементам стека.
    //     for (pi = st.Top; pi; pi = pi->next)
    //     {
    //             printf("%d,", pi->inf);
    //     }
  return 0;
}
