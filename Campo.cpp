/*
ALUNOS:
SAVIO HENRIQUE M. LAGE    2016003268
BRUNO RODRIGUES PINALI    2016007006
BRENNER BATISTA ROCHA     2016008193
MAURICIO MARQUES          2016013451

*/

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
const int TAM_FIG = 45;

using namespace std;

struct Campo
{
    bool Aberto; //PARA VERIFICAR SE A CASA ESTA ABERTA OU FECHADA
    bool Bomba;  //PARA VERIFICAR SE A CASA TEM BOMBA OU NAO
    bool Duvida; //PARA VERIFICAR SE O USUARIO QUER MARCAR COMO DUVIDA(BANDEIRA)
    int Quant;   //NUMERO DE BOMBAS EM VOLTA
};

class Tabuleiro
{
  private:
    Campo **Matriz;
    int Linhas;
    int Colunas;
    int Max_Bombas;

  public:
    Tabuleiro(int, int, int);           //CONSTRUTOR
    void CriaMatriz();                  //INICIA OS VALORES DE CADA CASA
    void ColocaBomba();                 //DISTRIBUI AS BOMBAS
    void ContadorBombas();              // CONTA AS BOMBAS
    void AbrirCasa(int, int, int, int); //ABRE A CASA DESEJADA
    void ColocaBandeira(int, int);      //COLOCA BANDEIRA DA CASA ESCOLHIDA
    void RetiraBandeira(int, int);      //RETIRA BANDEIRA DA CASA ESCOLHIDA
    bool VerificaAberto(int, int);      //VERIFICA SE A CASA ESCOLHIDA EST� ABERTA
    bool VerificaBomba(int, int);       //VERIFICA SE A CASA TEM BOMBA
    bool VerificaBandeira(int, int);    //VERIFICA SE A CASA TEM BANDEIRA
    int VerificaNum(int, int);          //RETORNA QUANTIDADE DE BOMBAS AO ARREDOR DA CASA
    int QuantAberto();                  //RETORNA QQUANTIDADE DE CASAS ABERTAS
};

Tabuleiro ::Tabuleiro(int lin, int col, int bomba) //CONSTRUTOR
{

    Max_Bombas = bomba;

    this->Linhas = lin;
    this->Colunas = col;
    Matriz = new Campo *[lin];
    for (int i = 0; i < lin; i++)
        Matriz[i] = new Campo[col]; // CRIA A MATRIZ DE PONTEIROS
}

void Tabuleiro ::CriaMatriz() //INCIALIZA TODAS AS VARIAVEIS

{
    for (int i = 0; i < Linhas; i++)
        for (int j = 0; j < Colunas; j++)
        {
            Matriz[i][j].Duvida = false; //DEFINE TUDO FALSO PARA INICIALIZA��O
            Matriz[i][j].Aberto = false;
            Matriz[i][j].Bomba = false;
            Matriz[i][j].Quant = 0;
        }
}

void Tabuleiro ::ColocaBomba() //DISTRIBUI AS BOMBAS PELO TABULEIRO
{
    int cont_bombas = 0, i = 0, j = 0;
    while (cont_bombas < Max_Bombas) //ENQUANTO TODAS AS BOMBAS NAO FOREM DISTIBUIDAS, O LA�O NAO PARA
    {
        i = rand() % Linhas;  // RAND DE LINHAS
        j = rand() % Colunas; //RAND DE COLUANS

        if (Matriz[i][j].Bomba == false) //VERIFICA SE O LUGAR ESPECIFICADO JA NAO POSSUI BOMBA
        {
            Matriz[i][j].Bomba = true; //COLOCA A BOMBA
            Matriz[i][j].Quant = -1;   //PARA DIFERENCIAR DAS OUTRAS OP�OES
            cont_bombas++;             //S� AUMENTA O CONTADOR SE A BOMBA FOR INSERIDA
        }
    }
}

void Tabuleiro ::ContadorBombas() //CONTA AS BOMBAS AOA ARREDOR DE CADA CASA
{
    int Contador = 0;                //CRIA O CONTADOR DE BOMBAS
    for (int i = 0; i < Linhas; i++) //PERCORRE  A MATRIZ
        for (int j = 0; j < Colunas; j++)
        {
            Contador = 0;                        //RESET DO CICLO
            for (int x = i - 1; x <= i + 1; x++) //PERCORRE EM VOLTA
                for (int y = j - 1; y <= j + 1; y++)
                    if (x >= 0 && y >= 0 && x < Linhas && y < Colunas) //ELIMINA A ANALISE EM VOLTA DA EXTREMIDADE (VALORES LIXOS FORA DA MATRIZ)
                        if (Matriz[x][y].Bomba)                        // VERIFICA SE TEM BOMBA
                            Contador++;                                // SE TIVER AUMENTA CONTADOR
            Matriz[i][j].Quant = Contador;                             //AO FINAL DO CILCO A CASA ANALISADA RECEBE A QUANTIDADE DE BOMBA
        }
}

void Tabuleiro ::AbrirCasa(int lin, int col, int MouseX, int MouseY) //FUN��O PARA ABRIR UMA CASA
{
    ALLEGRO_BITMAP *vazio = al_load_bitmap("Images/Aberto.jpg");
    ALLEGRO_BITMAP *img_um = al_load_bitmap("Images/1.png");
    ALLEGRO_BITMAP *img_dois = al_load_bitmap("Images/2.png");
    ALLEGRO_BITMAP *img_tres = al_load_bitmap("Images/3.png");
    ALLEGRO_BITMAP *img_quatro = al_load_bitmap("Images/4.png");
    ALLEGRO_BITMAP *img_cinco = al_load_bitmap("Images/5.png");
    ALLEGRO_BITMAP *img_seis = al_load_bitmap("Images/6.png");
    ALLEGRO_BITMAP *img_sete = al_load_bitmap("Images/7.png");
    ALLEGRO_BITMAP *img_oito = al_load_bitmap("Images/8.png");
    ALLEGRO_BITMAP *img_bomba = al_load_bitmap("Images/Bomba.jpg");

    if ((lin >= 0 && lin < Linhas) && (col >= 0 && col < Colunas)) //PARA ELIMINAR VALORES LIXOS NAS EXTREMIDADES
    {
        if (Matriz[lin][col].Aberto == false && Matriz[lin][col].Bomba == false) // SE A CASA TA SEM BOMBAE FECHADA
        {
            switch (Matriz[lin][col].Quant) //ANALISE DO NUMERO DE BOMBAS AO ARREDOR DE CADA CASA
            {
            case 0:
                al_draw_bitmap(vazio, (MouseX / TAM_FIG) * TAM_FIG, (MouseY / TAM_FIG) * TAM_FIG, 0); //COLOCA GRAFICAMENTE O QUADRADO ABERTO
                al_flip_display();
                Matriz[lin][col].Aberto = true;                                  //ABRE A CASA
                AbrirCasa(lin + 1, col, MouseX + TAM_FIG, MouseY);               //ABRE CASAS AO ARREDOR
                AbrirCasa(lin + 1, col + 1, MouseX + TAM_FIG, MouseY + TAM_FIG); //ABRE CASAS AO ARREDOR
                AbrirCasa(lin - 1, col, MouseX - TAM_FIG, MouseY);               //ABRE CASAS AO ARREDOR
                AbrirCasa(lin + 1, col - 1, MouseX + TAM_FIG, MouseY - TAM_FIG); //ABRE CASAS AO ARREDOR
                AbrirCasa(lin, col + 1, MouseX, MouseY + TAM_FIG);               //ABRE CASAS AO ARREDOR
                AbrirCasa(lin - 1, col + 1, MouseX - TAM_FIG, MouseY + TAM_FIG); //ABRE CASAS AO ARREDOR
                AbrirCasa(lin, col - 1, MouseX, MouseY - TAM_FIG);               //ABRE CASAS AO ARREDOR
                AbrirCasa(lin - 1, col - 1, MouseX - TAM_FIG, MouseY - TAM_FIG); //ABRE CASAS AO ARREDOR

                break;

            case 1:
                al_draw_bitmap(img_um, (MouseX / TAM_FIG) * TAM_FIG, (MouseY / TAM_FIG) * TAM_FIG, 0); // COLOCA A IMAGEM 1
                al_flip_display();
                Matriz[lin][col].Aberto = true; // ABRE A CASA
                break;

            case 2:
                al_draw_bitmap(img_dois, (MouseX / TAM_FIG) * TAM_FIG, (MouseY / TAM_FIG) * TAM_FIG, 0); //COLOCA IMAGEM 2
                al_flip_display();
                Matriz[lin][col].Aberto = true; // ABRE A CASA
                break;

            case 3:
                al_draw_bitmap(img_tres, (MouseX / TAM_FIG) * TAM_FIG, (MouseY / TAM_FIG) * TAM_FIG, 0); //COLOCA IMAGEM 3
                al_flip_display();
                Matriz[lin][col].Aberto = true; //ABRE A CASA
                break;

            case 4:
                al_draw_bitmap(img_quatro, (MouseX / TAM_FIG) * TAM_FIG, (MouseY / TAM_FIG) * TAM_FIG, 0); //COLOCA IMAGEM 4
                al_flip_display();
                Matriz[lin][col].Aberto = true; //ABRE A CASA
                break;

            case 5:
                al_draw_bitmap(img_cinco, (MouseX / TAM_FIG) * TAM_FIG, (MouseY / TAM_FIG) * TAM_FIG, 0); //COLOCA IMAGEM 5
                al_flip_display();
                Matriz[lin][col].Aberto = true; //ABRE A CASA
                break;

            case 6:
                al_draw_bitmap(img_seis, (MouseX / TAM_FIG) * TAM_FIG, (MouseY / TAM_FIG) * TAM_FIG, 0); //COLOCA IMAGEM 6
                al_flip_display();
                Matriz[lin][col].Aberto = true; // ABRE A CASA
                break;

            case 7:
                al_draw_bitmap(img_sete, (MouseX / TAM_FIG) * TAM_FIG, (MouseY / TAM_FIG) * TAM_FIG, 0); //COLOCA IMAGEM 7
                al_flip_display();
                Matriz[lin][col].Aberto = true; // ABRE A CASA
                break;

            case 8:
                al_draw_bitmap(img_oito, (MouseX / TAM_FIG) * TAM_FIG, (MouseY / TAM_FIG) * TAM_FIG, 0); //COLOCA IMAGEM 8
                al_flip_display();
                Matriz[lin][col].Aberto = true; //ABRE A CASA
                break;
            }
        }
    }
}

void Tabuleiro ::ColocaBandeira(int lin, int col) //FUN��O PARA COLOCAR UMA BANDEIRA
{
    if (Matriz[lin][col].Aberto == false) //CASA FECHADA
        Matriz[lin][col].Duvida = true;   //COLOCA BANDEIRA
}

void Tabuleiro ::RetiraBandeira(int lin, int col) //FUN��O PARA RETIRAR UMA BANDEIRA
{
    if (Matriz[lin][col].Duvida == true && Matriz[lin][col].Aberto == false) //CASA FECHADA E COM BANDEIRA
        Matriz[lin][col].Duvida = false;                                     //RETIRA BANDEIRA
}

bool Tabuleiro ::VerificaAberto(int lin, int col)
{
    if (Matriz[lin][col].Aberto) //SE TA ABERTO RETORNA TRUE SE NAO, RETORNA FALSE
        return true;
    return false;
}

bool Tabuleiro ::VerificaBandeira(int lin, int col) //SE TEM BANDEIRA RETORNA TRUE, SE NAO, RETORNA FALSE
{
    if (Matriz[lin][col].Duvida)
        return true;
    return false;
}

bool Tabuleiro::VerificaBomba(int lin, int col) //SE TEM BOMBA RETORNA TRUE, SE NAO, RETORNA FALSE
{
    if (Matriz[lin][col].Bomba)
        return true;
    return false;
}

int Tabuleiro::VerificaNum(int lin, int col) // RETORNA O NUMERO DE BOMBAS AO ARREDOR DA CASA
{
    if (Matriz[lin][col].Quant >= 0)
        return Matriz[lin][col].Quant;
    return -1;
}

int Tabuleiro ::QuantAberto() //RETORNA A QUANTIDADE DE CASAS ABERTAS
{
    int cont = 0;
    for (int i = 0; i < Linhas; i++)
        for (int j = 0; j < Colunas; j++)
            if (Matriz[i][j].Aberto)
                cont++;
    return cont;
}
