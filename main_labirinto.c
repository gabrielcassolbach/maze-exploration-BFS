/*===========================================================================*/
/*                             LABIRINTO                                     */
/*===========================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pqueue.c"

/*===========================================================================*/

#define FILENAME "teste3.txt"
#define CLEAR "clear" // Mude para "cls" no Windows.

#define BUFLEN 1024 // Altura/largura máxima do labirinto + 2

#define PAREDE -2
#define CAMINHO -1

typedef struct {
  int x;
  int y;
} lab_pos;  

/*===========================================================================*/
/*                              FUNÇÕES                                      */
/*===========================================================================*/
int** carregaLabirinto (char* filename, int* w, int* h, int* x_rato, int* y_rato, int* x_queijo, int* y_queijo);
void mostraLabirinto (int** lab, int w, int h, int x_rato, int y_rato, int x_queijo, int y_queijo);
void preencheMatrizCusto (int** m, int w, int h, int x_queijo, int y_queijo);
void imprimeMatriz (int** m, int w, int h);
int calculaCaminho (int** m, int w, int h, int x_rato, int y_rato, lab_pos** caminho);
void mostraCaminho (int** m, int w, int h, int x_queijo, int y_queijo, lab_pos* caminho, int tam);
void observaPontosAdjacentes(int** m, int w, int h, int y, int x, int k, Queue* q);
int pos_valid(int w, int h, int y, int x);
lab_pos retornaMelhorAlternativa(int** m, int w, int h, int y, int x);
/*===========================================================================*/

int main ()
{
    int **lab; 
    int w, h, tam;
    int rato_x, rato_y, queijo_x, queijo_y;
    lab_pos *caminho; 
    lab = carregaLabirinto (FILENAME, &w, &h, &rato_x, &rato_y, &queijo_x, &queijo_y);
    
    if (!lab)
    {
        printf ("Erro abrindo %s\n", FILENAME);
        return 1;
    }
  
    preencheMatrizCusto (lab, w, h, queijo_x, queijo_y);
    tam = calculaCaminho (lab, w, h, rato_x, rato_y, &caminho);
    mostraCaminho (lab, w, h, queijo_x, queijo_y, caminho, tam);
   
    for (int i = 0; i < h; i++)
        free (lab [i]);
    free (lab);
    free (caminho);

    return 0;
}

int** carregaLabirinto (char* filename, int* w, int* h, int* x_rato, int* y_rato, int* x_queijo, int* y_queijo)
{
    int i, j;
    char foostring [BUFLEN];
    FILE* f;
    int** m;

    f = fopen (filename, "r");
    if (!f)
        return NULL;

    *x_rato = -1;
    *y_rato = -1;
    *x_queijo = -1;
    *y_queijo = -1;

    if (!fgets (foostring, BUFLEN, f))
        return NULL;
    *w = strlen (foostring)-1;
    foostring [*w] = 0;

    m = (int**) malloc (sizeof (int*) * BUFLEN);

    for (i = 0; foostring [0]; i++)
    {
        
        m [i] = (int*) malloc (sizeof (int) * *w);
        for (j = 0; j < *w; j++)
        {
            
            if (foostring [j] == 'r' || foostring [j] == 'R')
            {
                *y_rato = i;
                *x_rato = j;
                foostring [j] = ' ';
            }
            
            else if (foostring [j] == 'q' || foostring [j] == 'Q')
            {
                *y_queijo = i;
                *x_queijo = j;
                foostring [j] = ' ';
            }

            if (foostring [j] == ' ')
                m [i][j] = CAMINHO;
            else
                m [i][j] = PAREDE;
        }

        if (!fgets (foostring, *w + 2, f))
            foostring [0] = 0; 
        foostring [*w] = 0;
    }

    *h = i;
    m = realloc (m, sizeof (int*) * (*h));

    fclose (f);

    return (m);
}

void mostraLabirinto (int** lab, int w, int h, int x_rato, int y_rato, int x_queijo, int y_queijo)
{
    int i, j;

    for (i = 0; i < h; i++)
    {
        for (j = 0; j < w; j++)
        {
            if (i == y_rato && j == x_rato)
                printf ("R");
            else if (i == y_queijo && j == x_queijo)
                printf ("Q");
            else if (lab [i][j] == PAREDE)
                printf ("|");
            else
                printf (" ");
        }

        printf ("\n");
    }
}

void preencheMatrizCusto (int** m, int w, int h, int x_queijo, int y_queijo)
{
    int k = 0;
    Queue* q = create(w*h+1);
   
    m[y_queijo][x_queijo] = k;    
    observaPontosAdjacentes(m, w, h, y_queijo, x_queijo, k + 1, q);
   
    while(!empty(q)){
        point p = dequeue(q);
        m[p.x][p.y] = p.value;
        observaPontosAdjacentes(m, w, h, p.x, p.y, p.value + 1, q); 
    }

    destroy(q);
}

void observaPontosAdjacentes(int** m, int w, int h, int y, int x, int k, Queue* q)
{
    if((y - 1 >= 0 && y - 1 < h) && (x >= 0 && x < w) && m[y - 1][x] == -1)
        enqueue(q, (point){y - 1, x, k});
   
    if((y + 1 < h) && (x < w) && m[y + 1][x] == -1)
        enqueue(q, (point){y + 1, x, k});
    
    if((y < h) && (x + 1 < w) && m[y][x + 1] == -1)
        enqueue(q, (point){y, x + 1, k});
   
    if((y < h) && (x - 1 >= 0 && x - 1 < w) && m[y][x - 1] == -1)
        enqueue(q, (point){y, x - 1, k});
}

void imprimeMatriz (int** m, int w, int h)
{
    int i, j;

    for (i = 0; i < h; i++)
    {
        for (j = 0; j < w; j++)
            printf ("%3d ", m[i][j]);
        printf ("\n");
    }
    printf("\n");
}

int calculaCaminho (int** m, int w, int h, int x_rato, int y_rato, lab_pos** caminho)
{   
    
    int i = 0;
    lab_pos point;
    lab_pos* v = (lab_pos*) malloc (sizeof (lab_pos) * (w*h));
    
    point.x = x_rato; 
    point.y = y_rato;
    v[i] = point;
    ++i;    

    while(m[point.y][point.x]){
        point = retornaMelhorAlternativa(m, w, h, point.y, point.x);
        v[i] = point;
        ++i;
    }

    *caminho = v;
    return (i);
}

lab_pos retornaMelhorAlternativa(int** m, int w, int h, int y, int x)
{
    int melhor_movimento;
    lab_pos p;
    melhor_movimento = p.y = p.x = INT_MAX;
    
    if(pos_valid(w, h, y + 1, x) && m[y + 1][x] != -2 && melhor_movimento > m[y + 1][x])
        {melhor_movimento = m[y + 1][x]; p.y = y + 1; p.x = x;}

    if(pos_valid(w, h, y - 1, x) && m[y - 1][x] != -2 && melhor_movimento > m[y - 1][x])
        {melhor_movimento = m[y - 1][x]; p.y = y - 1; p.x = x;}

    if(pos_valid(w, h, y, x + 1) && m[y][x + 1] != -2 && melhor_movimento > m[y][x + 1])
        {melhor_movimento = m[y][x + 1]; p.y = y; p.x = x + 1;}

    if(pos_valid(w, h, y, x - 1) && m[y][x - 1] != -2 && melhor_movimento > m[y][x - 1])
        {melhor_movimento = m[y][x - 1]; p.y = y; p.x = x - 1;}

    return p;
}

int pos_valid(int w, int h, int y, int x)
{
    return (y >= 0 && y < h && x >= 0 && x < w);
}

void mostraCaminho (int** m, int w, int h, int x_queijo, int y_queijo, lab_pos* caminho, int tam)
{
    int i;
    for (i = 0; i < tam; i++)
    {
        system (CLEAR);
        mostraLabirinto (m, w, h, caminho[i].x, caminho[i].y, x_queijo, y_queijo);
        getchar();
    }
}

/*===========================================================================*/
