#ifndef ISCA_H
#define ISCA_H

#include "utils.h"

// enum que guarda cada tipo de isca
enum TIPOS_ISCAS {
    VELHA, 
    BOIA,      
    ESVERDEADA,        
    REALISTA,           
    PEQUENO_PEIXE,
    TOTAL_TIPOS_ISCAS
};

// estrutura que guarda os dados de uma isca
typedef struct {
    Ponto posicao;
    float velocidade;
    float tamanho;
    float angulo;
    float vel_angulo;
    float upgrade_atracao;
    float forca_barco;        // O quão rápido a tecla espaço puxa o peixe
    float comprimento_linha;   // O limite máximo da linha em pixels
    float tensao_linha;       // Uma barra invisível de 0 a 100 sendo 100 ela quebra
    short tipo;
    bool esta_com_peixe;
} Isca;

/**
 * @brief Inicializa as propriedades padrão de uma isca recém-criada.
 * * @param isca Ponteiro para a estrutura da isca a ser inicializada.
 */
void inicializar_isca(Isca *isca);

/**
 * @brief Renderiza a isca na tela utilizando seu sprite e propriedades atuais.
 * * @param isca Ponteiro para a estrutura da isca a ser desenhada.
 * @param spr_iscas Array de ponteiros contendo os bitmaps (sprites) das iscas carregadas.
 */
void desenhar_isca(Isca *isca, ALLEGRO_BITMAP* spr_iscas[]);

/**
 * @brief Atualiza a posição da isca baseada na gravidade, velocidade e controles do jogador.
 * @param isca Ponteiro para a estrutura da isca.
 * @param virar_esquerda Booleano indicando se o jogador está segurando a seta para a esquerda.
 * @param virar_direita Booleano indicando se o jogador está segurando a seta para a direita.
 * @param cima Booleano indicando se o jogador está segurando a seta para a cima ou o espaco.
 */
void atualizar_isca(Isca *isca, bool virar_esquerda, bool virar_direita, bool cima);

/**
 * @brief Desenha a linha de pesca saindo da vara no barco até o engate da isca.
 * @param barco_x Posição X central do barco.
 * @param barco_y Posição Y central do barco.
 * @param barco_angulo O ângulo atual de inclinação do barco.
 * @param isca_x Posição X atual da isca.
 * @param isca_y Posição Y atual da isca.
 * @param isca_angulo O ângulo visual de rotação da isca.
 * @param isca_tamanho A escala (zoom) aplicada no sprite da isca.
 */
void desenhar_linha_pesca(float barco_x, float barco_y, float barco_angulo, Isca *isca);

void desenhar_barra_tensao(Isca *isca);


#endif
