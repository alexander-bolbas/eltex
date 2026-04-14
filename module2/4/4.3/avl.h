#ifndef AVL_H
#define AVL_H

#include "types.h"

// Инициализация пустого дерева
void avl_init(avl_tree_t *tree);

// Освобождение дерева
void avl_free(avl_tree_t *tree);

// Вставка контакта в дерево
int avl_insert(avl_tree_t *tree, const contact_t *data);

// Удаление контакта по ID
int avl_delete(avl_tree_t *tree, int id);

// Поиск контакта по ID
contact_t* avl_find_by_id(avl_tree_t *tree, int id);

// Обход дерева в порядке (симметричный): callback(node, userdata)
void avl_inorder(contact_t *node, void (*callback)(contact_t*, void*), void *userdata);

// Полная ребалансировка дерева (сбор-сортировка-перестройка)
void avl_rebuild(avl_tree_t *tree);

#endif // AVL_H
