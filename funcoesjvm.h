#ifndef FUNCOESJVM_H
#define FUNCOESJVM_H

#include "minijvm.h"
#include <stdint.h>

void iconst_0(minijvm* jvm);
void pop(minijvm* jvm);
void dup(minijvm* jvm);
void iadd(minijvm* jvm);
void isub(minijvm* jvm);
void imul(minijvm* jvm);
void idiv(minijvm* jvm);
void irem(minijvm* jvm);
void ishr(minijvm* jvm);
void return_op(minijvm* jvm);
void iprint(minijvm* jvm);
void bipush(minijvm* jvm, char b);
void iload(minijvm* jvm, int n);
void istore(minijvm* jvm, int n);
void iinc(minijvm* jvm, int n, char d);
void ifeq(minijvm* jvm, uint8_t byte1, uint8_t byte2); // Nova função para remover elemento da pilha se for igual a zero e adicionar offset ao PC
void goto_op(minijvm* jvm, uint8_t byte1, uint8_t byte2); // Nova função para adicionar o valor de offset ao contador de programa (PC)

#endif
