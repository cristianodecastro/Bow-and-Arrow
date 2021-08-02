// INCLUSÃO DE BIBLIOTECAS
#include <stdio.h>
#include <math.h>
#include <conio.h>
#include <windows.h>
#include <locale.h>
#include <time.h>

// VALORES CONSTANTES
#define LIN 35
#define COL 80
#define N_FLECHAS_NIVEL1 15
#define N_FLECHAS_NIVEL2 30
#define N_MONSTROS 30
#define X_INICIAL_ARQUEIRO 2
#define X_INICIAL_BALOES 17
#define X_INICIAL_MONSTROS 73
#define Y_INICIAL_ARQUEIRO 15
#define Y_INICIAL_BALOES 31
#define VEL_BALOES 930
#define VEL_FLECHAS 988
#define VEL_MONSTROS 985
#define VEL_SURG_MONSTROS 500
#define NUMDEPTS 5

// LISTA DE FUNÇÕES
void gotoxy(int x, int y);
char comando();
void cria_caixa(int y);
void cria_menu();
void selecao();
void maiores_placares();
int novo_recorde(int pt, int flechas_usadas);
void contagem_reg();
void limpa_numero();
void cabecalho(int nivel, int maior);
void cria_arqueiro(int y);
int movimenta_arqueiro(int y, char c);
void cria_baloes(int y, int mapa[]);
int movimenta_baloes(int y, int mapa[]);
void cria_flecha(int x, int y);
void quebra_flecha(int x, int y);
int movimenta_flecha(int x, int y);
void cria_monstro(int x, int y);
void morte_monstro(int x, int y);
int movimenta_monstro(int x, int y);
int colisao_flecha_monstro(int xf, int yf, int xm, int ym);
int colisao_arqueiro_monstro(int xa, int ya, int xm, int ym);
void primeiro_nivel();
void segundo_nivel(int pt);
int pontuacao_flechas(int flechas, int nivel);
void jogo_ganho(int pt, int maior, int flechas_usadas);
void jogo_perdido(int pt, int maior, int flechas_usadas, int nivel);

// ESTRUTURAS

typedef struct elemento // Struct para armazenar as coordenadas das flechas e monstro
{
    int x;
    int y;
} ELEM;

typedef struct jogador // Struct para armazenar os dados dos melhores jogadores
{
    char nome[40];
    int  escore;
} JOGADOR;

// FUNÇÕES

void gotoxy(int x, int y) // função responsável por posicionar o cursor em uma determinada coordenada na tela
{
    COORD coord = {0, 0};
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

char comando() // função responsável por retornar o caractere correspondente à tecla apertada pelo usuário
{
    if(kbhit())
        return getch();
}

void cria_caixa(int y) // função responsável por criar as bordas do cenário do jogo. É usada sempre que uma nova tela é formada
{
    int i;
    for(i = 1; i < LIN - 1; i++)
    {
        gotoxy(0, i);
        printf("!");
        gotoxy(79, i);
        printf("!");
    }
    for(i = 1; i < COL - 1; i++)
    {
        gotoxy(i, 0);
        printf("=");
        gotoxy(i, 34);
        printf("=");
    }
    for(i = 1; i < COL - 1; i++)
    {
        gotoxy(i, y); // Y usado para criar a linha de cabeçalho da caixa
        printf("-");
    }
}

void cria_menu() // função que chama a função void cria_caixa(int y), escreve o nome do jogo na tela e chama a função void selecao()
{
    cria_caixa(12); // chamada da função para criar a caixa do jogo
    gotoxy(5, 5);
    printf(" ___   ___   _          _         __    ___   ___   ___   _      ");
    gotoxy(5, 6);
    printf("| |_) / / \\ \\ \\    /   | |\\ |    / /\\  | |_) | |_) / / \\ \\ \\    /");
    gotoxy(5, 7);
    printf("|_|_) \\_\\_/  \\_\\/\\/    |_| \\|   /_/--\\ |_| \\ |_| \\ \\_\\_/  \\_\\/\\/ ");
    selecao(); // chamada da função para criar a seleção de opções do menu
}

void selecao() // função responsável por identificar a escolha do usuário entre começar um novo jogo, visualizar a lista das 5 maiores pontuações já registradas e sair do jogo
{
    int y = 17;
    char c;
    gotoxy(32, 17);
    printf("NOVO JOGO");
    gotoxy(32, 19);
    printf("MAIORES PLACARES");
    gotoxy(32, 21);
    printf("SAIR");
    gotoxy(23, 24);
    printf("SELECIONE A OPÇÃO DESEJADA");
    gotoxy(27, y);
    printf("-->");
    while(c != 13)  // loop que roda até uma opção ser selecionada
    {
        c = comando();
        if((c == 's' || c == 'S') && y < 21)
        {
            gotoxy(27, y);
            printf("   ");
            y = y + 2;
        }
        else if((c == 'w' || c == 'W') && y > 17)
        {
            gotoxy(27, y);
            printf("   ");
            y = y - 2;
        }
        gotoxy(27, y);
        printf("-->");
    }
    system("cls");
    if(y == 17)  // se a posição da coordenada y da seta for 17, é iniciado o primeiro nível do jogo
        primeiro_nivel();

    else if(y == 19)   // ou se o y for 19, iremos para a tela que mostra os maiores placares
        maiores_placares();

    else	// caso nenhuma das duas opções acima seja escolhida, o programa para de executar
    {
        gotoxy(0, 35);
        exit(0);
    }
}

void maiores_placares() // função responsável por exibir a lista das 5 maiores pontuações já registradas juntamente com os nomes dos jogadores que as atingiram
{
    FILE *arq;
    JOGADOR jogador[NUMDEPTS]; // estrutura que armazenam os nomes e as pontuações dos jogadores
    int  i;
    char c;
    cria_caixa(2);
    gotoxy(2, 1);
    printf("MAIORES PLACARES");
    arq = fopen("highscores.bin", "rb"); // abertura do arquivo de melhores placares
    fread(jogador, sizeof(JOGADOR), NUMDEPTS, arq);
    gotoxy(18, 10);
    printf("%-35s %s", "NOME:", "ESCORE:");
    printf("\n");
    for (i = 0; i < NUMDEPTS; i++)  // imprimindo a lista de pontuações após a leitura do arquivo
    {
        gotoxy(18, 12 + (i * 2));
        printf("%-35s %d", jogador[i].nome, jogador[i].escore);
    }
    fclose(arq); // fechando o arquivo
    printf("\n");
    gotoxy(24, 25);
    printf("Pressione ENTER para retornar!"); // espera do comando para retornar ao menu principal
    while(c != 13)
        c = comando();

    system("cls");
    cria_menu();
}

int novo_recorde(int pt, int flechas_usadas) // função responsável por verificar se a nova pontuação se enquadra entre as 5 maiores já registradas e, caso se enquadre,
// solicita ao usuário seu nome e adiciona-o juntamente a sua pontuação à lista. Essa função pode retornar a maior pontuação já registrada
{
    FILE *arq;
    JOGADOR jogador[NUMDEPTS], novo; // estruturas que armazenam os nomes e as pontuações dos jogadores
    char nomedoarq[] = "highscores.bin";
    char c, caso = 0;
    int i, b, aux, a = 0;
    novo.escore = pt;
    arq = fopen(nomedoarq, "rb");
    fread(jogador, sizeof(JOGADOR), NUMDEPTS, arq);
    if (flechas_usadas == 0)
        return jogador[0].escore;
    else
    {
        for (i = 0; i < NUMDEPTS; i++)
        {
            /* verificando se a nova pontuação é maior que a pontuação da iteração atual já presente na estrutura e se
               a quantidade de modificações no arranjo é igual a 0 */
            if ((novo.escore > jogador[i].escore) && (!a))
            {
                for (aux = NUMDEPTS - 2; aux >= i; aux--)  // deslocando a estrutura da iteração atual um índice "para baixo" no arranjo
                {
                    jogador[aux + 1] = jogador[aux];
                }
                b = i; // atribuindo a nova estrutura ao índice da estrutura da iteração atual
                printf("\n");
                caso = 's'; // indicando que a nova pontuação se enquadra entre as 5 maiores
                a++;
            }
        }
        if (caso == 's')  // verificando se a nova pontuação se enquadra entre as 5 maiores
        {
            gotoxy(4, 15);
            printf("VOCÊ ENTROU NA LISTA DAS 5 MAIORES PONTUAÇÕES (VOCÊ ESTÁ NA POSIÇÃO %d)!", b + 1); // Informando ao usuário que a nova pontuação está entre as 5 maiores
            gotoxy(4, 17);
            printf("DIGITE SEU NOME EM ATÉ 30 CARACTERES: ");
            fflush(stdin);
            scanf("%[^\n]s", novo.nome);
            gotoxy(7, 19);
            printf("PARA CONFERIR A LISTA, VÁ ATÉ A OPÇÃO \"MAIORES PLACARES\" NO MENU\n");
            jogador[b] = novo;
            arq = fopen(nomedoarq, "wb"); // escrevendo um arquivo a partir de uma estrutura
            fwrite(jogador, sizeof(JOGADOR), NUMDEPTS, arq);
            gotoxy(22, 25);
            printf("Pressione ENTER para voltar ao menu!");
            while(c != 13)
                c = comando();
        }
        fclose(arq); // fechando o arquivo
    }
}

void contagem_reg() // função responsável por realizar uma contagem regressiva de 5 segundos (exibindo os números na tela) no início de cada um dos níveis
{
    int x = 36, y = 10;
    gotoxy(x, y);
    printf(" _____ ");
    gotoxy(x, y + 1);
    printf("| ____|");
    gotoxy(x, y + 2);
    printf("| |__  ");
    gotoxy(x, y + 3);
    printf("|___ \\");
    gotoxy(x, y + 4);
    printf(" ___) |");
    gotoxy(x, y + 5);
    printf("|____/ ");
    limpa_numero();
    gotoxy(x, y );
    printf(" _  _   ");
    gotoxy(x, y + 1);
    printf("| || |  ");
    gotoxy(x, y + 2);
    printf("| || |_ ");
    gotoxy(x, y + 3);
    printf("|__   _|");
    gotoxy(x, y + 4);
    printf("   | |  ");
    gotoxy(x, y + 5);
    printf("   |_|  ");
    limpa_numero();
    gotoxy(x, y);
    printf(" ____   ");
    gotoxy(x, y + 1);
    printf("|___ \\ ");
    gotoxy(x, y + 2);
    printf("  __) | ");
    gotoxy(x, y + 3);
    printf(" |__ <  ");
    gotoxy(x, y + 4);
    printf(" ___) | ");
    gotoxy(x, y + 5);
    printf("|____/  ");
    limpa_numero();
    gotoxy(x, y);
    printf(" ___  ");
    gotoxy(x, y + 1);
    printf("|__ \\ ");
    gotoxy(x, y + 2);
    printf("   ) |");
    gotoxy(x, y + 3);
    printf("  / / ");
    gotoxy(x, y + 4);
    printf(" / /_ ");
    gotoxy(x, y + 5);
    printf("/____|");
    limpa_numero();
    gotoxy(x, y);
    printf(" ___ ");
    gotoxy(x, y + 1);
    printf("|_  |");
    gotoxy(x, y + 2);
    printf("  | |");
    gotoxy(x, y + 3);
    printf("  | |");
    gotoxy(x, y + 4);
    printf("  | |");
    gotoxy(x, y + 5);
    printf("  |_|");
    limpa_numero();
}

void limpa_numero() // função responsável por substituir os caracteres que formam os números que aparecem na tela durante a execução da função void contagem_reg() por espaços
{
    int i;
    Sleep(1000); // delay de 1 segundo para fazer a contagem regressiva
    for (i = 10; i < 16; i++)
    {
        gotoxy(35, i);
        printf("           ");
    }
}

void cabecalho(int nivel, int maior) // função que chama a função void cria_caixa(int y) e exibe informações pertinentes ao usuário, como sua pontuação atual, a maior
// pontuação registrada até agora no jogo, o nível em que o jogador se encontra e a quantidade de flechas restantes
{
    cria_caixa(3);
    gotoxy(31, 0);
    printf("> BOW AND ARROW <");
    gotoxy(2, 1);
    printf("PONTUAÇÃO: ");
    gotoxy(2, 2);
    printf("MAIOR PONTUAÇÃO: %d", maior);
    gotoxy(36, 1);
    printf("NÍVEL %d", nivel);
    gotoxy(31, 2);
    printf("FLECHAS RESTANTES");
    gotoxy(68, 2);
    if(nivel == 1) // cabeçalho criado para o nível 1, com o número certo de flechas
    {
        printf("--> %d <--", N_FLECHAS_NIVEL1);
    }
    else if(nivel == 2) // cabeçalho criado para o nível 2, com o número certo de flechas
    {
        printf("--> %d <--", N_FLECHAS_NIVEL2);
    }
}

void cria_arqueiro(int y) // função responsável por ler o arquivo de texto que contém o formato do arqueiro e escrevê-lo em uma determinada posição da tela
{
    FILE *arq;
    int i, j, x = X_INICIAL_ARQUEIRO; // coordenada x do arqueiro é sempre a mesma
    char arqueiro[4][9];
    arq = fopen("arqueiro.txt", "r"); // abertura do arquivo do arqueiro
    fread(arqueiro, sizeof(arqueiro), 1, arq);
    gotoxy(x, y); // renderiza um novo arqueiro na tela, de acordo com a coordenada y passada por parâmetro
    for(i = 0; i < 4; i++)
    {
        for (j = 0; j < 8; j++)
        {
            gotoxy(x + j, y + i);
            printf("%c", arqueiro[i][j]);
        }
    }
    fclose(arq); // fechando o arquivo
}

int movimenta_arqueiro(int y, char c) // função que move o arqueiro verticalmente na tela de acordo com a tecla apertada – W ou w para cima, ou S ou s para baixo.
// Essa função retorna o próximo y do arqueiro
{
    int i, x = X_INICIAL_ARQUEIRO;
    if ( (c == 's' || c == 'S' || c == 'w' || c == 'W') && y < 30 && y > 4 ) // em caso de um comando ser acionado, a "imagem do arqueiro" é apagada
    {
        for(i = y; i < (y + 4); i++)
        {
            gotoxy(x, i);
            printf("         ");
        }
    }
    if ( (c == 's' || c == 'S') && y < 30 ) // se o comando for um "s", o y é aumentado, para assim, o arqueiro ser criado em uma posição mais abaixo
    {
        y++;
        cria_arqueiro(y);
    }
    else if( (c == 'w' || c == 'W') && y > 4 ) // se o comando for um "y", o y é diminuido, para assim, o arqueiro ser criado em uma posição mais acima
    {
        y--;
        cria_arqueiro(y);
    }
    if (y == 5)
    {
        gotoxy(x, y - 1);
        printf("         ");
    }
    else if (y == 29)
    {
        gotoxy(x, y + 4);
        printf("         ");
    }
    return y; // retorna o novo Y atualizado para a variável dentro da função do jogo
}

void cria_baloes(int y, int mapa[]) // função responsável por ler o arquivo de texto que contém o formato do balão e escrever a quantidade adequada de balões (que é um
// dos parâmetros da função) em determinadas posições da tela
{
    FILE *arq;
    int i, j, n, x = X_INICIAL_BALOES;
    char balao[3][4];
    arq = fopen("balao.txt", "r"); // abertura do arquivo do balão
    fread(balao, sizeof(balao), 1, arq);
    for(n = 0; n < 15; n++) // loop responsável pela criação dos 15 balões
    {
        if(mapa[n] == 1) // verifica, pela posição dos balões, se um balão já está estourado, para assim, não escreve-lo
            continue;
        for(i = 0; i < 3; i++)
        {
            for(j = 0; j < 3; j++)
            {
                gotoxy(x + (n * 4) + j, y + i);
                printf("%c", balao[i][j]);
            }
        }
    }
    fclose(arq); // fechando o arquivo
}

int movimenta_baloes(int y, int mapa[]) // função responsável por mover verticalmente os balões escritos na tela. Essa função retorna o próximo y dos balões
{
    int i, n, d, x = X_INICIAL_BALOES;
    for(n = 0; n < 15; n++)
    {
        for(i = y; i < (y + 3); i++)
        {
            gotoxy(x + (n * 4), i); // imagem dos balões apagadas para haver a atualização de suas alturas
            printf("   ");
        }
    }
    if(y == 4) // verifica se os balões já chegaram na parte de cima da tela, para então, cria-los novamente embaixo
        y = 31;
    else
        y--;
    cria_baloes(y, mapa);
    return y; // retorna o novo Y atualizado para a variável dentro da função do jogo
}

void cria_flecha(int x, int y) // função responsável por ler o arquivo de texto que contém o formato da flecha e escrevê-lo em uma determinada posição da tela
{
    FILE *arq;
    int i, a;
    char flecha[3];
    arq = fopen("flecha.txt", "r"); // abertura do arquivo da flecha
    fread(flecha, sizeof(flecha), 1, arq);
    for(i = x; i < (x + 3); i++)
    {
        gotoxy(x, y);
        for (a = 0; a < 3; a++)
            printf("%c", flecha[a]);
    }
    fclose(arq); // fechando o arquivo
}

void quebra_flecha(int x, int y) // função responsável por substituir os caracteres que formavam a flecha por espaços
{
    gotoxy(x, y);
    printf("   ");
}

int movimenta_flecha(int x, int y) // função responsável por mover horizontalmente as flechas escritas na tela. Essa função retorna o próximo x da flecha
{
    quebra_flecha(x, y); // chamada da função que apaga a flecha, para assim, ser criada uma nova imagem dela com o X atualizado
    x++;
    if(x < 77) // o X da flecha será atualizado apenas enquanto ela não chegar na borda direita da tela
        cria_flecha(x, y);
    else
        x = 0;
    return x; // retorna o novo X atualizado para a variável dentro da função do jogo
}

void cria_monstro(int x, int y) // função responsável por ler o arquivo de texto que contém o formato do monstro e escrevê-lo em determinada posição da tela
{
    FILE *arq;
    char monstro[5][7];
    int i, j;
    arq = fopen("monstro.txt", "r"); // abertura do arquivo do monstro
    fread(monstro, sizeof(monstro), 1, arq);
    gotoxy(x, y);
    for(i = 0; i < 5; i++)
    {
        for(j = 0; j < 6; j++)
        {
            gotoxy(x + j, y + i);
            printf("%c", monstro[i][j]);
        }
    }
    fclose(arq); // fechando o arquivo
}

void morte_monstro(int x, int y) // função responsável por substituir os caracteres que formam a flecha por espaços
{
    int i;
    for(i = y; i < (y + 5); i++)
    {
        gotoxy(x, i);
        printf("      ");
    }
}

int movimenta_monstro(int x, int y) // função responsável por mover horizontalmente os monstros escritos na tela. Essa função retorna o próximo x do monstro
{
    int i;
    morte_monstro(x, y); // chamada da função que apaga o monstro, para assim, ser criada uma nova imagem dele com o X atualizado
    x--;
    if(x > 0) // o X do monstro será atualizado apenas enquanto ele não chegar na borda esquerda da tela
        cria_monstro(x, y);
    return x; // retorna o novo X atualizado para a variável dentro da função do jogo
}

int colisao_flecha_monstro(int xf, int yf, int xm, int ym) // função responsável por verificar se há uma colisão entre a ponta de uma flecha e alguma parte do corpo de um
// monstro que não seja seu cabelo. Essa função retorna 1 se há uma colisão e 0 se não há
{
    if( (xf + 2 == xm) || (xf + 2 == xm + 1) ) // verificão do contato entre as coordenadas x das flechas e monstros
    {
        if ( (yf == ym + 1) || (yf == ym + 2) || (yf == ym + 3) || (yf == ym + 4) ) // verificão do contato entre as coordenadas y das flechas e monstros
            return 1;
    }
    return 0;
}

int colisao_arqueiro_monstro(int xa, int ya, int xm, int ym) // Função responsável por verificar se há uma colisão entre qualquer parte do corpo do arqueiro e qualquer parte
// do corpo do monstro. Essa função retorna 1 se há uma colisão e 0 se não há
{
    int i;
    if (xm != 0)
    {
        for (i = 0; i < 4; i++)
        {
            if ( (xa + i == xm) || (xa + i == xm + 1) || (xa + i == xm + 2) || (xa + i == xm + 3) || (xa + i == xm + 4) ) // coordenadas x do arqueiro e monstros
            {
                if ( (ya + i == ym) || (ya + i == ym + 1) || (ya + i == ym + 2) || (ya + i == ym + 3) || (ya + i == ym + 4) ) // coordenadas y do arqueiro e monstros
                    return 1;
            }
        }
    }
    return 0;
}

void primeiro_nivel() // função responsável por coordenar todos os elementos do nível 1. Nela, são definidas as velocidades do arqueiro, dos balões e das flechas e são determinados
// os instantes em que esses elementos serão movimentados no cenário. Também nela é verificado constantemente se há colisões entre flechas e balões, é atualizada a pontuação,
// as posições dos balões estourados e o número de flechas restantes
{
    ELEM flechas[N_FLECHAS_NIVEL1]; // vetor que armazena o x e o y de cada uma das flechas individualmente
    char c;
    int i, j, maior, timer = 0, flechas_usadas = 0, pt = 0;
    int y_arq = Y_INICIAL_ARQUEIRO;
    int y_bal = Y_INICIAL_BALOES;
    int mapa_baloes[15];
    int mapa_estourados[15];
    for (i = 0; i < 15; i++)
    {
        mapa_baloes[i] = X_INICIAL_BALOES + (i * 4); // para pegar o x inicial de cada um dos balões. Os y, sempre serão iguais para todos os balões
        mapa_estourados[i] = 0; // os zeros representam que o balão ainda não foi estourado
    }
    maior = novo_recorde(pt, flechas_usadas); // captura do maior placar
    cabecalho(1, maior);
    contagem_reg(); // contagem regressiva para começar a fase
    cria_arqueiro(y_arq); // cria o arqueiro em tela
    cria_baloes(Y_INICIAL_BALOES, mapa_estourados); // cria os balões em tela
    while (timer >= 0) // loop principal do nível 1
    {
        // movimento do arqueiro
        c = comando(); // captura do comando
        y_arq = movimenta_arqueiro(y_arq, c); // atualização da posição do arqueiro, podendo ocorrer a qualquer momento no loop
        // movimento dos balões
        if ( (timer % (1000 - VEL_BALOES) == 0) && (timer != 0) ) // divisão do tempo de execução para os balões
            y_bal = movimenta_baloes(y_bal, mapa_estourados); // atualização da posição dos balões
        // disparo de flechas
        if (c == ' ' && flechas_usadas < N_FLECHAS_NIVEL1) // executado ao apertar o comando e caso ainda existam flechas guardadas
        {
            cria_flecha(11, y_arq + 1);
            flechas[flechas_usadas].x = 11; // definição das coordenadas para cada flecha disparada
            flechas[flechas_usadas].y = y_arq + 1;
            flechas_usadas++; // acréscimo no número de flechas usadas
            gotoxy(72, 2);
            printf("%02d", N_FLECHAS_NIVEL1 - flechas_usadas); // atualização do contador de flechas na tela
        }
        // movimento das flechas
        if ( (timer % (1000 - VEL_FLECHAS) == 0) && (timer != 0) ) // divisão do tempo de execução para as flechas
        {
            for (i = 0; i < flechas_usadas; i++)
            {
                if (flechas[i].x != 0)
                    flechas[i].x = movimenta_flecha(flechas[i].x, flechas[i].y); // x de cada uma das flechas sendo atualizado
                for (j = 0; j < 15; j++)
                {
                    if ( ( (flechas[i].x + 2) == mapa_baloes[j] ) && (flechas[i].y == y_bal || flechas[i].y == y_bal + 1) )
                    {
                        // verifica se o x e o y dos balões e das flechas são os mesmos (colisão)
                        int balao = floor((mapa_baloes[j] - X_INICIAL_BALOES) / 4); // converte o x do balão atingido para o número de ordenamento dele
                        mapa_estourados[balao] = 1; // muda o valor de 0 para 1
                    }
                }
            }
        }
        // cronômetro
        Sleep(1); // delay de 1 milésimo de segundo
        if (timer == 1000) // quando se passar 1 segundo, o contador é zerado
            timer = 0;
        else
            timer++;
        // pontuação
        if (timer % 200 == 0) // definido uma faixa de tempo para a execução para deixa o programa mais rápido
        {
            pt = 0;
            for (i = 0; i < 15; i++)
            {
                if(mapa_estourados[i] == 1) // verifica constantemente o número de balões estourados
                    pt += 100;
            }
            gotoxy(13, 1);
            printf("%d", pt); // atualização em tela da pontuação
        }
        // fim
        if (pt == 1500 || flechas[N_FLECHAS_NIVEL1 - 1].x == 0) // em caso da última flecha ser gasta ou todos os balões serem estourados, o nível 1 acaba
            timer = -1;
    }
    system("cls"); // limpeza da tela
    if (flechas[N_FLECHAS_NIVEL1 - 1].x == 0) // se o jogo acabou por falta de flechas, o jogo foi perdido
        jogo_perdido(pt, maior, flechas_usadas, 1);
    pt += pontuacao_flechas(flechas_usadas, 1); // senão, é somada a pontuação pelas flechas que sobraram
    segundo_nivel(pt); // início do segundo nível
}

void segundo_nivel(int pt) // função responsável por coordenar todos os elementos do nível 2. Nela, são criadas aleatoriamente as posições iniciais dos 30 monstros em y,
// são definidas as velocidades do arqueiro, dos monstros e das flechas e são determinados os instantes em que esses elementos serão movimentados no cenário. Também nela
// é verificado constantemente se há colisões entre flechas e monstros ou entre o arqueiro e um monstro, é atualizada a pontuação, a quantidade de monstros ainda a surgir
// e o número de flechas restantes
{
    ELEM flechas[N_FLECHAS_NIVEL2];
    ELEM monstros[N_MONSTROS];
    int i, j, maior, n_monstros = 0, n_mortos = 0, timer = 0, flechas_usadas = 0;
    int y_arq = Y_INICIAL_ARQUEIRO;
    char c;
    srand(time(0));
    for(i = 0; i < N_MONSTROS; i++)
    {
        monstros[i].x = X_INICIAL_MONSTROS; // x inicial dos monstros padronizado
        monstros[i].y = (rand() % 20) + 5; // definição da altura em que os monstros surgem, de forma aleatória
    }
    maior = novo_recorde(pt, flechas_usadas); // captura do maior placar
    cabecalho(2, maior);
    gotoxy(13, 1);
    printf("%d", pt);
    contagem_reg(); // contagem regressiva para começar a fase
    cria_arqueiro(y_arq); // cria o arqueiro em tela
    while (timer >= 0) // loop principal do nível 2
    {
        // movimento do arqueiro
        c = comando(); // captura do comando
        y_arq = movimenta_arqueiro(y_arq, c); // atualização da posição do arqueiro, podendo ocorrer a qualquer momento no loop
        // criação de monstros
        if ((timer % VEL_SURG_MONSTROS == 0) && (timer != 0) && (n_monstros < N_MONSTROS)) // cria novos monstros em um intervalo de tempo previamente definido
        {
            cria_monstro(monstros[n_monstros].x, monstros[n_monstros].y);
            n_monstros++; // atualiza quantos monstros já surgiram
        }
        // movimento dos monstros
        if ( (timer % (1000 - VEL_MONSTROS) == 0) && (timer != 0) ) // divisão do tempo de execução para os monstros
        {
            for (i = 0; i < n_monstros; i++)
            {
                if (monstros[i].x != 0)
                {
                    monstros[i].x = movimenta_monstro(monstros[i].x, monstros[i].y); // atualização da posição de cada monstro individualmente
                    if (monstros[i].x == 0)
                        n_mortos++; // acréscimo no número de monstros mortos ou que já passaram pelo jogador
                }
            }
        }
        // disparo de flechas
        if (c == ' ' && flechas_usadas < N_FLECHAS_NIVEL2) // executado ao apertar o comando e caso ainda existam flechas guardadas
        {
            cria_flecha(10, y_arq + 1);
            flechas[flechas_usadas].x = 10; // definição das coordenadas para cada flecha disparada
            flechas[flechas_usadas].y = y_arq + 1;
            flechas_usadas++; // acréscimo no número de flechas usadas
            gotoxy(72, 2);
            printf("%02d", N_FLECHAS_NIVEL2 - flechas_usadas); // atualização do contador de flechas na tela
        }
        // movimento das flechas
        if ( (timer % (1000 - VEL_FLECHAS) == 0) && (timer != 0) ) // divisão do tempo de execução para as flechas
        {
            for (i = 0; i < flechas_usadas; i++)
            {
                if (flechas[i].x != 0) // se a flecha ainda estiver em movimento...
                    flechas[i].x = movimenta_flecha(flechas[i].x, flechas[i].y); // o seu valor de y continua sendo atualizado
                else
                    flechas[i].y = 0;
            }
        }
        // colisões
        for (i = 0; i < flechas_usadas; i++)
        {
            for (j = 0; j < n_monstros; j++)
            {
                if (colisao_flecha_monstro(flechas[i].x, flechas[i].y, monstros[j].x, monstros[j].y)) // verifica se as coordenadas das flechas coicidem com as dos monstros
                {
                    quebra_flecha(flechas[i].x, flechas[i].y); // flecha excluída após acertar um montros
                    flechas[i].x = 0;
                    morte_monstro(monstros[j].x, monstros[j].y); // monstro excluído após tomar uma flechada
                    monstros[j].x = 0;
                    n_mortos++; // acréscimo no número de monstros mortos ou que já passaram pelo jogador
                    // pontuação
                    pt += 200;
                    gotoxy(13, 1);
                    printf("%d", pt);
                }
            }
        }
        // cronômetro
        Sleep(1); // delay de 1 milésimo de segundo
        if (timer == 2000) // após 2 segundos, o contador reinicia
            timer = 0;
        else
            timer++;
        // fim
        for (i = 0; i < n_monstros; i++)
        {
            if (colisao_arqueiro_monstro(X_INICIAL_ARQUEIRO, y_arq, monstros[i].x, monstros[i].y)) // condição para o jogo acabar caso um monstro acerte o arqueiro
                timer = -1;
        }
        if (n_mortos == N_MONSTROS) // se o número de monstros mortos somados com os que cruzaram todo o cenário, foi igual ao número total de monstros, o loop finaliza
            timer = -2;

    }
    system("cls"); // limpeza da tela
    if (timer == -2) // caso de vitória
    {
        pt += pontuacao_flechas(flechas_usadas, 2); // atualização da pontuação pelas flechas que sobraram
        jogo_ganho(pt, maior, flechas_usadas); // chamada da função para a mensagem de vitória
    }
    else
        jogo_perdido(pt, maior, flechas_usadas, 2); // chamada da função para a mensagem de derrota
}

int pontuacao_flechas(int flechas, int nivel) // função responsável por atribuir uma pontuação a cada flecha não utilizada em cada nível que o jogador finaliza. Essa função
// retorna essa pontuação adquirida pelo jogador pelas flechas não utilizadas em um nível finalizado
{
    if(nivel == 1)
        return ((N_FLECHAS_NIVEL1 - flechas) * 50); // uso do número de flechas do nível 1
    else
        return ((N_FLECHAS_NIVEL2 - flechas) * 50); // uso do número de flechas do nível 2
}

void jogo_ganho(int pt, int maior, int flechas_usadas) // função responsável por informar ao jogador que ele finalizou o jogo e por chamar a função int
// novo_recorde(int pt, int flechas_usadas) para verificar se a pontuação final desse jogador se enquadra entre as 5 maiores já registradas. Finalmente, ela chama a função
// void cria_menu()
{
    cabecalho(2, maior); // cria a estrutura para apresentar a mensagem de vitória do jogo. Como o jogo só finaliza com vitória no nível 2, o parâmetro não precisa ser uma variável
    gotoxy(72, 2);
    printf("00");
    gotoxy(13, 1);
    printf("%d", pt);
    gotoxy(36, 15);
    printf("PARABÉNS!\n");
    gotoxy(25, 16);
    printf("VOCÊ CONSEGUIU ZERAR O JOGO!");
    Sleep(3000);
    gotoxy(19, 15);
    printf("                                            ");
    gotoxy(19, 16);
    printf("                                            ");
    novo_recorde(pt, flechas_usadas); // chamada da função para verificar se houve um novo recorde
    system("cls"); // limpeza da tela
    cria_menu(); // retorno ao menu
}

void jogo_perdido(int pt, int maior, int flechas_usadas, int nivel) // função responsável por informar ao jogador que ele não conseguiu completar o nível do jogo em que se
// encontrava e por chamar a função int novo_recorde(int pt, int flechas_usadas) para verificar se a pontuação final desse jogador se enquadra entre as 5 maiores já registradas.
// Finalmente, ela chama a função void cria_menu()
{
    cabecalho(nivel, maior); // cria a estrutura para apresentar a mensagem de perda do jogo
    gotoxy(72, 2);
    printf("00");
    gotoxy(13, 1);
    printf("%d", pt);
    gotoxy(35, 15);
    printf("QUE PENA!\n");
    gotoxy(21, 16);
    printf("VOCÊ NÃO CONSEGUIU COMPLETAR O NÍVEL %d.", nivel); // nível como variável, pois o jogador pode perder em ambos os níveis
    Sleep(3000);
    gotoxy(19, 15);
    printf("                                            ");
    gotoxy(19, 16);
    printf("                                            ");
    novo_recorde(pt, flechas_usadas); // chamada da função para verificar se houve um novo recorde
    system("cls"); // limpeza da tela
    cria_menu(); // retorno ao menu
}

int main() // função principal
{
    setlocale(LC_ALL, "Portuguese"); // usado para o programa reconhecer ç e acentos
    cria_menu(); // início do programa, gerando o menu do jogo
    return 0;
}
