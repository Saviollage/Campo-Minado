/*
ALUNOS:
SAVIO HENRIQUE M. LAGE    2016003268
BRUNO RODRIGUES PINALI    2016007006
BRENNER BATISTA ROCHA     2016008193
MAURICIO MARQUES          2016013451

*/
#include <iostream>
#include <windows.h>
#include <cstdlib>
#include <cstdio>
#include "Campo.cpp"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

using namespace std;

int main()
{

    int Linhas = 0, Colunas = 0, Quant_Bombas = 0;

    if (!al_init()) //INICIALIZAR O ALLEGRO
    {
        fprintf(stderr, "Falha ao inicializar a Allegro.\n");
        return -1;
    }

    if (!al_init_image_addon()) // INICIALIZAR A EXTENSAO DE IMAGEM
    {
        fprintf(stderr, "Falha ao inicializar add-on allegro_image.\n");
        return -1;
    }
    const int LARGURA_MENU = 720; //DEFINE A LARGURA DA JANELA DE MENU
    const int ALTURA_MENU = 480;  //DEFINE A ALTURA DA JANELA DE MENU

    ALLEGRO_DISPLAY *Menu = al_create_display(LARGURA_MENU, ALTURA_MENU);

    if (!Menu) // VERIFICA FUN��O DE CRIAR O MENU
    {
        fprintf(stderr, "Falha ao criar Menu.\n");
        return -1;
    }

    al_set_window_title(Menu, "CAMPO MINADO"); //DEFINE O TITULO DA JANELA

    ALLEGRO_BITMAP *Img_Menu = al_load_bitmap("Images/Jogar.jpg"); //DEFINE MENU COMO PLANO DE FUNDO

    if (!Img_Menu) //VERIFICA A IMAGEM DO MENU
    {
        fprintf(stderr, "Falha ao carregar o arquivo de imagem do menu.\n");
        al_destroy_display(Menu);
        return -1;
    }

    if (!al_install_mouse()) // VERIFICA FUNCAO PARA USO DO MOUSE
    {
        fprintf(stderr, "Falha ao inicializar o mouse.\n");
        return -1;
    }

    if (!al_set_system_mouse_cursor(Menu, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT)) //ATRIBUI CURSOR PADRAO DO SISTEMA PARA SER USADO
    {
        fprintf(stderr, "Falha ao atribuir ponteiro do mouse.\n");
        return -1;
    }

    ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
    ALLEGRO_BITMAP *Img_final = NULL;
    ALLEGRO_EVENT_QUEUE *fila_eventos2 = NULL; //CRIA UMA FILA DE EVENTOS
    fila_eventos2 = al_create_event_queue();

    if (!fila_eventos2) //VERIFICA FILA DE EVENTOS
    {
        fprintf(stderr, "Falha ao criar fila de eventos.\n");
        al_destroy_display(Menu);
        return -1;
    }

    al_register_event_source(fila_eventos2, al_get_display_event_source(Menu));

    al_register_event_source(fila_eventos2, al_get_mouse_event_source());

    al_draw_bitmap(Img_Menu, 0, 0, 0);
    al_flip_display();

    while (1)
    {
        ALLEGRO_EVENT evento;
        al_wait_for_event(fila_eventos2, &evento);

        if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) //PARA FECHAR A JANELA CASO CLIQUE NO X
        {
            exit(0);
        }

        if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) // VERIFICA CLIQUE COM O BOTAO ESQUERDO
        {
            if (evento.mouse.y > 160 && evento.mouse.y < 240) // NIVEL FACIL (COORDENADAS Y DA IMAGEM)
            {
                Linhas = 9;
                Colunas = 9;
                Quant_Bombas = 10;
                break;
            }
            else if (evento.mouse.y > 280 && evento.mouse.y < 340) // NIVEL MEDIO (COORDENADAS Y DA IMAGEM)
            {
                Linhas = 16;
                Colunas = 16;
                Quant_Bombas = 40;
                break;
            }
            else if (evento.mouse.y > 400 && evento.mouse.y < 470) // NIVEL DIFICIL (COORDENADAS Y DA IMAGEM)
            {
                Linhas = 16;
                Colunas = 16;
                Quant_Bombas = 80;
                break;
            }
        }
    }
    al_destroy_display(Menu); //FECHA O MENU
    srand((unsigned)time(0)); //PARA FUN��O RAND DAS BOMBAS

    Tabuleiro CM(Linhas, Colunas, Quant_Bombas); //INICIA O TABULEIRO
    CM.CriaMatriz();                             // CRIA A MATRIZ
    CM.ColocaBomba();                            //DISTRIBUI AS BOMBAS
    CM.ContadorBombas();                         //DEFINE O NUMERO DE BOMBAS EM VOLTA DAS CASAS

    const int LARGURA_TELA = Linhas * TAM_FIG; //PARA DEFINIR O TAMANHO DA NOVA JANELA COM O TABULEIRO
    const int ALTURA_TELA = Colunas * TAM_FIG; //PARA DEFINIR O TAMANHO DA NOVA JANELA COM O TABULEIRO

    ALLEGRO_DISPLAY *janela = al_create_display(LARGURA_TELA, ALTURA_TELA); //INICIA A JANELA COM O TABULEIRO
    if (!janela)
    {
        fprintf(stderr, "Falha ao criar janela.\n");
        return -1;
    }
    al_set_window_title(janela, "CAMPO MINADO"); //TITULO DA JANELA

    ALLEGRO_BITMAP *QuadradoFechado = al_load_bitmap("Images/Fechado.jpg");
    if (!QuadradoFechado)
    {
        fprintf(stderr, "Falha ao carregar o arquivo de imagem para o Quadrado Fechado.\n");
        al_destroy_display(janela);
        return -1;
    }
    ALLEGRO_BITMAP *QuadradoAberto = al_load_bitmap("Images/Aberto.jpg");
    if (!QuadradoAberto)
    {
        fprintf(stderr, "Falha ao carregar o arquivo de imagem para o Quadrado Aberto.\n");
        al_destroy_display(janela);
        return -1;
    }
    ALLEGRO_BITMAP *Bandeira_Img = al_load_bitmap("Images/Bandeira.jpg");
    if (!Bandeira_Img)
    {
        fprintf(stderr, "Falha ao carregar o arquivo de imagem para a Bandeira.\n");
        al_destroy_display(janela);
        return -1;
    }

    ALLEGRO_BITMAP *img_um = al_load_bitmap("Images/1.png");
    if (!img_um)
    {
        fprintf(stderr, "Falha ao carregar o arquivo de imagem para numero 1.\n");
        al_destroy_display(janela);
        return -1;
    }

    ALLEGRO_BITMAP *img_dois = al_load_bitmap("Images/2.png");
    if (!img_dois)
    {
        fprintf(stderr, "Falha ao carregar o arquivo de imagem para numero 2.\n");
        al_destroy_display(janela);
        return -1;
    }

    ALLEGRO_BITMAP *img_tres = al_load_bitmap("Images/3.png");
    if (!img_tres)
    {
        fprintf(stderr, "Falha ao carregar o arquivo de imagem para numero 3.\n");
        al_destroy_display(janela);
        return -1;
    }

    ALLEGRO_BITMAP *img_quatro = al_load_bitmap("Images/4.png");
    if (!img_quatro)
    {
        fprintf(stderr, "Falha ao carregar o arquivo de imagem para numero 4.\n");
        al_destroy_display(janela);
        return -1;
    }

    ALLEGRO_BITMAP *img_cinco = al_load_bitmap("Images/5.png");
    if (!img_cinco)
    {
        fprintf(stderr, "Falha ao carregar o arquivo de imagem para numero 5.\n");
        al_destroy_display(janela);
        return -1;
    }

    ALLEGRO_BITMAP *img_seis = al_load_bitmap("Images/6.png");
    if (!img_seis)
    {
        fprintf(stderr, "Falha ao carregar o arquivo de imagem para numero 6.\n");
        al_destroy_display(janela);
        return -1;
    }

    ALLEGRO_BITMAP *img_sete = al_load_bitmap("Images/7.png");
    if (!img_sete)
    {
        fprintf(stderr, "Falha ao carregar o arquivo de imagem para numero 7.\n");
        al_destroy_display(janela);
        return -1;
    }

    ALLEGRO_BITMAP *img_oito = al_load_bitmap("Images/8.png");
    if (!img_oito)
    {
        fprintf(stderr, "Falha ao carregar o arquivo de imagem para numero 8.\n");
        al_destroy_display(janela);
        return -1;
    }

    ALLEGRO_BITMAP *img_bomba = al_load_bitmap("Images/Bomba.jpg");
    if (!img_bomba)
    {
        fprintf(stderr, "Falha ao carregar o arquivo de imagem para Bomba.\n");
        al_destroy_display(janela);
        return -1;
    }

    fila_eventos = al_create_event_queue();
    if (!fila_eventos)
    {
        fprintf(stderr, "Falha ao criar fila de eventos.\n");
        al_destroy_display(janela);
        return -1;
    }

    al_register_event_source(fila_eventos, al_get_display_event_source(janela));

    for (int i = 0; i < LARGURA_TELA; i += TAM_FIG)    //PARA PREENCHER A NOVA JANELA COM AS CASAS FECHADAS
        for (int j = 0; j < ALTURA_TELA; j += TAM_FIG) //PARA PREENCHER A NOVA JANELA COM AS CASAS FECHADAS
            al_draw_bitmap(QuadradoFechado, i, j, 0);  // DESENHA AS CASAS FECHADAS

    al_flip_display();
    al_register_event_source(fila_eventos, al_get_mouse_event_source());

    int Quant_Bandeiras = 0;
    int Game_Over = 0; // GAMEOVER = 0 -> JOGO CONTINUA
    // GAMEOVER = 1 -> DERROTA
    // GAMEOVER = 2 -> VITORIA

    while (1)
    {
        ALLEGRO_EVENT evento;
        al_wait_for_event(fila_eventos, &evento);

        if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) //PARA FECHAR JANELA CASO CLIQUE NO X
        {
            break;
        }
        if (Game_Over == 0) // JOGO CONTINUA
        {

            if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) //SE TIVE UM CLIQUE
            {
                int i = (evento.mouse.x / TAM_FIG) % Linhas;  //i RECEBE A COORDENADA X DO CLIQUE E DEFINE A CASA NESSA COORDENADA
                int j = (evento.mouse.y / TAM_FIG) % Colunas; //j RECEBE A COORDENADA Y DO CLIQUE E DEFINE A CASA NESSA COORDENADA

                if (evento.mouse.button & 1) //CLIQUE COM O BOTAO ESQUERDO
                {
                    if (!CM.VerificaAberto(i, j)) //VERIFICA SE A CASA EST� FECHADA
                    {
                        if (CM.VerificaBomba(i, j)) //VERIFICA SE A CASA EM QUESTAO TEM BOMBA
                        {
                            al_draw_bitmap(img_bomba, (evento.mouse.x / TAM_FIG) * TAM_FIG, (evento.mouse.y / TAM_FIG) * TAM_FIG, 0); //DESENHA A BOMBA
                            al_flip_display();
                            CM.AbrirCasa(i, j, evento.mouse.x, evento.mouse.y);

                            for (int k = 0; k < Linhas; k++)
                            {
                                for (int l = 0; l < Colunas; l++)
                                {
                                    if (CM.VerificaBomba(k, l)) //PERCORRE A MATRIZ E ABRE TODAS AS BOMBAS PARA MOSTRAR
                                    {

                                        al_draw_bitmap(img_bomba, k * TAM_FIG, l * TAM_FIG, 0);
                                        al_flip_display();
                                    }
                                }
                            }
                            Game_Over = 1; //PERDE O JOGO
                            al_rest(1.0);  //ESPERA 1 SEGUNDO, PARA O USUARIO VER ONDE ESTAO TODAS AS BOMBAS
                            Img_final = al_load_bitmap("Images/Perdeu.png");
                            al_draw_bitmap(Img_final, 0, 0, 0); //MOSTRA IMAGEM DE GAME OVER
                            al_flip_display();
                            al_rest(2.0); //ESPERA DOIS SEGUNDOS

                            exit(0);
                        }
                        else //SE A CASA NAO TIVER BOMBA ABRE A QUANTIDADE DE BOMBAS AO ARREDOR DA CASA
                        {
                            switch (CM.VerificaNum(i, j))
                            {
                            case 0:
                                CM.AbrirCasa(i, j, evento.mouse.x, evento.mouse.y); //ABRE A CASA
                                break;

                            case 1:
                                al_draw_bitmap(img_um, (evento.mouse.x / TAM_FIG) * TAM_FIG, (evento.mouse.y / TAM_FIG) * TAM_FIG, 0);
                                al_flip_display();
                                CM.AbrirCasa(i, j, evento.mouse.x, evento.mouse.y);
                                break;

                            case 2:
                                al_draw_bitmap(img_dois, (evento.mouse.x / TAM_FIG) * TAM_FIG, (evento.mouse.y / TAM_FIG) * TAM_FIG, 0);
                                al_flip_display();
                                CM.AbrirCasa(i, j, evento.mouse.x, evento.mouse.y);
                                break;

                            case 3:
                                al_draw_bitmap(img_tres, (evento.mouse.x / TAM_FIG) * TAM_FIG, (evento.mouse.y / TAM_FIG) * TAM_FIG, 0);
                                al_flip_display();
                                CM.AbrirCasa(i, j, evento.mouse.x, evento.mouse.y);
                                break;

                            case 4:
                                al_draw_bitmap(img_quatro, (evento.mouse.x / TAM_FIG) * TAM_FIG, (evento.mouse.y / TAM_FIG) * TAM_FIG, 0);
                                al_flip_display();
                                CM.AbrirCasa(i, j, evento.mouse.x, evento.mouse.y);
                                break;

                            case 5:
                                al_draw_bitmap(img_cinco, (evento.mouse.x / TAM_FIG) * TAM_FIG, (evento.mouse.y / TAM_FIG) * TAM_FIG, 0);
                                al_flip_display();
                                CM.AbrirCasa(i, j, evento.mouse.x, evento.mouse.y);
                                break;

                            case 6:
                                al_draw_bitmap(img_seis, (evento.mouse.x / TAM_FIG) * TAM_FIG, (evento.mouse.y / TAM_FIG) * TAM_FIG, 0);
                                al_flip_display();
                                CM.AbrirCasa(i, j, evento.mouse.x, evento.mouse.y);
                                break;

                            case 7:
                                al_draw_bitmap(img_sete, (evento.mouse.x / TAM_FIG) * TAM_FIG, (evento.mouse.y / TAM_FIG) * TAM_FIG, 0);
                                al_flip_display();
                                CM.AbrirCasa(i, j, evento.mouse.x, evento.mouse.y);
                                break;

                            case 8:
                                al_draw_bitmap(img_oito, (evento.mouse.x / TAM_FIG) * TAM_FIG, (evento.mouse.y / TAM_FIG) * TAM_FIG, 0);
                                al_flip_display();
                                CM.AbrirCasa(i, j, evento.mouse.x, evento.mouse.y);
                                break;
                            }
                        }
                    }
                }
                else if (evento.mouse.button & 2) //SE O CLIQUE FOI COM  O BOT�O DIREITO
                {
                    if (!CM.VerificaAberto(i, j)) //VERIFICA SE EST� FECHADO
                    {
                        if (!CM.VerificaBandeira(i, j)) //VERIFICA SE NAO TEM BANDEIRA
                        {
                            al_draw_bitmap(Bandeira_Img, (evento.mouse.x / TAM_FIG) * TAM_FIG, (evento.mouse.y / TAM_FIG) * TAM_FIG, 0); //DESENHA A BANDEIRA
                            CM.ColocaBandeira(i, j);                                                                                     //COLOCA BANDEIRA
                            Quant_Bandeiras++;                                                                                           //ADICIONA CONTADOR
                        }
                        else //SE TEM BANDEIRA, O CLIQUE COM O BOTAO DIREITO A RETIRA
                        {
                            al_draw_bitmap(QuadradoFechado, (evento.mouse.x / TAM_FIG) * TAM_FIG, (evento.mouse.y / TAM_FIG) * TAM_FIG, 0); //DESENHA O QUADRADO FECHADO
                            CM.RetiraBandeira(i, j);                                                                                        //RETIRA A BANDEIRA
                            Quant_Bandeiras--;                                                                                              //RETIRA CONTADOR
                        }
                    }
                    al_flip_display();
                }

                if (Quant_Bandeiras == Quant_Bombas && CM.QuantAberto() == (Linhas * Colunas) - Quant_Bombas)
                {
                    /*SE A QUANTIDADE DE BANDEIRAS FOR
                    IGUAL AO NUMRO DE BOMAS E A QUANTIDADE DE CASAS
                    ABERTAS FOR IGUAL A QUANTIDADE DE CASAS SEM BOMBA
                    ENTAO O USUARIO GANHOU O JOGO   */

                    Game_Over = 2; //VITORIA
                    al_rest(1.0);  //ESPERA UM SEGUNDO
                    Img_final = al_load_bitmap("Images/Venceu.png");
                    al_draw_bitmap(Img_final, 0, 0, 0); //MOSTRA IMAGEM ED PARABENS
                    al_flip_display();
                    al_rest(2.0); //ESPERA 2 SEGUNDOS
                    exit(0);
                    break;
                }
            }
        }
    }
    al_destroy_display(janela); //FECHA A JANELA
    al_destroy_event_queue(fila_eventos);
    return 0;
}
