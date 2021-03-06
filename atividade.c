//Bryan Bialokur da Cruz, RA: 11052316
//Melissa Gabriela Pereira da Soledade Perrone, RA: 11072216

#include<stdio.h>
#include<stdlib.h>

/*
 * Comprimento do percurso: 100km
 * Correr: 10km/h
 * Andar: 5km/h
*/

typedef struct s_funcao funcao;

struct s_funcao{
  double a;
  double b;
};

funcao nova_funcao(double a, double b){
    funcao f = {a, b};
    return f;
}

funcao soma(funcao f, funcao g){
  funcao h = nova_funcao(f.a + g.a, f.b + g.b);
  return h;
}

funcao subtracao(funcao f, funcao g){
  funcao h = nova_funcao(f.a - g.a, f.b - g.b);
  return h;
}

funcao multiplicacao(funcao f, double c){
  funcao h = nova_funcao(c*f.a, c*f.b);
  return h;
}

funcao inversa(funcao f){
  funcao h = nova_funcao(1/f.a, -f.b/f.a);
  return h;
}

funcao composicao(funcao f, funcao g){
  funcao h = nova_funcao(f.a*g.a, f.a*g.b + f.b);
  return h;
}

double resultado_funcao(funcao f, double x){
  return f.a*x + f.b;
}

typedef struct s_instrucao * instrucao;

struct s_instrucao{
    double v;
    double fracao;
    char tipo;
    instrucao prox;
};

instrucao nova_instrucao(double v, double fracao, char tipo){
    instrucao i = (instrucao) malloc(sizeof(struct s_instrucao));
    if(i == NULL){
      fprintf(stderr, "Erro na alocação de memória\n");
      exit(1);
    }

    i->v = v;
    i->fracao = fracao;
    i->tipo = tipo;
    i->prox = NULL;
    return i;
}

void deletar_instrucao(instrucao i){
  free(i);
}

int contar_instrucoes(instrucao instrucoes){
  int n = 0;
  instrucao i = instrucoes;
  while(i != NULL){
    n++;
    i = i->prox;
  }
  return n;
}

double calcular_tempo(instrucao instrucoes){
  int n = contar_instrucoes(instrucoes);

  funcao funcoes_espacos[n];
  funcao funcoes_tempos[n];

  int j = 0;
  instrucao i = instrucoes;
  while(j < n && instrucoes != NULL){
    if(j == 0){
      //So = v * to
      funcoes_espacos[j] = nova_funcao(i->v, 0);

      //to = to
      funcoes_tempos[j] = nova_funcao(1, 0);

    }else{

      if(i->tipo == 'E'){
        //S - Sa = fracao*S -> S = Sa / (1 - fracao)
        funcoes_espacos[j] = composicao(nova_funcao(1/(1-i->fracao), 0), funcoes_espacos[j-1]);

        //S = v * (t - ta) + Sa -> t = S/v + (ta - Sa/v)
        funcoes_tempos[j] = soma(composicao(nova_funcao(1/i->v, 0), funcoes_espacos[j]),
           subtracao(funcoes_tempos[j-1], multiplicacao(funcoes_espacos[j-1], 1/i->v)));
      }else{
        //t - ta = fracao*t -> t = ta / (1 - fracao)
        funcoes_tempos[j] = composicao(nova_funcao(1/(1-i->fracao), 0), funcoes_tempos[j-1]);

        //S = v * (t - ta) + Sa -> S = v*t + (Sa - v*ta)
        funcoes_espacos[j] = soma(composicao(nova_funcao(i->v, 0), funcoes_tempos[j]),
           subtracao(funcoes_espacos[j-1], multiplicacao(funcoes_tempos[j-1], i->v)));
      }
    }
    i = i->prox;
    j++;
  }

  double t0 = resultado_funcao(inversa(funcoes_espacos[n-1]), 100);
  double tf = resultado_funcao(funcoes_tempos[n-1], t0);

  return tf;
}

char ler_caractere(){
  char c;
  int leu;
  do{
    leu = scanf("%c", &c);
    if(!leu) return 0; //se nao conseguiu ler nada, para a leitura
  }while(c == ' ');

  return c;
}

int ler_inteiro(){
  int n;
  scanf("%d", &n);

  return n;
}

int main(){
  char aux, qual_v, tipo;
  int frac1, frac2;
  while(aux = ler_caractere()){
    instrucao instrucoes = NULL;

    do{
      //leitura dos dados
      qual_v = aux;
      frac1 = ler_inteiro();
      frac2 = ler_inteiro();
      tipo = ler_caractere();

      //cria a instrução e salva ela na lista
      double v = qual_v == 'A'? 5 : 10;
      double fracao = (1.0*frac1)/frac2;
      instrucao i = nova_instrucao(v, fracao, tipo);
      i->prox = instrucoes;
      instrucoes = i;

    }while((aux = ler_caractere()) && aux != '\n');

    double t = calcular_tempo(instrucoes);
    printf("%.2lf\n", t);

    //libera a memoria
    instrucao i = instrucoes;
    while(i != NULL){
      instrucao aux = i->prox;
      deletar_instrucao(i);
      i = aux;
    }
  }

	return 0;
}
