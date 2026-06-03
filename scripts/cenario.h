#ifndef CENARIO_H
#define CENARIO_H

#include "utils.h"

/**
 * @brief Desenha um quadrado com gradiente para escurecer gradativamente o fundo do mar.
 * * @param y_inicial Ponto Y onde o gradiente escuro começa.
 * @param y_final Ponto Y onde o gradiente escuro atinge sua opacidade máxima.
 * @param largura A largura total do retângulo a ser desenhado.
 * @param R Componente vermelha da cor (0.0 a 1.0).
 * @param G Componente verde da cor (0.0 a 1.0).
 * @param B Componente azul da cor (0.0 a 1.0).
 */
void desenhar_quadrado_escuro(float y_inicial, float y_final, float largura, double R, double G, double B);

/**
 * @brief Calcula a posição Y e o ângulo de inclinação do barco simulando o balanço do mar.
 * * @param base_y Altura base em que o barco flutua.
 * @param tempo O relógio contínuo da simulação.
 * @param amp_y Amplitude da oscilação vertical (subida e descida).
 * @param amp_rotacao Amplitude do ângulo de inclinação do barco.
 * @param velocidade Fator multiplicador da velocidade da onda.
 * @param out_y Ponteiro para armazenar a nova posição Y calculada.
 * @param out_angulo Ponteiro para armazenar o novo ângulo calculado.
 */
void calcular_oscilacao_barco(float base_y, float tempo, float amp_y, float amp_rotacao, float velocidade, float *out_y, float *out_angulo);

void resetar_camera(Camera *camera);

#endif