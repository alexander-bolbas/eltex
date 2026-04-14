#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avl.h"
#include "contact.h"

// Получить высоту узла (если не существует - высота 0)
static int node_height(const contact_t *n) {
    return n ? n->height : 0;
}

// Обновить высоту узла на основе высот его потомков
static void node_update_height(contact_t *n) {
    if (!n) return;
    int lh = node_height(n->left);  // высота левого поддерева
    int rh = node_height(n->right); // высота правого поддерева
    n->height = 1 + (lh > rh ? lh : rh); // высота = 1 + макс(левая, правая)
}

// Получить баланс узла (баланс = высота_левая - высота_правая)
static int node_balance(const contact_t *n) {
    return n ? node_height(n->left) - node_height(n->right) : 0;
}

// Правая ротация (для разрешения левого дисбаланса)
static contact_t *rotate_right(contact_t *y) {
    contact_t *x = y->left;     // Левый потомок становится новым корнем
    contact_t *t2 = x->right;   // Правое поддерево левого потомка используется для перестановки
    
    x->right = y;
    y->left = t2;
    
    node_update_height(y);
    node_update_height(x);
    
    return x;
}

// Левая ротация (для разрешения правого дисбаланса)
static contact_t *rotate_left(contact_t *x) {
    contact_t *y = x->right;    // Правый потомок становится новым корнем
    contact_t *t2 = y->left;    // Левое поддерево правого потомка используется для перестановки
    
    y->left = x;
    x->right = t2;
    
    node_update_height(x);
    node_update_height(y);
    
    return y;
}

// Ребалансировка узла: проверяет баланс и выполняет необходимые ротации
static contact_t *avl_rebalance(contact_t *n, int new_id) {
    node_update_height(n);
    int bal = node_balance(n);
    
    // Left Left - левое поддерево левого потомка тяжелее
    if (bal > 1 && new_id < n->left->id)
        return rotate_right(n);
    
    // Right Right - правое поддерево правого потомка тяжелее
    if (bal < -1 && new_id > n->right->id)
        return rotate_left(n);
    
    // Left Right - правое поддерево левого потомка тяжелее
    if (bal > 1 && new_id > n->left->id) {
        n->left = rotate_left(n->left);
        return rotate_right(n);
    }
    
    // Right Left - левое поддерево правого потомка тяжелее
    if (bal < -1 && new_id < n->right->id) {
        n->right = rotate_right(n->right);
        return rotate_left(n);
    }
    
    return n;
}

// Вспомогательная рекурсивная функция вставки элемента в AVL дерево
static contact_t *avl_insert_node(contact_t *node, const contact_t *data, int *inserted) {
    if (!node) {
        // Если достигли пустого узла - создаем новый
        contact_t *new_node = contact_create(data);
        if (new_node) *inserted = 1; // флаг успешной вставки
        return new_node;
    }
    
    // Рекурсивно вставляем в левое или правое поддерево в зависимости от ID
    if (data->id < node->id)
        node->left = avl_insert_node(node->left, data, inserted);
    else if (data->id > node->id)
        node->right = avl_insert_node(node->right, data, inserted);
    else {
        // Элемент с таким ID уже существует - ничего не вставляем
        return node;
    }
    
    return avl_rebalance(node, data->id);
}

// Вспомогательная рекурсивная функция удаления элемента из AVL дерева
static contact_t *avl_delete_node(contact_t *node, int id, int *deleted) {
    if (!node) return NULL;
    
    // Рекурсивно ищем узел для удаления в левом или правом поддереве
    if (id < node->id)
        node->left = avl_delete_node(node->left, id, deleted);
    else if (id > node->id)
        node->right = avl_delete_node(node->right, id, deleted);
    else {
        // Найден нужный узел для удаления
        *deleted = 1; // флаг успешного удаления
        
        // Случай 1: узел - листовой (нет потомков)
        if (!node->left && !node->right) {
            contact_free(node);
            return NULL;
        }
        
        // Случай 2: есть только правый потомок
        if (!node->left) {
            contact_t *tmp = node->right;
            contact_free(node);
            return tmp;
        }
        
        // Случай 3: есть только левый потомок
        if (!node->right) {
            contact_t *tmp = node->left;
            contact_free(node);
            return tmp;
        }
        
        // Случай 4: есть оба потомка - находим минимальный элемент в правом поддереве
        contact_t *cur = node->right;
        while (cur->left)
            cur = cur->left;
        
        // Копируем данные минимального узла на место удаляемого
        node->id = cur->id;
        contact_copy_data(node, cur);
        
        // Удаляем минимальный узел из правого поддерева
        node->right = avl_delete_node(node->right, cur->id, deleted);
    }
    
    if (!node) return NULL;
    return avl_rebalance(node, node->id);
}

// Вспомогательная рекурсивная функция для сбора всех узлов в массив (обход в порядке inorder)
static void collect_inorder(contact_t *node, contact_t **arr, int *idx) {
    if (!node) return;
    
    // Левое поддерево
    collect_inorder(node->left, arr, idx);
    // Текущий узел
    arr[(*idx)++] = node;
    // Правое поддерево
    collect_inorder(node->right, arr, idx);
}

// Вспомогательная рекурсивная функция для построения идеально сбалансированного дерева из массива
static contact_t *build_balanced(contact_t **arr, int lo, int hi) {
    if (lo > hi) return NULL;
    
    // Выбираем средний элемент как корень поддерева
    int mid = (lo + hi) / 2;
    contact_t *root = arr[mid];
    // Рекурсивно строим левое и правое поддеревья
    root->left = build_balanced(arr, lo, mid - 1);
    root->right = build_balanced(arr, mid + 1, hi);
    // Обновляем высоту новосозданного узла
    node_update_height(root);
    
    return root;
}

// Инициализация AVL дерева
void avl_init(avl_tree_t *tree) {
    if (!tree) return;
    tree->root = NULL;
    tree->count = 0;
    tree->id_counter = 1;
    tree->op_count = 0;
}

// Вспомогательная рекурсивная функция для полной очистки дерева (освобождение памяти)
static void avl_free_tree(contact_t *node) {
    if (!node) return;
    
    // Рекурсивно очищаем левое и правое поддеревья
    avl_free_tree(node->left);
    avl_free_tree(node->right);
    // Освобождаем память текущего узла
    contact_free(node);
}

// Освобождение всего дерева
void avl_free(avl_tree_t *tree) {
    if (!tree) return;
    
    avl_free_tree(tree->root);
    
    tree->root = NULL;
    tree->count = 0;
}

// Вставить новый контакт в AVL дерево
int avl_insert(avl_tree_t *tree, const contact_t *data) {
    if (!tree || !data) return -1;
    
    // Инициализируем новый контакт с уникальным ID
    contact_t new_contact = *data;
    new_contact.id = tree->id_counter++;
    
    // Вставляем в дерево
    int inserted = 0;
    tree->root = avl_insert_node(tree->root, &new_contact, &inserted);
    
    if (inserted) {
        tree->count++; // увеличиваем счетчик контактов
        tree->op_count++; // увеличиваем счетчик операций
        
        // Периодическая полная ребалансировка для сохранения оптимальной высоты дерева
        if (tree->op_count % AVL_REBUILD_EVERY == 0) {
            avl_rebuild(tree);
        }
        
        return new_contact.id;
    }
    
    return -1;
}

// Удалить контакт из AVL дерева по ID
int avl_delete(avl_tree_t *tree, int id) {
    if (!tree) return -1;
    
    // Удаляем из дерева
    int deleted = 0;
    tree->root = avl_delete_node(tree->root, id, &deleted);
    
    if (deleted) {
        tree->count--; // уменьшаем счетчик контактов
        tree->op_count++; // увеличиваем счетчик операций
        
        // Периодическая полная ребалансировка
        if (tree->op_count % AVL_REBUILD_EVERY == 0) {
            avl_rebuild(tree);
        }
        
        return 0;
    }
    
    return -1;
}

// Поиск контакта в AVL дереве по ID (бинарный поиск)
contact_t* avl_find_by_id(avl_tree_t *tree, int id) {
    if (!tree) return NULL;
    
    // Начинаем с корня дерева
    contact_t *cur = tree->root;
    while (cur) {
        if (id == cur->id)
            return cur; // Найдено
        else if (id < cur->id)
            cur = cur->left; // Ищем в левом поддереве
        else
            cur = cur->right; // Ищем в правом поддереве
    }
    
    return NULL; // Не найдено
}

// Обход дерева в порядке inorder с вызовом callback функции для каждого контакта
void avl_inorder(contact_t *node, void (*callback)(contact_t*, void*), void *userdata) {
    if (!node || !callback) return;
    
    // Левое поддерево
    avl_inorder(node->left, callback, userdata);
    // Текущий узел
    callback(node, userdata);
    // Правое поддерево
    avl_inorder(node->right, callback, userdata);
}

// Полная ребалансировка дерева: преобразует его в идеально сбалансированное
void avl_rebuild(avl_tree_t *tree) {
    if (!tree || tree->count == 0) return;
    
    // Выделяем временный массив для хранения всех контактов
    contact_t **arr = malloc(tree->count * sizeof(contact_t *));
    if (!arr) return;
    
    // Собираем все контакты в отсортированном порядке
    int idx = 0;
    collect_inorder(tree->root, arr, &idx);
    
    // Перестраиваем дерево в идеально сбалансированное
    tree->root = build_balanced(arr, 0, tree->count - 1);
    
    // Освобождаем временный массив
    free(arr);
}
