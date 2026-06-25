Descrição:
Visualização grafica para FFT

Como usar:
Ao iniciar, o programa já pré-carrega uma onda senoidal para visualização. É possivel acessar outras funcionalidades pelo MENU, como a de abrir arquivos.

É possivel arrastar a visualização com o botão esquerdo e aumentar ou diminuir a escala com scroll do mouse. A razão da escala é possivel de ser ajustada com scroll do mouse enquanto precionado "CTRL".

Como compilar:
O projeto utiliza cmake para criação dos projetos. E necessario configurar o projeto para poder compila-lo. Para onfigurar para o CodeBlocks basta abrir o arquivo Configurar CodeBlocks.cmd ou executar o comando cmake -B CodeBlocks -G "CodeBlocks - MinGW Makefiles"

Após configurado o projeto ja pode ser compilado através do CodeBlocks ou pelo seguinte comando: cmake --build CodeBlocks --config release
