#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct _Node
{
	uint32_t			data;
	struct _Node*		left;
	struct _Node*		right;
} Node, *pNode;

typedef struct _StackNode
{
	pNode				node;
	struct _StackNode*	prev;
} StackNode, *pStackNode;

typedef struct _Stack
{
	pStackNode top;
} Stack, *pStack;

pNode create_tree_node()
{
	pNode	node;

	node = (pNode) malloc(sizeof(Node));
	memset(node, '\0', sizeof(node));

	return node;
}

pNode add_node_to_tree(pNode root, pNode newNode)
{
	pNode temp;

	if (root == NULL)
	{
		root = newNode;
		return root;
	}
	temp = root;
	while (temp != NULL)
	{
		if (newNode->data < temp->data)
		{
			if (temp->left == NULL)
			{
				temp->left = newNode;
				return root;
			}
			else
				temp = temp->left;
		}
		else if (newNode->data >= temp->data)
		{
			if (temp->right == NULL)
			{
				temp->right = newNode;
				return root;
			}
			else
				temp = temp->right;
		}
	}
}

pStack create_stack()
{
	pStack stack;

	stack = (pStack) malloc(sizeof(Stack));
	memset(stack, '\0', sizeof(stack));
	stack->top = NULL;

	return stack;
}

uint32_t is_stack_empty(pStack stack)
{
	return (stack->top == NULL);
}

pStack stack_push(pStack stack, pStackNode newNode)
{
	if (stack->top == NULL)
	{
		stack->top = newNode;
	}
	else
	{
		newNode->prev = stack->top;
		stack->top = newNode;
	}
	return stack;
}

pStack stack_pop(pStack stack)
{
	if (is_stack_empty(stack))
	{
		return NULL;
	}
	stack->top = stack->top->prev;
	return stack;
}

pNode stack_top(pStack stack)
{
	return stack->top->node;
}

pNode del_tree_node(pNode root, pNode node)
{
	pNode prev;
	pNode i;

	if (root == NULL)
	{
		return NULL;
	}

	if (node->left != NULL)
	{
		prev = node->left;
		i = node->left;

		while (i->right != NULL)
		{
			prev = i;
			i = i->right;
		}
		node->data = i->data;

		if (i != node->left)
		{
			prev->right = i->left;
		}
		else
		{
			node->left = i->left;
		}
		free(i);
		return root;
	}
	else
	{
		node = node->right;
		return root;
	}

}

void print_tree(pNode root)
{
	pStackNode	stack_temp;
	pNode		tree_temp;
	pStack stack = NULL;
	
	stack = create_stack();
	
	tree_temp = root;
	while (tree_temp != NULL || is_stack_empty(stack) == 0)
	{
		while (tree_temp != NULL)
		{
			stack_temp = (pStackNode) malloc(sizeof(StackNode));
			stack_temp->prev = NULL;
			stack_temp->node = tree_temp;
			stack = stack_push(stack, stack_temp);
			tree_temp = tree_temp->left;
		}
		if (is_stack_empty(stack) == 0)
		{
			tree_temp = stack_top(stack);
			printf("%d    ", tree_temp->data);
			stack = stack_pop(stack);
			tree_temp = tree_temp->right;	
		}
	}
	printf("\n");
}

void print_tree_digui(pNode root)
{
	if (root != NULL)
	{
		if (root->left != NULL)
			print_tree_digui(root->left);
		printf("%d    ", root->data);
		if (root->right != NULL)
			print_tree_digui(root->right);
	}
}

void main() 
{
	pNode root = NULL;
	pNode newNode;
	uint32_t count = 10;

	srand(time(NULL));
	
	while (count--)
	{
		newNode = create_tree_node();
		newNode->data = rand()%100;
		printf("data=%d\n", newNode->data);
		root = add_node_to_tree(root, newNode);
	}
	print_tree(root);
	print_tree_digui(root);
}