@echo off
echo Configurando o projeto para CodeBlocks...
timeout /t 3 /nobreak > nul

cmake -B CodeBlocks -G "CodeBlocks - MinGW Makefiles"

if %errorlevel% equ 0 (
    color 
    echo . & echo Configuração conluida....
) else (
    color 
    echo. & echo Configuração falhou....
    echo Verifique se o cmake esta instalado
)

pause