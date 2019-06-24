# Simulador de Memória Cache
Implementa uma ferramenta que simula o comportamento interno de uma cache L1. O simulador recebe como entrada uma sequencia de comandos que podem ser de leitura ou escrita e o endereço solicitado. A cache simulada verifica a presença ou não do bloco contendo o dado solicitado (acarretando em um Hit ou Miss).

O simulador é configurável no que diz respeito aos aspectos: Mapeamento(Direto, Totalmente Associativo e Parcialmente associativo) e Politica de Substituição(Aleatório, FIFO e LFU).

O simulador aceita como entrada 3 comandos: Read, Write e Show. O comando read recebe como parâmetro o endereço a ser lido. Como resposta o simulador deve informar o resultado (hit ou miss) e a linha da cache que contém o bloco solicitado. O comando write funciona de maneira similar, entretanto além do endereço do dado a ser escrito ele tem como parâmetro o novo valor a ser escrito. Por fim, o comando show mostra o conteúdo de toda a cache e de toda a memória principal.

## Aluna:
Camila Duarte

## Compilação:
1. `g++ src/*.cpp include/*.h`

## Exemplo de execução:
2. `./a.out config.txt`

## OBS:

- A política de substituição `FIFO` foi feita especialmente para o mapeamento parcialmente associativo e a política de substituição `LFU` foi feita especialmente para o mapeamento totalmente associativo.

- Todos os endereços por padrão começam com uma unidade a mais do tamanho da mamória principal(no nosso exemplo sendo 17); e todos os blocos começam sendo 0.

- Ao digitar o comando "Read" ou "Write" onde o endereço não está na cache, por padrão uma linha do bloco recebe tal endereço e o restante do bloco fica com 0 na posição do endereço e do conteúdo.

## Exemplo de como utilizar os comandos:
1. `Read 25`
2. `Write 25 41`
3. `Show`

## Arquivo de Configuração(config.txt):
- Linha 1: Tamanho do bloco (em número de palavras)
- Linha 2: Numero de linhas da cache
- Linha 3: Numero de blocos da memória principal
- Linha 4: Mapeamento (1 – Direto; 2 – Totalmente Associativo; 3 – Parcialmente Associativo)
- Linha 5: Numero de conjuntos (caso não seja Parcialmente Associativo, ler o valor normalmente mas desconsidere-o)
- Linha 6: Política de substituição (1 – Aleatório; 2 – FIFO; 3 – LFU)
