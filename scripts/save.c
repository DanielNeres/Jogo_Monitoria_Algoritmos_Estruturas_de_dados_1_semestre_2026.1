#include "save.h"

static void preencher_save_dados(SaveDados *dados, Menus *menus, PeixeCapturado capturados[], int capacidade);
static bool save_valido(SaveDados *dados);
static void aplicar_save_dados(SaveDados *dados, Menus *menus, PeixeCapturado capturados[], int capacidade);

bool salvar_jogo(const char *caminho, Menus *menus, PeixeCapturado capturados[], int capacidade) {
    SaveDados dados;
    FILE *arquivo;

    if (caminho == NULL || menus == NULL || capturados == NULL) {
        return false;
    }

    preencher_save_dados(&dados, menus, capturados, capacidade);

    arquivo = fopen(caminho, "wb");

    if (arquivo == NULL) {
        return false;
    }

    if (fwrite(&dados, sizeof(SaveDados), 1, arquivo) != 1) {
        fclose(arquivo);
        return false;
    }

    fclose(arquivo);
    return true;
}

bool carregar_save(const char *caminho, Menus *menus, PeixeCapturado capturados[], int capacidade) {
    SaveDados dados;
    FILE *arquivo;

    if (caminho == NULL || menus == NULL || capturados == NULL) {
        return false;
    }

    arquivo = fopen(caminho, "rb");

    if (arquivo == NULL) {
        return salvar_jogo(caminho, menus, capturados, capacidade);
    }

    if (fread(&dados, sizeof(SaveDados), 1, arquivo) != 1) {
        fclose(arquivo);
        return salvar_jogo(caminho, menus, capturados, capacidade);
    }

    fclose(arquivo);

    if (!save_valido(&dados)) {
        return salvar_jogo(caminho, menus, capturados, capacidade);
    }

    aplicar_save_dados(&dados, menus, capturados, capacidade);
    return true;
}

static void preencher_save_dados(SaveDados *dados, Menus *menus, PeixeCapturado capturados[], int capacidade) {
    dados->versao = SAVE_VERSAO;

    dados->dinheiro = menus->dinheiro;
    dados->nivel_atracao = menus->nivel_atracao;
    dados->nivel_forca = menus->nivel_forca;
    dados->nivel_linha = menus->nivel_linha;
    dados->isca_selecionada = menus->isca_selecionada;
    dados->ordem_diario = menus->ordem_diario;
    dados->encomenda_tipo = menus->encomenda_tipo;
    dados->encomenda_recompensa = menus->encomenda_recompensa;

    for (int i = 0; i < TOTAL_TIPOS_ISCAS; i++) {
        dados->isca_comprada[i] = menus->isca_comprada[i];
    }

    for (int i = 0; i < TOTAL_TIPOS_PEIXES; i++) {
        dados->peixe_descoberto[i] = menus->peixe_descoberto[i];
        dados->quantidade_capturada[i] = menus->quantidade_capturada[i];
        dados->maior_tamanho[i] = menus->maior_tamanho[i];
        dados->melhor_venda[i] = menus->melhor_venda[i];
    }

    dados->quantidade_capturados = capacidade;

    if (dados->quantidade_capturados > MAX_PEIXES_CAPTURADOS) {
        dados->quantidade_capturados = MAX_PEIXES_CAPTURADOS;
    }

    for (int i = 0; i < MAX_PEIXES_CAPTURADOS; i++) {
        if (i < dados->quantidade_capturados) {
            dados->capturados[i] = capturados[i];
        } else {
            dados->capturados[i].tipo = -1;
            dados->capturados[i].tamanho = 1.0;
            dados->capturados[i].preco_venda = 0;
            dados->capturados[i].ocupado = false;
        }
    }
}

static bool save_valido(SaveDados *dados) {
    return dados->versao == SAVE_VERSAO;
}

static void aplicar_save_dados(SaveDados *dados, Menus *menus, PeixeCapturado capturados[], int capacidade) {
    menus->dinheiro = dados->dinheiro;
    menus->nivel_atracao = limitar_float(dados->nivel_atracao, 0, NIVEL_UPGRADE_MAX);
    menus->nivel_forca = limitar_float(dados->nivel_forca, 0, NIVEL_UPGRADE_MAX);
    menus->nivel_linha = limitar_float(dados->nivel_linha, 0, NIVEL_UPGRADE_MAX);

    for (int i = 0; i < TOTAL_TIPOS_ISCAS; i++) {
        menus->isca_comprada[i] = dados->isca_comprada[i];
    }

    for (int i = 0; i < TOTAL_TIPOS_PEIXES; i++) {
        menus->peixe_descoberto[i] = dados->peixe_descoberto[i];
        menus->quantidade_capturada[i] = dados->quantidade_capturada[i];
        menus->maior_tamanho[i] = dados->maior_tamanho[i];
        menus->melhor_venda[i] = dados->melhor_venda[i];
    }

    menus->ordem_diario = dados->ordem_diario;
    if (menus->ordem_diario < ORDEM_DIARIO_NOME || menus->ordem_diario > ORDEM_DIARIO_QUANTIDADE) {
        menus->ordem_diario = ORDEM_DIARIO_NOME;
    }

    menus->encomenda_tipo = dados->encomenda_tipo;
    menus->encomenda_recompensa = dados->encomenda_recompensa;
    if (menus->encomenda_tipo < 0 || menus->encomenda_tipo >= TOTAL_TIPOS_PEIXES) {
        menus->encomenda_tipo = PERCA_AMARELA;
        menus->encomenda_recompensa = 90;
    }

    menus->isca_comprada[VELHA] = true;

    if (dados->isca_selecionada >= 0 && dados->isca_selecionada < TOTAL_TIPOS_ISCAS && menus->isca_comprada[dados->isca_selecionada]) {
        menus->isca_selecionada = dados->isca_selecionada;
    } else {
        menus->isca_selecionada = VELHA;
    }

    int limite = dados->quantidade_capturados;

    if (limite > capacidade) {
        limite = capacidade;
    }

    if (limite > MAX_PEIXES_CAPTURADOS) {
        limite = MAX_PEIXES_CAPTURADOS;
    }

    for (int i = 0; i < capacidade; i++) {
        if (i < limite) {
            capturados[i] = dados->capturados[i];
        } else {
            capturados[i].tipo = -1;
            capturados[i].tamanho = 1.0;
            capturados[i].preco_venda = 0;
            capturados[i].ocupado = false;
        }
    }
}
