// 2013 - mauricio

#include <iostream>
#include <time.h>
#include <Windows.h>

#define MAX_CROMOSSOMOS 1000
#define MAX_GENES 9
#define MAX_GERACAO 1000

using namespace std;

int puzzle_final[3][3] =
{
	{ 0,1,2 },
	{ 3,4,5 },
	{ 6,7,8 }
};

int populacao[MAX_CROMOSSOMOS][MAX_GENES];
int pop_fitness[MAX_CROMOSSOMOS];

int random(int low, int high)
{
	return rand() % (high - low + 1) + low;
}

void imprimirPuzzle(int puzzle[3][3])
{
	int i, j;

	for (i = 0; i<3; i++)
	{
		for (j = 0; j<3; j++)
			cout << puzzle[i][j];
		cout << "\n";
	}
}

bool verificar(int puzzle[3][3])
{
	int i, j;

	for (i = 0; i<3; i++)
		for (j = 0; j<3; j++)
			if (puzzle[i][j] != puzzle_final[i][j])
				return false;

	return true;
}

void vetorToPuzzle(int vetor[MAX_GENES], int puzzle[3][3])
{
	int i, j, x;

	x = 0;
	for (i = 0; i<3; i++)
		for (j = 0; j<3; j++)
		{
			puzzle[i][j] = vetor[x];
			x++;
		}
}

bool jaExiste(int cromo, int ate, int num)
{
	int i;

	for (i = 0; i<ate; i++)
		if (populacao[cromo][i] == num)
			return true;
	return false;
}

bool jaExiste2(int v[MAX_GENES], int ate, int num)
{
	int i;

	for (i = 0; i<ate; i++)
		if (v[i] == num)
			return true;
	return false;
}

void criarIndividuo(int v[MAX_GENES])
{
	int i, numero;

	numero = random(0, MAX_GENES - 1);

	for (i = 0; i<MAX_GENES; i++)
	{
		while (jaExiste2(v, i, numero))
		{
			numero = random(0, 8);
		}
		v[i] = numero;
	}
}

void preencherCromossomo(int cromossomo)
{
	int i, numero;

	numero = random(0, MAX_GENES - 1);

	for (i = 0; i<MAX_GENES; i++)
	{
		while (jaExiste(cromossomo, i, numero))
		{
			numero = random(0, 8);
		}
		populacao[cromossomo][i] = numero;
	}

}

void gerarPopulacao()
{
	int i;
	int p[3][3];

	for (i = 0; i<MAX_CROMOSSOMOS; i++)
	{
		preencherCromossomo(i);
		vetorToPuzzle(populacao[i], p);
		//imprimirPuzzle(p);
		//cout << "-\n";
	}
}

int mod(int num)
{
	if (num >= 0)
		return num;
	if (num<0)
		return(-num);
}

void buscarNumero(int p[3][3], int &linha, int &coluna, int num)
{
	int i, j;
	for (i = 0; i<3; i++)
		for (j = 0; j<3; j++)
			if (p[i][j] == num)
			{
				linha = i;
				coluna = j;
				return;
			}
}

int manhattan(int puzzle[3][3])
{
	int n;
	int linha_puzzle, coluna_puzzle;
	int linha_final, coluna_final;
	int som = 0;

	for (n = 0; n<MAX_GENES; n++)
	{
		buscarNumero(puzzle, linha_puzzle, coluna_puzzle, n);
		buscarNumero(puzzle_final, linha_final, coluna_final, n);
		som += (mod(linha_puzzle - linha_final) + mod(coluna_puzzle - coluna_final));
	}

	return som;
}

int fitness(int v[MAX_GENES], int cromo)
{
	int p[3][3];
	int man;
	vetorToPuzzle(v, p);
	man = manhattan(p);

	return (man);
}

void copiarVetor(int recebidor[MAX_GENES], int fonte[MAX_GENES])
{
	int i = 0;

	for (i = 0; i<MAX_GENES; i++)
	{
		recebidor[i] = fonte[i];
	}
}

void zerarVetor(int v[MAX_GENES])
{
	int i;

	for (i = 0; i<MAX_GENES; i++)
		v[i] = 0;
}

bool vetoresIguais(int v1[MAX_GENES], int v2[MAX_GENES])
{
	int i;
	for (i = 0; i<MAX_GENES; i++)
		if (v1[i] != v2[i])
			return false;

	return true;
}

int jaExiste3(int v[MAX_GENES], int de, int ate, int num)
{
	int i;

	for (i = de; i<ate; i++)
		if (v[i] == num)
			return i;
	return 0;
}

bool jaExiste4(int v[MAX_GENES], int num)
{
	int i;

	for (i = 0; i<MAX_GENES; i++)
	{
		if (v[i] == num)
			return true;
	}
	return false;
}

int novoNumero(int v[MAX_GENES])
{
	int num = random(0, 8);
	while (jaExiste4(v, num))
	{
		num = random(0, 8);
	}
	return num;
}

void crossover(int pai[MAX_GENES], int mae[MAX_GENES], int filho[MAX_GENES])
{
	int i, j, pivo;

	// faz um random para saber o limite entre o que copiar do pai e o que copiar da mae
	pivo = random(1, MAX_GENES - 2);

	// Copia para o filho
	for (i = 0; i <= pivo; i++)
		filho[i] = pai[i];
	for (; i<MAX_GENES; i++)
		filho[i] = mae[i];

	//fazer mutacao nos gens do filho que ficaram repetidos
	j = 0;
	for (i = 0; i <= pivo; i++)
		if ((j = jaExiste3(filho, pivo + 1, MAX_GENES, filho[i])) != 0)
		{
			filho[j] = novoNumero(filho);
		}

	return;
}

void evolve()
{
	int i, j, media = 0, min = 99999;
	int new_populacao[MAX_CROMOSSOMOS][MAX_GENES];
	int filho[MAX_GENES];

	// Zera o vetor new_populacao
	for (i = 0; i<MAX_CROMOSSOMOS; i++)
		zerarVetor(new_populacao[i]);

	// calcula o fitness_medio e o fitness_minimo
	for (i = 0; i<MAX_CROMOSSOMOS; i++) {
		media += pop_fitness[i];
		if (pop_fitness[i] < min)
			min = pop_fitness[i];
	}
	media /= MAX_CROMOSSOMOS;
	//cout << "fitness_medio=" << media << "\n";
	//cout << "fitness_minimo=" << min << "\n";

	// copia para a nova_populacao somente os cromossomos/individuos fodasticos (fitness<media) da populacao antiga
	j = 0;
	for (i = 0; i<MAX_CROMOSSOMOS; i++) {
		if (pop_fitness[i]<media)
		{
			copiarVetor(new_populacao[j++], populacao[i]);
		}
	}
	//cout << "copiados=" << j << "\n";


	// Se todos os individuos/cromossos foram copiados, pegue so metade para fazer o cross
	if (j == MAX_CROMOSSOMOS)
		j = j / 2;

	// i vai ser igual a metade se todos foram menores que a media OOOOU i vai ser igual aos copiados (com isso, só os individuos da populacao total que nao foram qualificados irao receber crossover)
	i = j;

	// faz o crossover e a mutacao (e guarda o filho nos espacos dos individuos que nao passaram na qualificacao) para tentar criar um individuo que seje a solucao
	for (; i<MAX_CROMOSSOMOS; i++)
	{
		int pai, mae;

		if (i == 0)
		{
			//system("pause");
			gerarPopulacao();
			cout << "**********\n";
			return;
		}

		pai = random(0, i);
		mae = random(0, i);
		while (pai == mae)
		{
			mae = random(0, i);
		}

		crossover(new_populacao[pai], new_populacao[mae], filho);
		copiarVetor(new_populacao[i++], filho);
	}

	// substitui a populacao antiga pela nova populacao crossoverzada
	for (i = 0; i<MAX_CROMOSSOMOS; i++)
		copiarVetor(populacao[i], new_populacao[i]);
}

void rodar()
{
	int geracao, i;
	int p[3][3];

	for (geracao = 0; geracao<MAX_GERACAO; geracao++)
	{
		cout << "geracao=" << geracao << "\n";
		for (i = 0; i<MAX_CROMOSSOMOS; i++)
		{
			pop_fitness[i] = fitness(populacao[i], i);
			//cout << "fitness=" << pop_fitness[i] << "\n";
			if (pop_fitness[i] == 0)
			{
				cout << "ACHOU A SOLUCAO NA GERACAO " << geracao << "!\n";
				vetorToPuzzle(populacao[i], p);
				imprimirPuzzle(p);
				return;
			}
		}
		evolve();
		Sleep(10);
	}
}

void imprimirVetor(int v[MAX_GENES])
{
	int i;

	for (i = 0; i<MAX_GENES; i++)
		cout << v[i] << " ";
}

char * IntToBinStr(char * bin32, int tam, int dec)
{
	for (int pos = tam - 2; pos >= 0; --pos)
	{
		if (dec % 2)
			bin32[pos] = '1';
		else
			bin32[pos] = '0';
		dec /= 2;
	}
	bin32[tam - 1] = 0;
	return bin32;
}

void imprimirPopulacao()
{


}

int main()
{
	while (1)
	{
		system("pause");

		imprimirPopulacao();

		srand(time(NULL));
		gerarPopulacao();
		srand(time(NULL) / 999);

		rodar();

		system("pause");
	}
	return 0;
}