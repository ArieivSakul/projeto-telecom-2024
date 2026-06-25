# Descri��o:
Visualiza��o grafica para FFT
 
# Como usar:
Ao iniciar, o programa j� pr�-carrega uma onda senoidal para visualiza��o. � poss�vel acessar outras funcionalidades pelo MENU, como
a de abrir arquivos.

� poss�vel arrastar a visualiza��o com o bot�o esquerdo e aumentar ou diminuir a escala com scroll do mouse. A raz�o da escala � poss�vel de ser
ajustada com scroll do mouse enquanto precionado "CTRL".


# Como compilar:
O projeto utiliza cmake para cria��o dos projetos. 
E necessario configurar o projeto para poder compila-lo. Para onfigurar para o CodeBlocks basta abrir o arquivo `Configurar CodeBlocks.cmd` ou executar o comando `cmake -B CodeBlocks -G "CodeBlocks - MinGW Makefiles"`

Ap�s configurado o projeto ja pode ser compilado atrav�s do CodeBlocks ou pelo seguinte comando: `cmake --build CodeBlocks --config release` 