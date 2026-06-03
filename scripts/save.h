#ifndef SAVE_H
#define SAVE_H

#include "menus.h"
#include "peixe.h"

#define CAMINHO_SAVE "save.bin"
#define SAVE_VERSAO 2

typedef struct {
    int versao;

    int dinheiro;
    int nivel_atracao;
    int nivel_forca;
    int nivel_linha;
    short isca_selecionada;
    bool isca_comprada[TOTAL_TIPOS_ISCAS];
    bool peixe_descoberto[TOTAL_TIPOS_PEIXES];
    int quantidade_capturada[TOTAL_TIPOS_PEIXES];
    float maior_tamanho[TOTAL_TIPOS_PEIXES];
    int melhor_venda[TOTAL_TIPOS_PEIXES];
    short ordem_diario;
    short encomenda_tipo;
    int encomenda_recompensa;

    int quantidade_capturados;
    PeixeCapturado capturados[MAX_PEIXES_CAPTURADOS];
} SaveDados;

bool carregar_save(const char *caminho, Menus *menus, PeixeCapturado capturados[], int capacidade);

bool salvar_jogo(const char *caminho, Menus *menus, PeixeCapturado capturados[], int capacidade);

#endif
