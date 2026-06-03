#ifndef MENUS_H
#define MENUS_H

#include "utils.h"
#include "peixe.h"
#include <allegro5/allegro_font.h>

#define DIFICULDADE_MIN 1
#define DIFICULDADE_MAX 10
#define NIVEL_UPGRADE_MAX 5

enum ICONES_MENU {
    ICONE_ENGRENAGEM,
    ICONE_LOJA,
    ICONE_LIVRO_FECHADO,
    ICONE_LIVRO_ABERTO,
    TOTAL_ICONES_MENU
};

enum ORDEM_DIARIO {
    ORDEM_DIARIO_NOME,
    ORDEM_DIARIO_PRECO,
    ORDEM_DIARIO_QUANTIDADE
};

typedef struct {
    bool aberto;
    bool loja_aberta;
    bool diario_aberto;

    int dinheiro;
    int nivel_atracao;
    int nivel_forca;
    int nivel_linha;
    bool isca_comprada[TOTAL_TIPOS_ISCAS];
    short isca_selecionada;
    bool peixe_descoberto[TOTAL_TIPOS_PEIXES];
    int quantidade_capturada[TOTAL_TIPOS_PEIXES];
    float maior_tamanho[TOTAL_TIPOS_PEIXES];
    int melhor_venda[TOTAL_TIPOS_PEIXES];
    short ordem_diario;
    short encomenda_tipo;
    int encomenda_recompensa;
    float mouse_x;
    float mouse_y;

    float icone_x;
    float icone_y;
    float icone_tamanho;

    float icone_loja_x;
    float icone_loja_y;
    float icone_loja_tamanho;

    float icone_livro_x;
    float icone_livro_y;
    float icone_livro_tamanho;

    float painel_x1;
    float painel_y1;
    float painel_x2;
    float painel_y2;

    float fechar_x1;
    float fechar_y1;
    float fechar_x2;
    float fechar_y2;

    float botao_menos_x1;
    float botao_menos_y1;
    float botao_menos_x2;
    float botao_menos_y2;

    float botao_mais_x1;
    float botao_mais_y1;
    float botao_mais_x2;
    float botao_mais_y2;

    float botao_vender_x1;
    float botao_vender_y1;
    float botao_vender_x2;
    float botao_vender_y2;

    float botao_upgrade_atracao_x1;
    float botao_upgrade_atracao_y1;
    float botao_upgrade_atracao_x2;
    float botao_upgrade_atracao_y2;

    float botao_upgrade_forca_x1;
    float botao_upgrade_forca_y1;
    float botao_upgrade_forca_x2;
    float botao_upgrade_forca_y2;

    float botao_upgrade_linha_x1;
    float botao_upgrade_linha_y1;
    float botao_upgrade_linha_x2;
    float botao_upgrade_linha_y2;

    float botao_isca_x1[TOTAL_TIPOS_ISCAS];
    float botao_isca_y1[TOTAL_TIPOS_ISCAS];
    float botao_isca_x2[TOTAL_TIPOS_ISCAS];
    float botao_isca_y2[TOTAL_TIPOS_ISCAS];

    float botao_entregar_x1;
    float botao_entregar_y1;
    float botao_entregar_x2;
    float botao_entregar_y2;

    float botao_ordem_nome_x1;
    float botao_ordem_nome_y1;
    float botao_ordem_nome_x2;
    float botao_ordem_nome_y2;

    float botao_ordem_preco_x1;
    float botao_ordem_preco_y1;
    float botao_ordem_preco_x2;
    float botao_ordem_preco_y2;

    float botao_ordem_quantidade_x1;
    float botao_ordem_quantidade_y1;
    float botao_ordem_quantidade_x2;
    float botao_ordem_quantidade_y2;

} Menus;

void inicializar_menus(Menus *menu);

void desenhar_icone_menu(Menus *menu, ALLEGRO_BITMAP *spr_icones_menu[]);

void desenhar_menus(Menus *menu, ALLEGRO_FONT *font, short dificuldade);

void desenhar_painel_loja(Menus *menu, ALLEGRO_FONT *font, PeixeCapturado capturados[], int capacidade);

void desenhar_painel_diario(Menus *menu, ALLEGRO_FONT *font);

bool mouse_em_retangulo(float mouse_x, float mouse_y, float x1, float y1, float x2, float y2);

bool mouse_sobre_icone_menu(Menus *menu, float mouse_x, float mouse_y);

bool mouse_sobre_icone_loja(Menus *menu, float mouse_x, float mouse_y);

bool mouse_sobre_icone_livro(Menus *menu, float mouse_x, float mouse_y);

bool mouse_sobre_x_menu(Menus *menu, float mouse_x, float mouse_y);

void processar_clique_menus(Menus *menu, float mouse_x, float mouse_y, short *dificuldade);

void processar_clique_painel_loja(Menus *menu, float mouse_x, float mouse_y, PeixeCapturado capturados[], int capacidade);

void processar_clique_painel_diario(Menus *menu, float mouse_x, float mouse_y);

void aplicar_upgrades_loja_na_isca(Menus *menu, Isca *isca);

int calcular_valor_peixes_capturados(PeixeCapturado capturados[], int capacidade);

void atualizar_mouse_menus(Menus *menu, float mouse_x, float mouse_y);

void registrar_peixe_no_diario(Menus *menu, Peixe *peixe);

#endif
