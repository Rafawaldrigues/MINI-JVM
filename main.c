#include <stdio.h>
#include <stdlib.h>
#include "minijvm.h"
#include "funcoesjvm.c"
#define MAX_CLASS_SIZE 1024 // Tamanho máximo do bytecode do arquivo .mclass

#define MAX_CLASS_SIZE 1024 // Tamanho máximo do bytecode do arquivo .mclass

char* jvm_read(const char* filename) {
    char full_filename[strlen(filename) + 8]; // Tamanho do nome do arquivo + 7 caracteres para ".mclass" e terminador nulo
    sprintf(full_filename, "%s.mclass", filename);

    FILE* file = fopen(full_filename, "rb"); // Abrir o arquivo para leitura binária
    if (!file) {
        fprintf(stderr, "Erro: Não foi possível abrir o arquivo %s\n", full_filename);
        exit(EXIT_FAILURE);
    }

    // Alocar vetor dinamicamente para armazenar o bytecode
    char* bytecode = (char*)malloc(MAX_CLASS_SIZE * sizeof(char));
    if (!bytecode) {
        fprintf(stderr, "Erro: Falha ao alocar memória para o bytecode.\n");
        exit(EXIT_FAILURE);
    }

    // Ler byte a byte e carregar no vetor
    size_t bytes_read = fread(bytecode, sizeof(char), MAX_CLASS_SIZE, file);
    if (bytes_read == 0) {
        fprintf(stderr, "Erro: Falha ao ler o arquivo %s\n", full_filename);
        fclose(file);
        free(bytecode);
        exit(EXIT_FAILURE);
    }

    fclose(file);
    return bytecode;
}

minijvm* jvm_init(const char* filename) {
    minijvm* jvm = (minijvm*)malloc(sizeof(minijvm));
    if (!jvm) {
        fprintf(stderr, "Erro: Falha ao alocar memória para a JVM.\n");
        exit(EXIT_FAILURE);
    }

    jvm->bytecode = jvm_read(filename);
    jvm->pc = jvm->bytecode; // Definir o contador de programa para o primeiro elemento do vetor de bytecodes
    jvm->operands = stack_create(); // Criar a pilha de operandos
    for (int i = 0; i < 10; i++) {
        jvm->locals[i] = 0; // Inicializar variáveis locais com zero
    }
    jvm->return_value = 0; // Inicializar o valor de retorno com zero

    return jvm;
}

void jvm_free(minijvm* jvm) {
    if (jvm) {
        free(jvm->bytecode); // Liberar memória do bytecode
        stack_free(jvm->operands); // Liberar memória da pilha de operandos
        free(jvm); // Liberar memória da JVM
    }
}

void jvm_run(minijvm* jvm) {
    while (true) {
        unsigned char b = *(jvm->pc);

        switch (b) {
            
            case INST_ILOAD:
                // Ativar função para executar iload
                break;
            case INST_RETURN:
                // Encerrar execução do programa e retornar ao Sistema Operacional
                return;
        }

        // Atualizar o PC para apontar para o próximo bytecode
        jvm->pc++;
    }
}

void jvm_usage()
{
    fprintf(stderr, "Usage: mjvm FILENAME\n");
    exit(EXIT_FAILURE);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        jvm_usage();
    }

    minijvm* jvm = jvm_init(argv[1]);

    while (1) {
        unsigned char b = *(jvm->pc);

        switch (b) {
            case 3:
                iconst_0(jvm);
                break;
            case 16:
                bipush(jvm, *(jvm->pc + 1));
                jvm->pc++;
                break;
            case 21:
                iload(jvm, *(jvm->pc + 1));
                jvm->pc++;
                break;
            case 54:
                istore(jvm, *(jvm->pc + 1));
                jvm->pc++;
                break;
            case 87:
                pop(jvm);
                break;
            case 89:
                dup(jvm);
                break;
            case 96:
                iadd(jvm);
                break;
            case 100:
                isub(jvm);
                break;
            case 104:
                imul(jvm);
                break;
            case 108:
                idiv(jvm);
                break;
            case 112:
                irem(jvm);
                break;
            case 122:
                ishr(jvm);
                break;
            case 132:
                iinc(jvm, *(jvm->pc + 1), *(jvm->pc + 2));
                jvm->pc += 2;
                break;
            case 153:
                ifeq(jvm, *(jvm->pc + 1), *(jvm->pc + 2));
                jvm->pc += 2;
                break;
            case 167:
                goto_op(jvm, *(jvm->pc + 1), *(jvm->pc + 2));
                jvm->pc += 2;
                break;
            case 177:
                return_op(jvm);
                jvm_free(jvm);
                return EXIT_SUCCESS;
            case 187:
                iprint(jvm);
                break;
            default:
                fprintf(stderr, "Bytecode não suportado: %d\n", b);
                jvm->return_value = 1;
                jvm_free(jvm);
                return EXIT_FAILURE;
        }

        jvm->pc++;
    }

    return EXIT_SUCCESS;
}

