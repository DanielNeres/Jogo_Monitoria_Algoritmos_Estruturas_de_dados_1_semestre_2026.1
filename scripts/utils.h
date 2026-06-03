#ifndef UTILS_H
#define UTILS_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>


#define TELA_W 2400
#define TELA_H 1450
#define FPS 60.0
#define PI 3.14


// Estrutura que descreve um ponto
typedef struct{
    float x, y;
} Ponto;

// Estrutura que descreve uma camera
typedef struct {
    Ponto atual;      
    Ponto alvo;       
    float zoom_atual; 
    float zoom_alvo;  
    float vel_suavizacao; 
} Camera;

enum TECLAS { ESQUERDA, DIREITA, CIMA, TOTAL_TECLAS };

/**
 * @brief Sorteia um número de ponto flutuante dentro de um intervalo.
 * * @param min O valor mínimo do intervalo.
 * @param max O valor máximo do intervalo.
 * @return O número float sorteado.
 */
float sortear_float(float min, float max);

float limitar_float(float valor, float minimo, float maximo);

float normalizar_angulo(float angulo);

float aproximar_angulo(float atual, float alvo, float velocidade_giro);

float normalizar_dificuldade(short dificuldade);

#endif