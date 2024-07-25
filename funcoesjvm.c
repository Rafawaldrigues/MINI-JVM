#include "funcoesjvm.h"
#include <stdio.h>

void iconst_0(minijvm* jvm) {
    stack_push(jvm->operands, 0);
}

void pop(minijvm* jvm) {
    int value;
    stack_pop(jvm->operands, &value);
}

void dup(minijvm* jvm) {
    int top_value;
    if (stack_peek(jvm->operands, &top_value)) {
        stack_push(jvm->operands, top_value);
    } else {
        fprintf(stderr, "Erro: Tentativa de duplicar elemento do topo da pilha, mas a pilha está vazia.\n");
        jvm->return_value = 1;
        return;
    }
}

void iadd(minijvm* jvm) {
    int value1, value2;
    if (stack_pop(jvm->operands, &value1) && stack_pop(jvm->operands, &value2)) {
        int result = value1 + value2;
        stack_push(jvm->operands, result);
    } else {
        fprintf(stderr, "Erro: Tentativa de somar elementos da pilha, mas a pilha contém menos de dois elementos.\n");
        jvm->return_value = 1;
        return;
    }
}

void isub(minijvm* jvm) {
    int value1, value2;
    if (stack_pop(jvm->operands, &value1) && stack_pop(jvm->operands, &value2)) {
        int result = value2 - value1;
        stack_push(jvm->operands, result);
    } else {
        fprintf(stderr, "Erro: Tentativa de subtrair elementos da pilha, mas a pilha contém menos de dois elementos.\n");
        jvm->return_value = 1;
        return;
    }
}

void imul(minijvm* jvm) {
    int value1, value2;
    if (stack_pop(jvm->operands, &value1) && stack_pop(jvm->operands, &value2)) {
        int result = value1 * value2;
        stack_push(jvm->operands, result);
    } else {
        fprintf(stderr, "Erro: Tentativa de multiplicar elementos da pilha, mas a pilha contém menos de dois elementos.\n");
        jvm->return_value = 1;
        return;
    }
}

void idiv(minijvm* jvm) {
    int value1, value2;
    if (stack_pop(jvm->operands, &value1) && stack_pop(jvm->operands, &value2)) {
        if (value1 == 0) {
            fprintf(stderr, "Erro: Tentativa de divisão por zero.\n");
            jvm->return_value = 1;
            return;
        }
        int result = value2 / value1;
        stack_push(jvm->operands, result);
    } else {
        fprintf(stderr, "Erro: Tentativa de dividir elementos da pilha, mas a pilha contém menos de dois elementos.\n");
        jvm->return_value = 1;
        return;
    }
}

void irem(minijvm* jvm) {
    int value1, value2;
    if (stack_pop(jvm->operands, &value1) && stack_pop(jvm->operands, &value2)) {
        if (value1 == 0) {
            fprintf(stderr, "Erro: Tentativa de calcular resto da divisão por zero.\n");
            jvm->return_value = 1;
            return;
        }
        int result = value2 % value1;
        stack_push(jvm->operands, result);
    } else {
        fprintf(stderr, "Erro: Tentativa de calcular resto da divisão, mas a pilha contém menos de dois elementos.\n");
        jvm->return_value = 1;
        return;
    }
}

void ishr(minijvm* jvm) {
    int value1, value2;
    if (stack_pop(jvm->operands, &value1) && stack_pop(jvm->operands, &value2)) {
        int result = value2 >> value1;
        stack_push(jvm->operands, result);
    } else {
        fprintf(stderr, "Erro: Tentativa de realizar deslocamento para a direita, mas a pilha contém menos de dois elementos.\n");
        jvm->return_value = 1;
        return;
    }
}

void return_op(minijvm* jvm) {
    jvm->return_value = 0;
}

void iprint(minijvm* jvm) {
    int top_value;
    if (stack_peek(jvm->operands, &top_value)) {
        printf("Topo da pilha: %d\n", top_value);
    } else {
        fprintf(stderr, "Erro: Tentativa de imprimir elemento do topo da pilha, mas a pilha está vazia.\n");
        jvm->return_value = 1;
        return;
    }
}

void bipush(minijvm* jvm, char b) {
    stack_push(jvm->operands, (int)b);
}

void iload(minijvm* jvm, int n) {
    stack_push(jvm->operands, jvm->locals[n]);
}

void istore(minijvm* jvm, int n) {
    int value;
    if (stack_pop(jvm->operands, &value)) {
        jvm->locals[n] = value;
    } else {
        fprintf(stderr, "Erro: Tentativa de armazenar elemento da pilha, mas a pilha está vazia.\n");
        jvm->return_value = 1;
        return;
    }
}

void iinc(minijvm* jvm, int n, char d) {
    jvm->locals[n] += d;
}

void ifeq(minijvm* jvm, uint8_t byte1, uint8_t byte2) {
    // Calcular o offset combinando os dois bytes em um valor inteiro de 16 bits
    int16_t offset = (byte1 << 8) | byte2;

    int value;
    // Verificar se há pelo menos um elemento na pilha de operandos
    if (stack_peek(jvm->operands, &value)) {
        // Se o valor do topo da pilha for zero, adicionar o offset ao contador de programa (PC)
        if (value == 0) {
            jvm->pc += offset;
            return;
        }
    } else {
        fprintf(stderr, "Erro: Tentativa de remover elemento da pilha para ifeq, mas a pilha está vazia.\n");
    }
    // Caso contrário, avançar para a próxima instrução normalmente
    jvm->pc += 2; // Avançar para o próximo bytecode
}


void goto_op(minijvm* jvm, uint8_t byte1, uint8_t byte2) {
    // Calcular o offset combinando os dois bytes em um valor inteiro de 16 bits
    int16_t offset = (int16_t)((byte1 << 8) | byte2);

    // Adicionar o offset ao contador de programa (PC)
    jvm->pc += offset;
}

