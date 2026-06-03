#include "utils.h"

float sortear_float(float min, float max) {
    return min + ((float)rand() / RAND_MAX) * (max - min);
}

float normalizar_dificuldade(short dificuldade) {
    if (dificuldade < 1) dificuldade = 1;
    if (dificuldade > 10) dificuldade = 10;

    /*
        Retorna de 0.0 até 1.0.
        dificuldade 1  -> 0.0
        dificuldade 10 -> 1.0
    */
    return (dificuldade - 1) / 9.0;
}

float limitar_float(float valor, float minimo, float maximo) {
    if (valor < minimo) return minimo;
    if (valor > maximo) return maximo;
    return valor;
}

float normalizar_angulo(float angulo) {
    angulo = fmod(angulo, 2.0 * PI);

    if (angulo > PI) {
        angulo -= 2.0 * PI;
    } else if (angulo < -PI) {
        angulo += 2.0 * PI;
    }

    return angulo;
}

float aproximar_angulo(float atual, float alvo, float velocidade_giro) {
    float diferenca = alvo - atual;
    diferenca = normalizar_angulo(diferenca);

    if (fabs(diferenca) <= velocidade_giro) {
        return alvo;
    }

    if (diferenca > 0) {
        atual += velocidade_giro;
    } else {
        atual -= velocidade_giro;
    }

    return normalizar_angulo(atual);
}