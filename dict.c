#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct node {
    struct node *left, *right;
    const char *value;
};

static unsigned int
hash(const char *s)
{
    unsigned int h = 0;
    int i = 0;t
    while(s[i]) {
        h = (h << 6 | h >> (32 - 6)) ^ (s[i] & 0xFF);
        i++;
    }
    return h;
}

int
compare(const char *w1, const char *w2)
{
    unsigned int h1 = hash(w1), h2 = hash(w2);
    if(h1 < h2)
        return -1;
    else if(h1 > h2)
        return +1;
    else
        return strcmp(w1, w2);
}

struct node *
new_node(const char *string)
{
    char *s;
    struct node *n;

    s = strdup(string);
    if(s == NULL)
        return NULL;
    n = malloc(sizeof(struct node));
    if(n == NULL) {
        free(s);
        return NULL;
    }
    n->left = n->right = NULL;
    n->value = s;
    return n;
}

struct node *
insert(const char *string, struct node *tree)
{
    struct node *n;
    int c;

    if(tree == NULL)
        return new_node(string);

    n = tree;
    while(1) {
        c = compare(string, n->value);
        if(c == 0) {
            return tree;
        } else if(c < 0) {
            if(n->left == NULL) {
                n->left = new_node(string);
                return tree;
            } else {
                n = n->left;
            }
        } else {
            if(n->right == NULL) {
                n->right = new_node(string);
                return tree;
            } else {
                n = n->right;
            }
        }
    }
}

int
member(const char *string, struct node *tree)
{
    struct node *n = tree;
    while(n) {
        int c = compare(string, n->value);
        if(c == 0)
            return 1;
        else if(c < 0)
            n = n->left;
        else
            n = n->right;
    }
    return 0;
}

void
destroy_tree(struct node *tree)
{
    if(tree == NULL)
        return;

    /* Allez, une fois n'est pas coutume, une fonction récursive. */
    destroy_tree(tree->left);
    destroy_tree(tree->right);
    free((void*)tree->value);
    free(tree);
}

struct node *
read_dict(char *filename)
{
    struct node *tree = NULL;
    FILE *f;

    f = fopen(filename, "r");
    if(f == NULL)
        return NULL;
    while(1) {
        char buf[100];
        char *s;
        int len;
        s = fgets(buf, 100, f);
        if(s == NULL)
            break;
        len = strlen(s);
        if(len == 0)
            continue;
        s[len - 1] = '\0';
        tree = insert(s, tree);
    }
    fclose(f);
    return tree;
}

int
main(int argc, char **argv)
{
    struct node *dict = read_dict("/usr/share/dict/words");

    if(dict == NULL) {
        perror("read_dict");
        exit(1);
    }

    while(1) {
        char buf[100];
        char *s;
        int len;
        s = fgets(buf, 100, stdin);
        if(s == NULL)
            break;
        if(strlen(s) == 0)
            continue;
        len = strlen(s);
        if(len == 0)
            continue;
        s[len - 1] = '\0';
        if(member(s, dict))
            printf("Ok.\n");
        else
            printf("KO.\n");
    }

    /* Pas nécessaire, mais permet d'exercer le code. */
    destroy_tree(dict);
}
