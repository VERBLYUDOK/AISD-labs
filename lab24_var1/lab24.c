#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_STACK_SIZE 256
#define MAX_TOKENS 256

// Определения структур и типов

// Стек операторов
typedef struct {
    char data[MAX_STACK_SIZE];
    int top;
} OperatorStack;

// Токенизатор
typedef enum {
    TOKEN_OPERATOR,
    TOKEN_NUMBER,
    TOKEN_VARIABLE,
    TOKEN_PARENTHESIS
} TokenType;
typedef struct {
    TokenType type;
    char value[64];
} Token;

// Дерево выражения
typedef enum { OPERATOR, VARIABLE, NUMBER } NodeType;
typedef struct Node {
    NodeType type;
    char operator;    // Используется, если type == OPERATOR
    int value;        // Используется, если type == NUMBER
    char* variable;   // Используется, если type == VARIABLE
    struct Node* left;
    struct Node* right;
} Node;

// Функции работы со стеком операторов
void initOperatorStack(OperatorStack* stack);
int isOperatorStackEmpty(OperatorStack* stack);
void pushOperator(OperatorStack* stack, char op);
char popOperator(OperatorStack* stack);
char peekOperator(OperatorStack* stack);

// Токенизатор
int tokenize(const char* expression, Token* tokens);
char getPreviousNonSpaceChar(const char* expression, int pos);

// Преобразование в постфиксную нотацию
int getOperatorPrecedence(char op);
int convertToPostfix(Token* infixTokens, int tokenCount, Token* postfixTokens);

// Построение и обработка дерева выражения
Node* createNumberNode(int value);
Node* createVariableNode(const char* variable);
Node* createOperatorNode(char operator, Node* left, Node* right);
Node* buildExpressionTree(Token* postfixTokens, int tokenCount);
void printExpression(Node* root);
void printTree(Node* root, int depth);
void freeTree(Node* root);
Node* simplify(Node* root);

// Функции работы со стеком узлов дерева
typedef struct {
    Node* data[MAX_STACK_SIZE];
    int top;
} NodeStack;

void initNodeStack(NodeStack* stack);
int isNodeStackEmpty(NodeStack* stack);
void pushNode(NodeStack* stack, Node* node);
Node* popNode(NodeStack* stack);


int main() {
    char expression[256];
    printf("Введите выражение: ");
    fgets(expression, sizeof(expression), stdin);
    expression[strcspn(expression, "\n")] = '\0';

    // Токенизация
    Token tokens[MAX_TOKENS];
    int tokenCount = tokenize(expression, tokens);

    // Преобразование в постфиксную нотацию
    Token postfixTokens[MAX_TOKENS];
    int postfixCount = convertToPostfix(tokens, tokenCount, postfixTokens);

    Node* root = buildExpressionTree(postfixTokens, postfixCount);

    printf("Исходное выражение: ");
    printExpression(root);
    printf("\n");

    printf("Дерево выражения:\n");
    printTree(root, 0);

    // Упрощение дерева
    root = simplify(root);

    printf("Упрощённое выражение: ");
    printExpression(root);
    printf("\n");

    printf("Упрощённое дерево выражения:\n");
    printTree(root, 0);

    freeTree(root);

    return 0;
}

// Стек операторов
void initOperatorStack(OperatorStack* stack) {
    stack->top = -1;
}
int isOperatorStackEmpty(OperatorStack* stack) {
    return stack->top == -1;
}
void pushOperator(OperatorStack* stack, char op) {
    if (stack->top + 1 ==  MAX_STACK_SIZE) {
        printf("Ошибка: переполнение стека операторов\n");
        exit(1);
    }
    stack->data[++stack->top] = op;
}
char popOperator(OperatorStack* stack) {
    if (stack->top == -1) {
        printf("Ошибка: стек операторов пуст\n");
        exit(1);
    }
    return stack->data[stack->top--];
}
char peekOperator(OperatorStack* stack) {
    if (stack->top == -1) {
        return '\0';
    }
    return stack->data[stack->top];
}

// Токенизатор
int tokenize(const char* expression, Token* tokens) {
    int tokenCount = 0;
    int pos = 0;
    int length = strlen(expression);

    while (pos < length) {
        char c = expression[pos];

        if (isspace(c)) {
            // Пропускаем пробелы
            pos++;
            continue;
        }
        if (c == '-') {
            char prevChar = getPreviousNonSpaceChar(expression, pos);
            if (pos == 0 || prevChar == '\0' || strchr("+-*/(", prevChar)) {
                // Унарный минус (отрицательное число)
                int start = pos;
                pos++; // Пропускаем '-'
                // Проверяем, есть ли число после '-'
                if (pos < length && isdigit(expression[pos])) {
                    while (pos < length && isdigit(expression[pos])) {
                        pos++;
                    }
                    int len = pos - start;
                    if (len >= sizeof(tokens[tokenCount].value)) {
                        printf("Ошибка: число слишком длинное\n");
                        exit(1);
                    }
                    strncpy(tokens[tokenCount].value, &expression[start], len);
                    tokens[tokenCount].value[len] = '\0';
                    tokens[tokenCount].type = TOKEN_NUMBER;
                    ++tokenCount;
                } else {
                    // Некорректный унарный минус
                    printf("Ошибка: некорректное использование унарного минуса\n");
                    exit(1);
                }
            }  else {
                // Оператор '-'
                tokens[tokenCount].value[0] = c;
                tokens[tokenCount].value[1] = '\0';
                tokens[tokenCount].type = TOKEN_OPERATOR;
                ++tokenCount;
                ++pos;
            }
        }

        else if (isdigit(c)) {
            // Число
            int start = pos;
            while (pos < length && (isdigit(expression[pos]))) {
                pos++;
            }
            int len = pos - start;
            if (len >= sizeof(tokens[tokenCount].value)) {
                printf("Ошибка: число слишком длинное\n");
                exit(1);
            }
            strncpy(tokens[tokenCount].value, &expression[start], len);
            tokens[tokenCount].value[len] = '\0';
            tokens[tokenCount].type = TOKEN_NUMBER;
            ++tokenCount;
        }

        else if (isalpha(c)) {
            // Переменная
            int start = pos;
            while (pos < length && (isalnum(expression[pos]))) {
                pos++;
            }
            int len = pos - start;
            if (len >= sizeof(tokens[tokenCount].value)) {
                printf("Ошибка: имя переменной слишком длинное\n");
                exit(1);
            }
            strncpy(tokens[tokenCount].value, &expression[start], len);
            tokens[tokenCount].value[len] = '\0';
            tokens[tokenCount].type = TOKEN_VARIABLE;
            ++tokenCount;
        }

        else if (strchr("+*/", c)) {
            // Оператор
            tokens[tokenCount].value[0] = c;
            tokens[tokenCount].value[1] = '\0';
            tokens[tokenCount].type = TOKEN_OPERATOR;
            ++tokenCount;
            ++pos;
        }

        else if (c == '(' || c == ')') {
            // Скобки
            tokens[tokenCount].value[0] = c;
            tokens[tokenCount].value[1] = '\0';
            tokens[tokenCount].type = TOKEN_PARENTHESIS;
            ++pos;
            ++tokenCount;
        }

        else {
            printf("Ошибка: неизвестный символ '%c'\n", c);
            exit(EXIT_FAILURE);
        }
    }

    return tokenCount;
}
char getPreviousNonSpaceChar(const char* expression, int pos) {
    pos--;
    while (pos >= 0 && isspace(expression[pos])) {
        pos--;
    }
    if (pos >= 0) {
        return expression[pos];
    }
    return '\0'; // Начало строки
}

// Алгоритм Дейкстры для преобразования в постфиксную форму
int getOperatorPrecedence(char op) {
    switch(op) {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
            return 2;
        default:
            return 0;
    }
}
int convertToPostfix(Token* infixTokens, int tokenCount, Token* postfixTokens) {
    OperatorStack opStack;
    initOperatorStack(&opStack);
    int postfixCount = 0;

    for (int i = 0; i < tokenCount; ++i) {
        Token token = infixTokens[i];

        if (token.type == TOKEN_NUMBER || token.type == TOKEN_VARIABLE) {
            postfixTokens[postfixCount++] = token;
        }

        else if (token.type == TOKEN_OPERATOR) {
            while (!isOperatorStackEmpty(&opStack) && peekOperator(&opStack) != '(' &&
            getOperatorPrecedence(peekOperator(&opStack)) >= getOperatorPrecedence(token.value[0])) {
                Token opToken;
                opToken.value[0] = popOperator(&opStack);
                opToken.value[1] = '\0';
                opToken.type = TOKEN_OPERATOR;
                postfixTokens[postfixCount++] = opToken; 
            }
            pushOperator(&opStack, token.value[0]);
        }

        else if (token.type == TOKEN_PARENTHESIS) {
            if (token.value[0] == '(') {
                pushOperator(&opStack, '(');
            }
            else {
                while (!isOperatorStackEmpty(&opStack) && peekOperator(&opStack) != '(') {
                    Token opToken;
                    opToken.type = TOKEN_OPERATOR;
                    opToken.value[0] = popOperator(&opStack);
                    opToken.value[1] = '\0';
                    postfixTokens[postfixCount++] = opToken;
                }
                if (isOperatorStackEmpty(&opStack)) {
                    printf("Ошибка: несогласованные скобки\n");
                    exit(1);
                }
                // Удаляем открывающую скобку из стека
                popOperator(&opStack);
            }
        }
    }

    // Выгружаем оставшиеся операторы из стека
    while (!isOperatorStackEmpty(&opStack)) {
        if (peekOperator(&opStack) == '(' || peekOperator(&opStack) == ')') {
            printf("Ошибка: несогласованные скобки\n");
            exit(1);
        }
        Token opToken;
        opToken.type = TOKEN_OPERATOR;
        opToken.value[0] = popOperator(&opStack);
        opToken.value[1] = '\0';
        postfixTokens[postfixCount++] = opToken;
    }

    return postfixCount;
}

// Дерево
void initNodeStack(NodeStack* stack) {
    stack->top = -1;
}
int isNodeStackEmpty(NodeStack* stack) {
    return stack->top == -1;
}
void pushNode(NodeStack* stack, Node* node) {
    if (stack->top >= MAX_STACK_SIZE - 1) {
        printf("Ошибка: переполнение стека узлов\n");
        exit(1);
    }
    stack->data[++stack->top] = node;
}
Node* popNode(NodeStack* stack) {
    if (isNodeStackEmpty(stack)) {
        printf("Ошибка: стек узлов пуст\n");
        exit(1);
    }
    return stack->data[stack->top--];
}
Node* createNumberNode(int value) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->type = NUMBER;
    node->value = value;
    node->left = node->right = NULL;
    node->operator = '\0';    // Обнуляем для безопасности
    node->variable = NULL;
    return node;
}
Node* createVariableNode(const char* variable) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->type = VARIABLE;
    node->variable = strdup(variable);
    node->left = node->right = NULL;
    node->operator = '\0';
    node->value = 0;
    return node;
}
Node* createOperatorNode(char operator, Node* left, Node* right) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->type = OPERATOR;
    node->operator = operator;
    node->left = left;
    node->right = right;
    node->value = 0;          // Обнуляем для безопасности
    node->variable = NULL;    // Обнуляем для безопасности
    return node;
}
Node* buildExpressionTree(Token* postfixTokens, int tokenCount) {
    NodeStack nodeStack;
    initNodeStack(&nodeStack);

    for (int i = 0; i < tokenCount; i++) {
        Token token = postfixTokens[i];

        if (token.type == TOKEN_NUMBER) {
            Node* node = createNumberNode(atoi(token.value));
            pushNode(&nodeStack, node);
        } else if (token.type == TOKEN_VARIABLE) {
            Node* node = createVariableNode(token.value);
            pushNode(&nodeStack, node);
        } else if (token.type == TOKEN_OPERATOR) {
            Node* rightNode = popNode(&nodeStack);
            Node* leftNode = popNode(&nodeStack);
            Node* opNode = createOperatorNode(token.value[0], leftNode, rightNode);
            pushNode(&nodeStack, opNode);
        }
    }

    if (nodeStack.top != 0) {
        printf("Ошибка: некорректное выражение\n");
        exit(1);
    }

    return popNode(&nodeStack);
}
void printExpression(Node* root) {
    if (root == NULL) {
        return;
    }
    if (root->type == NUMBER) {
        printf("%d", root->value);
    } else if (root->type == VARIABLE) {
        printf("%s", root->variable);
    } else if (root->type == OPERATOR) {
        printf("(");
        printExpression(root->left);
        printf(" %c ", root->operator);
        printExpression(root->right);
        printf(")");
    }
}
void printTree(Node* root, int depth) {
    if (root == NULL) {
        return;
    }
    printTree(root->right, depth + 1);

    for (int i = 0; i < depth; i++) {
        printf("\t");
    }

    if (root->type == OPERATOR) {
        printf("%c\n", root->operator);
    } else if (root->type == VARIABLE) {
        printf("%s\n", root->variable);
    } else if (root->type == NUMBER) {
        printf("%d\n", root->value);
    }

    printTree(root->left, depth + 1);
}
void freeTree(Node* root) {
    if (root == NULL) {
        return;
    }
    freeTree(root->left);
    freeTree(root->right);
    if (root->type == VARIABLE) {
        free(root->variable);
    }
    free(root);
}
Node* simplify(Node* root) {
    if (root == NULL) {
        return NULL;
    }

    // Рекурсивно упрощаем левое и правое поддеревья
    root->left = simplify(root->left);
    root->right = simplify(root->right);

    // Если узел - оператор '+'
    if (root->type == OPERATOR && root->operator == '+') {
        // Если оба потомка - числа
        if (root->left && root->left->type == NUMBER &&
            root->right && root->right->type == NUMBER) {
            // Вычисляем их сумму
            int sum = root->left->value + root->right->value;

            // Заменяем текущий узел на число
            root->type = NUMBER;
            root->value = sum;

            // Освобождаем память
            freeTree(root->left);
            freeTree(root->right);
            root->left = NULL;
            root->right = NULL;
        }
        // Если один из потомков - число, а другой - оператор '+'
        else if ((root->left && root->left->type == NUMBER) && 
                 (root->right && root->right->type == OPERATOR && root->right->operator == '+')) {
            // Попытка собрать числа в правом поддереве
            Node* right = root->right;
            if (right->left && right->left->type == NUMBER) {
                int sum = root->left->value + right->left->value;
                // Создаём новый узел числа
                Node* newNumberNode = createNumberNode(sum);
                // Создаём новый узел '+', соединяющий сумму и правое поддерево
                Node* newPlusNode = createOperatorNode('+', newNumberNode, right->right);
                // Заменяем текущий узел на новый узел '+'
                freeTree(root->left);
                freeTree(right->left);
                free(root->right);
                root->left = newPlusNode->left;
                root->right = newPlusNode->right;
                root->operator = '+';
                root->type = OPERATOR;
                free(newPlusNode);
                // Рекурсивно упрощаем снова
                root = simplify(root);
            } else {
                int sum = root->left->value + right->right->value;
                Node* newNumberNode = createNumberNode(sum);
                Node* newPlusNode = createOperatorNode('+', newNumberNode, right->left);
                freeTree(root->left);
                freeTree(right->right);
                free(root->right);
                root->left = newPlusNode->left;
                root->right = newPlusNode->right;
                root->operator = '+';
                root->type = OPERATOR;
                free(newPlusNode);
                root = simplify(root);
            }
        }
        // Аналогично для случая, когда правый потомок - число, а левый - оператор '+'
        else if ((root->right && root->right->type == NUMBER) && 
                 (root->left && root->left->type == OPERATOR && root->left->operator == '+')) {
            Node* left = root->left;
            if (left->right && left->right->type == NUMBER) {
                int sum = root->right->value + left->right->value;
                Node* newNumberNode = createNumberNode(sum);
                Node* newPlusNode = createOperatorNode('+', newNumberNode, left->left);
                freeTree(root->right);
                freeTree(left->right);
                free(root->left);
                root->left = newPlusNode->left;
                root->right = newPlusNode->right;
                root->operator = '+';
                root->type = OPERATOR;
                free(newPlusNode);
                root = simplify(root);
            } else {
                int sum = root->right->value + left->left->value;
                Node* newNumberNode = createNumberNode(sum);
                Node* newPlusNode = createOperatorNode('+', newNumberNode, left->right);
                freeTree(root->right);
                freeTree(left->left);
                free(root->left);
                root->left = newPlusNode->left;
                root->right = newPlusNode->right;
                root->operator = '+';
                root->type = OPERATOR;
                free(newPlusNode);
                root = simplify(root);
            }
        }
        // Если один из потомков - число 0 (т.е. 0 + 4 * x <=> 4 * x)
        else if (root->left && root->left->type == NUMBER && root->left->value == 0) {
            Node* temp = root->right;
            freeTree(root->left);
            free(root);
            return temp;
        }
        // Если правый потомок - число 0
        else if (root->right && root->right->type == NUMBER && root->right->value == 0) {
            Node* temp = root->left;
            freeTree(root->right);
            free(root);
            return temp;
        }
    }

    return root;
}
